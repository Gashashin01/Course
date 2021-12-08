#include <iostream>
#include <windows.h>
#include <psapi.h>
#include <cwchar>

class HandleWrapper {
public:
    HandleWrapper(HANDLE handle) : m_handle(handle) {}
    ~HandleWrapper() {
        if (m_handle != INVALID_HANDLE_VALUE && m_handle != 0) {
            CloseHandle(m_handle);
            m_handle = INVALID_HANDLE_VALUE;
        }
    }
    HANDLE Get() const {
        return m_handle;
    }
    operator HANDLE() const {
        return m_handle;
    }
    operator bool() const {
        return m_handle != 0;
    }
private:
    HANDLE m_handle = INVALID_HANDLE_VALUE;
};
bool DeleteByName(DWORD processId, const std::wstring& processName) {
    if (!processId) {
        return false;
    }
    std::wstring currentProcessName(MAX_PATH, L'\0');
    HandleWrapper process(OpenProcess(PROCESS_ALL_ACCESS, TRUE, processId));
    if (process) {
        DWORD nameSize = GetModuleBaseName(process, 0, &currentProcessName[0], currentProcessName.size());
        currentProcessName.resize(nameSize);
        if (currentProcessName == processName) {
            bool isProcTerminate = TerminateProcess(process, -1);
            if (isProcTerminate) {
                return true;
            }
        }
    }
    return false;
}

int wmain(int argc, wchar_t* argv[])
{
    DWORD processes[4096];
    DWORD needed = 0;
    if (!EnumProcesses(processes, sizeof(processes), &needed)) {
        return -1;
    }
    const DWORD countProcesses = needed / sizeof(DWORD);
    for (DWORD i = 0; i < countProcesses; ++i) {
        if (DeleteByName(processes[i], argv[1])) {
            return 0;
        }
    }
}
