#include "parse_todo.h"

#include <argparse/argparse.hpp>
#include <yaml-cpp/yaml.h>

#include <filesystem>
#include <iostream>

void parse_arguments(argparse::ArgumentParser &program, int argc, char *argv[]) {
  std::string cwd = std::filesystem::current_path();
  program.add_argument("-t", "--todo")
  .default_value(std::string(cwd.append("/resources/hello_world.yaml")));
  program.add_argument("-d", "--database")
  .default_value(std::string(cwd.append("/resources/database.db")));
  try {
    program.parse_args(argc, argv);
  }
  catch (const std::exception& err) {
    std::cerr << err.what() << std::endl;
    std::exit(1);
  }
}

int main(int argc, char *argv[]) {
  argparse::ArgumentParser program(argv[0]);
  parse_arguments(program, argc, argv);
  std::filesystem::path todo_path{program.get<std::string>("todo")};

  YAML::Node todo_node = parse_todo::ParseTodo::read(todo_path);

  return 0;
}
