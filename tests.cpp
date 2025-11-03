#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "MyContainer.hpp" 
#include <sstream>          
#include <vector>            
#include <string>          
#include <algorithm>      

using namespace ex4;        


// Helper custom type for simple non-primitive coverage (kept minimal).
// Note: We avoid “Student”; here we use a small “Book” struct.

struct Book {
    std::string title;
    int pages{};
    // Equality: both fields must match
    bool operator==(const Book& other) const { return title == other.title && pages == other.pages; }
    // Ordering by pages to allow sorted iterators to operate
    bool operator<(const Book& other)  const { return pages <  other.pages; }
    bool operator>(const Book& other)  const { return pages >  other.pages; }
    friend std::ostream& operator<<(std::ostream& os, const Book& b) {
        os << b.title << " (" << b.pages << ")";
        return os;
    }
};

// MyContainer core functions: addElement, removeElement, size

TEST_CASE("MyContainer::addElement and MyContainer::size - basic growth") {
    // Create an empty container of int; default size must be zero.
    MyContainer<int> c;
    CHECK(c.size() == 0); // Verify initial size

    c.addElement(7);
    c.addElement(15);
    c.addElement(6);
    CHECK(c.size() == 3);

    CHECK(c.getData()[0] == 7);
    CHECK(c.getData()[1] == 15);
    CHECK(c.getData()[2] == 6);
}

TEST_CASE("MyContainer::removeElement - removes ALL occurrences") {
    // Prepare a container with duplicates to ensure all instances are removed.
    MyContainer<int> c;
    c.addElement(1);
    c.addElement(2);
    c.addElement(2);
    c.addElement(3);
    CHECK(c.size() == 4); // Two occurrences of 2 exist

    c.removeElement(2);
    CHECK(c.size() == 2);
    // Remaining elements should be {1, 3} in their original relative order
    CHECK(c.getData()[0] == 1);
    CHECK(c.getData()[1] == 3);
}

TEST_CASE("MyContainer::removeElement - throws when value does not exist") {
    // Attempting to remove a value that is not present should throw std::runtime_error
    MyContainer<int> c;
    c.addElement(10);
    c.addElement(20);
    CHECK_THROWS_AS(c.removeElement(999), std::runtime_error);

    // Optionally verify the exact message (depends on the implementation’s string).
    CHECK_THROWS_WITH(c.removeElement(999), "This element does not exist in the container");
}

TEST_CASE("MyContainer::size - multiple growth and shrink steps") {
    // Repeated add/remove with size checks in between to ensure consistency.
    MyContainer<int> c;
    CHECK(c.size() == 0);

    c.addElement(5);
    CHECK(c.size() == 1);
    c.addElement(5);
    CHECK(c.size() == 2);
    c.addElement(7);
    CHECK(c.size() == 3);

    c.removeElement(5);
    CHECK(c.size() == 1);
    CHECK(c.getData()[0] == 7);

    c.removeElement(7);
    CHECK(c.size() == 0);
}

TEST_CASE("operator<< prints elements as: \"x y ... \\n\"") {
    // The container’s print/streaming behavior is part of our implementation; test format precisely.
    MyContainer<int> c;
    c.addElement(42);
    c.addElement(17);

    std::ostringstream oss;
    oss << c;                           // Use the overloaded operator<<
    CHECK(oss.str() == std::string("42 17 \n")); // Exact expected format
}

// Iterators:
// Order, ReverseOrder, AscendingOrder, DescendingOrder, SideCrossOrder, MiddleOutOrder
// Tests cover: normal sequences, empty container, single element, duplicates,
// negatives, strings, custom type.

// Common test data for integers
static const std::vector<int> baseA = {7, 15, 6, 1, 2};       // Mixed order
static const std::vector<int> baseB = {10, -20, 190, 190, 5}; // Duplicates + negatives
static const std::vector<int> baseC = {1};                    // Single element
static const std::vector<int> baseD = {};                     // Empty

// Order iterator
TEST_CASE("Order iterator - preserves insertion order") {
    MyContainer<int> c;
    for (int x : baseA) c.addElement(x);

    // Expected traversal equals insertion order exactly.
    std::vector<int> expected = baseA;
    std::size_t i = 0;
    for (auto it = c.begin_order(); it != c.end_order(); ++it, ++i) {
        CHECK(*it == expected[i]);
    }
    CHECK(i == expected.size()); // Ensure we visited all elements
}

TEST_CASE("Order iterator - empty container yields begin==end") {
    MyContainer<int> c;                 // No elements
    CHECK(c.begin_order() == c.end_order()); // begin == end
}

