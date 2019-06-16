#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>

#include <boost/context/execution_context.hpp>

TEST_CASE("boost::context::execution_context", "[context]") {
    namespace ctx = boost::context;
    int n = 35;
    ctx::execution_context<int> source(
            [n](ctx::execution_context<int> &&sink, int) mutable {
                int a = 0;
                int b = 1;
                while (n-- > 0) {
                    auto result = sink(a);
                    sink = std::move(std::get<0>(result));
                    auto next = a + b;
                    a = b;
                    b = next;
                }
                return std::move(sink);
            });
    for (int i = 0; i < 10; ++i) {
        auto result = source(i);
        source = std::move(std::get<0>(result));
        std::cout << std::get<1>(result) << " ";
    }
}
