#include "db_handler.h"

#include <SQLiteCpp/SQLiteCpp.h>
#include <yaml-cpp/yaml.h>

#include <algorithm>
#include <iostream>
#include <list>
#include <sstream>

namespace db_handler {

std::ostream &operator<<(std::ostream &stream, std::list<std::string> l) {
  stream << "[";
  auto last = l.back();
  for (auto it = l.begin(); it != l.end(); ++it) {
    stream << *it;
    if (*it != last) {
      stream << ", ";
    }
  }
  stream << "]";
  return stream;
}

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

SQLite::Database DbHandler::update_db(const std::filesystem::path &db_path,
                                      const YAML::Node &todo_node) {
  std::list<std::string> tables_from_yaml;
  for (auto it = todo_node.begin(); it != todo_node.end(); ++it) {
    try {
      tables_from_yaml.push_back(it->first.as<std::string>());
    } catch (std::exception &e) {
      std::cerr << e.what() << std::endl;
    }
  }

  std::cout << "All tables read from YAML are: " << tables_from_yaml << "."
            << std::endl;

  SQLite::Database loaded_db = load_db(db_path);

  // Filter out to obtain non-existing tables
  std::list<std::string> new_tables_from_yaml(tables_from_yaml);
  SQLite::Statement select_tables_query(
      loaded_db,
      "SELECT name FROM sqlite_master WHERE type='table' ORDER BY name;");
  while (select_tables_query.executeStep()) {
    auto search = select_tables_query.getColumn(0).getString();
    auto found = std::find(new_tables_from_yaml.begin(),
                           new_tables_from_yaml.end(), search);
    if (found != new_tables_from_yaml.end()) {
      std::cout << "Table " << *found << " already exists in the table."
                << std::endl;
      new_tables_from_yaml.erase(found);
    }
  }
  std::cout << "the tables from yaml which are new and must be inserted: "
            << new_tables_from_yaml << "." << std::endl;

  for (auto &table : new_tables_from_yaml) {
    try {
      std::cout << "Creating table " << table << "." << std::endl;
      ;
      std::stringstream cmd_stream;
      cmd_stream << "CREATE TABLE IF NOT EXISTS " << table
                 << "(id INTEGER PRIMARY KEY)";
      std::string cmd = cmd_stream.str();
      loaded_db.exec(cmd);
    } catch (std::exception &e) {
      std::cerr << e.what() << std::endl;
    }
  }
  return loaded_db;
}
} // namespace db_handler
