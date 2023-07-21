export module sl.source_location;
export namespace std
{
    struct source_location
    {
        [[nodiscard]] static consteval source_location current(
            const unsigned int line = __builtin_LINE(),
            const unsigned int column = __builtin_COLUMN(),
            const char* const file = __builtin_FILE(),
            const char* const function = __builtin_FUNCTION()) noexcept
        {
            source_location result;
            result.mline = line;
            result.mcolumn = column;
            result.file = file;
            result.function = function;
            return result;
        }
        [[nodiscard]] constexpr source_location() noexcept = default;
        [[nodiscard]] constexpr unsigned int line() const noexcept
        {
            return mline;
        }
        [[nodiscard]] constexpr unsigned int column() const noexcept
        {
            return mcolumn;
        }
        [[nodiscard]] constexpr const char* file_name() const noexcept
        {
            return file;
        }
        [[nodiscard]] constexpr const char* function_name() const noexcept
        {
            return function;
        }
    private:
        unsigned int mline{};
        unsigned int mcolumn{};
        const char* file = "";
        const char* function = "";
    };
}