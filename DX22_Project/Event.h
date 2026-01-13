// Event.h
#pragma once
#include <vector>
#include <functional>
#include <cstdint>
#include <memory>
#include <utility>

/// ===========================================================
/// 汎用 Event クラス
///   Event<Args...> として、引数付きイベントにも対応可能。
///   例) Event<>                : 引数なし
///       Event<int>            : int を 1 つ渡すイベント
///       Event<float, int>     : (float, int) を渡すイベント
///
/// 内部的に shared_ptr / weak_ptr を使うことで
///   - Subject(Event) 破棄後に Subscriber 側の RAII が動いても安全
///   - 将来の拡張（スレッドセーフ化等）もしやすい
/// ===========================================================
template<class... Args>
class Event
{
public:
    using Listener = std::function<void(Args...)>;
    using ListenerId = std::uint32_t;
    using Unsubscriber = std::function<void()>;   // RAII 用

private:
    struct Slot
    {
        ListenerId id = 0;
        Listener   func;
        bool       active = true;
    };

    struct Impl
    {
        std::vector<Slot> slots{};
        ListenerId        nextId = 1;
        bool              dirty = false;

        ListenerId Add(const Listener& f)
        {
            if (!f) return 0;
            Slot s;
            s.id = nextId++;
            s.func = f;
            s.active = true;
            slots.push_back(std::move(s));
            return s.id;
        }

        void RemoveById(ListenerId id)
        {
            if (id == 0) return;
            for (auto& s : slots)
            {
                if (s.id == id)
                {
                    s.active = false;
                    s.func = nullptr;
                    dirty = true;
                    break;
                }
            }
        }

        void Clear()
        {
            slots.clear();
            nextId = 1;
            dirty = false;
        }

        void Compact()
        {
            if (!dirty) return;

            std::vector<Slot> alive;
            alive.reserve(slots.size());
            for (auto& s : slots)
            {
                if (s.active && s.func)
                {
                    alive.push_back(std::move(s));
                }
            }
            slots.swap(alive);
            dirty = false;
        }
    };

    std::shared_ptr<Impl> impl;

public:
    Event()
        : impl(std::make_shared<Impl>())
    {
    }

    // コピー / ムーブは impl を共有する（=同じイベント）
    Event(const Event&) = default;
    Event& operator=(const Event&) = default;
    Event(Event&&) = default;
    Event& operator=(Event&&) = default;

    /// リスナー登録
    ListenerId AddListener(const Listener& f)
    {
        if (!impl) return 0;
        return impl->Add(f);
    }

    /// リスナー解除（手動）
    void RemoveListener(ListenerId id)
    {
        if (!impl) return;
        impl->RemoveById(id);
    }

    /// 全リスナー解除
    void Clear()
    {
        if (!impl) return;
        impl->Clear();
    }

    /// リスナーの有無
    bool HasListeners() const
    {
        return impl && !impl->slots.empty();
    }

    /// イベント発火
    void Invoke(Args... args)
    {
        if (!impl) return;

        impl->Compact(); // 死んだスロットの整理（必要に応じて軽量化は可能）

        for (auto& s : impl->slots)
        {
            if (s.active && s.func)
            {
                s.func(args...);
            }
        }
    }

    /// RAII 用の Unsubscriber を生成
    ///  - weak_ptr 経由で Impl を参照するので、Event 側が破棄されても安全
    Unsubscriber MakeUnsubscriber(ListenerId id) const
    {
        std::weak_ptr<Impl> weak = impl;
        return [weak, id]()
            {
                if (auto p = weak.lock())
                {
                    p->RemoveById(id);
                }
            };
    }
};

/// ===========================================================
/// RAII で購読解除を自動化するサブスクリプション
///   - Event<Args...> とセットで使用
///   - 例:
///       Event<> ev;
///       EventSubscription<> sub(ev, ev.AddListener(...));
/// ===========================================================
template<class... Args>
class EventSubscription
{
public:
    using EventType = Event<Args...>;
    using ListenerId = typename EventType::ListenerId;
    using Unsubscriber = typename EventType::Unsubscriber;

private:
    Unsubscriber unsubscriber{};

public:
    EventSubscription() = default;

    // Event と ListenerId から生成
    EventSubscription(const EventType& ev, ListenerId id)
    {
        if (id != 0)
        {
            unsubscriber = ev.MakeUnsubscriber(id);
        }
    }

    // Unsubscriber を直接渡すコンストラクタ
    explicit EventSubscription(Unsubscriber unsub)
        : unsubscriber(std::move(unsub))
    {
    }

    // コピー禁止
    EventSubscription(const EventSubscription&) = delete;
    EventSubscription& operator=(const EventSubscription&) = delete;

    // ムーブのみ許可
    EventSubscription(EventSubscription&& other) noexcept
        : unsubscriber(std::move(other.unsubscriber))
    {
        other.unsubscriber = nullptr;
    }

    EventSubscription& operator=(EventSubscription&& other) noexcept
    {
        if (this != &other)
        {
            Unsubscribe();
            unsubscriber = std::move(other.unsubscriber);
            other.unsubscriber = nullptr;
        }
        return *this;
    }

    ~EventSubscription()
    {
        Unsubscribe();
    }

    /// 手動解除
    void Unsubscribe()
    {
        if (unsubscriber)
        {
            unsubscriber();
            unsubscriber = nullptr;
        }
    }

    bool IsValid() const
    {
        return static_cast<bool>(unsubscriber);
    }
};

/// 引数なしイベントのためのエイリアス
using SimpleEvent = Event<>;
using SimpleEventSubscription = EventSubscription<>;
