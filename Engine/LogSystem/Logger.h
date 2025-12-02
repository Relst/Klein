//
// Created by Karanbir Singh on 24/11/2025.
//

#ifndef LOGGER_H
#define LOGGER_H

#include <cstdio>
#include <ctime>


#ifdef _WIN32
#define LOG_COLOR_RESET        2
#define LOG_COLOR_APP_WARN     6
#define LOG_COLOR_APP_ERROR    4
#define LOG_COLOR_ENGINE_LOG   1
#define LOG_COLOR_ENGINE_WARN  3
#define LOG_COLOR_ENGINE_ERROR 5
#else
#define LOG_COLOR_RESET        "\x1b[92m" // Green
#define LOG_COLOR_APP_WARN     "\x1b[93m" // Yellow
#define LOG_COLOR_APP_ERROR    "\x1b[91m" // Red
#define LOG_COLOR_ENGINE_LOG   "\x1b[94m" //
#define LOG_COLOR_ENGINE_WARN  "\x1b[96m" //
#define LOG_COLOR_ENGINE_ERROR "\x1b[95m" //
#endif

namespace KleinLogger
{
    class Logger
    {
    public:
        template <typename... Args>
        static void Log(const char* msg, Args... args)
        {
            Log(LOG_COLOR_RESET, false, "LOG", msg, args...);
        }

        template <typename... Args>
        static void Warn(const char* msg, Args... args)
        {
            Log(LOG_COLOR_APP_WARN, false, "WARN", msg, args...);
        }

        template <typename... Args>
        static void Error(const char* msg, Args... args)
        {
            Log(LOG_COLOR_APP_ERROR, false, "ERROR", msg, args...);
        }

        // Engine
        template <typename... Args>
        static void EngineLog(const char* msg, Args... args)
        {
            Log(LOG_COLOR_ENGINE_LOG, true, "Log", msg, args...);
        }

        template <typename... Args>
        static void EngineWarn(const char* msg, Args... args)
        {
            Log(LOG_COLOR_ENGINE_WARN, true, "WARN", msg, args...);
        }

        template <typename... Args>
        static void EngineError(const char* msg, Args... args)
        {
            Log(LOG_COLOR_ENGINE_ERROR, true, "ERROR", msg, args...);
        }
    private:
        template <typename... Args>
        #ifdef _WIN32
        static inline void Log(int colorType, bool engine, const char* pre, const char* msg, Args... args)
        #else
        static inline void Log(const char* colorType, bool engine, const char* pre, const char* msg, Args... args)
        #endif
        {
            std::time_t t = std::time(nullptr);
            std::tm now;

            #ifdef _WIN32
            localtime_s(&now, &t);
            #else
            localtime_r(&t, &now);
            #endif

            #ifdef _WIN32
            switch (colorType)
            {
            case 0:
                SetColor(15, 0);
                break;
            case 1:
                SetColor(14, 0);
                break;
            case 2:
                SetColor(12, 0);
                break;
            case 3:
                SetColor(2, 0);
                break;
            case 4:
                SetColor(6, 0);
                break;
            case 5:
                SetColor(0, 12);
                break;
            }
            printf("[%02d:%02d:%02d %s %s] ", now.tm_hour, now.tm_min, now.tm_sec, (engine ? "Engine" : "App"), pre);
            printf(msg, args...);
            SetColor(15, 0);
            printf("\n");
            #else
            printf("%s[%02d:%02d:%02d %s %s] ", colorType, now.tm_hour, now.tm_min, now.tm_sec, (engine ? "Engine" : "App"), pre);
            printf(msg, args...);
            printf(LOG_COLOR_RESET "\n");
            #endif
        }

        #ifdef _WIN32
        static void SetColor(int textColor, int bgColor);
        #endif
    };
}



#endif //LOGGER_H
