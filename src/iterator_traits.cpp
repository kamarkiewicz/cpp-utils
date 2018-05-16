/** How to work with std::iterator_traits
 * Based on stl_iterator_base_funcs/types.h
 *
 * http://en.cppreference.com/w/cpp/iterator/iterator_traits
 */
#include <iterator>
#include <cassert>

template<class InputIterator, class Distance>
void advance_impl(InputIterator &i, Distance n, std::random_access_iterator_tag) {
    i += n;
}

template<class InputIterator, class Distance>
void advance_impl(InputIterator &i, Distance n, std::bidirectional_iterator_tag) {
    if (n < 0) {
        while (n++) --i;
    } else {
        while (n--) ++i;
    }
}

template<class InputIterator, class Distance>
void advance_impl(InputIterator &i, Distance n, std::input_iterator_tag) {
    assert(n >= 0);
    while (n--) ++i;
}

template<class InputIterator, class Distance>
void myadvance(InputIterator &i, Distance n) {
    advance_impl(i, n,
                 typename std::iterator_traits<InputIterator>::iterator_category());
}


#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <vector>
#include <list>

TEST_CASE("Works on bidirectional list iterator", "[myadvance]") {
    // Given
    std::list<int> lst = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    auto iter = lst.begin();
    std::advance(iter, 5);
    const auto fifth = iter;

    // When
    iter = lst.begin();
    myadvance(iter, 5);

    // Then
    REQUIRE(iter == fifth);
}

TEST_CASE("Works on random access vector iterator", "[myadvance]") {
    // Given
    std::vector<int> vec = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    auto iter = vec.begin();
    std::advance(iter, 5);
    const auto fifth = iter;

    // When
    iter = vec.begin();
    myadvance(iter, 5);

    // Then
    REQUIRE(iter == fifth);
}
