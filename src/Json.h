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

        void printSpaces(const std::size_t level) const noexcept
        {
            for(std::size_t i{0}; i < (level * 4); ++i)
                std::cout << " "; 
        }

        void printJsonObject(std::size_t spaceLevel) const noexcept
        {
            std::cout << "{" << std::endl;
            ++spaceLevel;
            const auto printObject = [this, spaceLevel](const auto& item) { 
                printSpaces(spaceLevel);
                std::cout << '\"' << item.first << "\": "; item.second.print(spaceLevel); std::cout << ',' << std::endl;
            };
            auto penultimateIt = std::prev(end(*object_));
            std::for_each(begin(*object_), penultimateIt, printObject);
            printSpaces(spaceLevel);
            std::cout << '\"' << penultimateIt->first << "\": ";
            penultimateIt->second.print(spaceLevel);
            std::cout << std::endl;
            printSpaces(--spaceLevel);
            std::cout << "}";
        }

        void printJsonArray(std::size_t spaceLevel) const noexcept
        {
            std::cout << "[" << std::endl;
            const auto printArray = [this, spaceLevel](const auto& item) { 
                printSpaces(spaceLevel); 
                item.print(spaceLevel); 
                std::cout << ',' << std::endl; 
            };
            auto penultimateIt = std::prev(end(*array_));
            std::for_each(begin(*array_), penultimateIt, printArray);
            printSpaces(spaceLevel);
            penultimateIt->print(spaceLevel);
            std::cout << std::endl;
            printSpaces(spaceLevel);
            std::cout << "]";
        }

        void print(const std::size_t spaceLevel = 0) const noexcept
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
