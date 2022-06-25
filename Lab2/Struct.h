#pragma once

#include<iostream>
#include<thread>
#include<functional>
#include<mutex>
#include<condition_variable>

struct PostBox
{
    bool transaction_demand = false;
    bool is_transaction_possible = false;
    std::condition_variable g_queuecheck;
};
struct Cell
{
    int wallet = 100;
    int one = 50;
    int two = 25;
    int five = 20;
    int ten = 15;
    int twenty = 10;
    int fifty = 5;
    int hundred = 5;

    void cash(int value)
    {
        //printf("You get : %d \n ", value);
        wallet += value;
        while (value != 0)
        {
            if (value >= 100)
            {
                value -= 100;
                hundred -= 1;
            }
            else if (value >= 50)
            {
                value -= 50;
                fifty -= 1;
            }
            else if (value >= 20)
            {
                value -= 20;
                twenty -= 1;
            }
            else if (value >= 10)
            {
                value -= 10;
                ten -= 1;
            }
            else if (value >= 5)
            {
                value -= 5;
                five -= 1;
            }
            else if (value >= 2)
            {
                value -= 2;
                two -= 1;
            }
            else if (value >= 1)
            {
                value -= 1;
                one -= 1;
            }
        }

    }
};