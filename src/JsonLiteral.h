#ifndef __JSON_LITERAL_HEADER__
#define __JSON_LITERAL_HEADER__

namespace json
{
    enum class JsonLiteral : std::uint8_t
    {
        OPEN_CURLY_BRACE,
        CLOSED_CURLY_BRACE,
        COLON,
        COMMA,
        OPEN_LIST_BRACE,
        CLOSED_LIST_BRACE,
        INTEGER,
        STRING,
        BOOLEAN,
        ARRAY,
        OBJECT,
        NIL,
        JSON_EOF,
        ERROR
    };

    std::string jsonLiteralToString(const JsonLiteral jsonLiteral)
    {
        switch(jsonLiteral)
        {
            case JsonLiteral::OPEN_CURLY_BRACE: return "OPEN_CURLY_BRACE";
            case JsonLiteral::CLOSED_CURLY_BRACE: return "CLOSED_CURLY_BRACE";
            case JsonLiteral::COLON: return "COLON";
            case JsonLiteral::COMMA: return "COMMA";
            case JsonLiteral::OPEN_LIST_BRACE: return "OPEN_LIST_BRACE";
            case JsonLiteral::CLOSED_LIST_BRACE: return "CLOSED_LIST_BRACE";
            case JsonLiteral::INTEGER: return "INTEGER";
            case JsonLiteral::STRING: return "STRING";
            case JsonLiteral::BOOLEAN: return "BOOLEAN";
            case JsonLiteral::NIL: return "NIL";
            case JsonLiteral::JSON_EOF: return "JSON_EOF";
            case JsonLiteral::ERROR: return "ERROR";
            default: return {};
        }
    }
}

#endif