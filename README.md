# 🧩 MyContainer Project  
---

## 📘 Project Overview  
This project implements a **generic container** (`MyContainer`) that can hold elements of any comparable type (e.g. `int`, `double`, `std::string`, or custom structs).  
The container supports **dynamic addition and removal** of elements, and provides several types of **iterators** that allow traversal of the elements in different logical orders.  

The main goal of this assignment is to demonstrate understanding of **C++ templates**, **iterators**, **functors**, and **containers**, as covered in lectures 7–10.

---

## 🎯 Main Objectives  
- Implement a **templated container** supporting any comparable type.  
- Implement **six different iterators**:
  1. **OrderIterator** – Traverse in insertion order.  
  2. **ReverseOrderIterator** – Traverse in reverse insertion order.  
  3. **AscendingOrderIterator** – Traverse elements sorted ascending.  
  4. **DescendingOrderIterator** – Traverse elements sorted descending.  
  5. **SideCrossOrderIterator** – Alternate smallest/largest elements.  
  6. **MiddleOutOrderIterator** – Start from middle, then expand left/right.  
- Add tests for each iterator and for core container functionality.  
- Check program correctness and memory safety using **Valgrind**.

---

## 🧱 Project Structure  

├── Iterators/
│ ├── AscendingOrder.hpp
│ ├── DescendingOrder.hpp
│ ├── SideCrossOrder.hpp
│ ├── ReverseOrder.hpp
│ ├── MiddleOutOrder.hpp
│ └── Order.hpp
│
├── MyContainer.hpp # Main container class template
├── Main.cpp # Demo program
├── tests.cpp # Unit tests (using doctest)
├── makefile # Build automation
└── README.md # Documentation (this file)


---

## ⚙️ Class Overview  

### 🧩 `MyContainer<T>`  
A generic container that stores elements in a dynamic `std::vector<T>`.  
Default type: `int`.  

**Public Methods:**
| Method | Description |
|---------|--------------|
| `addElement(const T& value)` | Adds a new element to the container. |
| `removeElement(const T& value)` | Removes all occurrences of a given element; throws if not found. |
| `size() const` | Returns the current number of elements. |
| `getData() const` | Returns a const reference to the internal vector. |
| `operator<<` | Prints all elements separated by spaces and a newline. |

**Iterators Provided:**
Each iterator is defined as a separate class in the `Iterators/` folder.  
The container exposes `begin_...()` and `end_...()` methods for each one.

| Iterator Type | File | Behavior |
|----------------|------|-----------|
| `Order` | `Iterators/Order.hpp` | Original insertion order |
| `ReverseOrder` | `Iterators/ReverseOrder.hpp` | Reverse of insertion order |
| `AscendingOrder` | `Iterators/AscendingOrder.hpp` | Sorted ascending |
| `DescendingOrder` | `Iterators/DescendingOrder.hpp` | Sorted descending |
| `SideCrossOrder` | `Iterators/SideCrossOrder.hpp` | Smallest → largest → next smallest... |
| `MiddleOutOrder` | `Iterators/MiddleOutOrder.hpp` | Starts at middle → alternates left/right |

---

## 🧪 Testing

All functionality is tested using **doctest** framework in `tests.cpp`.  
The tests cover:
- Adding and removing elements  
- Correct `size()` behavior  
- Iterator traversal correctness  
- Empty container and single-element edge cases  
- Handling of duplicates and negative values  
- Basic string-type compatibility  

## ▶️ Running the Project

### Requirements
- **g++** compiler supporting **C++17** or higher  
- **valgrind** *(optional, for memory leak analysis)*  
- **doctest** *(header-only testing framework, included with the project)*  

---

### Build and Run
Use the provided **Makefile** to compile and run the project easily:

```bash
# Compile and run the demo program (Main.cpp)
make Main

# Compile and run all unit tests (test.cpp)
make test

# Run Valgrind to check for memory leaks across the whole project
make valgrind

# Clean build and binary files
make clean
```

## Authors:
- Ron Avraham
- ronavraham1999@gmail.com

- Michael Idan
- michael9090124@gmail.com