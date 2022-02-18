#include <gtest/gtest.h>
#include "jsonlib.h"
#include "JsonLiteral.h"

using namespace json;

TEST(LexerCallOperatorTest, LexSimpleFlatJson)
{
    const std::string jsonExample {R"_json(
    {
      "name": "software rendering list",
      "some boolean": true,
      "some integer": 8
    })_json"};

    auto tokens = Lexer {}(jsonExample);

    const std::vector<JsonLiteral> expectedTokens {
        JsonLiteral::OPEN_CURLY_BRACE,
        JsonLiteral::STRING,
        JsonLiteral::COLON,
        JsonLiteral::STRING,
        JsonLiteral::COMMA,
        JsonLiteral::STRING,
        JsonLiteral::COLON,
        JsonLiteral::BOOLEAN,
        JsonLiteral::COMMA,
        JsonLiteral::STRING,
        JsonLiteral::COLON,
        JsonLiteral::INTEGER,
        JsonLiteral::CLOSED_CURLY_BRACE,
        JsonLiteral::JSON_EOF
    };

    constexpr auto checkEquality = [](const Token& lhs, const JsonLiteral rhs) { return  lhs.type_ == rhs; };
    auto isEqual = std::equal(begin(tokens), end(tokens), begin(expectedTokens), checkEquality);

    ASSERT_TRUE(isEqual);
}

TEST(LexerCallOperatorTest, LexEmptyJson)
{
    const std::string jsonExample {R"_json({})_json"};

    auto tokens = Lexer {}(jsonExample);

    const std::vector<JsonLiteral> expectedTokens {
        JsonLiteral::OPEN_CURLY_BRACE,
        JsonLiteral::CLOSED_CURLY_BRACE,
        JsonLiteral::JSON_EOF
    };

    constexpr auto checkEquality = [](const Token& lhs, const JsonLiteral rhs) { return  lhs.type_ == rhs; };
    auto isEqual = std::equal(begin(tokens), end(tokens), begin(expectedTokens), checkEquality);

    ASSERT_TRUE(isEqual);
}

TEST(LexerCallOperatorTest, LexNullJson)
{
    const std::string jsonExample {R"_json()_json"};

    auto tokens = Lexer {}(jsonExample);

    const std::vector<JsonLiteral> expectedTokens {
        JsonLiteral::JSON_EOF
    };

    constexpr auto checkEquality = [](const Token& lhs, const JsonLiteral rhs) { return  lhs.type_ == rhs; };
    auto isEqual = std::equal(begin(tokens), end(tokens), begin(expectedTokens), checkEquality);

    ASSERT_TRUE(isEqual);
}

TEST(LexerCallOperatorTest, LexNullAsValueJson)
{
    const std::string jsonExample {R"_json(
        {
            "null value": null
        }
    )_json"};

    auto tokens = Lexer {}(jsonExample);

    const std::vector<JsonLiteral> expectedTokens {
        JsonLiteral::OPEN_CURLY_BRACE,
        JsonLiteral::STRING,
        JsonLiteral::COLON,
        JsonLiteral::NIL,
        JsonLiteral::CLOSED_CURLY_BRACE,
        JsonLiteral::JSON_EOF
    };

    constexpr auto checkEquality = [](const Token& lhs, const JsonLiteral rhs) { return  lhs.type_ == rhs; };
    auto isEqual = std::equal(begin(tokens), end(tokens), begin(expectedTokens), checkEquality);

    ASSERT_TRUE(isEqual);
}