#include "shader-parser.hpp"
#include <cstddef>
#include <fstream>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>

using std::ifstream;
using std::optional;
using std::size_t;
using std::string;
using std::string_view;
using std::unordered_map;

optional<unordered_map<string, string>> parseShader(string_view filename) {
    // VEXING PARSE
    auto file = ifstream(string(filename));
    if (!file.is_open()) {
        return std::nullopt;
    }
    unordered_map<string, string> shaderMap;
    string currentShaderName;
    string currentShaderContents;
    string line;
    while (std::getline(file, line)) {
        constexpr string_view delimiter = "/* SHADER ";
        if (line.starts_with(delimiter)) {
            if (!currentShaderName.empty()) {
                shaderMap.insert({currentShaderName, currentShaderContents});
            }
            size_t startIndex = delimiter.size(), endIndex = startIndex;
            for (; endIndex < line.size(); endIndex++) {
                char ch = line[endIndex];
                if (ch == ' ' || ch == '*') {
                    break;
                }
            }
            currentShaderName = line.substr(startIndex, endIndex - startIndex);
            currentShaderContents.clear();
        }
        if (!currentShaderName.empty()) {
            currentShaderContents.append(line + '\n');
        }
    }
    if (!currentShaderName.empty()) {
        shaderMap.insert({currentShaderName, currentShaderContents});
    }
    return shaderMap;
}
