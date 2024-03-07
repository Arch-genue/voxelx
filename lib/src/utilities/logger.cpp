#include "logger.h"

LOGLEVEL Logger::_loglevel = LOGLEVEL::DEBUG;

void Logger::setLogLevel(LOGLEVEL loglevel) {
    _loglevel = loglevel;
}

void Logger::eprint(std::string subject, std::string msg, LOGLEVEL type) {
    if (_loglevel != LOGLEVEL::DEBUG && type != LOGLEVEL::ALL) {
        if (_loglevel == LOGLEVEL::SUCCESS && type != LOGLEVEL::SUCCESS) {
            return;
        } else if (_loglevel == LOGLEVEL::INFO && (type != LOGLEVEL::INFO && type != LOGLEVEL::SUCCESS)) {
            return;
        } else if (_loglevel == LOGLEVEL::WARNING && (type != LOGLEVEL::WARNING && type != LOGLEVEL::ERROR)) {
            return;
        } else if (_loglevel == LOGLEVEL::ERROR && type != LOGLEVEL::ERROR) {
            return;
        }
    }

    std::string msgtype;
    std::string clr;
    std::string spacer;
    switch(type) {
        case LOGLEVEL::ERROR:
            msgtype = "ERROR";
            spacer = "    ";
            clr = BRIGHT_RED_COLOR;
            break;
        case LOGLEVEL::WARNING:
            msgtype = "WARNING";
            spacer = "  ";
            clr = BRIGHT_YELLOW_COLOR;
            break;
        case LOGLEVEL::INFO:
            msgtype = "INFO";
            spacer = "     ";
            clr = BRIGHT_CYAN_COLOR;
            break;
        case LOGLEVEL::SUCCESS:
            msgtype = "SUCCESS";
            spacer = "  ";
            clr = BRIGHT_GREEN_COLOR;
            break;
        case LOGLEVEL::DEBUG:
        case LOGLEVEL::ALL:
            msgtype = "DEBUG";
            spacer = "  ";
            clr = BRIGHT_WHITE_COLOR;
            break;
    }
    std::string text = clr + std::string("[" + msgtype + "]") + spacer + BRIGHT_WHITE_COLOR + std::string("[" + subject + "]  ") + RESET_COLOR + msg + "\n";
    printf(text.c_str());
}