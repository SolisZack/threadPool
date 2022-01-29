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

int main() {
    example();
}

