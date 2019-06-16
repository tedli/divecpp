#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>

#include "continuation.hpp"

TEST_CASE("boost::context::continuation", "[context]") {
    continuation::main();
}
