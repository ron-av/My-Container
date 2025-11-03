#pragma once
#include <vector>   
#include <cstddef>  

namespace ex4 { 

template <typename T> class MyContainer;

/**
 * @class MiddleOutOrder
 * @brief Iterator that traverses a container in a "middle-out" pattern.
 * Overview:
 * - Starts from the middle element of the container.
 * - Then alternates moving left and right from the center, visiting elements outward.
 * - Continues until all elements have been visited.
 * 
 */
template <typename T>
class MiddleOutOrder {
    std::vector<T> view;  /** Local view of the container's elements arranged in middle-out order. */
    std::size_t idx;      /** Current traversal index (0..view.size()). */

public:
    /**
     * @brief Constructs an iterator using a prepared traversal view and starting position.
     * @param v  Vector representing the traversal order of elements.
     * @param i  Starting index (defaults to 0 for begin, `view.size()` for end).
     *
     */
    MiddleOutOrder(std::vector<T> v, std::size_t i = 0)
        : view(std::move(v)),  /** Move-construct the local traversal view. */
          idx(i)               /** Initialize traversal index. */
    {}

    /**
     * @brief Factory function to create a begin/end pair of middle-out iterators.
     * @param c  The container whose data will be used to build the traversal sequence.
     * @return   A pair {begin, end} representing the start and end iterators.
     */
    static std::pair<MiddleOutOrder, MiddleOutOrder> make(const MyContainer<T>& c) {
        const std::vector<T>& base = c.getData();      /**  Retrieve the base data. */
        const std::size_t n = base.size();             /** Number of elements in the container. */
        std::vector<T> seq;                            /** Sequence to hold the middle-out traversal. */
        seq.reserve(n);                                /** Reserve space to avoid reallocations. */

        if (n == 0)
            return { MiddleOutOrder(seq, 0), MiddleOutOrder(seq, 0) }; /** Empty container → both begin and end are same. */

        long mid   = static_cast<long>((n - 1) / 2);   /**  Compute middle index (lower middle if even). */
        long left  = mid - 1;                          /**  Initialize left pointer one step before middle. */
        long right = mid + 1;                          /**  Initialize right pointer one step after middle. */

        seq.push_back(base[static_cast<std::size_t>(mid)]); /**  Start with the middle element. */

        bool take_left = true;                         /** Flag to alternate between left and right sides. */

        while (seq.size() < n) {
            if (take_left && left >= 0) {              /** Take from left if available. */
                seq.push_back(base[static_cast<std::size_t>(left--)]);
            } else if (!take_left && right < static_cast<long>(n)) { /** Take from right if available. */
                seq.push_back(base[static_cast<std::size_t>(right++)]);
            }

            if (left < 0)
                take_left = false;                     /** No more left elements → only take from right. */
            else if (right >= static_cast<long>(n))
                take_left = true;                      /** No more right elements → only take from left. */
            else
                take_left = !take_left;                /** Otherwise, continue alternating sides. */
        }

        return {
            MiddleOutOrder(seq, 0),                    /** Begin iterator (first element). */
            MiddleOutOrder(seq, seq.size())            /** End iterator (one past last element). */
        };
    }

    /**
     * @brief Dereference operator to access the current element.
     * @return A const reference to the element currently pointed to.
     * @throws std::out_of_range if `idx` is not a valid index (checked via `std::vector::at`).
     *
     */
    const T& operator*() const { return view.at(idx); }

    /**
     * @brief Arrow operator for pointer-like access to members of the element.
     * @return Pointer to the current element.
     * @throws std::out_of_range if `idx` is invalid (checked via `at`).
     *
     */
    const T* operator->() const { return &view.at(idx); }

    /**
     * @brief Prefix increment operator.
     * @return Reference to this iterator after advancing.
     *
     * Moves the iterator to the next element in the middle-out sequence.
     */
    MiddleOutOrder& operator++() { ++idx; return *this; }

    /**
     * @brief Postfix increment operator.
     * @return Copy of the iterator state before incrementing.
     *
     * Used when you need the current value and then move to the next one.
     */
    MiddleOutOrder operator++(int) {
        MiddleOutOrder tmp = *this; /** Store current iterator state. */
        ++(*this);                  /** Advance to next element. */
        return tmp;                 /** Return the old iterator. */
    }

    /**
     * @brief Equality comparison operator.
     * @param other Another MiddleOutOrder iterator.
     * @return true if both iterators refer to the same view and index.
     *
     * Used in loop termination (e.g., `for (it != end)`).
     */
    bool operator==(const MiddleOutOrder& other) const {
        return idx == other.idx && view == other.view;
    }

    /**
     * @brief Inequality comparison operator.
     * @param other Another MiddleOutOrder iterator.
     * @return true if iterators are not equal (logical negation of operator==).
     */
    bool operator!=(const MiddleOutOrder& other) const { return !(*this == other); }
};

} 
