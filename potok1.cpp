#include <iostream>
#include <thread>

using namespace std;

void vvivodCisel() {
    for (int i = 1; i <= 100; ++i) {
        cout << "Число: " << i << endl;
    }
}

void vvivodBukv() {
    for (char c = 'A'; c <= 'Z'; ++c) {
        cout << "Буква: " << c << endl;
    }
}

int main() {
    setlocale(LC_ALL, "ru");
    thread cisleniiThread(vvivodCisel);
    thread bukviThread(vvivodBukv);

    cisleniiThread.join();
    bukviThread.join();

    cout << "Оба потока завершили работу." << endl;

    return 0;
}
