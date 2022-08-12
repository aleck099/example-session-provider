#ifndef EP_MULTIPLAYER_MESSAGES_H
#define EP_MULTIPLAYER_MESSAGES_H

#include <cstdint>
#include <string>

namespace Multiplayer {

enum class MessageCategory {
	System,
	S2C, // server to client
	C2S, // client to server
};

enum class MessageType {
	// system messages
	QUIT,
	// S2C messages
	CONNECT,
	DISCONNECT,
	R_SPRITE,
	R_POSITION,
	R_SPEED,
	// C2S messages
	ROOM_SWITCH,
	S_SPRITE,
	S_POSITION,
	S_SPEED,
};

class AbstractMessage {
public:
	virtual ~AbstractMessage() = default;
	virtual MessageType Type() const = 0;

	MessageCategory Category() const {
		using T = MessageType;
		using C = MessageCategory;
		switch (Type()) {
		case T::QUIT:
			return C::System;
		case T::CONNECT:
		case T::DISCONNECT:
		case T::R_SPRITE:
		case T::R_POSITION:
		case T::R_SPEED:
			return C::S2C;
		case T::ROOM_SWITCH:
		case T::S_SPRITE:
		case T::S_POSITION:
		case T::S_SPEED:
			return C::C2S;
		}
	}
};

namespace Messages {

class Quit : public AbstractMessage {
public:
	MessageType Type() const override { return MessageType::QUIT; }
};

class Connect : public AbstractMessage {
public:
	MessageType Type() const override { return MessageType::CONNECT; }
	uint16_t client_id;
};

class Disconnect : public AbstractMessage {
public:
	MessageType Type() const override { return MessageType::DISCONNECT; }
	uint16_t client_id;
};

class ReceiveSprite : public AbstractMessage {
public:
	MessageType Type() const override { return MessageType::R_SPRITE; }
	uint16_t client_id;
	std::string name;
	uint8_t index;
};

class ReceivePosition : public AbstractMessage {
public:
	MessageType Type() const override { return MessageType::R_POSITION; }
	uint16_t client_id;
	uint16_t x, y;
	uint8_t direction;
};

class ReceiveSpeed : public AbstractMessage {
public:
	MessageType Type() const override { return MessageType::R_SPEED; }
	uint16_t client_id;
	uint8_t speed;
};

class SwitchRoom : public AbstractMessage {
public:
	MessageType Type() const override { return MessageType::ROOM_SWITCH; }
	uint16_t room;
};

class SendSprite : public AbstractMessage {
public:
	MessageType Type() const override { return MessageType::S_SPRITE; }
	std::string name;
	uint8_t index;
};

class SendPosition : public AbstractMessage {
public:
	MessageType Type() const override { return MessageType::S_POSITION; }
	uint16_t x, y;
	uint8_t direction;
};

class SendSpeed : public AbstractMessage {
public:
	MessageType Type() const override { return MessageType::S_SPEED; }
	uint8_t speed;
};

}

}

#endif

