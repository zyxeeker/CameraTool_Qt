//
// Created by zyx on 2021/7/3.
//

#ifndef TEST_LOGGER_H
#define TEST_LOGGER_H

#include "param.h"
#include <string>

namespace LOG {
    struct LogLevel {
        enum LOG_LEVEL {
            UNKNOWN,
            DEBUG,
            INFO,
            WARN,
            ERROR,
            FATAL
        };

        std::string toString(LOG::LogLevel::LOG_LEVEL);
    };

    class logger {
    public:
        logger(LOG::LogLevel::LOG_LEVEL, std::string content);

        logger(LOG::LogLevel::LOG_LEVEL, QString content, bool mark);

    private:
        LOG::LogLevel m_level_trans;
    };
}

#endif //TEST_LOGGER_H
