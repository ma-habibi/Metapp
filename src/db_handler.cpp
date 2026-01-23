#include "db_handler.h"

#include <iostream>

namespace db_handler {
SQLite::Database DbHandler::update_db(const std::filesystem::path &db_path) {
  try {
    SQLite::Database database(db_path);
    std::cout << "Successfully loaded database '" << db_path << "'."
              << std::endl;
    return database;
  } catch (std::exception &e) {
    std::cerr << e.what() << " path : " << db_path << std::endl;
    std::exit(1);
  }
}
} // namespace db_handler
