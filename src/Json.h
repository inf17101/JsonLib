#ifndef __JSON_HEADER__
#define __JSON_HEADER__

#include <vector>
#include <optional>
#include <map>
#include "JsonLiteral.h"

namespace json
{
    struct Json {
    std::optional<std::string> string_;
    std::optional<int> integer_;
    std::optional<bool> boolean_;
    std::optional<std::vector<Json>> array_;
    std::optional<std::map<std::string, Json>> object_;
    JsonLiteral type_;

    void print() const noexcept
    {
        if (string_)
        {
            std::cout << *string_;
        }
        else if (boolean_)
        {
            std::cout << std::boolalpha;
            std::cout << *boolean_;
        }
        else if (integer_)
        {
            std::cout << *integer_;
        }
        else if (array_)
        {
            std::cout << "[";
            auto printArray = [](const auto& item) { item.print(); std::cout << ','; };
            std::for_each(begin(*array_), --end(*array_), printArray);
            decltype(auto) endIt = (*array_).back();
            endIt.print();
            std::cout << "]" << std::endl;
        }
        else if (object_)
        {
            std::cout << "{" << std::endl;
            auto printObject = [](const auto& item) { std::cout << item.first << ": "; item.second.print(); std::cout << ',' << std::endl; };
            std::for_each(begin(*object_), --end(*object_), printObject);
            auto endIt = std::prev(end(*object_));
            std::cout << endIt->first << ": ";
            endIt->second.print();
            std::cout << std::endl;
            std::cout << "}" << std::endl;
        }
    }
    };
}

#endif
