#ifndef __PARSER_HEADER__
#define __PARSER_HEADER__

#include <vector>
#include <map>
#include "Tools.h"
#include "Token.h"
#include "Lexer.h"
#include "JsonLiteral.h"
#include "Json.h"

template<typename JsonLexer>
class Parser
{
public:
    std::tuple<Json, std::string> parse(const std::string& rawJson, JsonLexer lexer)
    {
        //Lexer lexer{};
        auto tokens = lexer(rawJson);
        auto [json, _, error] = parse(tokens);
        /*std::size_t i {0};
        for(const auto& t : tokens)
        {
            std::cout << i << ": " << JsonLiteralToString(t.type_) << std::endl;
            i++;
        }*/
        return { json, error };
    }

private:
    bool isSyntaxLiteral(const JsonLiteral literal)
    {
        return literal == JsonLiteral::OPEN_CURLY_BRACE || literal == JsonLiteral::CLOSED_CURLY_BRACE
            || literal == JsonLiteral::COLON || literal == JsonLiteral::COMMA || literal == JsonLiteral::COMMA
            || literal == JsonLiteral::OPEN_LIST_BRACE || literal == JsonLiteral::CLOSED_LIST_BRACE;
    }

    std::tuple<std::vector<Json>, std::size_t, std::string> 
        parseJsonArray(const std::vector<Token>& tokens, std::size_t index)
        {
            std::vector<Json> children;
            auto amountOfTokens { tokens.size() };
            while(index < amountOfTokens)
            {
                auto token = tokens[index];
                if(isSyntaxLiteral(token.type_))
                {
                    if(token.type_ == JsonLiteral::CLOSED_LIST_BRACE)
                        return {children, index + 1, {}};
                    if(token.type_ == JsonLiteral::COMMA)
                    {
                        token = tokens[index];
                        index++;
                    }else if(notEmpty(children))
                    {
                        return { {}, index, "Expected a comma between Json Array elements." };
                    }
                }

                auto [child, newIndex, error] = parse(tokens, index);
                if(notEmpty(error))
                    return { {}, index, error };
                
                children.push_back(std::move(child));
                index = newIndex;
            }
            return { {}, index, "Unexpected EOF while parsing array" };
        }

    std::tuple<std::map<std::string, Json>, std::size_t, std::string>
        parseJsonObject(const std::vector<Token> tokens, std::size_t index)
        {
            std::map<std::string, Json> values = {};
            auto amountOfTokens { tokens.size() };
            while(index < amountOfTokens)
            {
                auto token = tokens[index];
                if(isSyntaxLiteral(token.type_))
                {
                    if(token.type_ == JsonLiteral::CLOSED_CURLY_BRACE)
                        return { values, index + 1, {} };

                    if(token.type_ == JsonLiteral::COMMA)
                    {
                        index++;
                        token = tokens[index];
                    }else if(notEmpty(values))
                    {
                        return { {}, index, "Expected comma after element in object." };
                    }else
                    {
                        return { {}, index, "Expected key-value pair or closing brace in object." };
                    }
                }

                auto [key, newIndex, error] = parse(tokens, index);
                if(notEmpty(error))
                    return { {}, index, error };
                
                if(key.type_ != JsonLiteral::STRING)
                    return { {}, index, "Expected string key in object." };
                
                index = newIndex;
                token = tokens[index];

                if(token.type_ != JsonLiteral::COLON)
                    return { {}, index, "Expected colon after key in object." };
                
                index++;
                token = tokens[index];
                auto [value, newIndex1, error1] = parse(tokens, index);
                if(notEmpty(error1))
                    return { {}, index, error1 };
                values[key.string_.value()] = std::move(value);
                index = newIndex1;
            }
            return { {}, index + 1, "" };
        }

    std::tuple<Json, std::size_t, std::string> 
        parse(const std::vector<Token>& tokens, std::size_t index = 0)
        {
            auto token = tokens[index];
            switch (token.type_)
            {
                case JsonLiteral::INTEGER: return { Json{ .integer_ = std::stoi(token.content_), 
                                                .type_ = JsonLiteral::INTEGER }, index + 1, {} };
                case JsonLiteral::STRING: return { Json{ .string_ = token.content_, 
                                                .type_ = JsonLiteral::STRING }, index + 1, {} };
                case JsonLiteral::BOOLEAN: return { Json{.boolean_ = token.content_ == "true",
                                                    .type_ = JsonLiteral::BOOLEAN }, index + 1, {} };
                case JsonLiteral::OPEN_LIST_BRACE: {
                    auto [array, newIndex, error] = parseJsonArray(tokens, index + 1);
                    return {Json{.array_ = array, .type_ = JsonLiteral::ARRAY}, newIndex, error};
                }
                case JsonLiteral::OPEN_CURLY_BRACE: {
                    auto [object, newIndex, error] = parseJsonObject(tokens, index + 1);
                    return { Json{.object_ = std::optional(object), .type_ = JsonLiteral::OBJECT}, newIndex, error };
                }
                case JsonLiteral::NIL: return { Json{ .string_ = token.content_, 
                                                .type_ = JsonLiteral::NIL }, index +1, {} };
                default: return { {}, index, "generic parsing error. the given type is not supported." };
            }
            return {{}, index, "Failed to parse."};
        }
};

#endif