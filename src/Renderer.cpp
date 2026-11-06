#include "Renderer.hpp"
#include <cstddef>
#include <iostream>
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

optional<unordered_map<string, string>> parseShader(string_view shaderSrc) {
    unordered_map<string, string> shaderMap;
    string currentShaderName;
    string currentShaderContents;
    string_view line;
    while (!shaderSrc.empty()) {
        auto pos = shaderSrc.find('\n');
        constexpr string_view delimiter{"/* SHADER "};
        line = shaderSrc.substr(0, pos);
        if (line.starts_with(delimiter)) {
            if (!currentShaderName.empty()) {
                shaderMap.insert({currentShaderName, currentShaderContents});
            }
            size_t startIndex{delimiter.size()}, endIndex{startIndex};
            for (; endIndex < line.size(); endIndex++) {
                char ch{line[endIndex]};
                if (ch == ' ' || ch == '*') {
                    break;
                }
            }
            currentShaderName = line.substr(startIndex, endIndex - startIndex);
            currentShaderContents.clear();
        }
        if (!currentShaderName.empty()) {
            currentShaderContents.append(line);
        }
        if (pos == string_view::npos)
            break;
        shaderSrc.remove_prefix(pos + 1);
    }
    if (!currentShaderName.empty()) {
        shaderMap.insert({currentShaderName, currentShaderContents});
    }
    return shaderMap;
}
