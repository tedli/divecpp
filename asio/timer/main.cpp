#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>

#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

TEST_CASE("Using a timer synchronously", "[asio]") {
    boost::asio::io_context io;
    boost::asio::steady_timer t(io, boost::asio::chrono::seconds(5));
    t.wait();
    std::cout << "Hello, world!" << std::endl;
}

void print(const boost::system::error_code & /*e*/) {
    std::cout << "Hello, world!" << std::endl;
}

TEST_CASE("Using a timer asynchronously", "[asio]") {
    boost::asio::io_context io;
    boost::asio::steady_timer t(io, boost::asio::chrono::seconds(5));
    t.async_wait(&print);
    io.run();
}

void print(const boost::system::error_code &, boost::asio::steady_timer *t, int *count) {
    if (*count < 5) {
        std::cout << *count << std::endl;
        ++(*count);
        t->expires_at(t->expiry() + boost::asio::chrono::seconds(1));
        t->async_wait(boost::bind(print, boost::asio::placeholders::error, t, count));
    }
}

TEST_CASE("Binding arguments to a handler", "[asio]") {
    boost::asio::io_context io;
    int count = 0;
    boost::asio::steady_timer t(io, boost::asio::chrono::seconds(1));
    t.async_wait(boost::bind(print, boost::asio::placeholders::error, &t, &count));
    io.run();
    std::cout << "Final count is " << count << std::endl;
}

class printer {
public:
    printer(boost::asio::io_context &io) : timer_(io, boost::asio::chrono::seconds(1)), count_(0) {
        timer_.async_wait(boost::bind(&printer::print, this));
    }

    ~printer() {
        std::cout << "Final count is " << count_ << std::endl;
    }

    void print() {
        if (count_ < 5) {
            std::cout << count_ << std::endl;
            ++count_;
            timer_.expires_at(timer_.expiry() + boost::asio::chrono::seconds(1));
            timer_.async_wait(boost::bind(&printer::print, this));
        }
    }

private:
    boost::asio::steady_timer timer_;
    int count_;
};

TEST_CASE("Using a member function as a handler", "[asio]") {
    boost::asio::io_context io;
    printer p(io);
    io.run();
}
