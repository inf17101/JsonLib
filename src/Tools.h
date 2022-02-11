#ifndef __TOOLS_HEADER__
#define __TOOLS_HEADER__

namespace json
{
    template<typename Item>
    inline bool notEmpty(const Item& item) noexcept
    {
        return item.size() > 0;
    }
}

#endif