#ifndef EP_MULTIPLAYER_SESSION_H
#define EP_MULTIPLAYER_SESSION_H

#include "messages.h"
#include <functional>
#include <memory>

namespace Multiplayer {

using MessageConsumer = std::function<void (std::unique_ptr<AbstractMessage>&&)>;

// session is created when first Scene_Map starts if multiplayer is enabled
// if player manually turns off multiplayer, session is destroyed
// and is re-created if he turns multiplayer on
//
// session is noexcept, all exceptions including accidental network problems
// should be handled inside session, easyrpg does not care them
//
// if severe exception happens and is unable to be handled, leave a QUIT
// message to us
class Session {
public:
	virtual ~Session() = default;

	// poll messages from external session provider
	// and call consumer with them one by one
	// return number of messages got
	// note that this method must be non-blocking
	virtual void PollMessages(MessageConsumer) noexcept = 0;

	// send message to session provider
	// note that this method must be non-blocking
	virtual void SendMessage(const AbstractMessage&) noexcept = 0;
};

}

#endif

