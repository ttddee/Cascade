/*
 *  Cascade Image Editor
 *
 *  Copyright (C) 2022 Till Dechent and contributors
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef LOG_H
#define LOG_H

#include <memory>

#include <QString>
#include <QFile>
#include <QTextStream>

namespace Cascade
{
    class Log
    {
    public:
        static void Init();

        static std::shared_ptr<Log>& getLogger()
        {
            return logger;
        }

        static void messageHandler(QtMsgType type, const QMessageLogContext& context, const QString & msg);

        static void debug(const QString& s);
        static void info(const QString& s);
        static void warning(const QString& s);
        static void critical(const QString& s);
        static void fatal(const QString& s);

        static void console(const QString& s);

    private:
        static void writeToFile(const QString& s);

        static std::shared_ptr<Log> logger;

        static QFile outFile;
        static QTextStream stream;
    };
}

#define CS_LOG_DEBUG(...)     ::Cascade::Log::getLogger()->debug(__VA_ARGS__);
#define CS_LOG_INFO(...)      ::Cascade::Log::getLogger()->info(__VA_ARGS__);
#define CS_LOG_WARNING(...)   ::Cascade::Log::getLogger()->warning(__VA_ARGS__);
#define CS_LOG_CRITICAL(...)  ::Cascade::Log::getLogger()->critical(__VA_ARGS__);
#define CS_LOG_FATAL(...)     ::Cascade::Log::getLogger()->fatal(__VA_ARGS__);

#define CS_LOG_CONSOLE(...)   ::Cascade::Log::getLogger()->console(__VA_ARGS__);


#endif // LOG_H
