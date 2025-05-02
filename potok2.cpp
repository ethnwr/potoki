#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <algorithm>

using namespace std;

vector<int> sharedArray;
mutex arrayMutex;

void addElements(int start, int end) {
    for (int i = start; i <= end; ++i) {
        arrayMutex.lock();
        sharedArray.push_back(i);
        cout << "Добавлено: " << i << ", размер массива: " << sharedArray.size() << endl;
        arrayMutex.unlock();
    }
}

void removeElements() {
    while (true) {
        arrayMutex.lock();
        if (!sharedArray.empty()) {
            int removedValue = sharedArray.front();
            sharedArray.erase(sharedArray.begin());
            cout << "Удалено: " << removedValue << ", размер массива: " << sharedArray.size() << endl;
        }
        else {
            arrayMutex.unlock(); 
            break;
        }
        arrayMutex.unlock();
    }
}


int main() {
    setlocale(LC_ALL, "ru");
    thread adderThread(addElements, 1, 20); 
    thread removerThread(removeElements);    

    adderThread.join();
    removerThread.join();

    cout << "Программа завершена." << endl;

    return 0;
}
