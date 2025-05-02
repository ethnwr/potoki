#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

mutex mtx;
condition_variable cv;
bool ready = false;

void workerThread() {
    unique_lock<mutex> lock(mtx);
    cout << "Рабочий поток: Я ЖДУ...\n" << endl;

    cv.wait(lock, [] { return ready; });

    cout << "Рабочий поток: Чиназес. Продолжаю работу." << endl;
    for (int i = 0; i < 5; ++i) {
        cout << "Рабочий поток:обрабатываю данные. " << i << endl;
        this_thread::sleep_for(chrono::milliseconds(500));
    }
}

int main() {
    setlocale(LC_ALL, "ru");
    thread worker(workerThread);

    cout << "Главный поток: Я СОЮИРАЮСЬ..." << endl;
    this_thread::sleep_for(chrono::seconds(2));

    {
        lock_guard<mutex> lock(mtx); 
        ready = true;
        cout << "Главный поток: Чиназес отправля. сигнал" << endl;
    }

    cv.notify_one(); 

    worker.join();

    cout << "Главный поток: Программа завершена." << endl;

    return 0;
}
