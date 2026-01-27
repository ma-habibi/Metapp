#include "db_handler.h"

#include <SQLiteCpp/SQLiteCpp.h>
#include <yaml-cpp/yaml.h>

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

bool DbHandler::column_exists(const SQLite::Database &db,
                              const std::string &table,
                              const std::string &column) {
  SQLite::Statement query(
      db, "SELECT count(*) FROM pragma_table_info(?) WHERE name=?");
  query.bind(1, table);
  query.bind(2, column);
  (void)query.executeStep();
  return (1 == query.getColumn(0).getInt());
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

  for (auto yaml_section_it = todo_node.begin();
       yaml_section_it != todo_node.end(); ++yaml_section_it) {
    auto yaml_section_name = yaml_section_it->first.as<std::string>();
    if (loaded_db.tableExists(yaml_section_name)) {
      std::cout << "Table " << yaml_section_name
                << " already exists in the database." << std::endl;
    } else {
      std::cout << "Creating new table " << yaml_section_name << "."
                << std::endl;
      std::string cmd =
          "CREATE TABLE " + yaml_section_name + "(id INTEGER PRIMARY KEY)";
      loaded_db.exec(cmd);
    }
    for (auto yaml_task_it = todo_node[yaml_section_name].begin();
         yaml_task_it != todo_node[yaml_section_name].end(); ++yaml_task_it) {
      auto yaml_task_name = yaml_task_it->first.as<std::string>();
      auto yaml_task_value = yaml_task_it->second.as<std::string>();
      if (DbHandler::column_exists(loaded_db, yaml_section_name,
                                   yaml_task_name)) {
        std::cout << "Found column '" << yaml_task_name << "' in the table."
                  << std::endl;
      } else {
        std::cout << "Creating new column '" << yaml_task_name
                  << "' in the table." << std::endl;
        loaded_db.exec("ALTER TABLE " + yaml_section_name + " ADD COLUMN " +
                       yaml_task_name);
      }
      std::cout << "Inserting value: '" << yaml_task_value
                << "' into the column '" << yaml_task_name << "'." << std::endl;
      loaded_db.exec("INSERT INTO " + yaml_section_name + "(" + yaml_task_name +
                     ")" + "VALUES(" + yaml_task_value + ")");
    }
  }

  return loaded_db;
}
} // namespace db_handler
