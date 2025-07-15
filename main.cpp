#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <atomic>

std::mutex mutex1;
std::mutex mutex2;
int counter = 0;
bool flag1 = false;
bool flag2 = false;

// TASK 1: Basic mutex-protected increment
void safe_increment_single_mutex() {
    for (int i = 0; i < 1'000'000; ++i) {
        std::lock_guard<std::mutex> lock(mutex1);
        ++counter;
    }
}

// TASK 2: Intentionally causes deadlock with two mutexes
void thread1_deadlock() {
    std::lock_guard<std::mutex> lock1(mutex1);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::lock_guard<std::mutex> lock2(mutex2);
    ++counter; // unreachable if deadlock
}

void thread2_deadlock() {
    std::lock_guard<std::mutex> lock1(mutex2);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::lock_guard<std::mutex> lock2(mutex1);
    ++counter; // unreachable if deadlock
}

// TASK 2b: Deadlock without mutexes (logical deadlock using flags)
void thread1_logic_deadlock() {
    while (!flag2); // busy wait for flag2
    flag1 = true;
    ++counter;
}

void thread2_logic_deadlock() {
    while (!flag1); // busy wait for flag1
    flag2 = true;
    ++counter;
}

// TASK 3: Fix deadlock using scoped_lock
void thread1_fixed() {
    std::scoped_lock lock(mutex1, mutex2);
    for (int i = 0; i < 1'000'000; ++i) {
        ++counter;
    }
}

void thread2_fixed() {
    std::scoped_lock lock(mutex1, mutex2);
    for (int i = 0; i < 1'000'000; ++i) {
        ++counter;
    }
}

int main() {
    std::cout << "Choose mode:\n"
              << "1 = safe_mutex\n"
              << "2 = mutex_deadlock\n"
              << "3 = scoped_lock\n"
              << "4 = logic_deadlock (no mutex)\n";
    int mode;
    std::cin >> mode;

    auto start = std::chrono::high_resolution_clock::now();
    std::thread t1, t2;

    switch (mode) {
        case 1:
            t1 = std::thread(safe_increment_single_mutex);
            t2 = std::thread(safe_increment_single_mutex);
            break;
        case 2:
            t1 = std::thread(thread1_deadlock);
            t2 = std::thread(thread2_deadlock);
            break;
        case 3:
            t1 = std::thread(thread1_fixed);
            t2 = std::thread(thread2_fixed);
            break;
        case 4:
            t1 = std::thread(thread1_logic_deadlock);
            t2 = std::thread(thread2_logic_deadlock);
            break;
        default:
            std::cerr << "Invalid mode.\n";
            return 1;
    }

    t1.join();
    t2.join();
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;
    std::cout << "\nFinal counter = " << counter << "\n";
    std::cout << "Elapsed time = " << elapsed.count() << " seconds\n";

    return 0;
}