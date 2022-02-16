#ifndef __PARSER_HEADER__
#define __PARSER_HEADER__

#include <vector>
#include <map>
#include "Tools.h"
#include "Token.h"
#include "Lexer.h"
#include "JsonLiteral.h"
#include "Json.h"

namespace json
{
    template<typename JsonLexer>
    class Parser
    {
    public:
        std::tuple<Json, std::string> parse(const std::string& rawJson)
        {
            tokens_ = JsonLexer{}(rawJson);
            auto [json, _, error] = parse();
            return { json, error };
        }

    private:
        inline bool isSyntaxLiteral(const JsonLiteral literal)
        {
            return literal == JsonLiteral::OPEN_CURLY_BRACE || literal == JsonLiteral::CLOSED_CURLY_BRACE
                || literal == JsonLiteral::COLON || literal == JsonLiteral::COMMA || literal == JsonLiteral::OPEN_LIST_BRACE 
                || literal == JsonLiteral::CLOSED_LIST_BRACE;
        }

        std::tuple<std::vector<Json>, std::size_t, std::string> parseJsonArray()
        {
            std::vector<Json> children;
            auto amountOfTokens { tokens_.size() };
            while(index_ < amountOfTokens)
            {
                auto token = tokens_[index_];
                if(isSyntaxLiteral(token.type_))
                {
                    if(token.type_ == JsonLiteral::CLOSED_LIST_BRACE)
                        return {children, ++index_, {}};
                    if(token.type_ == JsonLiteral::COMMA)
                    {
                        token = tokens_[index_];
                        index_++;
                    }else if(notEmpty(children))
                    {
                        return { {}, index_, "Expected a comma between Json Array elements." };
                    }
                }

                auto [child, newIndex, error] = parse();
                if(notEmpty(error))
                    return { {}, index_, error };
                
                children.push_back(std::move(child));
                index_ = newIndex;
            }
            return { {}, index_, "Unexpected EOF while parsing array" };
        }

        std::tuple<std::map<std::string, Json>, std::size_t, std::string> parseJsonObject()
        {
            std::map<std::string, Json> values = {};
            auto amountOfTokens { tokens_.size() };
            while(index_ < amountOfTokens)
            {
                auto token = tokens_[index_];
                if(isSyntaxLiteral(token.type_))
                {
                    if(token.type_ == JsonLiteral::CLOSED_CURLY_BRACE)
                        return { values, ++index_, {} };

                    if(token.type_ == JsonLiteral::COMMA)
                    {
                        index_++;
                        token = tokens_[index_];
                    }else if(notEmpty(values))
                    {
                        return { {}, index_, "Expected comma after element in object." };
                    }else
                    {
                        return { {}, index_, "Expected key-value pair or closing brace in object." };
                    }
                }

                auto [key, newIndex, error] = parse();
                if(notEmpty(error))
                    return { {}, index_, error };
                
                if(key.type_ != JsonLiteral::STRING)
                    return { {}, index_, "Expected string key in object." };
                
                index_ = newIndex;
                token = tokens_[index_];

                if(token.type_ != JsonLiteral::COLON)
                    return { {}, index_, "Expected colon after key in object." };
                
                index_++;
                token = tokens_[index_];
                auto [value, newIndex1, error1] = parse();
                if(notEmpty(error1))
                    return { {}, index_, error1 };
                values[key.string_.value()] = std::move(value);
                index_ = newIndex1;
            }
            return { {}, ++index_, "" };
        }

        std::tuple<Json, std::size_t, std::string> parse()
        {
            auto token = tokens_[index_];
            switch (token.type_)
            {
                case JsonLiteral::INTEGER: return { Json{ .integer_ = std::stoi(token.content_), 
                                                .type_ = JsonLiteral::INTEGER }, ++index_, {} };
                case JsonLiteral::STRING: return { Json{ .string_ = token.content_, 
                                                .type_ = JsonLiteral::STRING }, ++index_, {} };
                case JsonLiteral::BOOLEAN: return { Json{.boolean_ = token.content_ == "true",
                                                    .type_ = JsonLiteral::BOOLEAN }, ++index_, {} };
                case JsonLiteral::OPEN_LIST_BRACE: {
                    index_++;
                    auto [array, newIndex, error] = parseJsonArray();
                    return {Json{.array_ = array, .type_ = JsonLiteral::ARRAY}, newIndex, error};
                }
                case JsonLiteral::OPEN_CURLY_BRACE: {
                    index_++;
                    auto [object, newIndex, error] = parseJsonObject();
                    return { Json{.object_ = std::optional(object), .type_ = JsonLiteral::OBJECT}, newIndex, error };
                }
                case JsonLiteral::NIL: return { Json{ .string_ = token.content_, 
                                                .type_ = JsonLiteral::NIL }, ++index_, {} };
                default: return { {}, index_, "generic parsing error. the given type is not supported." };
            }
            return {{}, index_, "Failed to parse."};
        }
    private:
        std::vector<Token> tokens_;
        std::size_t index_{0};
    };
}

#endif
