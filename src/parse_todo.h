#ifndef PARSE_TODO_H
#define PARSE_TODO_H

#include <yaml-cpp/yaml.h>

#include <filesystem>

namespace parse_todo {
class ParseTodo {
public:
  static YAML::Node read(const std::filesystem::path& path);
};
} // parse_todo

#endif //PARSE_TODO_H
