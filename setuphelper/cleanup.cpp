#include "stdafx.h"

/**
 * Deletes specified IE virtualized registry subkey for every user.
 * Used when a BHO is uninstalling to ensure proper cleanup.
 * Example: WipeInternetRegistrySubKey(L"Software\\<softwarekeyformybho>")
 */
extern "C" void __stdcall WipeInternetRegistrySubKey(const wchar_t *subKey)
{
  HKEY hKeyVirtUser = 0;
  LONG l;
  l = RegOpenKeyExW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Internet Explorer\\InternetRegistry\\REGISTRY\\USER", 0, KEY_ALL_ACCESS, &hKeyVirtUser);
  if (l != ERROR_SUCCESS)
  {
    return;
  }

  WCHAR name[MAX_PATH+1];
  for (DWORD index = 0; ; index ++) {
    DWORD nameLen = MAX_PATH;
    l = RegEnumKeyExW(hKeyVirtUser, index, name, &nameLen, NULL, NULL, NULL, NULL);
    if (l != ERROR_SUCCESS)
    {
      break;
    }

    HKEY hk = 0;
    l = RegOpenKeyExW(hKeyVirtUser, name, 0, KEY_ALL_ACCESS, &hk);
    if (l == ERROR_SUCCESS)
    {
      l = SHDeleteKeyW(hk, subKey);
    }
    RegCloseKey(hk);
  }

  RegCloseKey(hKeyVirtUser);
}
