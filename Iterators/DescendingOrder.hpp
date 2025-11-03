#pragma once
#include <vector>       
#include <algorithm>   
#include <functional>  
#include <cstddef>      

namespace ex4 {       

template <typename T> class MyContainer;

/**
 * @class DescendingOrder
 * @brief A lightweight forward iterator for traversing elements of a container in descending order.
 * Overview:
 *  - This iterator maintains a local copy (`view`) of the container's elements sorted in descending order.
 *  - It uses an index (`idx`) to track the current position during traversal.
 *  - The iterator supports standard operations: dereference, prefix/postfix increment, and equality/inequality comparisons.
 * 
 */
template <typename T>
class DescendingOrder {
    std::vector<T> view;   /** Local copy of elements sorted in descending order. */
    std::size_t idx;       /** Current position within the `view`. */

public:
    /**
     * @brief Constructor initializing the iterator with a given view and starting index.
     * @param v   A vector representing the traversal view (already sorted or ready to be sorted).
     * @param i   The starting index (defaults to 0).
     */
    DescendingOrder(std::vector<T> v, std::size_t i = 0)
        : view(std::move(v)),  /** Move `v` into `view` to avoid copying. */
          idx(i)               /** Initialize the iterator position to `i`. */
    {}

    /**
     * @brief Factory function that constructs a begin/end pair for descending traversal.
     * @param c   The source container whose data will be copied and sorted.
     * @return    A pair {begin, end} of DescendingOrder iterators.
     */
    static std::pair<DescendingOrder, DescendingOrder> make(const MyContainer<T>& c) {
        std::vector<T> v = c.getData();              /** Copy elements from the source container. */
        std::sort(v.begin(), v.end(), std::greater<T>()); /**  Sort in descending order. */
        return {
            DescendingOrder(v, 0),                   /**  Iterator pointing to the first (largest) element. */
            DescendingOrder(v, v.size())             /**  Iterator pointing one past the last element. */
        };
    }

    /**
     * @brief Dereference operator providing access to the current element.
     * @return A constant reference to the element currently pointed to by the iterator.
     * @throws std::out_of_range if the iterator index is invalid (guarded via `vector::at`).
     */
    const T& operator*() const { return view.at(idx); }

    /**
     * @brief Arrow operator providing pointer-like access to members of the current element.
     * @return A pointer to the current element in the `view`.
     * @throws std::out_of_range if the index is invalid (guarded via `vector::at`).
     */
    const T* operator->() const { return &view.at(idx); }

    /**
     * @brief Prefix increment operator (advances first, then returns this iterator).
     * @return Reference to this iterator after advancing.
     */
    DescendingOrder& operator++() { ++idx; return *this; }

    /**
     * @brief Postfix increment operator (returns a copy, then advances).
     * @return Iterator state before advancing.
     */
    DescendingOrder operator++(int) {
        DescendingOrder tmp = *this; /** Save current state before advancing. */
        ++(*this);                   /** Reuse prefix version to increment position. */
        return tmp;                  /** Return the saved copy. */
    }

    /**
     * @brief Equality comparison operator.
     * @param other Another DescendingOrder iterator.
     * @return true if both iterators refer to the same view and index.
     */
    bool operator==(const DescendingOrder& other) const {
        return idx == other.idx && view == other.view;
    }

    /**
     * @brief Inequality comparison operator.
     * @param other Another DescendingOrder iterator.
     * @return true if the two iterators differ (negation of operator==).
     */
    bool operator!=(const DescendingOrder& other) const { return !(*this == other); }
};

} 
