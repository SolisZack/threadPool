//
// Created by zack solis on 2022/1/29.
//

#include "test.h"
#include "threadPool.h"
#include <iostream>
#include <memory>

void example()
{
    // 创建3个线程的线程池
    ThreadPool pool(3);
    // 初始化线程池
    pool.init();
    // 提交乘法操作，总共30个
    for (int i = 1; i <= 3; ++i)
        for (int j = 1; j <= 10; ++j){
            pool.submitFunc(multiply, i, j);
        }

    // 使用ref传递的输出参数提交函数
    int output_ref;
    auto future1 = pool.submitFunc(multiply_output, std::ref(output_ref), 5, 6);

    // 等待乘法输出完成
    future1.get();
    std::cout << "Last operation result is equals to " << output_ref << std::endl;

    // 使用return参数提交函数
    auto future2 = pool.submitFunc(multiply_return, 5, 3);

    // 等待乘法输出完成
    int res = future2.get();
    std::cout << "Last operation result is equals to " << res << std::endl;

    // 关闭线程池
    pool.shutPool();
}

// 对于submitFunc()的理解
void myTest() {
    auto testFunc = [](int x, int y) {
        return (x + y) * 4;
    };
    // 获取一个函数并与需要的参数绑定，形成f()格式，免去传参步骤
    std::function<int()> testBindFunc = std::bind(testFunc, 3, 4);
    // 打包成packaged_task，用于异步执行
    std::packaged_task<int()> testPackFunc = std::packaged_task<int()>(testBindFunc);
    // 打包成指针，方便最后形成void()格式，因为2点：1.方便funcQueue管理；2.packaged_task::operator()会调用其所封装的可调用对象，但是它返回值是void，即无返回值。
    // 因为std::packaged_task的设计主要是用来进行异步调用，因此计算结果是通过future::get来获取的。该函数会忠实地将R的计算结果反馈给future，即使R抛出异常
    // 同时要注意，与std::promise一样，std::packaged_task支持move，但!不!支!持!拷!贝!(copy)，所以把它打包成指针的时候要用std::move
    std::shared_ptr<std::packaged_task<int()>> testVoidFuncPtr = std::make_shared<std::packaged_task<int()>>(std::move(testPackFunc));
    // 基于指针打包成void()格式
    auto testVoidFunc = [testVoidFuncPtr](){(*testVoidFuncPtr)();};
    auto testFuture = testVoidFuncPtr->get_future();
    std::thread myThread (testVoidFunc);
    myThread.join();
    std::cout << testFuture.get();
}

int main() {
    example();
}

