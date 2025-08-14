#pragma once
#include <iostream>
#include <string>
#include <format>
#include <cstdarg>
namespace raylib {
#include "raylib/raylib.h"
}

class ConsoleColor {
public:
    static constexpr const char* ResetCode = "\033[0m";
    static constexpr const char* BoldCode = "\033[1m";

    static constexpr const char* Red = "\033[31m";
    static constexpr const char* Green = "\033[32m";
    static constexpr const char* Yellow = "\033[33m";
    static constexpr const char* Blue = "\033[34m";
    static constexpr const char* Magenta = "\033[35m";
    static constexpr const char* Cyan = "\033[36m";
    static constexpr const char* White = "\033[37m";
};


// Base class for tags
class Colorizer
{
public:
    static size_t VisibleLength(const std::string& s)
    {
        size_t len = 0;
        for (size_t i = 0; i < s.size();)
        {
            if (s[i] == '\033' && i + 1 < s.size() && s[i + 1] == '[')
            {
                // Skip until a letter ends the ANSI code
                i += 2;
                while (i < s.size() && !std::isalpha(static_cast<unsigned char>(s[i])))
                    ++i;
                if (i < s.size()) ++i; // skip the final letter
            }
            else
            {
                ++len;
                ++i;
            }
        }
        return len;
    }


    // Bold variant of any color
    static std::string ColorBold(const std::string& text, const char* color)
    {
        return std::string(ConsoleColor::BoldCode) + std::string(color) + text + ConsoleColor::ResetCode;
    }

    // Color wrapper
    static std::string Color(const std::string& text, const char* color)
    {
        return std::string(color) + text + ConsoleColor::ResetCode;
    }
};

enum class LogLevel : uint32_t
{
    LOG_LEVEL_NONE = 0,
    LOG_LEVEL_DEBUG = 1 << 0,
    LOG_LEVEL_INFO = 1 << 1,
    LOG_LEVEL_WARNING = 1 << 2,
    LOG_LEVEL_ERROR = 1 << 3,
    LOG_LEVEL_FATAL = 1 << 4,

    LOG_LEVEL_1 = LOG_LEVEL_INFO | LOG_LEVEL_WARNING,
    LOG_LEVEL_2 = LOG_LEVEL_1 | LOG_LEVEL_ERROR,
    LOG_LEVEL_3 = LOG_LEVEL_2 | LOG_LEVEL_FATAL,
    LOG_LEVEL_ALL = LOG_LEVEL_DEBUG | LOG_LEVEL_3
};

// Bitwise operators for LogLevel
inline constexpr LogLevel operator|(LogLevel lhs, LogLevel rhs)
{
    return static_cast<LogLevel>(
        static_cast<uint32_t>(lhs) | static_cast<uint32_t>(rhs)
        );
}

inline constexpr LogLevel operator&(LogLevel lhs, LogLevel rhs)
{
    return static_cast<LogLevel>(
        static_cast<uint32_t>(lhs) & static_cast<uint32_t>(rhs)
        );
}

inline constexpr bool hasFlag(LogLevel mask, LogLevel flag)
{
    return (static_cast<uint32_t>(mask) & static_cast<uint32_t>(flag)) != 0;
}

// Define combinations
static constexpr LogLevel LOG_LEVEL_1 = LogLevel::LOG_LEVEL_INFO | LogLevel::LOG_LEVEL_WARNING;
static constexpr LogLevel LOG_LEVEL_2 = LOG_LEVEL_1 | LogLevel::LOG_LEVEL_ERROR;
static constexpr LogLevel LOG_LEVEL_3 = LOG_LEVEL_2 | LogLevel::LOG_LEVEL_FATAL;
static constexpr LogLevel LOG_LEVEL_ALL = LogLevel::LOG_LEVEL_DEBUG | LOG_LEVEL_3;

// TraceLog stays as a utility class
class TraceLog
{
private:
    TraceLog() = default; // Prevent instantiation
    static TraceLog& GetInstance()
    {
        static TraceLog instance; // Guaranteed to be destroyed, instantiated on first use
        return instance;
    }

    LogLevel current_log_level = LOG_LEVEL_3;

public:
    static constexpr LogLevel MapRaylibLogLevel(int raylibLevel)
    {
        switch (raylibLevel)
        {
        case raylib::LOG_INFO:    return LogLevel::LOG_LEVEL_INFO;
        case raylib::LOG_ERROR:   return LogLevel::LOG_LEVEL_ERROR;
		case raylib::LOG_FATAL:   return LogLevel::LOG_LEVEL_FATAL;
		case raylib::LOG_TRACE:   return LogLevel::LOG_LEVEL_DEBUG;
        case raylib::LOG_WARNING: return LogLevel::LOG_LEVEL_WARNING;
        case raylib::LOG_DEBUG:   return LogLevel::LOG_LEVEL_DEBUG;
        default:          return LogLevel::LOG_LEVEL_NONE;
        }
    }

