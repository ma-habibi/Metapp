#include "parse_todo.h"
#include "db_handler.h"

#include <argparse/argparse.hpp>
#include <yaml-cpp/yaml.h>
#include <SQLiteCpp/SQLiteCpp.h>

#include <filesystem>
#include <iostream>

/**
 * Parse the CLI arguments.
 * @param program The reference to program object, that builds CLI arguments
 * within itself.
 * @param argc The argument count of the entry point.
 * @param argv The vector of arguments from the entry point.
 */
void parse_arguments(argparse::ArgumentParser &program, int argc,
                     char *argv[]) {
  std::string cwd = std::filesystem::current_path();
  program.add_argument("-t", "--todo")
      .default_value(std::string(cwd.append("/resources/todo.yaml")));
  program.add_argument("-d", "--database")
      .default_value(std::string(cwd.append("/resources/example.db3")));
  try {
    program.parse_args(argc, argv);
  } catch (const std::exception &err) {
    std::cerr << err.what() << std::endl;
    std::exit(1);
  }
}

int main(int argc, char *argv[]) {
  argparse::ArgumentParser program(argv[0]);
  parse_arguments(program, argc, argv);
  std::filesystem::path todo_path{program.get<std::string>("todo")};
  YAML::Node todo_node = parse_todo::ParseTodo::read(todo_path);

  try {
    SQLite::Database updated_db = db_handler::DbHandler::update_db(program.get<std::string>("database"));
    SQLite::Statement query(updated_db, "SELECT * FROM test");
  }
  catch (std::exception &e) {
    std::cerr << e.what();
    return 1;
  }

  return 0;
}
