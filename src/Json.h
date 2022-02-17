#ifndef __JSON_HEADER__
#define __JSON_HEADER__

#include <iostream>
#include <vector>
#include <optional>
#include <map>
#include <algorithm>
#include "JsonLiteral.h"

namespace json
{
    struct Json 
    {
        std::optional<std::string> string_;
        std::optional<int> integer_;
        std::optional<bool> boolean_;
        std::optional<std::vector<Json>> array_;
        std::optional<std::map<std::string, Json>> object_;
        JsonLiteral type_;

        void printJsonObject(std::size_t spaceLevel) const noexcept
        {
            std::cout << "{" << std::endl;
            ++spaceLevel;
            auto copySpaceLevel{spaceLevel};
            ++copySpaceLevel;
            const auto printObject = [this, copySpaceLevel](const auto& item) { printSpaces(copySpaceLevel); std::cout << '\"' << item.first << '\"' << ": "; item.second.print(copySpaceLevel); std::cout << ',' << std::endl; };
            auto endIt = std::prev(end(*object_));
            std::for_each(begin(*object_), endIt, printObject);
            printSpaces(copySpaceLevel);
            std::cout << '\"' << endIt->first << "\": ";
            endIt->second.print(copySpaceLevel);
            std::cout << std::endl;
            printSpaces(--spaceLevel);
            std::cout << "}";
        }

        void printSpaces(const std::size_t level) const noexcept
        {
            for(std::size_t i{0}; i < (level * 2); ++i)
                std::cout << " "; 
        }

        void printJsonArray(std::size_t spaceLevel) const noexcept
        {
            std::cout << "[" << std::endl;
            const auto printArray = [this, spaceLevel](const auto& item) mutable { printSpaces(spaceLevel); item.print(spaceLevel); std::cout << ',' << std::endl; };
            std::for_each(begin(*array_), std::prev(end(*array_)), printArray);
            decltype(auto) endIt = (*array_).back();
            printSpaces(spaceLevel);
            endIt.print(spaceLevel);
            std::cout << std::endl;
            printSpaces(spaceLevel);
            std::cout << "]";
        }

        void print(std::size_t spaceLevel) const noexcept
        {
            if (string_)
            {
                std::cout << '\"' << *string_ << '\"';
            }else if (boolean_)
            {
                std::cout << std::boolalpha;
                std::cout << *boolean_;
            }else if (integer_)
            {
                std::cout << *integer_;
            }else if (array_)
            {
                printJsonArray(spaceLevel);
            }else if (object_)
            {
                printJsonObject(spaceLevel);
            }
        }
    };
}

#endif
