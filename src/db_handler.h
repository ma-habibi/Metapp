#ifndef DB_HANDLER_H
#define DB_HANDLER_H

#include <SQLiteCpp/SQLiteCpp.h>

namespace db_handler {
class DbHandler {
public:
  static SQLite::Database update_db(const std::filesystem::path& db_path);
};
} // db_handler

#endif //DB_HANDLER_H
