#ifndef _ANSI__H_
#define _ANSI__H_

using namespace std::string_literals;

// styles
#define ESC_RESET "\033[0m"s
#define ESC_BOLD "\033[1m"s
#define ESC_UNDERLINE "\033[4m"s
#define ESC_BLINK "\033[5m"s
#define ESC_INVERSE "\033[7m"s

// fg colors
#define ESC_BLACK "\033[30m"s
#define ESC_RED "\033[31m"s
#define ESC_GREEN "\033[32m"s
#define ESC_YELLOW "\033[33m"s
#define ESC_BLUE "\033[34m"s
#define ESC_MAGENTA "\033[35m"s
#define ESC_CYAN "\033[36m"s
#define ESC_WHITE "\033[37m"s

// bg colors
#define ESC_BLACK_BG "\033[40m"s
#define ESC_RED_BG "\033[41m"s
#define ESC_GREEN_BG "\033[42m"s
#define ESC_YELLOW_BG "\033[43m"s
#define ESC_BLUE_BG "\033[44m"s
#define ESC_MAGENTA_BG "\033[45m"s
#define ESC_CYAN_BG "\033[46m"s
#define ESC_WHITE_BG "\033[47m"s

#endif //_ANSI__H_
