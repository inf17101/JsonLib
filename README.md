# Easy Json Library (Header only)

### How to use?

__Example 1:__ parse a complex and nested json<br>

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

__Example 2:__ Print the parsed json to stdout<br>

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
      "some boolean": true,
      "some null value": null,
      "some integer": 8
    })_json"};

    Lexer lexer{};
    Parser<Lexer> parser{};
    auto [json, error] = parser.parse(jsonExample);
    if(notEmpty(error))
        std::cout << error << std::endl;
    else
    {
        json.print(); // print the parsed json to stdout
    }
    return 0;
}
```

### How to run?

Just add one of the examples above into a file called `main.cpp`.<br>
You can compile with GCC (g++) compiler and run with the following commands:<br>
```
g++ -Wall -g -std=c++17 main.cpp
./a.out
```

### How to run unittests?

Every tests of a class is separated into an own file inside the tests folder.<br>
Feel free to add more unittests to an existing test file or extend the test collection with new files for other classes of the library.<br>
If you want to new tests for a class just add a new file called `Test<class_name>.cpp` inside the `tests` folder and define the unittests there.<br>
Simply include the new unittest file inside the `runalltests.cpp` file.<br>

You can compile and run the tests with the following commands:<br>
```
g++ -o run_tests -I/home/nea/Cpp/JsonLib/src/ -std=c++17 -Wall -g -pthread runalltests.cpp -lgtest
./run_tests
```

If you do not have installed the Gtest unittest library than you can do it like the following on Ubuntu:<br>
```
sudo apt update
sudo apt install libgtest-dev
sudo apt install cmake
cd /usr/src/gtest
sudo cmake CMakeLists.txt
sudo make
sudo cp lib/*.a /usr/lib
```

After the installation you can build and execute the unittests.