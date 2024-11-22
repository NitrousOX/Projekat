#ifndef CIRCULAR_BUFFER_HPP
#define CIRCULAR_BUFFER_HPP

#include <mutex>
#include <condition_variable>
#include "ClientRequest.h"
#include <array>

using namespace std;

class CircularBuffer {
private:
    array<ClientRequest, 10> mainBuffer;  // Fixed-size buffer of ClientRequest objects
    size_t head = 0;                      // Points to the next position to remove data (oldest)
    size_t tail = 0;                      // Points to the next position to add data (newest)
    bool full = false;                    // Flag to indicate if the buffer is full
    mutex mtx;                            // Mutex for thread safety
    condition_variable cv;                // Condition variable for synchronization

public:
    CircularBuffer() {}

    // Function to check if the buffer is full
    bool isFull() const {
        return full;
    }

    // Function to check if the buffer is empty
    bool isEmpty() const {
        return (!full && (head == tail));
    }

    // Function to add a ClientRequest to the buffer
    void add(const ClientRequest& request) {
        unique_lock<mutex> lock(mtx);  // Lock the mutex for thread-safe access

        // If the buffer is full, overwrite the oldest data (move the head forward)
        if (full) {
            head = (head + 1) % mainBuffer.size();  // Move the head forward to overwrite
        }

        mainBuffer[tail] = request;  // Add the request to the current position
        tail = (tail + 1) % mainBuffer.size();  // Move the tail forward

        // If the tail caught up with the head, the buffer is full
        if (tail == head) {
            full = true;
        }

        // Notify any waiting threads that data is available
        cv.notify_all();
    }
};

#endif // CIRCULAR_BUFFER_HPP
