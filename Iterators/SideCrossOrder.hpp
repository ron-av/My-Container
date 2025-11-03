#pragma once
#include <vector>     
#include <algorithm>  
#include <cstddef>    

namespace ex4 {    

template <typename T> class MyContainer;

/**
 * @class SideCrossOrder
 * @brief Iterator that traverses a container in an alternating low–high pattern (side-cross order).
 *
 * Overview:
 *  - Start from the smallest element (after sorting ascending).
 *  - Then move to the largest element.
 *  - Then the second smallest, then the second largest, and so on...
 *  - Continue alternating sides until all elements are visited.
 */
template <typename T>
class SideCrossOrder {
    std::vector<T> view;  /** Local traversal view in side-cross order. */
    std::size_t idx;      /** Current traversal position (0..view.size()). */

public:
    /**
     * @brief Constructor initializing the iterator with a traversal view and start index.
     * @param v  Vector of elements representing traversal order.
     * @param i  Starting index (default = 0 for begin).
     *
     * Moves `v` into the iterator for efficiency (no deep copy).
     */
    SideCrossOrder(std::vector<T> v, std::size_t i = 0)
        : view(std::move(v)),  /** Move-construct local traversal view. */
          idx(i)               /** Initialize iterator position. */
    {}

    /**
     * @brief Factory function constructing begin/end iterators for side-cross traversal.
     * @param c  The source container.
     * @return   A pair {begin, end} of SideCrossOrder iterators.
     *
     * Steps:
     *  1) Copy the container’s data.
     *  2) Sort it in ascending order.
     *  3) Initialize two indices:
     *       - i → beginning (lowest value)
     *       - j → end (highest value)
     *  4) Alternate pushing elements from low and high ends into a result vector.
     *  5) Stop when all elements have been included.
     *
     * Time Complexity: O(n log n) (due to sorting)
     * Space Complexity: O(n) for the traversal vector.
     */
    static std::pair<SideCrossOrder, SideCrossOrder> make(const MyContainer<T>& c) {
        std::vector<T> sorted = c.getData();           /** 1) Copy all elements from the source container. */
        std::sort(sorted.begin(), sorted.end());       /** 2) Sort ascending. */

        std::vector<T> seq;                            /** Vector to store the final traversal order. */
        seq.reserve(sorted.size());                    /** Reserve space for efficiency. */

        std::size_t i = 0;                             /** 3a) Left (low) index. */
        std::size_t j = sorted.size() ? sorted.size() - 1 : 0; /** 3b) Right (high) index (handle empty case). */
        bool take_low = true;                          /** 4) Flag controlling which side to take from. */

        /** 5) Alternate between low and high until indices meet. */
        while (!sorted.empty() && i <= j) {
            if (take_low)
                seq.push_back(sorted[i++]);            /** Take from the low end (ascending). */
            else
                seq.push_back(sorted[j--]);            /** Take from the high end (descending). */
            take_low = !take_low;                      /** Flip the flag for next iteration. */
        }

        /** 6) Return begin/end iterator pair built from the resulting sequence. */
        return {
            SideCrossOrder(seq, 0),                    /** Begin iterator (first element). */
            SideCrossOrder(seq, seq.size())            /** End iterator (one past the last). */
        };
    }

    /**
     * @brief Dereference operator providing access to the current element.
     * @return Constant reference to the current element.
     * @throws std::out_of_range if the index is invalid (checked via `vector::at`).
     *
     */
    const T& operator*() const { return view.at(idx); }

    /**
     * @brief Arrow operator providing pointer-style access.
     * @return Pointer to the current element.
     * @throws std::out_of_range if index is invalid.
     *
     * Enables syntax like `it->member`.
     */
    const T* operator->() const { return &view.at(idx); }

    /**
     * @brief Prefix increment operator.
     * @return Reference to this iterator after advancing to the next element.
     *
     * Example usage:
     *  @code
     *  ++it;
     *  @endcode
     */
    SideCrossOrder& operator++() { ++idx; return *this; }

    /**
     * @brief Postfix increment operator.
     * @return Copy of the iterator before it was advanced.
     *
     * Example usage:
     *  @code
     *  it++;
     *  @endcode
     */
    SideCrossOrder operator++(int) {
        SideCrossOrder tmp = *this; /** Save current iterator state. */
        ++(*this);                  /** Advance to next element. */
        return tmp;                 /** Return saved iterator. */
    }

    /**
     * @brief Equality comparison operator.
     * @param other Another SideCrossOrder iterator.
     * @return true if both iterators refer to the same view and index.
     *
     * Used for loop termination conditions like:
     *  @code
     *  for (auto it = begin; it != end; ++it)
     *  @endcode
     */
    bool operator==(const SideCrossOrder& other) const {
        return idx == other.idx && view == other.view;
    }

    /**
     * @brief Inequality comparison operator.
     * @param other Another SideCrossOrder iterator.
     * @return true if the two iterators are not equal.
     */
    bool operator!=(const SideCrossOrder& other) const { return !(*this == other); }
};

} 
