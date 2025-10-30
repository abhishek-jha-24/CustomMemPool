High-Performance Thread-Safe Memory Pool with Smart Reference Counting in C++
Description:
This project implements a custom memory pool manager in C++ designed for high-performance, multithreaded environments where dynamic memory allocation overhead and contention can significantly affect system performance. The memory pool leverages pre-allocated nodes, manual memory reuse, and thread-safe access using a lightweight spinlock mechanism.

Each memory node in the pool tracks the number of threads accessing it through a smart reference counter (smartCount), similar in concept to shared pointers but with fine-grained control. This allows safe memory sharing across threads and ensures that memory is only returned to the pool once no thread holds a reference to that node.

Key Features:
✅ Custom memory allocator using pre-allocated node pools.

🔁 Smart reference counting to allow memory reuse across threads.

🔐 Thread-safe operations using atomic operations and a custom spinlock.

⚡ Least heap fragmentation and reduced allocation/deallocation latency.

🔧 In-place construction of objects via placement new.

♻️ Automatic memory recycling when smartCount reaches zero.

Use Cases:
High-frequency trading systems

Game engines or real-time simulations

Embedded systems or devices with limited memory

Low-latency distributed services with controlled memory usage

Technologies Used:
C++ (C++11 and above)

Atomic operations (std::atomic)

Custom SpinLock

Manual memory management (placement new, reinterpret_cast)

