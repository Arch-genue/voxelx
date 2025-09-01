#include "utilities/logger.hpp"

class LoggerApi {
public:
    static void reg(sol::state& lua) {
        lua.create_named_table("Logger",
            "log", [](const std::string& msg) {
                Logger::instance().log(LogLevel::INFO, "Lua", msg);
            }
        );
    }
};