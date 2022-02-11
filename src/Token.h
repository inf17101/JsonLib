#ifndef __TOKEN_HEADER__
#define __TOKEN_HEADER__

#include <string>
#include "JsonLiteral.h"

namespace json
{
    struct Token
    {
        std::string content_{};
        JsonLiteral type_{JsonLiteral::NIL};
        std::size_t position_{0};
    };
}

#endif