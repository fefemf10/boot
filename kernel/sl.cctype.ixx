export module sl.cctype;

export namespace std
{
    __forceinline int tolower(const int c)
    {
        if (c >= 'A' && c <= 'Z')
        {
            return c - ('A' - 'a');
        }
        return c;
    }
    __forceinline int toupper(const int c)
    {
        if (c >= 'a' && c <= 'z')
        {
            return c - ('a' - 'A');
        }
        return c;
    }
    __forceinline int iswalpha(const int c)
    {
        return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
    }
    __forceinline int iswdigit(const int c)
    {
        return c >= '0' && c <= '9';
    }
}