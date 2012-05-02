#include "stdafx.h"
#include <tlhelp32.h>
#include <Psapi.h>

BOOL PathsEqual(const char *first, const char *second)
{
  HANDLE h1 = CreateFile(first, GENERIC_READ, 7 /* share all */, NULL, OPEN_EXISTING, 0, NULL);
  if (h1 == INVALID_HANDLE_VALUE)
  {
    return FALSE;
  }

  HANDLE h2 = CreateFile(second, GENERIC_READ, 7 /* share all */, NULL, OPEN_EXISTING, 0, NULL);
  if (h2 == INVALID_HANDLE_VALUE)
  {
    CloseHandle(h1);
    return FALSE;
  }

  BOOL result = FALSE;
  BY_HANDLE_FILE_INFORMATION fileInfo1, fileInfo2;
  if (GetFileInformationByHandle(h1, &fileInfo1) && GetFileInformationByHandle(h2, &fileInfo2))
  {
    result = ((fileInfo1.dwVolumeSerialNumber == fileInfo2.dwVolumeSerialNumber) &&
              (fileInfo1.nFileIndexHigh == fileInfo2.nFileIndexHigh) &&
              (fileInfo1.nFileIndexLow == fileInfo2.nFileIndexLow));
  }

  CloseHandle(h2);
  CloseHandle(h1);
  return result;
}

EXPORTED(void, KillAllProcessesByAboslutePath)(const char *exeAbsolutePath)
{
  PROCESSENTRY32 entry;
  entry.dwSize = sizeof(PROCESSENTRY32);
  CHAR exePath[MAX_PATH + 1];

  HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

  if (Process32First(snapshot, &entry))
  {
    do {
      HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_TERMINATE, FALSE, entry.th32ProcessID);
      if (hProcess && (GetModuleFileNameEx(hProcess, NULL, exePath, MAX_PATH) > 0))
      {
        if (PathsEqual(exePath, exeAbsolutePath))
        {
          TerminateProcess(hProcess, 0);
        }
      }

      CloseHandle(hProcess);
    } while (Process32Next(snapshot, &entry));
  }

  CloseHandle(snapshot);
}
