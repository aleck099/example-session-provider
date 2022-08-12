#ifndef EXSP_MOD
#define EXSP_MOD

#include "session_provider.h"

class mod : public Multiplayer::SessionProvider {
public:
    struct config_t {
        std::string ip;
        uint16_t port;
    };

    mod(config_t _cfg) : cfg(std::move(_cfg)) {}
    Multiplayer::Session* CreateSession() override;
private:
    config_t cfg;
};

#endif

