#ifndef EXSP_CONNECTION
#define EXSP_CONNECTION

#include "session.h"
#include "synchorized_queue.h"

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/awaitable.hpp>

#include <thread>

class connection : public Multiplayer::Session {
public:
    connection(boost::asio::ip::tcp::endpoint re);
    ~connection();

    boost::asio::awaitable<void> connect();
    boost::asio::awaitable<void> receive();
    boost::asio::awaitable<void> send(std::vector<char> bytes);

    void send_quit();

    void PollMessages(Multiplayer::MessageConsumer) noexcept override;
    void SendMessage(const Multiplayer::AbstractMessage&) noexcept override;
private:
    std::thread sep_thread;
    boost::asio::io_context ioctx;

    boost::asio::ip::tcp::endpoint remote_endpoint;
    boost::asio::ip::tcp::socket sock;
    synchorized_queue<std::unique_ptr<Multiplayer::AbstractMessage>> q_recv;

};

#endif

