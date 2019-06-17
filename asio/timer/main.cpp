#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>

#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>

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
    explicit printer(boost::asio::io_context &io) : timer_(io, boost::asio::chrono::seconds(1)), count_(0) {
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

class printer_mt {
public:
    explicit printer_mt(boost::asio::io_context &io)
            : strand_(boost::asio::make_strand(io)),
              timer1_(io, boost::asio::chrono::seconds(1)),
              timer2_(io, boost::asio::chrono::seconds(1)),
              count_(0) {
        timer1_.async_wait(boost::asio::bind_executor(
                strand_,
                boost::bind(&printer_mt::print1, this)));
        timer2_.async_wait(boost::asio::bind_executor(
                strand_,
                boost::bind(&printer_mt::print2, this)));
    }

    ~printer_mt() {
        std::cout << "Final count is " << count_ << std::endl;
    }

    void print1() {
        if (count_ < 10) {
            std::cout << "Timer 1: " << count_ << std::endl;
            ++count_;
            timer1_.expires_at(timer1_.expiry() + boost::asio::chrono::seconds(1));
            timer1_.async_wait(boost::asio::bind_executor(
                    strand_,
                    boost::bind(&printer_mt::print1, this)));
        }
    }

    void print2() {
        if (count_ < 10) {
            std::cout << "Timer 2: " << count_ << std::endl;
            ++count_;
            timer2_.expires_at(timer2_.expiry() + boost::asio::chrono::seconds(1));
            timer2_.async_wait(boost::asio::bind_executor(
                    strand_,
                    boost::bind(&printer_mt::print2, this)));
        }
    }

private:
    boost::asio::strand<boost::asio::io_context::executor_type> strand_;
    boost::asio::steady_timer timer1_;
    boost::asio::steady_timer timer2_;
    int count_;
};

TEST_CASE("Synchronising handlers in multithreaded programs", "[asio]") {
    boost::asio::io_context io;
    printer_mt p(io);
    boost::thread t(boost::bind(&boost::asio::io_context::run, &io));
//    io.run();
    t.join();
}
