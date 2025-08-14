#pragma once
#include <string>
#include <algorithm>
#include <vector>
#include <sstream>

#if defined(_WIN32)
#undef min
#undef max
#endif

namespace stdutil {
    static inline std::string to_upper(const std::string& inStr)
    {
        std::string str = inStr;
        std::transform(str.begin(), str.end(), str.begin(),
            [](unsigned char c) { return static_cast<char>(std::toupper(c)); });
        return str;
    }

	static inline std::string to_lower(const std::string& inStr)
	{
		std::string str = inStr;
		std::transform(str.begin(), str.end(), str.begin(),
			[](unsigned char c) { return static_cast<char>(std::tolower(c)); });
		return str;
	}

    static inline std::string replace_str(const std::string& str, const std::string& from, const std::string& to)
    {
        if (from.empty())
            return str;

        std::string result;
        result.reserve(str.size());

        size_t pos = 0, found;
        while ((found = str.find(from, pos)) != std::string::npos)
        {
            result.append(str, pos, found - pos);
            result.append(to);
            pos = found + from.size();
        }

        result.append(str, pos);
        return result;
    }

    static inline std::string truncate_str(const std::string& str, size_t start, size_t end)
    {
        if (start >= str.size() || start >= end)
            return {};
        return str.substr(start, std::min(end, str.size()) - start);
    }

    static inline std::string insert_str(const std::string& str, const std::string& insert, size_t index) noexcept
    {
        auto result = str;
        result.insert(std::min(index, str.size()), insert);
        return result;
    }

    static inline std::string trim_str(const std::string& str)
    {
        auto start = str.find_first_not_of(" \t");
        auto end = str.find_last_not_of(" \t");
        return (start == std::string::npos) ? "" : str.substr(start, end - start + 1);
    }

	static inline std::string trim_str_start(const std::string& str)
	{
		auto start = str.find_first_not_of(" \t");
		return (start == std::string::npos) ? "" : str.substr(start);
	}

	static inline std::string trim_str_end(const std::string& str)
	{
		auto end = str.find_last_not_of(" \t");
		return (end == std::string::npos) ? "" : str.substr(0, end + 1);
	}

	static inline std::string pad_left(const std::string& str, size_t width, char padChar = ' ')
	{
		if (str.length() >= width)
			return str;
		return std::string(width - str.length(), padChar) + str;
	}

	static inline std::string pad_right(const std::string& str, size_t width, char padChar = ' ')
	{
		if (str.length() >= width)
			return str;
		return str + std::string(width - str.length(), padChar);
	}

    static inline std::string str_join(std::vector<std::string> strings, const std::string& delimiter)
    {
        if (strings.empty())
            return "";

        std::ostringstream oss;
        auto it = strings.begin();
        oss << *it++;
        for (; it != strings.end(); ++it)
        {
            oss << delimiter << *it;
        }
        return oss.str();
    }

    static inline std::vector<std::string> split_str(const std::string& str, const std::string& delimiter)
    {
        std::vector<std::string> tokens;
        if (delimiter.empty())
        {
            tokens.push_back(str);
            return tokens;
        }

        size_t start = 0;
        size_t end;
        while ((end = str.find(delimiter, start)) != std::string::npos)
        {
            tokens.push_back(str.substr(start, end - start));
            start = end + delimiter.size();
        }
        tokens.push_back(str.substr(start));
        return tokens;
    }

    static inline bool contains_str(const std::string& str, const std::string& value)
    {
        return !value.empty() && str.find(value) != std::string::npos;
    }

    static inline std::string string_empty() { return ""; }
	static inline bool is_empty_or_whitespace(const std::string& str)
	{
		return str.empty() || std::all_of(str.begin(), str.end(), [](unsigned char c) { return std::isspace(c); });
	}
}