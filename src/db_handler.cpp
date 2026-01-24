#include "db_handler.h"

#include <SQLiteCpp/SQLiteCpp.h>

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

SQLite::Database DbHandler::update_db(const std::filesystem::path &db_path) {
  SQLite::Database loaded_db = load_db(db_path);
  return loaded_db;
}
} // namespace db_handler
