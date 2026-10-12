#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>

// Makes a hashmap based on delimiting comments (e.g. "/* SHADER vertex */")
// creates a key "vertex" and a value of its contents.
std::optional<std::unordered_map<std::string, std::string>>
parseShader(std::string_view filename);
