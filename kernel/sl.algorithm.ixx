export module sl.algorithm;

export namespace std
{
    template <class _InIt, class _OutIt, class _Fn>
    constexpr _OutIt transform(const _InIt _First, const _InIt _Last, _OutIt _Dest, _Fn _Func)
    {
        auto _UFirst = _First;
        const auto _ULast = _Last;
        auto _UDest = _Dest;
        for (; _UFirst != _ULast; ++_UFirst, (void) ++_UDest)
        {
            *_UDest = _Func(*_UFirst);
        }
        return _Dest;
    }

    template <class _InIt1, class _InIt2, class _OutIt, class _Fn>
    constexpr _OutIt transform(const _InIt1 _First1, const _InIt1 _Last1, const _InIt2 _First2, _OutIt _Dest, _Fn _Func)
    {
        auto _UFirst1 = _First1;
        const auto _ULast1 = _Last1;
        const auto _Count = _ULast1 - _UFirst1;
        auto _UFirst2 = _First2;
        auto _UDest = _Dest;
        for (; _UFirst1 != _ULast1; ++_UFirst1, (void) ++_UFirst2, ++_UDest)
        {
            *_UDest = _Func(*_UFirst1, *_UFirst2);
        }
        return _Dest;
    }

    template<class T>
    constexpr const T& min(const T& a, const T& b)
    {
        return (b < a) ? b : a;
    }
    template<class T>
    constexpr const T& max(const T& a, const T& b)
    {
        return (a < b) ? b : a;
    }
}