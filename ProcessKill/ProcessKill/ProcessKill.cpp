#include <iostream>
#include <windows.h>
#include <psapi.h>
#include <cwchar>

bool isEqual(WCHAR firstName[], WCHAR secondName[]) {
  int firstSize = std::wcslen(firstName);
  int secondSize = std::wcslen(secondName);
  
  if (firstSize != secondSize) {
    return false;
  }
  for (int i = 0; i < firstSize; ++i) {
    if (firstName[i] != secondName[i]) {
      return false;
    }
  }
  return true;
}

BOOL DeleteByName(DWORD processId, WCHAR processName[]) {
  //DWORD currentProcessName[MAX_PATH] = {'a','a', 'a', 'a'};
  WCHAR currentProcessName[MAX_PATH];
  HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, TRUE, processId);
  if (hProcess != 0) {
    HMODULE hMod;
    DWORD needed;
    bool procModuleStat = EnumProcessModulesEx(hProcess, &hMod, sizeof(hMod), &needed, 0x02);
    if (!procModuleStat) {
      return false;
    }
    if (procModuleStat)
    {
      GetModuleBaseName(hProcess, hMod, currentProcessName,
        sizeof(currentProcessName) / sizeof(WCHAR));
      if (isEqual(currentProcessName, processName)) {
        bool isProcTerminate = TerminateProcess(hProcess, -1);
        if (isProcTerminate) {
          CloseHandle(hProcess);
          return true;
        }
      }
    }
  }
  CloseHandle(hProcess);
  return false;
}

int wmain(int argc, wchar_t* argv[])
{
  DWORD Processes[4096];
  DWORD needed;
  DWORD countProcesses;

  if (!EnumProcesses(Processes, sizeof(Processes), &needed)) {
    return 1;
  }
  //WCHAR name[14] = {'n','o','t','e','p','a','d', '+', '+', '.', 'e','x','e','\0'};
  //WCHAR name[12] = {'m', 's', 'e', 'd', 'g', 'e', '.', 'e', 'x', 'e', '\0'};
  countProcesses = needed / sizeof(DWORD);
  for (int i = 0; i < countProcesses; ++i) {
    if (DeleteByName(Processes[i], argv[1])) {
      return 0;
    }
  }
}