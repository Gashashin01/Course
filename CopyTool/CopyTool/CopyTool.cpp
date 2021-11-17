#include <iostream>
#include <windows.h>

bool Copy(const char* pathFrom, const char* pathTo) {
    return CopyFileExA(pathFrom, pathTo, 0, 0, 0, COPY_FILE_FAIL_IF_EXISTS);
}

int main(int argc, char* argv[])
{
    bool copyState = Copy(argv[1], argv[2]);
    if (copyState) {
        std::cout << "Copied succesfully";
    }
    else {
        std::cout << "Failed";
    }
}
