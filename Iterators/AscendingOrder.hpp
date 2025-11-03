#pragma once
#include <vector>    
#include <algorithm>   
#include <cstddef>    

namespace ex4 {      

template <typename T> class MyContainer;

/**
 * @class AscendingOrder
 * @brief Lightweight forward iterator over a *snapshot* of the container arranged in ascending order.
 *
* Overview:
 *  - This iterator maintains a local copy (`view`) of the container's elements sorted in ascending order.
 *  - It uses an index (`idx`) to track the current position during traversal.
 *  - It uses an index (`idx`) to track the current position during traversal.
 */
template <typename T>
class AscendingOrder {
private:
    std::vector<T> view;  /** Local, sorted copy of the container’s elements. */
    std::size_t idx;      /** Current position within `view` (0..view.size()). */

public:
    /**
     * @brief Constructs an iterator over a given view starting at position `i`.
     * @param v   The traversal view (already sorted or to-be-sorted by the caller).
     * @param i   Starting index (defaults to 0; `view.size()` typically marks the end).
     */
    AscendingOrder(std::vector<T> v, std::size_t i = 0)
        : view(std::move(v)),  /** Move-construct the local view to avoid copying. */
          idx(i)               /** Initialize the current index. */
    {}

    /**
     * @brief Factory that builds a begin/end pair for ascending traversal of a MyContainer.
     * @param c   The source container.
     * @return    {begin, end} pair of AscendingOrder iterators.
     */
    static std::pair<AscendingOrder, AscendingOrder> make(const MyContainer<T>& c) {
        std::vector<T> v = c.getData();          /** Copy current data from the container. */
        std::sort(v.begin(), v.end());           /**  Sort ascending in-place. */
        return {
            AscendingOrder(v, 0),                /**  Begin: points to the first element. */
            AscendingOrder(v, v.size())          /**  End: points one-past-the-last element. */
        };
    }

    /**
     * @brief Dereference operator (read-only).
     * @return A const reference to the current element.
     * @throws std::out_of_range if `idx` is not a valid position (guards via `vector::at`).
     */
    const T& operator*() const { return view.at(idx); }

    /**
     * @brief Arrow operator (read-only).
     * @return Pointer to the current element.
     * @throws std::out_of_range if `idx` is out of bounds (guards via `vector::at`).
     */
    const T* operator->() const { return &view.at(idx); }

    /**
     * @brief Prefix increment: advance to the next element, then return *this.
     * @return Reference to this iterator after advancing.
     */
    AscendingOrder& operator++() { ++idx; return *this; }

    /**
     * @brief Postfix increment: return a copy of the current iterator, then advance *this.
     * @return The iterator state prior to incrementing.
     */
    AscendingOrder operator++(int) {
        AscendingOrder tmp = *this;  /** Save current state. */
        ++(*this);                   /** Reuse prefix increment to advance. */
        return tmp;                  /** Return the prior state. */
    }

    /**
     * @brief Equality comparison.
     * @param other Another AscendingOrder iterator.
     * @return true if both iterators refer to the same view and index.
     */
    bool operator==(const AscendingOrder& other) const {
        return idx == other.idx && view == other.view;
    }

    /**
     * @brief Inequality comparison (logical negation of operator==).
     * @param other Another AscendingOrder iterator.
     * @return true if iterators are not equal.
     */
    bool operator!=(const AscendingOrder& other) const { return !(*this == other); }
};

} 
