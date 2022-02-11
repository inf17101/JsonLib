#ifndef __JSON_HEADER__
#define __JSON_HEADER__

#include <vector>
#include <optional>
#include <map>
#include "JsonLiteral.h"

struct Json {
  std::optional<std::string> string_;
  std::optional<int> integer_;
  std::optional<bool> boolean_;
  std::optional<std::vector<Json>> array_;
  std::optional<std::map<std::string, Json>> object_;
  JsonLiteral type_;

  void print() const noexcept
  {
    if(string_)
    {
        std::cout << *string_ << std::endl;
    }
    else if(boolean_)
    {
        std::cout << std::boolalpha;
        std::cout << *boolean_ << std::endl;
    }else if(integer_)
    {
        std::cout << *integer_ << std::endl;
    }

    for(const auto& item : array_.value_or(std::vector<Json> {}))
    {
        item.print();
    }

    for(const auto& [key, value] : object_.value_or(std::map<std::string, Json> {}))
    {
        std::cout << key << ": ";
        value.print();
    }
  }
};

#endif