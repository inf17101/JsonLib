#include <gtest/gtest.h>
#include "jsonlib.h"
#include "JsonLiteral.h"

using namespace json;

TEST(ParserParseTest, ParseComplicatedJson)
{ 
    const std::string jsonExample {R"_json(
    {
      "name": "software rendering list",
      "version": "10.9",
      "entries": [
        {
          "id": 1,
          "9" : true,
          "description": "ATI Radeon X1900 is not compatible with WebGL on the Mac",
          "webkit_bugs": [47028, 233],
          "os": {
            "type": "macosx"
          },
          "vendor_id": "0x1002",
          "device_id": ["0x7249"],
          "features": [
            "webgl",
            "flash_3d",
            "flash_stage3d"
          ]
        },
        {
          "id": 3,
          "description": "GL driver is software rendered. GPU acceleration is disabled",
          "cr_bugs": [59302, 315217],
          "os": {
            "type": "linux"
          },
          "gl_renderer": "(?i).*software.*",
          "features": [
            "all"
          ]
        }
      ]
    })_json"};

    Lexer lexer{};
    Parser<Lexer> parser{};
    auto [json, error] = parser.parse(jsonExample);

    ASSERT_FALSE(notEmpty(error));
    ASSERT_EQ(json.type_, JsonLiteral::OBJECT);
}

TEST(ParserParseTest, ParseSimpleFlatJson)
{ 
    const std::string jsonExample {R"_json(
    {
      "name": "software rendering list",
      "version": "10.9",
      "some boolean": true,
      "some integer": 8
    })_json"};

    Lexer lexer{};
    Parser<Lexer> parser{};
    auto [json, error] = parser.parse(jsonExample);

    ASSERT_FALSE(notEmpty(error));
    ASSERT_EQ(json.type_, JsonLiteral::OBJECT);
}

TEST(ParserParseTest, ParseEmptyJson)
{ 
    const std::string jsonExample {R"_json({})_json"};

    Lexer lexer{};
    Parser<Lexer> parser{};
    auto [json, error] = parser.parse(jsonExample);

    ASSERT_FALSE(notEmpty(error));
    ASSERT_EQ(json.type_, JsonLiteral::OBJECT);
}