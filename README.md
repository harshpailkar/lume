# Lume: High-Performance Limit Order Book (LOB)

**Lume** is a low-latency matching engine core implemented in C++. It manages buy and sell orders using price-time priority, optimized for $O(1)$ order cancellation and $O(\log n)$ limit price insertion.

## 🚀 Key Features

- **Price-Time Priority:** Implements standard exchange matching logic where orders are filled first by the best price, then by the time they were received (FIFO).
- **Fixed-Point Arithmetic:** Uses `int64_t` for price representation to avoid floating-point precision errors common in financial systems.
- **Constant-Time Cancellation:** Utilizes an `unordered_map` to store iterators, allowing orders to be found and removed in $O(1)$ time.
- **Memory Efficient:** Minimizes copies by using pointers to `LimitLevel` structures and `std::list` for stable memory addressing.

## 🏗️ Architecture & Data Structures

| Component | Data Structure | Complexity | Purpose |
| :--- | :--- | :--- | :--- |
| **Bids Side** | `std::map<Price, Level, Greater>` | $O(\log n)$ | Keeps buy orders sorted highest-to-lowest. |
| **Asks Side** | `std::map<Price, Level, Less>` | $O(\log n)$ | Keeps sell orders sorted lowest-to-highest. |
| **Price Level** | `std::list<Order>` | $O(1)$ | Maintains FIFO time priority at a specific price. |
| **Lookup Map** | `std::unordered_map<ID, Entry>` | $O(1)$ | Maps Order IDs to their exact memory location. |

## 🛠️ Getting Started

### Prerequisites
- A C++17 compatible compiler (GCC, Clang, or MSVC).

### Compilation
Use the `-O3` flag for maximum compiler optimization:
```bash
g++ -O3 main.cpp lume.cpp -o lume_engine
