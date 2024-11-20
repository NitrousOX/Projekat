#ifndef CIRCULAR_BUFFER_HPP
#define CIRCULAR_BUFFER_HPP

#include <vector>
#include <mutex>
#include <condition_variable>

using namespace std;

template <typename T>
class CircularBuffer {
private:
    vector<T> buffer;       // Internal storage
    size_t head;                 // Index of the next item to write
    size_t tail;                 // Index of the next item to read
    size_t capacity;             // Total capacity of the buffer
    size_t count;                // Current number of elements in the buffer
    mutex mtx;              // Mutex for thread-safe access
    condition_variable cv; // Condition variable for synchronization

public:
    explicit CircularBuffer(size_t size)
        : buffer(size), head(0), tail(0), capacity(size), count(0) {}

    // Add an item to the buffer
    void add(const T& item) {
        unique_lock<mutex> lock(mtx);

        // Wait until there's space in the buffer
        cv.wait(lock, [this]() { return count < capacity; });

        buffer[head] = item;
        head = (head + 1) % capacity;
        count++;

        // Notify readers
        cv.notify_all();
    }

    // Remove and get an item from the buffer
    T remove() {
        unique_lock<mutex> lock(mtx);

        // Wait until there's data in the buffer
        cv.wait(lock, [this]() { return count > 0; });

        T item = buffer[tail];
        tail = (tail + 1) % capacity;
        count--;

        // Notify writers
        cv.notify_all();

        return item;
    }

    // Check if the buffer is empty
    bool isEmpty() const {
        lock_guard<mutex> lock(mtx);
        return count == 0;
    }

    // Check if the buffer is full
    bool isFull() const {
        lock_guard<mutex> lock(mtx);
        return count == capacity;
    }

    // Get the current size of the buffer
    size_t size() const {
        lock_guard<mutex> lock(mtx);
        return count;
    }
};

#endif // CIRCULAR_BUFFER_HPP
