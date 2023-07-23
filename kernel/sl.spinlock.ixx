export module sl.spinlock;
import sl.atomic;
import intrinsic0;
export namespace std
{
    class spinlock {
    public:
        void lock()
        {
            bool was_locked;
            int times = 0;
            [[unlikely]] do {
                [[unlikely]] if (times++ > 8)
                {
                    _mm_pause();
                }
                was_locked = locked.exchange(true, std::memory_order_acquire);
            } while ((was_locked));
        }
        bool try_lock()
        {
            bool was_locked = locked.exchange(true, std::memory_order_acquire);
            [[likely]] return !was_locked;
        }
        void unlock()
        {
            locked.store(false, std::memory_order_release);
        }

    private:
        std::atomic_bool locked{ false };
    };
}