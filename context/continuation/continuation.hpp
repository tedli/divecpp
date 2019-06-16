#include <iostream>
#include <boost/context/continuation.hpp>

class continuation {
public:
    inline static void main() noexcept;
};

inline void continuation::main() noexcept {
    namespace ctx = boost::context;
    int data = 0;
    ctx::continuation c = ctx::callcc([&data](ctx::continuation &&c) {
        std::cout << "f1: entered first time: " << data << std::endl;
        data += 1;
        c = c.resume();
        std::cout << "f1: entered second time: " << data << std::endl;
        data += 1;
        c = c.resume();
        std::cout << "f1: entered third time: " << data << std::endl;
        return std::move(c);
    });
    std::cout << "f1: returned first time: " << data << std::endl;
    data += 1;
    c = c.resume();
    std::cout << "f1: returned second time: " << data << std::endl;
    data += 1;
    c = c.resume_with([&data](ctx::continuation &&c) {
        std::cout << "f2: entered: " << data << std::endl;
        data = -1;
        return std::move(c);
    });
    std::cout << "f1: returned third time" << std::endl;
}
