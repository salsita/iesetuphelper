#include "stdafx.h"

/**
 * Deletes specified IE virtualized registry subkey for every user.
 * Used when a BHO is uninstalling to ensure proper cleanup.
 * Example: WipeInternetRegistrySubKey("Software\\<softwarekeyformybho>")
 */
EXPORTED(void, WipeInternetRegistrySubKey)(const char *subKey)
{
  HKEY hKeyVirtUser = 0;
  LONG l;
  l = RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Internet Explorer\\InternetRegistry\\REGISTRY\\USER", 0, KEY_ALL_ACCESS, &hKeyVirtUser);
  if (l != ERROR_SUCCESS)
  {
    return;
  }

  CHAR name[MAX_PATH+1];
  for (DWORD index = 0; ; index ++) {
    DWORD nameLen = MAX_PATH;
    l = RegEnumKeyEx(hKeyVirtUser, index, name, &nameLen, NULL, NULL, NULL, NULL);
    if (l != ERROR_SUCCESS)
    {
      break;
    }

    HKEY hk = 0;
    l = RegOpenKeyEx(hKeyVirtUser, name, 0, KEY_ALL_ACCESS, &hk);
    if (l == ERROR_SUCCESS)
    {
      l = SHDeleteKey(hk, subKey);
    }
    RegCloseKey(hk);
  }

  RegCloseKey(hKeyVirtUser);
}
