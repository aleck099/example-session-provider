#ifndef EP_MULTIPLAYER_SESSION_PROVIDER_H
#define EP_MULTIPLAYER_SESSION_PROVIDER_H

#include "session.h"
#include <string>

namespace Multiplayer {

class SessionProvider {
public:
	virtual ~SessionProvider() = default;

	virtual Session* CreateSession() = 0;

	static SessionProvider* LoadSessionProvider(const std::string& path,
			const std::string& arg);

	static const char default_mod_name[];
};

constexpr char PROVIDER_API[]{ "EP_Multiplayer_GetSessionProvider" };
using API = SessionProvider* (*)(const char*);

}

#endif

