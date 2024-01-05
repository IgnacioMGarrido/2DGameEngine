#pragma once
#include <cstdio>
#include <widemath.h>

namespace 
{
enum class TextColor : uint8_t
{
	TEXT_COLOR_BLACK,
	TEXT_COLOR_RED,
	TEXT_COLOR_GREEN,
	TEXT_COLOR_YELLOW,
	TEXT_COLOR_BLUE,
	TEXT_COLOR_MAGENTA,
	TEXT_COLOR_CYAN,
	TEXT_COLOR_WHITE,
	TEXT_COLOR_BRIGHT_BLACK,
	TEXT_COLOR_BRIGHT_GREEN,
	TEXT_COLOR_BRIGHT_RED,
	TEXT_COLOR_BRIGHT_YELLOW,
	TEXT_COLOR_BRIGHT_BLUE,
	TEXT_COLOR_BRIGHT_MAGENTA,
	TEXT_COLOR_BRIGHT_CYAN,
	TEXT_COLOR_BRIGHT_WHITE,
	TEXT_COLOR_COUNT
};
template<typename ...Args>
void _log(const char* i_prefix, const char* i_message, TextColor i_textColor, Args... args)
{
	static const char* s_textColorTable[(uint8_t)TextColor::TEXT_COLOR_COUNT] =
	{
		"\x1b[30m", //TEXT_COLOR_BLACK
		"\x1b[31m", //TEXT_COLOR_RED,
		"\x1b[32m", //TEXT_COLOR_GREEN,
		"\x1b[33m", //TEXT_COLOR_YELLOW,
		"\x1b[34m", //TEXT_COLOR_BLUE,
		"\x1b[35m", //TEXT_COLOR_MAGENTA,
		"\x1b[36m", //TEXT_COLOR_CYAN,
		"\x1b[37m", //TEXT_COLOR_WHITE,
		"\x1b[90m", //TEXT_COLOR_BRIGHT_BLACK,
		"\x1b[91m", //TEXT_COLOR_BRIGHT_GREEN,
		"\x1b[92m", //TEXT_COLOR_BRIGHT_RED,
		"\x1b[93m", //TEXT_COLOR_BRIGHT_YELLOW,
		"\x1b[94m", //TEXT_COLOR_BRIGHT_BLUE,
		"\x1b[95m", //TEXT_COLOR_BRIGHT_MAGENTA,
		"\x1b[96m", //TEXT_COLOR_BRIGHT_CYAN,
		"\x1b[97m" //TEXT_COLOR_BRIGHT_WHITE,
	};

	char formatBuffer[8192] = {};
	sprintf(formatBuffer, "%s %s\t %s \033[0m", s_textColorTable[(uint8_t)i_textColor], i_prefix, i_message);

	char textBuffer[8912] = {};
	sprintf(textBuffer, formatBuffer, args...);

	puts(textBuffer);
}
} //namespace

#ifdef DEBUG
#define LOG_TRACE(msg, ...) _log("TRACE: ", msg, TextColor::TEXT_COLOR_GREEN, ##__VA_ARGS__)
#define LOG_WARNING(msg, ...) _log("WARNING: ", msg, TextColor::TEXT_COLOR_YELLOW, ##__VA_ARGS__)
#define LOG_ERROR(msg, ...) _log("ERROR: ", msg, TextColor::TEXT_COLOR_RED, ##__VA_ARGS__)
#else
#define LOG_TRACE(msg, ...) do{}while(false)
#define LOG_WARNING(msg, ...) do{}while(false)
#define LOG_ERROR(msg, ...) do{}while(false)
#endif

