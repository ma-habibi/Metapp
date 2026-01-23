#include "parse_todo.h"

#include <yaml-cpp/yaml.h>

#include <iostream>

namespace parse_todo {
YAML::Node ParseTodo::read(const std::filesystem::path &path) {
  try {
    YAML::Node todo_node = YAML::LoadFile(path.string());
    return todo_node;
  } catch (YAML::BadFile &e) {
    std::cerr << e.what() << " path: " << path.string();
    std::exit(1);
  }
}
} // namespace parse_todo
