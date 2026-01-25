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
  SQLite::Database loaded_db = load_db(db_path);

  for (auto it = todo_node.begin(); it != todo_node.end(); ++it) {
    auto yaml_section = it->first.as<std::string>();
    if (loaded_db.tableExists(yaml_section)) {
      std::cout << "Table " << yaml_section
                << " already exists in the database." << std::endl;
    } else {
      std::cout << "Creating new table " << yaml_section << "." << std::endl;
      std::string cmd =
          "CREATE TABLE " + yaml_section + "(id INTEGER PRIMARY KEY)";
      loaded_db.exec(cmd);
    }
  }
  return loaded_db;
}
} // namespace db_handler
