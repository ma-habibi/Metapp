#include "db_handler.h"

#include <SQLiteCpp/SQLiteCpp.h>
#include <yaml-cpp/yaml.h>

#include <algorithm>
#include <iostream>
#include <list>
#include <sstream>

namespace db_handler {

SQLite::Database DbHandler::load_db(const std::filesystem::path &db_path) {
  try {
    SQLite::Database database(db_path,
                              SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
    std::cout << "Successfully loaded database '" << db_path << "'."
              << std::endl;
    return database;
  } catch (std::exception &e) {
    std::cerr << e.what() << " path : " << db_path << std::endl;
    std::exit(1);
  }
}

SQLite::Database DbHandler::update_db(const std::filesystem::path &db_path, const YAML::Node &todo_node) {
  SQLite::Database loaded_db = load_db(db_path);

  std::list<std::string> new_columns_from_yaml;
  for (auto it = todo_node.begin(); it != todo_node.end(); ++it) {
    try {
      new_columns_from_yaml.push_back(it->first.as<std::string>());
    }
    catch (std::exception &e) {
      std::cerr << e.what() << std::endl;
    }
  }

  std::cout << "New columns from YAML are: ";
  std::cout << '[';
  for (auto &col : new_columns_from_yaml)
    std::cout << col << ", ";
  std::cout << ']' << std::endl;
  return loaded_db;
}
} // namespace db_handler
