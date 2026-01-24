#ifndef DB_HANDLER_H
#define DB_HANDLER_H

#include <SQLiteCpp/SQLiteCpp.h>
#include <yaml-cpp/yaml.h>

namespace db_handler {
class DbHandler {
private:
  static SQLite::Database load_db(const std::filesystem::path &db_path);

public:
  static SQLite::Database update_db(const std::filesystem::path &db_path, const YAML::Node &todo_node);
};
} // namespace db_handler

#endif // DB_HANDLER_H
