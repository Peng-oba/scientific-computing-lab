#include <cassert>
#include <cstdio>
#include <iostream>
#include <stdexcept>

// A small educational owner for a real temporary-file handle.
// Production C++ code can normally use an existing RAII stream/wrapper.
class FileHandle {
public:
    inline static int open_handles = 0;
    inline static bool closes_ok = true;

    explicit FileHandle(char const* label) : file_(std::tmpfile()), label_(label) {
        if (!file_) { throw std::runtime_error("tmpfile failed"); }
        ++open_handles;
        std::cout << "open " << label_ << '\n';
    }
    ~FileHandle() {
        if (std::fclose(file_) != 0) { closes_ok = false; }
        --open_handles;
        std::cout << "close " << label_ << '\n';
    }
    FileHandle(FileHandle const&) = delete;
    FileHandle& operator=(FileHandle const&) = delete;

    void write() {
        if (std::fputs("one record\n", file_) == EOF) {
            throw std::runtime_error("write failed");
        }
    }
private:
    std::FILE* file_;
    char const* label_;  // Borrowed string literal in this exercise.
};

void normal() {
    FileHandle handle("normal");
    handle.write();
    std::cout << "normal body end\n";
}
void early() {
    FileHandle handle("early");
    handle.write();
    std::cout << "early return\n";
    return;
}
void throwing() {
    FileHandle handle("exception");
    handle.write();
    std::cout << "throw\n";
    throw std::runtime_error("planned exception");
}

int main() {
    normal();
    assert(FileHandle::open_handles == 0);
    early();
    assert(FileHandle::open_handles == 0);
    try { throwing(); }
    catch (std::runtime_error const&) { std::cout << "caught\n"; }
    assert(FileHandle::open_handles == 0 && FileHandle::closes_ok);
    std::cout << "D1: all 3 paths closed their handle\n";
}
