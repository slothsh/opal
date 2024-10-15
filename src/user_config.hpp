#ifndef OPAL_USER_CONFIG_EGL_HPP
#define OPAL_USER_CONFIG_EGL_HPP
#include <cstdlib>
#include <string>
#include <optional>
#include <string_view>

struct UserConfig {
    static constexpr std::string_view DEFAULT_PATH = "/usr/local/etc/opal-shell/opal.toml";
    static inline std::optional<std::string> USER_PATH = (std::getenv("HOME") != nullptr)
        ? std::make_optional(std::string{std::getenv("HOME")} + "/.config/opal")
        : std::nullopt;
};

#endif // @END of OPAL_USER_CONFIG_EGL_HPP
