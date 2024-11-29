#ifndef CIRCULAR_BUFFER_HPP
#define CIRCULAR_BUFFER_HPP

#include <mutex>
#include <condition_variable>
#include "ExtendedRequest.hpp"
#include <array>

using namespace std;

class CircularBuffer {
private:
    array<ExtendedClientRequest, 10> mainBuffer;  
    size_t head = 0;                     
    size_t tail = 0;                     
    bool full = false;                   
    mutex mtx;                            
    condition_variable cv;               

public:
    CircularBuffer() {}

    bool isFull() const {
        return full;
    }

    bool isEmpty() const {
        return (!full && (head == tail));
    }

    void add(const ExtendedClientRequest& request) {
        unique_lock<mutex> lock(mtx); 

        if (full) {
            head = (head + 1) % mainBuffer.size();
        }

        mainBuffer[tail] = request; 
        tail = (tail + 1) % mainBuffer.size(); 

        if (tail == head) {
            full = true;
        }
        cv.notify_all();
    }

    void printBuffer() {
        unique_lock<mutex> lock(mtx);  // Locking the buffer to ensure thread safety.

        if (isEmpty()) {
            cout << "Buffer is empty!" << endl;
            return;
        }

        size_t index = head;
        cout << "-------------------------------------------------------------------" << endl;
        while (index != tail) {
            cout << "Request at index " << index << ": " << mainBuffer[index] << endl;
            index = (index + 1) % mainBuffer.size();  // Move to the next element in a circular manner
        }
        if (full) {
            cout << "Buffer is full!" << endl;
        }
    }

    bool remove(ExtendedClientRequest& request) {
        unique_lock<mutex> lock(mtx);  // Lock the buffer for thread safety.

        // Wait until the buffer is not empty.
        cv.wait(lock, [this]() { return !isEmpty(); });

        if (isEmpty()) {
            return false;  // If the buffer is still empty, return false.
        }

        request = mainBuffer[head];  // Retrieve the request at the head.
        head = (head + 1) % mainBuffer.size();  // Move the head pointer.

        full = false;  // The buffer is no longer full after removing an item.

        // If the buffer becomes empty, we update the full flag.
        if (head == tail) {
            full = false;
        }

        cv.notify_all();  // Notify any threads waiting to add an item to the buffer.
        return true;
    }


};

#endif
