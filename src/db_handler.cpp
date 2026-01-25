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

  /*
   *impl std::bool column_exists(db, std::string tab, std::string col)
   *  using SELECT count(*) FROM pragma_table_info(tab) WHERE name=col;
   *
   * for yml_section = node(all)
   *  if table exist
   *    log
   *  else
   *    insert table
   *    log
   *
   *  for yml_task  = node[yml_section]
   *    if column_exists
   *      log
   *      INSERT to update value
   *    else
   *      log
   *      INSERT to initiate the value
   */

  return loaded_db;
}
} // namespace db_handler