TEST_CASE("Order iterator - single element container traverses exactly once") {
    MyContainer<int> c;
    for (int x : baseC) c.addElement(x);

    auto it = c.begin_order();
    CHECK(it != c.end_order());
    CHECK(*it == 1);      // Single element
    ++it;
    CHECK(it == c.end_order()); // One step traversal
}



// ReverseOrder iterator

TEST_CASE("ReverseOrder iterator - reverse of insertion order") {
    MyContainer<int> c;
    for (int x : baseA) c.addElement(x);

    // Expected reverse of baseA: 2 1 6 15 7
    std::vector<int> expected = {2, 1, 6, 15, 7};
    std::size_t i = 0;
    for (auto it = c.begin_reverse_order(); it != c.end_reverse_order(); ++it, ++i) {
        CHECK(*it == expected[i]);
    }
    CHECK(i == expected.size());
}

TEST_CASE("ReverseOrder iterator - empty and single-element") {
    // Empty
    {
        MyContainer<int> c;
        CHECK(c.begin_reverse_order() == c.end_reverse_order());
    }
    // Single
    {
        MyContainer<int> c;
        c.addElement(99);
        auto it = c.begin_reverse_order();
        CHECK(*it == 99);
        ++it;
        CHECK(it == c.end_reverse_order());
    }
}

// AscendingOrder iterator

TEST_CASE("AscendingOrder iterator - sorted ascending") {
    MyContainer<int> c;
    for (int x : baseA) c.addElement(x);

    // Expected ascending: 1 2 6 7 15
    std::vector<int> expected = {1, 2, 6, 7, 15};
    std::size_t i = 0;
    for (auto it = c.begin_ascending_order(); it != c.end_ascending_order(); ++it, ++i) {
        CHECK(*it == expected[i]);
    }
    CHECK(i == expected.size());
}

TEST_CASE("AscendingOrder iterator - duplicates and negatives") {
    MyContainer<int> c;
    for (int x : baseB) c.addElement(x);

    // baseB = {10, -20, 190, 190, 5} 
    // expected = {-20, 5, 10, 190, 190}
    std::vector<int> expected = {-20, 5, 10, 190, 190};
    std::size_t i = 0;
    for (auto it = c.begin_ascending_order(); it != c.end_ascending_order(); ++it, ++i) {
        CHECK(*it == expected[i]);
    }
    CHECK(i == expected.size());
}

TEST_CASE("AscendingOrder iterator - empty and single-element") {
    // Empty
    {
        MyContainer<int> c;
        CHECK(c.begin_ascending_order() == c.end_ascending_order());
    }
    // Single
    {
        MyContainer<int> c;
        c.addElement(-1);
        auto it = c.begin_ascending_order();
        CHECK(*it == -1);
        ++it;
        CHECK(it == c.end_ascending_order());
    }
}

// DescendingOrder iterator

TEST_CASE("DescendingOrder iterator - sorted descending") {
    MyContainer<int> c;
    for (int x : baseA) c.addElement(x);

    // Expected descending: 15 7 6 2 1
    std::vector<int> expected = {15, 7, 6, 2, 1};
    std::size_t i = 0;
    for (auto it = c.begin_descending_order(); it != c.end_descending_order(); ++it, ++i) {
        CHECK(*it == expected[i]);
    }
    CHECK(i == expected.size());
}

TEST_CASE("DescendingOrder iterator - duplicates and negatives") {
    MyContainer<int> c;
    for (int x : baseB) c.addElement(x);

    // expected = {190, 190, 10, 5, -20}
    std::vector<int> expected = {190, 190, 10, 5, -20};
    std::size_t i = 0;
    for (auto it = c.begin_descending_order(); it != c.end_descending_order(); ++it, ++i) {
        CHECK(*it == expected[i]);
    }
    CHECK(i == expected.size());
}

TEST_CASE("DescendingOrder iterator - empty and single-element") {
    // Empty
    {
        MyContainer<int> c;
        CHECK(c.begin_descending_order() == c.end_descending_order());
    }
    // Single
    {
        MyContainer<int> c;
        c.addElement(77);
        auto it = c.begin_descending_order();
        CHECK(*it == 77);
        ++it;
        CHECK(it == c.end_descending_order());
    }
}

// SideCrossOrder iterator

