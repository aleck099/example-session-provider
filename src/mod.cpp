#include "mod.h"
#include "connection.h"
#include <string>
#include <charconv>

#if defined(_WIN32)
#define MODEXPORT __declspec(dllexport)
#else
#define MODEXPORT __attribute__((visibility("default")))
#endif

Multiplayer::Session* mod::CreateSession() {
    boost::asio::ip::tcp::endpoint ep{
        boost::asio::ip::make_address(cfg.ip),
        cfg.port
    };
    return new connection(std::move(ep));
}

extern "C" MODEXPORT Multiplayer::SessionProvider* EP_Multiplayer_GetSessionProvider(const char* args) {
    std::string_view s{args};
    if (s.empty())
        return nullptr;
    mod::config_t cfg;
    std::string_view port_str;
    if (s[0] == '[') { // ipv6
        size_t p2 = s.find(']');
        if (p2 == s.npos)
            return nullptr;
        cfg.ip = s.substr(1, p2 - 1);
        port_str = s.substr(p2 + 2);
    } else {
        size_t p2 = s.find(':');
        if (p2 == s.npos)
            return nullptr;
        cfg.ip = s.substr(0, p2);
        port_str = s.substr(p2 + 1);
    }
    auto result = std::from_chars(port_str.data(), port_str.data() + port_str.size(), cfg.port);
    return result.ec == std::errc() ? new mod(std::move(cfg)) : nullptr;
}

