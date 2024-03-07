// DeadLock.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

class Account {
private:
int Id;

int Balance;

public:

    mutex mut;
    int GetBalance() {
        return Balance;
    }
    int GetID() {
        return Id;
    }
    void Giver(int numb) {
       this-> Balance -=  numb;
    }
    void Taker(int numb) {
        this->Balance += numb;
    }
    Account(int id, int balance) {
        this->Id = id;
        this->Balance = balance;
    }
    void Print() {
        cout << GetBalance();

        cout << endl;
    }

};

void Rightmover(Account& from, Account& to, int numb)
{
    if (from.GetID() < to.GetID()) {
        
        lock_guard<mutex> lockFrom(from.mut);
        lock_guard<mutex> lockTo(to.mut);
        to.Taker(numb);
        from.Giver(numb);

    }
    else {
        lock_guard<mutex> lockTo(to.mut);
        lock_guard<mutex> lockFrom(from.mut);
        to.Taker(numb);
        from.Giver(numb);

    }
}



int main()
{   
     Account s1(1,50);
     Account s2(2,100);
    int numb = 25;

    thread first(Rightmover,ref(s1), ref(s2),25);
    thread second(Rightmover, ref(s2), ref(s1), 25);

    thread third(Rightmover, ref(s1), ref(s2), 5);//s2 получает от s1 5
    thread f4(Rightmover, ref(s2), ref(s1), 10);//s1 получает от s2 10
    first.join();
    second.join();
    third.join();
    f4.join();
    s1.Print();
    s2.Print();

}

/*
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <Windows.h>

class Account {
private:
    int id;
    double bankBalance;

public:
    std::mutex mut;
    int GetId() {
        return id;
    }

    double GetBalance() {
        return bankBalance;
    }

    Account(int id, double balance) {
        this->id = id;
        this->bankBalance = balance;
    }

    void WithdrawMoney(double sum) {
        this->bankBalance -= sum;

    }

    void DepositMoney(double sum) {
        this->bankBalance += sum;
    }
};

void TransferDeadLock(Account& from, Account& to, double sum) {
    from.mut.lock();
    from.WithdrawMoney(sum);
    to.mut.lock();
    to.DepositMoney(sum);
    to.mut.unlock();
    from.mut.unlock();
}

void TransferWithoutDeadLock(Account& from, Account& to, double sum, int idT) {
    printf("Transfer in thread %d\n", idT);
    if (from.GetId() < to.GetId()) {
        std::lock_guard<std::mutex> lockFrom(from.mut);
        std::lock_guard<std::mutex> lockTo(to.mut);
        printf("Thread %d locked mutex %d %d\n", idT, from.GetId(), to.GetId());
        from.WithdrawMoney(sum);
        to.DepositMoney(sum);
    }
    else {
        std::lock_guard<std::mutex> lockTo(to.mut);
        std::lock_guard<std::mutex> lockFrom(from.mut);
        printf("Thread %d locked mutex %d %d\n", idT, from.GetId(), to.GetId());
        from.WithdrawMoney(sum);
        to.DepositMoney(sum);
    }
    printf("Thread %d transfer is over \n", idT);
}

int main()
{
    Account a1(0, 10);
    Account a2(1, 30);
    std::thread thread1(TransferDeadLock, std::ref(a1), std::ref(a2), 5);
    std::thread thread2(TransferDeadLock, std::ref(a2), std::ref(a1), 10);
    std::thread thread3(TransferDeadLock, std::ref(a1), std::ref(a2), 2);
    std::thread thread4(TransferDeadLock, std::ref(a2), std::ref(a1), 3);
    thread1.join();
    thread2.join();
    thread3.join();
    thread4.join();
    printf("Balance in acc1: %f\n", a1.GetBalance());
    printf("Balance in acc2: %f\n", a2.GetBalance());
}
*/


// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
