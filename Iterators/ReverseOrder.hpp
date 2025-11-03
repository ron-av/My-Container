#pragma once
#include <vector>    
#include <algorithm> 
#include <cstddef>   

namespace ex4 {      
/** Forward declaration to reference MyContainer<T> without including its full definition. */
template <typename T> class MyContainer;

/**
 * @class ReverseOrder
 * @brief Iterator that traverses a container’s elements in reverse order.
 *
 * Overview:
 *  - Builds a reversed copy of the container’s contents (`view`).
 *  - Maintains a current index (`idx`) representing traversal position.
 *  - Provides prefix/postfix increment, dereference, and comparison operators.
 *
 */
template <typename T>
class ReverseOrder {
    std::vector<T> view;  /** Local copy of container elements, reversed. */
    std::size_t idx;      /** Current traversal position (0..view.size()). */

public:
    /**
     * @brief Constructs an iterator with a prepared view and starting index.
     * @param v  Vector representing traversal order (already reversed).
     * @param i  Starting index (default = 0 for begin).
     *
     * Uses move semantics for efficient transfer of data.
     */
    ReverseOrder(std::vector<T> v, std::size_t i = 0)
        : view(std::move(v)),  /** Move constructor for efficiency. */
          idx(i)               /** Initialize iterator position. */
    {}

    /**
     * @brief  Method that constructs a begin/end iterator pair for reverse traversal.
     * @param c  The source container.
     * @return   A pair {begin, end} representing iterators over reversed data.
     *
     */
    static std::pair<ReverseOrder, ReverseOrder> make(const MyContainer<T>& c) {
        std::vector<T> v = c.getData();          /** Copy original data. */
        std::reverse(v.begin(), v.end());        /** Reverse the entire vector in place. */
        return {
            ReverseOrder(v, 0),                  /** Begin iterator (first element of reversed vector). */
            ReverseOrder(v, v.size())            /** End iterator (one past the last element). */
        };
    }

    /**
     * @brief Dereference operator for read-only access to the current element.
     * @return A constant reference to the element currently pointed to.
     * @throws std::out_of_range if index is invalid (checked by `vector::at`).
     *
     * Example:
     *  @code
     *  ReverseOrder<int> it = ...;
     *  std::cout << *it;
     *  @endcode
     */
    const T& operator*() const { return view.at(idx); }

    /**
     * @brief Arrow operator providing pointer-like access.
     * @return Pointer to the current element in the reversed view.
     * @throws std::out_of_range if index is invalid.
     *
     * Example:
     *  @code
     *  it->someMethod(); // if T is a class with members
     *  @endcode
     */
    const T* operator->() const { return &view.at(idx); }

    /**
     * @brief Prefix increment operator.
     * @return Reference to this iterator after moving to the next element.
     *
     * Advances the iterator to the next element in the reversed sequence.
     * Example: `++it;`
     */
    ReverseOrder& operator++() { ++idx; return *this; }

    /**
     * @brief Postfix increment operator.
     * @return A copy of the iterator before incrementing.
     *
     * Allows syntax like `it++`.
     * The returned iterator represents the previous position.
     */
    ReverseOrder operator++(int) {
        ReverseOrder tmp = *this;  /** Save current state. */
        ++(*this);                 /** Advance iterator. */
        return tmp;                /** Return saved version. */
    }

    /**
     * @brief Equality comparison operator.
     * @param other Another ReverseOrder iterator.
     * @return true if both iterators refer to the same view and index.
     *
     * Commonly used for loop termination: `while (it != end)`.
     */
    bool operator==(const ReverseOrder& other) const {
        return idx == other.idx && view == other.view;
    }

    /**
     * @brief Inequality comparison operator.
     * @param other Another ReverseOrder iterator.
     * @return true if iterators differ.
     */
    bool operator!=(const ReverseOrder& other) const { return !(*this == other); }
};

} 
