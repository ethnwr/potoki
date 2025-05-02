#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <random>
#include <numeric> 

using namespace std;

const int BUFFER_SIZE = 5;
const int NUM_ITEMS = 20;

// Буфер
queue<int> buffer;
mutex mtx;
condition_variable cv;

//рандомайзер
random_device rd;
mt19937 gen(rd());
uniform_int_distribution<> distrib(1, 100);

class Proizv {
public:
    void produce() {
        for (int i = 0; i < NUM_ITEMS; ++i) {
            int item = distrib(gen); 

            unique_lock<mutex> lock(mtx); 
            cv.wait(lock, [&]() { return buffer.size() < BUFFER_SIZE; }); 

            buffer.push(item);
            cout << "|Производитель: произведено " << item<<"|" << endl;

            cv.notify_one(); 
        }
    }
};

class Potreb {
public:
    void consume() {
        int total = 0;
        int count = 0;
        while (count < NUM_ITEMS) {
            unique_lock<mutex> lock(mtx); 
            cv.wait(lock, [&]() { return !buffer.empty() || count == NUM_ITEMS; }); 

            if (buffer.empty() && count == NUM_ITEMS) break; 

            int item = buffer.front();
            buffer.pop();
            cout << "||Потребитель: потреблено " << item <<"||" << endl;
            total += item;
            count++;

            cv.notify_one(); 
        }

        cout << "Потребитель: Среднее значение = " << (double)total / NUM_ITEMS << endl;
    }
};


int main() {
    setlocale(LC_ALL, "ru");
    Proizv producer;
    Potreb consumer;

    thread producerThread(&Proizv::produce, &producer); 
    thread consumerThread(&Potreb::consume, &consumer); 
    
    producerThread.join(); 
    consumerThread.join(); 

    return 0;
}
