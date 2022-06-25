#include "Struct.h"

void Thread1(int& sum, Cell& cell, PostBox& port);
void Thread2(int& sum, Cell& cell, PostBox& port);
std::mutex mutex_thread;
bool transaction;

int main()
{
    Cell cell;
    PostBox port;
    int sum = 0;

    std::thread thread1(Thread1, std::ref(sum), std::ref(cell), std::ref(port));
    std::thread thread2(Thread2, std::ref(sum), std::ref(cell), std::ref(port));
    thread1.join();
    thread2.join();

    std::cout << "\nYour wallet balance now: " << cell.wallet << std::endl;
    std::cout << "\nRemaining banknotes:" << std::endl;
    std::cout << "\"Hundred\" currency unit  - "<<cell.hundred<<std::endl;
    std::cout << "\"Fifty\" currency unit - "<<cell.fifty<<std::endl;
    std::cout << "\"Twenty\" currency unit - "<<cell.twenty<<std::endl;
    std::cout << "\"Ten\" currency unit - "<<cell.ten<<std::endl;
    std::cout << "\"Five\" currency unit - "<<cell.five<<std::endl;
    std::cout << "\"Two\" currency unit - "<<cell.two<<std::endl;
    std::cout << "\"One\" currency unit - "<<cell.one<<std::endl;

    return 0;
}

void Thread1(int& sum, Cell& cell, PostBox& port)   // Signalizing thread A
{
    std::unique_lock<std::mutex> lk(mutex_thread);

    //printf("Thread1 started \n ");    // debug solution
    std::cout << "ATM has: " << cell.wallet << std::endl;
    std::cout << "Choose sum: ";
    std::cin >> sum;

    port.transaction_demand = true;
    if (sum > cell.wallet) 
    { 
        port.is_transaction_possible = false; 
    }
    else
    {
        port.is_transaction_possible = true;
    }
    transaction = true;
    lk.unlock();
    port.g_queuecheck.notify_one(); // Opens thread2
}

void Thread2(int& sum, Cell& cell, PostBox& port)   // Executing thread B
{
    //printf("Thread2 started \n\n ");  // debug solution

    std::unique_lock<std::mutex> lk(mutex_thread);

    port.g_queuecheck.wait(lk, [] {return transaction; });

    if (port.is_transaction_possible == false) 
    {
        std::cout << "Your request higher than the available amount" << std::endl;
        std::terminate(); 
    }
    else if (port.is_transaction_possible == true && port.transaction_demand == true) 
    { 
        cell.cash(sum); 
    }

    lk.unlock();
    port.g_queuecheck.notify_one();
}
