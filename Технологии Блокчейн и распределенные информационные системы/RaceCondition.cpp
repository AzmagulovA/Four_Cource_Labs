// DeadLock.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <thread>
#include <mutex>


using namespace std;

void RaceCondition(int cnt)
{
    thread first([&]()//определение первого потока
        {
            for (int i = 0; i < 1000; ++i)
                ++cnt;
        });

    thread second([&]()//определение второго потока
        {
            for (int i = 0; i < 1000; ++i)
                ++cnt;
        });
    first.join();
    second.join();
    cout << "RaceConditionNumber: " << cnt << endl;
}

void SolveRaceCondition(int cnt)
{
    mutex helper;
    thread first([&]()//определение первого потока
        {
            for (int i = 0; i < 1000; ++i)
            {
                helper.lock();
                ++cnt;
                helper.unlock();
            }
        });

    thread second([&]()//определение второго потока
        {
            for (int i = 0; i < 1000; ++i)
            {
                helper.lock();
                ++cnt;
                helper.unlock();
            }
        });
    first.join();
    second.join();
    cout << "SolveRaceConditionNumber: " << cnt << endl;
}

int main()
{
    int cnt = 0;
    RaceCondition(cnt);
    cnt = 0;
    SolveRaceCondition(cnt);
}


