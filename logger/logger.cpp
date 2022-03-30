//
// Created by zyx on 2021/7/3.
//

#include "logger.h"
#include <QDebug>
#include <string>

std::string LOG::LogLevel::toString(LogLevel::LOG_LEVEL level) {
    switch (level) {
        default:
            return "UNKNOWN";
        case DEBUG:
            return "DEBUG";
        case INFO:
            return "INFO";
        case WARN:
            return "WARN";
        case FATAL:
            return "FATAL";
    }
}

LOG::logger::logger(LOG::LogLevel::LOG_LEVEL level, std::string content) {
    qDebug() << QString::fromStdString("[" + m_level_trans.toString(level) + "]: " + content);
}

LOG::logger::logger(LOG::LogLevel::LOG_LEVEL level, QString content, bool mark) {
    qDebug() << QString::fromStdString("[" + m_level_trans.toString(level) + "]: ") + content;
}



