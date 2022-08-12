#include "connection.h"
#include "serializer.h"

#include <boost/asio/co_spawn.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>

#include <thread>

using std::move;
using boost::asio::ip::tcp;
using boost::asio::co_spawn;
using boost::asio::awaitable;
using boost::asio::use_awaitable;
using boost::asio::buffer;
using boost::asio::async_read;
using boost::asio::async_write;
using boost::asio::transfer_all;

auto ata = transfer_all;

auto push_quit = [] (connection& c) {
    return [&c] (std::exception_ptr ep) {
        if (ep)
            c.send_quit();
    };
};

connection::connection(tcp::endpoint re) :
    remote_endpoint{ move(re) },
    sock{ ioctx } {
    
    co_spawn(ioctx, connect(), push_quit(*this));
    auto m = [this] () {
        ioctx.run();
    };
    sep_thread = std::thread{ move(m) };
}

connection::~connection() {
    ioctx.stop();
    sep_thread.join();
}

awaitable<void> connection::connect() {
    co_await sock.async_connect(remote_endpoint, use_awaitable);
    auto loop_receive = [this] () -> awaitable<void> { for (;;) co_await receive(); };
    co_spawn(ioctx, loop_receive, push_quit(*this));
}

awaitable<void> connection::receive() {
    union {
        char buf[4];
        uint8_t u8;
        uint16_t u16;
    } number_buf; // little endian

    auto nread = [&number_buf, this] (size_t size) -> awaitable<void> {
        co_await async_read(sock, buffer(number_buf.buf, size), ata(), use_awaitable);
    };

    co_await nread(1);
    auto mtype = serializer::get_message_type(number_buf.u8);

    using T = Multiplayer::MessageType;
    namespace M = Multiplayer::Messages;
    std::unique_ptr<Multiplayer::AbstractMessage> r;

    switch (mtype) {
    case T::CONNECT:
    {
        auto p = std::make_unique<M::Connect>();
        co_await nread(2);
        p->client_id = number_buf.u16;
        r = move(p);
        break;
    }
    case T::DISCONNECT:
    {
        auto p = std::make_unique<M::Disconnect>();
        co_await nread(2);
        p->client_id = number_buf.u16;
        r = move(p);
        break;
    }
    case T::R_SPRITE:
    {
        auto p = std::make_unique<M::ReceiveSprite>();
        co_await nread(2);
        p->client_id = number_buf.u16;
        co_await nread(1);
        p->name.resize(number_buf.u8);
        co_await async_read(sock, buffer(p->name, p->name.size()), ata(), use_awaitable);
        co_await nread(1);
        p->index = number_buf.u8;
        r = move(p);
        break;
    }
    case T::R_POSITION:
    {
        auto p = std::make_unique<M::ReceivePosition>();
        co_await nread(2);
        p->client_id = number_buf.u16;
        co_await nread(2);
        p->x = number_buf.u16;
        co_await nread(2);
        p->y = number_buf.u16;
        co_await nread(1);
        p->direction = number_buf.u8;
        r = move(p);
        break;
    }
    case T::R_SPEED:
    {
        auto p = std::make_unique<M::ReceiveSpeed>();
        co_await nread(2);
        p->client_id = number_buf.u16;
        co_await nread(1);
        p->speed = number_buf.u8;
        r = move(p);
        break;
    }
    default:
        throw std::runtime_error("invalid nid");
    }
    {
        auto l = q_recv.new_unique_lock();
        q_recv.q.emplace(move(r));
    }
}

awaitable<void> connection::send(std::vector<char> bytes) {
    co_await async_write(sock, buffer(bytes), ata(), use_awaitable);
}

void connection::send_quit() {
    auto l = q_recv.new_unique_lock();
    q_recv.q.emplace(std::make_unique<Multiplayer::Messages::Quit>());
}

void connection::PollMessages(Multiplayer::MessageConsumer f) noexcept {
    auto l = q_recv.new_unique_lock();
    auto& q = q_recv.q;
    while (!q.empty()) {
        f(std::move(q.front()));
        q.pop();
    }
}

void connection::SendMessage(const Multiplayer::AbstractMessage& m) noexcept {
    co_spawn(ioctx, send(serializer::serialize(m)), push_quit(*this));
}

