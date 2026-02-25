#pragma once

#include <filesystem>
#include <fmt/chrono.h>
#include <fmt/color.h>
#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/os.h>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

inline bool shouldLogColor = true;

#define COLOR_TIME         fmt::color::light_blue
#define COLOR_INFO_PREFIX  fmt::color::light_sea_green
#define COLOR_INFO_TEXT    fmt::terminal_color::white
#define COLOR_WARN_PREFIX  fmt::terminal_color::bright_yellow
#define COLOR_WARN_TEXT    fmt::terminal_color::yellow
#define COLOR_ERROR_PREFIX fmt::terminal_color::bright_red
#define COLOR_ERROR_TEXT   fmt::terminal_color::red

#define LOG_PREFIX(prefix, color1, color2)                                                                             \
    fmt::print(                                                                                                        \
        shouldLogColor ? fmt::fg(color1) : fmt::text_style(),                                                          \
        fmt::format("{:%H:%M:%S}", fmt::localtime(_time64(0)))                                                         \
    );                                                                                                                 \
    fmt::print(shouldLogColor ? fmt::fg(color2) : fmt::text_style(), fmt::format(prefix, fmt::localtime(_time64(0))));
#define LOG(color1, color2, prefix)                                                                                    \
    LOG_PREFIX(prefix, color1, color2);                                                                                \
    std::string str  = fmt::format("[Glacie] ", fmt::localtime(_time64(0)));                                           \
    str             += fmt::format(fmt::runtime(formatStr), args...);                                                  \
    str.append(1, '\n');

template <typename... Args>
void inline Info(const std::string& formatStr, const Args&... args) {
    LOG(COLOR_TIME, COLOR_INFO_PREFIX, " INFO ");
    fmt::print(fmt::fg(COLOR_INFO_TEXT), str);
}

template <typename... Args>
void inline Warn(const std::string& formatStr, const Args&... args) {
    LOG(COLOR_TIME, COLOR_WARN_PREFIX, " WARN ");
    fmt::print(fmt::fg(COLOR_WARN_TEXT) | fmt::emphasis::bold, str);
}

template <typename... Args>
void inline Error(const std::string& formatStr, const Args&... args) {
    LOG(COLOR_TIME, COLOR_ERROR_PREFIX, " ERROR ");
    fmt::print(fmt::fg(COLOR_ERROR_TEXT) | fmt::emphasis::bold, str);
}
