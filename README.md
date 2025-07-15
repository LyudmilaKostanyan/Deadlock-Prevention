# Deadlock Prevention

## Overview

This project demonstrates multithreaded synchronization in C++ using `std::mutex`, `std::lock_guard`, and `std::scoped_lock`. It intentionally introduces and resolves both traditional and logical deadlocks, and serves as a practical lab for studying threading behavior, deadlock patterns, and safe resource access in concurrent systems.

## Problem Description

Concurrent programs often manipulate shared resources (e.g., variables, buffers). Without proper synchronization, multiple threads can access and modify shared memory simultaneously, causing **data races** or even **deadlocks**. These bugs are difficult to reproduce and debug.

This project illustrates:

* Correct usage of `std::mutex` to protect critical sections
* Deadlocks caused by locking order violations
* Logic-based deadlocks without mutexes
* Safe multi-mutex locking using `std::scoped_lock`

## Explanation of Key Concepts

### Data Race

A **data race** occurs when two or more threads access the same memory location concurrently, and **at least one of them performs a write**, and the threads are not synchronized. This leads to undefined behavior. Mutexes are used to avoid data races by serializing access to shared resources.

### `std::lock_guard` vs `std::scoped_lock`

| Feature            | `std::lock_guard`        | `std::scoped_lock`                 |
| ------------------ | ------------------------ | ---------------------------------- |
| Introduced in      | C++11                    | C++17                              |
| Purpose            | Lock one mutex           | Lock multiple mutexes safely       |
| Syntax             | `std::lock_guard<mutex>` | `std::scoped_lock<mutex1, mutex2>` |
| Deadlock Avoidance | Manual lock ordering     | Automatically locks in order       |
| Use Case           | Single mutex usage       | Multiple mutexes (deadlock-prone)  |

Use `std::scoped_lock` when you need to lock **two or more mutexes** at once — it guarantees deadlock-free locking.

## Example Output

```text
Choose mode:
1 = safe_mutex
2 = mutex_deadlock
3 = scoped_lock
4 = logic_deadlock (no mutex)

Final counter = 2000000
Elapsed time = 0.242789 seconds
```

## Explanation of Output

* Based on the selected mode, the program spawns threads that perform concurrent tasks.
* If the code path is correct, you will see the final value of the shared counter and elapsed time.
* In modes `2` and `4`, the program will **hang** due to deadlock unless stopped manually or by a CI timeout.

## How to Compile and Run

### 1. Clone the Repository

```bash
git clone https://github.com/LyudmilaKostanyan/Deadlock-Prevention.git
cd Deadlock-Prevention
```

### 2. Build the Project

Ensure you have CMake and a C++ compiler (supporting C++17) installed:

```bash
cmake -S . -B build
cmake --build build
```

### 3. Run the Program

#### For Windows Users

```bash
./build/Debug/Deadlock.exe
```

#### For Linux/macOS Users

```bash
./build/Deadlock
```

## Parameters

The program takes a **number from stdin** at runtime to choose the test mode:

| Mode | Description                                          |
| ---- | ---------------------------------------------------- |
| 1    | Safe mutex usage with no deadlocks                   |
| 2    | Intentional deadlock using two mutexes               |
| 3    | Safe multi-mutex usage via `std::scoped_lock`        |
| 4    | Logical deadlock using `bool` flags and busy waiting |
