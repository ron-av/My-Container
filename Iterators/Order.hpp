#pragma once
#include <vector>   
#include <cstddef>   

namespace ex4 {     

    template <typename T> class MyContainer;

/**
 * @class Order
 * @brief A simple iterator that traverses elements in the order they were originally inserted.
 *
 * Overview:
 *  - This iterator is essentially a lightweight wrapper around a vector copy (`view`) of the container’s data.
 *  - The traversal order is identical to the insertion order.
 *  - Maintains a single index (`idx`) pointing to the current position.
 *  - Implements basic forward-iteration behavior: dereference, prefix/postfix ++, equality/inequality.
 */
template <typename T>
class Order {
    std::vector<T> view;  /** Local copy of container elements in insertion order. */
    std::size_t idx;      /** Current position index within the view (0..view.size()). */

public:
    /**
     * @brief Constructor initializing the iterator with a given view and start index.
     * @param v  A vector representing the elements to iterate over.
     * @param i  Starting index (defaults to 0).
     */
    Order(std::vector<T> v, std::size_t i = 0)
        : view(std::move(v)),  /** Move-construct local view to avoid unnecessary copying. */
          idx(i)               /** Initialize the current index. */
    {}

    /**
     * @brief Factory function that constructs a begin/end pair of iterators for normal order traversal.
     * @param c  The source container to extract data from.
     * @return   A pair {begin, end} representing the traversal range.
     *
     */
    static std::pair<Order, Order> make(const MyContainer<T>& c) {
        std::vector<T> v = c.getData();          /** Copy container elements in their original order. */
        return { 
            Order(v, 0),                         /** Begin iterator (first element). */
            Order(v, v.size())                   /** End iterator (past last element). */
        };
    }

    /**
     * @brief Dereference operator providing read-only access to the current element.
     * @return Constant reference to the current element.
     * @throws std::out_of_range if the index is invalid (guarded by `vector::at`).
     *
     */
    const T& operator*() const { return view.at(idx); }

    /**
     * @brief Arrow operator providing pointer-style access to the current element.
     * @return Pointer to the current element in the view.
     * @throws std::out_of_range if index is invalid.
     *
     * Example:
     *  @code
     *  it->someMethod(); // if T is a class with methods
     *  @endcode
     */
    const T* operator->() const { return &view.at(idx); }

    /**
     * @brief Prefix increment operator.
     * @return Reference to this iterator after advancing.
     *
     * Advances the iterator to the next element in the sequence.
     */
    Order& operator++() { ++idx; return *this; }

    /**
     * @brief Postfix increment operator.
     * @return A copy of the iterator before it was advanced.
     *
     * Returns the previous state, then advances the iterator by one position.
     */
    Order operator++(int) {
        Order tmp = *this;   /** Save current state. */
        ++(*this);           /** Advance to next position. */
        return tmp;          /** Return saved iterator. */
    }

    /**
     * @brief Equality comparison operator.
     * @param other Another Order iterator.
     * @return true if both iterators refer to the same view and same position.
     *
     */
    bool operator==(const Order& other) const {
        return idx == other.idx && view == other.view;
    }

    /**
     * @brief Inequality comparison operator.
     * @param other Another Order iterator.
     * @return true if iterators are not equal.
     */
    bool operator!=(const Order& other) const { return !(*this == other); }
};

}
