// CreateFile.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <windows.h>
#include <stdio.h>
#include <string>
#include <algorithm>

void main(int argc, char* argv[])
{
    HANDLE secretFile;
    HANDLE protectedFile;
    DWORD  dwBytesWritten;

    secretFile = CreateFileA(argv[1],
        GENERIC_READ,             // open for reading
        0,                        // do not share
        NULL,                     // no security
        OPEN_EXISTING,            // existing file only
        FILE_ATTRIBUTE_NORMAL,    // normal file
        NULL);                    // no attr. template

    if (secretFile == INVALID_HANDLE_VALUE)
    {
        printf("Could not open first file");
        return;
    }

    protectedFile = CreateFileA(argv[2], // open Two.txt
        GENERIC_WRITE,            // open for writing
        0,                        // allow multiple readers
        NULL,                     // no security
        CREATE_NEW,               // open or create
        FILE_ATTRIBUTE_NORMAL,    // normal file
        NULL);                    // no attr. template

    if (protectedFile == INVALID_HANDLE_VALUE)
    {
        printf("Could not open second file");
        return;
    }

    const size_t bufSize = 4096;
    DWORD  dwBytesRead = 0;
    DWORD dwPos = 0;
    std::string buf(bufSize, '\0');
    /*ReadFile(secretFile, &buf[0], buf.size(), &dwBytesRead, NULL);
    buf.resize(dwBytesRead);*/
    const std::string searchString = "password: "; // 10
    size_t startOfPassword = 0;
    size_t curPos = 0;
//password: hvdbvih password: 1212 sometext
//password: hvdbvih password: 1212 sometext
//password: hvdbvih password: 1212 sometext
    while (ReadFile(secretFile, &buf[0], buf.size(), &dwBytesRead, NULL) && dwBytesRead > 0) {
        while (startOfPassword != std::string::npos) {
            startOfPassword = buf.find(searchString, curPos); // 1
            if (startOfPassword != std::string::npos) {
                startOfPassword += searchString.size(); // startOfPassword == 11
                curPos = startOfPassword; // 11
                size_t endOfPassword = buf.find(" ", startOfPassword); //16
                if (endOfPassword != std::string::npos) {
                    const size_t passwordLength = endOfPassword - startOfPassword; // 5
                    buf.replace(startOfPassword, passwordLength, passwordLength, '*');
                }
            }
        }
        startOfPassword = 0;
        curPos = 0;
        WriteFile(protectedFile, buf.data(), dwBytesRead, &dwBytesWritten, NULL);
    }

    // Close both files.

    CloseHandle(secretFile);
    CloseHandle(protectedFile);
}