TEST_CASE("SideCrossOrder iterator - alternating low/high from sorted sequence") {
    // Example 1: 
    //[7, 15, 6, 1, 2] 
    //sorted [1,2,6,7,15] 
    //side-cross [1,15,2,7,6]
    {
        MyContainer<int> c;
        for (int x : baseA) c.addElement(x);
        std::vector<int> expected = {1, 15, 2, 7, 6};
        std::size_t i = 0;
        for (auto it = c.begin_side_cross_order(); it != c.end_side_cross_order(); ++it, ++i) {
            CHECK(*it == expected[i]);
        }
        CHECK(i == expected.size());
    }

    // Example 2 (even length): 
    //[1,2,3,4]
    //sorted [1,2,3,4] 
    //side-cross [1,4,2,3]
    {
        MyContainer<int> c;
        for (int x : std::vector<int>{1,2,3,4}) c.addElement(x);
        std::vector<int> expected = {1, 4, 2, 3};
        std::size_t i = 0;
        for (auto it = c.begin_side_cross_order(); it != c.end_side_cross_order(); ++it, ++i) {
            CHECK(*it == expected[i]);
        }
        CHECK(i == expected.size());
    }

    // Empty
    {
        MyContainer<int> c;
        CHECK(c.begin_side_cross_order() == c.end_side_cross_order());
    }

    // Single
    {
        MyContainer<int> c;
        c.addElement(5);
        auto it = c.begin_side_cross_order();
        CHECK(*it == 5);
        ++it;
        CHECK(it == c.end_side_cross_order());
    }
}

// MiddleOutOrder iterator
TEST_CASE("MiddleOutOrder iterator - odd size starts from middle, then alternates left/right") {
    // For baseA = {7, 15, 6, 1, 2}
    // Expected traversal: [6, 15, 1, 7, 2]
    MyContainer<int> c;
    for (int x : baseA) c.addElement(x);

    std::vector<int> expected = {6, 15, 1, 7, 2};
    std::size_t i = 0;
    for (auto it = c.begin_middle_out_order(); it != c.end_middle_out_order(); ++it, ++i) {
        CHECK(*it == expected[i]);
    }
    CHECK(i == expected.size());
}

TEST_CASE("MiddleOutOrder iterator - even size (lower middle policy)") {
    // For {1,2,3,4}, 
    // Expected traversal: [2, 1, 3, 4]
    MyContainer<int> c;
    for (int x : std::vector<int>{1,2,3,4}) c.addElement(x);

    std::vector<int> expected = {2, 1, 3, 4};
    std::size_t i = 0;
    for (auto it = c.begin_middle_out_order(); it != c.end_middle_out_order(); ++it, ++i) {
        CHECK(*it == expected[i]);
    }
    CHECK(i == expected.size());
}

TEST_CASE("MiddleOutOrder iterator - empty and single-element") {
    // Empty
    {
        MyContainer<int> c;
        CHECK(c.begin_middle_out_order() == c.end_middle_out_order());
    }
    // Single
    {
        MyContainer<int> c;
        c.addElement(-3);
        auto it = c.begin_middle_out_order();
        CHECK(*it == -3);
        ++it;
        CHECK(it == c.end_middle_out_order());
    }
}

// Iterators with std::string and with custom type Book

TEST_CASE("Ascending/Descending with std::string") {
    MyContainer<std::string> c;
    c.addElement("banana");
    c.addElement("apple");
    c.addElement("carrot");

    // Ascending should be: apple, banana, carrot
    {
        std::vector<std::string> expected = {"apple", "banana", "carrot"};
        std::size_t i = 0;
        for (auto it = c.begin_ascending_order(); it != c.end_ascending_order(); ++it, ++i) {
            CHECK(*it == expected[i]);
        }
        CHECK(i == expected.size());
    }

    // Descending should be: carrot, banana, apple
    {
        std::vector<std::string> expected = {"carrot", "banana", "apple"};
        std::size_t i = 0;
        for (auto it = c.begin_descending_order(); it != c.end_descending_order(); ++it, ++i) {
            CHECK(*it == expected[i]);
        }
        CHECK(i == expected.size());
    }
}

TEST_CASE("Descending with Book (sorted by pages)") {
    MyContainer<Book> c;
    c.addElement({"Short", 120});
    c.addElement({"Long", 400});
    c.addElement({"Medium", 250});

    std::vector<Book> expected = {
        {"Long",   400},
        {"Medium", 250},
        {"Short",  120}
    };

    std::size_t i = 0;
    for (auto it = c.begin_descending_order(); it != c.end_descending_order(); ++it, ++i) {
        CHECK(*it == expected[i]);
    }
    CHECK(i == expected.size());
}

//  Iterator operator semantics — prefix/postfix ++ and end guard

TEST_CASE("Iterator operators: prefix/postfix ++ and end dereference throws (Order)") {
    MyContainer<int> c;
    c.addElement(10);
    c.addElement(20);
    c.addElement(30);

    // Prefix ++
    auto it = c.begin_order();
    CHECK(*it == 10);
    ++it;
    CHECK(*it == 20);

    // Postfix ++
    it++;
    CHECK(*it == 30);

    // Move to end and verify dereferencing end throws (guarded by vector::at).
    ++it;
    CHECK(it == c.end_order());
    CHECK_THROWS_AS(*it, std::out_of_range);
}
