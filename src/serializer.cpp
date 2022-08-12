#include "serializer.h"
#include <cassert>
#include <concepts>

template<std::integral T>
void write_value(std::vector<char>& v, T val) {
    auto p = reinterpret_cast<char*>(&val);
    v.insert(v.end(), p, p + sizeof(T));
}

std::vector<char> serializer::serialize(const Multiplayer::AbstractMessage& m) {
    namespace M = Multiplayer::Messages;

    std::vector<char> buf;
    auto nid = get_message_nid(m.Type());
    assert(nid != 0);
    buf.emplace_back(nid);
    using T = Multiplayer::MessageType;
    switch (m.Type()) {
    case T::ROOM_SWITCH:
        write_value(buf, static_cast<const M::SwitchRoom&>(m).room);
        break;
    case T::S_SPEED:
        write_value(buf, static_cast<const M::SendSpeed&>(m).speed);
        break;
    case T::S_SPRITE:
    {
        auto& p = static_cast<const M::SendSprite&>(m);
        write_value(buf, static_cast<uint8_t>(p.name.size()));
        buf.insert(buf.end(), p.name.begin(), p.name.end());
        write_value(buf, p.index);
        break;
    }
    case T::S_POSITION:
    {
        auto& p = static_cast<const M::SendPosition&>(m);
        write_value(buf, p.x);
        write_value(buf, p.y);
        write_value(buf, p.direction);
        break;
    }
    default:
        assert(false);
    }
    return buf;
}

