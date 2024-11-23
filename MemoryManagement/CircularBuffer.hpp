#ifndef CIRCULAR_BUFFER_HPP
#define CIRCULAR_BUFFER_HPP

#include <mutex>
#include <condition_variable>
#include "ClientRequest.h"
#include <array>

using namespace std;

class CircularBuffer {
private:
    array<ClientRequest, 10> mainBuffer;  
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

    void add(const ClientRequest& request) {
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
        while (index != tail) {
            cout << "Request at index " << index << ": " << mainBuffer[index] << endl;
            index = (index + 1) % mainBuffer.size();  // Move to the next element in a circular manner
        }
        if (full) {
            cout << "Buffer is full!" << endl;
        }
    }


};

#endif
