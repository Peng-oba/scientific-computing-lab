#include <cassert>
#include <cstdio>
#include <iostream>
#include <stdexcept>

class FileHandle
{
public:
    // 所有类成员共享
    inline static int open_handles = 0;
    inline static bool closes_ok = true;

    // 构造函数
    // explicit 避免隐式转换
    explicit FileHandle(char const *label) : file_(std::tmpfile()), label_(label)
    {
        if (!file_) { throw std::runtime_error("tmpfile failed"); }
        ++open_handles;

        std::cout << "open " << label_ << '\n';
    }

    // 析构函数
    ~FileHandle()
    {
        if (std::fclose(file_) != 0) { closes_ok = false; }
        --open_handles;

        std::cout << "close " << label_ << '\n';
    }

    // 禁止 FileHandle 对象的拷贝构造和拷贝赋值
    FileHandle(FileHandle const&) = delete;
    FileHandle& operator=(FileHandle const&) = delete;

    void write()
    {
        if (std::fputs("one record\n", file_) == EOF)
        {
            throw std::runtime_error("write failed");
        }
    }

private:
    std::FILE *file_;
    char const *label_;
};

void normal()
{
    FileHandle handle("normal");
    handle.write();

    std::cout << "normal body end\n";
    std::cout << "number of open_handles: " << handle.open_handles << '\n';
}

void early()
{
    FileHandle handle("early");
    handle.write();

    std::cout << "early return\n";
    std::cout << "number of open_handles: " << handle.open_handles << '\n';

    // return 结束函数执行, 离开作用域
    // 局部对象 handle 被自动析构
    return;
}

void throwing()
{
    FileHandle handle("exception");
    handle.write();

    std::cout << "throw\n";

    // 抛出异常, throw 立即中断函数
    // 已构造的局部对象 (handle) 被自动析构
    throw std::runtime_error("planned exception");
}

int main()
{
    normal();
    assert(FileHandle::open_handles == 0);

    std::cout << "==================" << '\n';

    early();
    assert(FileHandle::open_handles == 0);

    std::cout << "==================" << '\n';

    try { throwing(); }
    catch (std::runtime_error const&) { std::cout << "caught\n"; }

    assert(FileHandle::open_handles == 0 && FileHandle::closes_ok);

    std::cout << "D1: all 3 paths closed their handle\n";

}
