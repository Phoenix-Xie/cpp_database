#define CATCH_CONFIG_MAIN
#include "catch.hpp"
using namespace std;


TEST_CASE("first Test"){
	REQUIRE(1 == 2);
	REQUIRE(2 == 2);
}