    static inline void HookRaylibLog()
    {
		raylib::SetTraceLogCallback([](int logLevel, const char* text, va_list args) {
            va_list argsCopy;
            va_copy(argsCopy, args);
            int len = std::vsnprintf(nullptr, 0, text, argsCopy);
            va_end(argsCopy);

            if (len < 0) return; // formatting error

            std::string message(len, '\0');
            std::vsnprintf(message.data(), message.size() + 1, text, args);

            Log(MapRaylibLogLevel(logLevel), "[RAYLIB]", message);
			});
    }

    static void SetLogLevel(LogLevel level)
    {
        GetInstance().current_log_level = level;
    }

    static void Log(LogLevel level, const std::string& tagStr, const std::string& message)
    {
        if (!hasFlag(GetInstance().current_log_level, level))
            return;

        std::string prefix;
        switch (level)
        {
        case LogLevel::LOG_LEVEL_DEBUG:   prefix = Colorizer::Color("[DEBUG]", ConsoleColor::Cyan); break;
        case LogLevel::LOG_LEVEL_INFO:    prefix = Colorizer::Color("[INFO]", ConsoleColor::White); break;
        case LogLevel::LOG_LEVEL_WARNING: prefix = Colorizer::Color("[WARNING]", ConsoleColor::Yellow); break;
        case LogLevel::LOG_LEVEL_ERROR:   prefix = Colorizer::Color("[ERROR]", ConsoleColor::Magenta); break;
        case LogLevel::LOG_LEVEL_FATAL:   prefix = Colorizer::Color("[FATAL]", ConsoleColor::Red); break;
        }

        const int totalWidth = 18; // adjust to the longest [LEVEL][TAG] length

        std::string combined = prefix + tagStr;
        size_t visibleLen = Colorizer::VisibleLength(combined);

        std::string padded = combined + std::string((visibleLen < totalWidth) ? totalWidth - visibleLen : 0, ' ');

        std::cout << padded << message << std::endl;
    }

    template<typename... Args>
    static void Log(LogLevel level, const std::string& tagStr, std::string_view format, Args&&... args)
    {
        std::string message = std::vformat(format, std::make_format_args(args...));
        Log(level, tagStr, message);
    }
};

/// Define specialized tag loggers
class ServerLog
{
private:
    static constexpr const char* tag_name = "[SERVER]";
    static constexpr const char* tag_color = ConsoleColor::Blue;
public:
    template<typename... Args>
    static void Debug(std::string_view format, Args&&... args)
    {
        TraceLog::Log(LogLevel::LOG_LEVEL_DEBUG, Colorizer::Color(tag_name, tag_color), std::string_view(format), std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void Info(std::string_view format, Args&&... args)
    {
        TraceLog::Log(LogLevel::LOG_LEVEL_INFO, Colorizer::Color(tag_name, tag_color), std::string_view(format), std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void Warning(std::string_view format, Args&&... args)
    {
        TraceLog::Log(LogLevel::LOG_LEVEL_WARNING, Colorizer::Color(tag_name, tag_color), std::string_view(format), std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void Error(std::string_view format, Args&&... args)
    {
        TraceLog::Log(LogLevel::LOG_LEVEL_ERROR, Colorizer::Color(tag_name, tag_color), std::string_view(format), std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void Fatal(std::string_view format, Args&&... args)
    {
        TraceLog::Log(LogLevel::LOG_LEVEL_FATAL, Colorizer::Color(tag_name, tag_color), std::string_view(format), std::forward<Args>(args)...);
    }
};

class ClientLog
{
private:
    static constexpr const char* tag_name = "[CLIENT]";
    static constexpr const char* tag_color = ConsoleColor::Blue;
public:
    template<typename... Args>
    static void Debug(std::string_view format, Args&&... args)
    {
        TraceLog::Log(LogLevel::LOG_LEVEL_DEBUG, Colorizer::Color(tag_name, tag_color), std::string_view(format), std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void Info(std::string_view format, Args&&... args)
    {
        TraceLog::Log(LogLevel::LOG_LEVEL_INFO, Colorizer::Color(tag_name, tag_color), std::string_view(format), std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void Warning(std::string_view format, Args&&... args)
    {
        TraceLog::Log(LogLevel::LOG_LEVEL_WARNING, Colorizer::Color(tag_name, tag_color), std::string_view(format), std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void Error(std::string_view format, Args&&... args)
    {
        TraceLog::Log(LogLevel::LOG_LEVEL_ERROR, Colorizer::Color(tag_name, tag_color), std::string_view(format), std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void Fatal(std::string_view format, Args&&... args)
    {
        TraceLog::Log(LogLevel::LOG_LEVEL_FATAL, Colorizer::Color(tag_name, tag_color), std::string_view(format), std::forward<Args>(args)...);
    }
};