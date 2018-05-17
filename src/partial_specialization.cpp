/** Partial specialization
 *
 * Based on http://en.cppreference.com/w/cpp/types/is_same
 */

/** std::integral_constant wraps a static constant of specified type.
 * It is the base class for the C++ type traits.
 *
 * Based on http://en.cppreference.com/w/cpp/types/integral_constant
 * @tparam T type
 * @tparam v value
 */
template<class T, T v>
struct integral_constant {
    static constexpr T value = v;
    typedef T value_type;
    constexpr explicit operator value_type() const noexcept { return value; }
    constexpr value_type operator()() const noexcept { return value; } //since c++14
};

template<class T, class U>
struct is_same : integral_constant<bool, false> {
};

template<class T>
struct is_same<T, T> : integral_constant<bool, true> {
};


#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <cstdint>

TEST_CASE("Just works", "[is_same]") {
    REQUIRE(is_same<int, int32_t>::value);
    REQUIRE_FALSE(is_same<int, int64_t>::value);
    REQUIRE_FALSE(is_same<float, int32_t>::value);
}

TEST_CASE("int is implicitly signed", "[is_same]") {
    REQUIRE(is_same<int, int>::value);
    REQUIRE_FALSE(is_same<int, unsigned int>::value);
    REQUIRE(is_same<int, signed int>::value);
}

TEST_CASE("unlike other types `char` is not `unsigned` and not `signed`", "[is_same]") {
    REQUIRE(is_same<char, char>::value);
    REQUIRE_FALSE(is_same<char, unsigned char>::value);
    REQUIRE_FALSE(is_same<char, signed char>::value);
}
