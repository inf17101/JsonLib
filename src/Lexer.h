#ifndef __LEXER_HEADER__
#define __LEXER_HEADER__

#include <cstdint>
#include <string>
#include <vector>
#include "Token.h"
#include "JsonLiteral.h"

class Lexer
{
public:
    auto operator()(const std::string& rawJson) noexcept
    {
        auto token = lex(rawJson);
        std::vector<Token> tokens { token };
        while(token.type_ != JsonLiteral::JSON_EOF)
        {
            token = lex(rawJson, token.position_);
            tokens.push_back(token);
        }

        return tokens;
    }

private:
    bool isEndOfJson(const std::string& rawJson, std::size_t position) const noexcept
    {
        return position >= rawJson.length();
    }

    bool notEndOfJson(const std::string& rawJson, std::size_t position) const noexcept
    {
        return position < rawJson.length();
    }

    std::size_t skipSpaces(const std::string& rawJson, std::size_t position)
    {
        while(std::isspace(rawJson[position]) > 0 && notEndOfJson(rawJson, position))
            position++;
        return position;
    }

    Token lexString(const std::string& rawJson, std::size_t position)
    {
        std::string buffer {};
        while(rawJson[position] != '"' && notEndOfJson(rawJson, position))
        {
            buffer += rawJson[position];
            position++;
        }
        //std::cout << buffer << std::endl;
        position++;
        return { buffer, JsonLiteral::STRING, position };
    }

    Token lexInteger(const std::string& rawJson, std::size_t position)
    {
        std::string number {};
        while (std::isdigit(rawJson[position]) > 0)
        {
            number += rawJson[position];
            position++;
        }
        //std::cout << number << std::endl;
        return { number, JsonLiteral::INTEGER, position }; 
    }

    inline bool isBooleanKeyword(const std::string& keyword) const noexcept
    {
        return keyword == "true" || keyword == "false";
    }

    inline bool isNullKeyword(const std::string& keyword) const noexcept
    {
        return keyword == "null";   
    }

    Token checkKeyword(const std::string& keyword, std::size_t position) const noexcept
    {
        if(isBooleanKeyword(keyword))
            return { keyword, JsonLiteral::BOOLEAN, position};
        else if(isNullKeyword(keyword))
            return { keyword, JsonLiteral::NIL, position};

        return { {}, JsonLiteral::ERROR, position};
    }

    inline bool notKeywordEnd(char indicator) const noexcept
    {
        return indicator != ',' && indicator != '}' && indicator != ']' && !std::isspace(indicator);
    }

    inline bool isKeyword(char indicator) const noexcept
    {
        return indicator == 't' || indicator == 'f' || indicator == 'n';
    }

    auto lexKeyword(const std::string& rawJson, std::size_t position)
    {
        std::string keyword {};
        if(isKeyword(rawJson[position]))
        {
            while(notKeywordEnd(rawJson[position]) && notEndOfJson(rawJson, position))
            {
                keyword += rawJson[position];
                position++;
            }
        }
        //std::cout << keyword << std::endl;
        return checkKeyword(keyword, position);
    }

    Token lex(const std::string& rawJson, std::size_t position = 0)
    {
        position = skipSpaces(rawJson, position);
        if(isEndOfJson(rawJson, position))
            return Token{ {}, JsonLiteral::JSON_EOF, position };

        switch(rawJson[position])
        {
            case '"': return lexString(rawJson, ++position);
            case '0': case '1': case '2': case '3': case '4': 
            case '5': case '6': case '7': case '8': 
            case '9': return lexInteger(rawJson, position);
            case '{': return { {}, JsonLiteral::OPEN_CURLY_BRACE, ++position };
            case '}': return { {}, JsonLiteral::CLOSED_CURLY_BRACE, ++position };
            case '[': return { {}, JsonLiteral::OPEN_LIST_BRACE, ++position };
            case ']': return { {}, JsonLiteral::CLOSED_LIST_BRACE, ++position };
            case ':': return { {}, JsonLiteral::COLON, ++position };
            case ',': return { {}, JsonLiteral::COMMA, ++position };
            case 't': case 'f': case 'n': return lexKeyword(rawJson, position);
        }
        return { {}, JsonLiteral::NIL, position };
    }
};

#endif