#include "parse_todo.h"

#include <yaml-cpp/yaml.h>

#include <fstream>
#include <iostream>

namespace parse_todo {
void ParseTodo::purge(const std::filesystem::path &path) {
  YAML::Node todo_node = YAML::LoadFile(path.string());
  YAML::Emitter out;
  out << YAML::BeginMap;

  for (auto todo_section_it = todo_node.begin();
       todo_section_it != todo_node.end(); ++todo_section_it) {
    auto todo_section_name = todo_section_it->first.as<std::string>();

    out << YAML::Key << todo_section_name;
    out << YAML::Value << YAML::BeginMap;
    for (auto todo_task_it = todo_node[todo_section_name].begin();
         todo_task_it != todo_node[todo_section_name].end(); ++todo_task_it) {
      auto todo_task_name = todo_task_it->first.as<std::string>();
      auto todo_task_value = todo_task_it->second.as<std::string>();
      out << YAML::Key << todo_task_name << YAML::Value << "0.0";
    }
    out << YAML::EndMap;
  }
  out << YAML::EndMap;

  std::cout << "Purged '" << path.c_str() << "' successfully." << std::endl;
  std::ofstream ofs(path, std::ofstream::trunc);
  ofs << out.c_str();
  ofs.close();
}

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
