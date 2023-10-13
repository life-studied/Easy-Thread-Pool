# Easy-Thread-Pool
A easy thread pool based on C++14, which is a **Single-task queue**.

## 1.支持的task类型（Supported task types）

* 普通函数（function name）
* 匿名函数（lambda）
* 仿函数（Overloaded `()` class or struct）
* 类成员函数（Class member functions）
* `std::function`/`std::packaged_task`

## 2.线程池使用

### 2.1 提交任务

```c++
std::future<returnType> res = submitTask(funcName, arg1, arg2, ...);
```

### 2.2 获取结果

```C++
res.get();
```

## 3.使用案例

```C++
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
    ThreadPool thread_pool{12};
    int taskNum = 30;
    std::vector<std::future<int>> results(taskNum);

    std::cout << "start to submit tasks..." << std::endl;
    for (size_t i = 0; i < taskNum; i++)
    {
        results[i] = thread_pool.submitTask(very_time_consuming_task, i, i + 1);
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

```

