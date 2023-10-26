#include <iostream>
#include <chrono>
#include "ThreadPool.h"

int very_time_consuming_task(int a, int b)  //模拟耗时函数
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return a + b;
}

int main()
{
    using namespace pooltech;
    ThreadPool thread_pool;
    int taskNum = 1000;
    std::vector<std::future<int>> results(taskNum);

    std::cout << "start to submit tasks..." << std::endl;
    for (size_t i = 0; i < taskNum; i++)
    {
        results[i] = thread_pool.submitTask(very_time_consuming_task, static_cast<int>(i), static_cast<int>(i + 1));
    }
    std::cout << "End submit tasks..." << std::endl;
    std::cout << "Main thread is doing something else..." << std::endl;

    //do something
    std::this_thread::sleep_for(std::chrono::seconds(10));
    std::cout << "Main thread task finished..." << std::endl;

    std::cout << "try to get the thread pool tasks..." << std::endl;
    for (size_t i = 0; i < taskNum; i++)
    {
        std::cout << "result[" << i << "] = " << results[i].get() << std::endl;
    }
    std::cout << "End of getting results" << std::endl;
    
}
