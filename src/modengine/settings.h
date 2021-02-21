#pragma once

#include <spdlog/spdlog.h>
#include <toml.hpp>

#include <locale>
#include <codecvt>
#include <string>

namespace modengine {

using namespace spdlog;

static std::wstring utf8_to_wide(const std::string& str)
{
    auto count = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.length(), NULL, 0);
    std::wstring wstr(count, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.length(), &wstr[0], count);
    return wstr;
}

struct ExtensionInfo {
    bool enabled;
    toml::value other;

    void from_toml(const toml::value& v)
    {
        this->enabled = toml::find_or<bool>(v, "enabled", false);
        this->other = v;
    }
};

struct ModInfo {
public:
    std::wstring name;
    std::wstring location;
    bool enabled;

    void from_toml(const toml::value& v)
    {
        this->name = utf8_to_wide(toml::find_or<std::string>(v, "name", "Unknown"));
        this->location = utf8_to_wide(toml::find<std::string>(v, "path"));
        this->enabled = toml::find_or<bool>(v, "enabled", false);
    }
};

class Settings {
public:
    Settings()
        : m_config()
    {
    }

    bool load_from(const std::string& path);

    const boolean is_debug_enabled() const;

    const ExtensionInfo extension(const std::string& name);

    const std::vector<ModInfo> mods() const;
private:
    toml::value m_config;
};

}