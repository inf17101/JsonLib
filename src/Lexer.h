#ifndef __LEXER_HEADER__
#define __LEXER_HEADER__

#include <cstdint>
#include <string>
#include <vector>
#include "Token.h"
#include "JsonLiteral.h"

namespace json
{
    class Lexer
    {
    public:
        auto operator()(const std::string& rawJson) noexcept
        {
            auto token = lex(rawJson);
            std::vector<Token> tokens { token };
            while(notEndOfJson(token.type_) && notError(token.type_))
            {
                token = lex(rawJson);
                tokens.push_back(token);
            }

            return tokens;
        }

    private:
        inline bool notEndOfJson(const JsonLiteral literal) const noexcept
        {
            return literal != JsonLiteral::JSON_EOF;
        }
        
        inline bool isEndOfJson(const std::string& rawJson) const noexcept
        {
            return position_ >= rawJson.length();
        }

        inline bool notEndOfJson(const std::string& rawJson) const noexcept
        {
            return position_ < rawJson.length();
        }

        inline bool notError(const JsonLiteral literal) const noexcept
        {
            return literal != JsonLiteral::ERROR;
        }

        void skipSpaces(const std::string& rawJson)
        {
            while(std::isspace(rawJson[position_]) > 0 && notEndOfJson(rawJson))
                position_++;
        }

        Token lexString(const std::string& rawJson)
        {
            std::string buffer {};
            while(rawJson[position_] != '"' && notEndOfJson(rawJson))
            {
                buffer += rawJson[position_];
                position_++;
            }
            position_++;
            return { buffer, JsonLiteral::STRING, position_ };
        }

        Token lexInteger(const std::string& rawJson)
        {
            std::string number {};
            while (std::isdigit(rawJson[position_]) > 0)
            {
                number += rawJson[position_];
                position_++;
            }
            return { number, JsonLiteral::INTEGER, position_ }; 
        }

        inline bool isBooleanKeyword(const std::string& keyword) const noexcept
        {
            return keyword == "true" || keyword == "false";
        }

        inline bool isNullKeyword(const std::string& keyword) const noexcept
        {
            return keyword == "null";   
        }

        Token checkKeyword(const std::string& keyword) const noexcept
        {
            if(isBooleanKeyword(keyword))
                return { keyword, JsonLiteral::BOOLEAN, position_};
            else if(isNullKeyword(keyword))
                return { keyword, JsonLiteral::NIL, position_};

            return { {}, JsonLiteral::ERROR, position_};
        }

        inline bool notKeywordEnd(char indicator) const noexcept
        {
            return indicator != ',' && indicator != '}' && indicator != ']' && !std::isspace(indicator);
        }

        inline bool isKeyword(char indicator) const noexcept
        {
            return indicator == 't' || indicator == 'f' || indicator == 'n';
        }

        auto lexKeyword(const std::string& rawJson)
        {
            std::string keyword {};
            if(isKeyword(rawJson[position_]))
            {
                while(notKeywordEnd(rawJson[position_]) && notEndOfJson(rawJson))
                {
                    keyword += rawJson[position_];
                    position_++;
                }
            }
            return checkKeyword(keyword);
        }

        Token lex(const std::string& rawJson)
        {
            skipSpaces(rawJson);
            if(isEndOfJson(rawJson))
                return Token{ {}, JsonLiteral::JSON_EOF, position_ };

            switch(rawJson[position_])
            {
                case '"': position_++; return lexString(rawJson);
                case '0': case '1': case '2': case '3': case '4': 
                case '5': case '6': case '7': case '8': 
                case '9': return lexInteger(rawJson);
                case '{': return { {}, JsonLiteral::OPEN_CURLY_BRACE, ++position_ };
                case '}': return { {}, JsonLiteral::CLOSED_CURLY_BRACE, ++position_ };
                case '[': return { {}, JsonLiteral::OPEN_LIST_BRACE, ++position_ };
                case ']': return { {}, JsonLiteral::CLOSED_LIST_BRACE, ++position_ };
                case ':': return { {}, JsonLiteral::COLON, ++position_ };
                case ',': return { {}, JsonLiteral::COMMA, ++position_ };
                case 't': case 'f': case 'n': return lexKeyword(rawJson);
                default: return { {}, JsonLiteral::ERROR, position_ };
            }
            return { {}, JsonLiteral::ERROR, position_ };
        }

        std::size_t position_ {0};
    };
}

#endif
