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

  std::cout << "All columns read from YAML are: ";
  std::cout << '[';
  for (auto &col : new_columns_from_yaml)
    std::cout << col << ", ";
  std::cout << ']' << std::endl;

  loaded_db.exec("CREATE TABLE IF NOT EXISTS todo (id INTEGER PRIMARY KEY)");

  SQLite::Statement select_all_query(loaded_db, "SELECT * FROM todo");

  for (int i = 0; i < select_all_query.getColumnCount(); ++i) {
    auto found = std::find(new_columns_from_yaml.begin(), new_columns_from_yaml.end(),
                           select_all_query.getColumnName(i));
    if (found != new_columns_from_yaml.end()) {
      std::cout << "Element " << *found << " already exists in the table."
                << std::endl;
      new_columns_from_yaml.erase(found);
    }
  }

  std::cout << "The columns from YAML which are new and must be inserted: ";
  std::cout << '[';
  for (auto &col : new_columns_from_yaml)
    std::cout << col << ", ";
  std::cout << ']' << std::endl;

  std::exit(0);

  return loaded_db;
}
} // namespace db_handler
