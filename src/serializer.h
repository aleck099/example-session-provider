#ifndef EXSP_SERIALIZER
#define EXSP_SERIALIZER

#include "messages.h"
#include <vector>
#include <ranges>

class serializer {
public:
    static std::vector<char> serialize(const Multiplayer::AbstractMessage& m);

    static uint8_t get_message_nid(Multiplayer::MessageType t) {
        using T = Multiplayer::MessageType;
        switch (t) {
        // case T::CONNECT:
        //     return 0x01;
        // case T::DISCONNECT:
        //     return 0x02;
        // case T::R_SPRITE:
        //     return 0x10;
        // case T::R_POSITION:
        //     return 0x11;
        // case T::R_SPEED:
        //     return 0x12;
        case T::ROOM_SWITCH:
            return 0x03;
        case T::S_SPRITE:
            return 0x10;
        case T::S_POSITION:
            return 0x11;
        case T::S_SPEED:
            return 0x12;
        default:
            return 0;
        }
    }

    static Multiplayer::MessageType get_message_type(uint8_t nid) {
        using T = Multiplayer::MessageType;
        switch (nid) {
        case 0x01:
            return T::CONNECT;
        case 0x02:
            return T::DISCONNECT;
        case 0x03:
            return T::ROOM_SWITCH;
        case 0x10:
            return T::R_SPRITE;
        case 0x11:
            return T::R_POSITION;
        case 0x12:
            return T::R_SPEED;
        default:
            return T::QUIT;
        }
    }
};

#endif

