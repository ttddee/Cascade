/*
 *  Cascade Image Editor
 *
 *  Copyright (C) 2020 The Cascade developers
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

        void debug(const QString& s);
        void info(const QString& s);
        void warning(const QString& s);
        void critical(const QString& s);
        void fatal(const QString& s);

        void console(const QString& s);

    private:
        void writeToFile(const QString& s);

        static std::shared_ptr<Log> logger;

        static QFile outFile;
    };
}

#define CS_LOG_DEBUG(...)     ::Cascade::Log::getLogger()->debug(__VA_ARGS__);
#define CS_LOG_INFO(...)      ::Cascade::Log::getLogger()->info(__VA_ARGS__);
#define CS_LOG_WARNING(...)   ::Cascade::Log::getLogger()->warning(__VA_ARGS__);
#define CS_LOG_CRITICAL(...)  ::Cascade::Log::getLogger()->critical(__VA_ARGS__);
#define CS_LOG_FATAL(...)     ::Cascade::Log::getLogger()->fatal(__VA_ARGS__);

#define CS_LOG_CONSOLE(...)   ::Cascade::Log::getLogger()->console(__VA_ARGS__);


#endif // LOG_H
