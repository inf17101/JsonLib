# Easy Json Library (Header only)

### How to use?

Example 1:<br>

```
#include <iostream>
#include "jsonlib.h"
using namespace json;

int main()
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
    if(notEmpty(error))
        std::cout << error << std::endl;
    return 0;
}
```
