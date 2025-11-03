#pragma once
#include <vector>      
#include <stdexcept>   
#include <cstddef>    
#include "Iterators/Order.hpp"          
#include "Iterators/ReverseOrder.hpp"   
#include "Iterators/AscendingOrder.hpp" 
#include "Iterators/DescendingOrder.hpp"
#include "Iterators/SideCrossOrder.hpp" 
#include "Iterators/MiddleOutOrder.hpp" 

namespace ex4 {  

/**
 * @class MyContainer
 * @brief Simple generic container with add/remove/size and multiple traversal orders.
 *
 * Notes:
 *  - Default template parameter is int, but any comparable T is supported by the sorted iterators.
 *  - Storage preserves insertion order in the `data` vector.
 *  - Iterators are snapshot-based (they receive a copied view when `begin_*` is called).
 */
template <typename T = int>
class MyContainer {
private:
    std::vector<T> data;  /** Underlying storage, preserves insertion order. */

    /**
     * @brief Helper to print elements to an output stream as: "x y z \n".
     * @param os Output stream (defaults to std::cout).
     */
    void print(std::ostream& os = std::cout) const {
        for (const auto& e : data) {
            os << e << ' ';          /** Print each element followed by a space. */
        }
        os << std::endl;             /** End line after printing all elements. */
    }

public:
    /** Default constructor: starts with an empty container. */
    MyContainer() = default;

    /**
     * @brief Append an element to the container (at the end).
     * @param value Value to insert.
     * Complexity: Amortized O(1).
     */
    void addElement(const T& value) { data.push_back(value); }

    /**
     * @brief Remove all occurrences of a given value.
     * @param value Value to remove (all duplicates removed).
     * @throws std::runtime_error if the value does not exist in the container.
     * Complexity: O(n) (linear scan + erase).
     */
    void removeElement(const T& value) {
        auto before = data.size();                                             /** Remember size before removal. */
        data.erase(std::remove(data.begin(), data.end(), value), data.end());  /** Erase all matches. */
        if (data.size() == before) {                                           /** If size unchanged → nothing removed. */
            throw std::runtime_error("This element does not exist in the container");
        }
    }

    /**
     * @brief Current number of elements in the container.
     * @return size in elements.
     */
    std::size_t size() const { return data.size(); }

    /**
     * @brief Stream insertion operator (prints elements separated by spaces, ends with newline).
     * @tparam U Element type of the container being printed.
     * @param os Output stream.
     * @param c  Container to print.
     * @return The same output stream (for chaining).
     */
    template <typename U>
    friend std::ostream& operator<<(std::ostream& os, const MyContainer<U>& c);

    /**
     * @brief Read-only access to the underlying storage (used by iterator factories).
     * @return const reference to internal std::vector<T>.
     *
     * Design note: Iterators copy from this vector to build their own traversal views.
     */
    const std::vector<T>& getData() const { return data; }

    // ===== Iterator entry points (each returns a snapshot-based iterator) =====

    /** @return begin/end for insertion order traversal. */
    Order<T> begin_order() const { return Order<T>::make(*this).first; }
    Order<T> end_order()   const { return Order<T>::make(*this).second; }

    /** @return begin/end for reverse insertion order traversal. */
    ReverseOrder<T> begin_reverse_order() const { return ReverseOrder<T>::make(*this).first; }
    ReverseOrder<T> end_reverse_order()   const { return ReverseOrder<T>::make(*this).second; }

    /** @return begin/end for ascending sorted traversal. */
    AscendingOrder<T> begin_ascending_order() const { return AscendingOrder<T>::make(*this).first; }
    AscendingOrder<T> end_ascending_order()   const { return AscendingOrder<T>::make(*this).second; }

    /** @return begin/end for descending sorted traversal. */
    DescendingOrder<T> begin_descending_order() const { return DescendingOrder<T>::make(*this).first; }
    DescendingOrder<T> end_descending_order()   const { return DescendingOrder<T>::make(*this).second; }

    /** @return begin/end for alternating low–high traversal. */
    SideCrossOrder<T> begin_side_cross_order() const { return SideCrossOrder<T>::make(*this).first; }
    SideCrossOrder<T> end_side_cross_order()   const { return SideCrossOrder<T>::make(*this).second; }

    /** @return begin/end for middle-out traversal (center, then left/right alternating). */
    MiddleOutOrder<T> begin_middle_out_order() const { return MiddleOutOrder<T>::make(*this).first; }
    MiddleOutOrder<T> end_middle_out_order()   const { return MiddleOutOrder<T>::make(*this).second; }
};

/** Out-of-class definition of operator<< (calls the private print helper). */
template <typename T>
std::ostream& operator<<(std::ostream& os, const MyContainer<T>& c) {
    c.print(os);
    return os;
}

} 
