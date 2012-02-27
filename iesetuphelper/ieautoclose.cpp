#include "stdafx.h"

#define CLOSEMSG(op) L"Internet Explorer must be closed for "##op##L" to continue.\nClick OK to automatically close all open Internet Explorer windows.\nClick Cancel to abort "##op##L"."
#define MANUALCLOSEMSG(op) L"Internet Explorer must be closed for "##op##L" to continue.\nPlease close all open Internet Explorer windows and click OK to continue.\nClick Cancel to abort "##op##L"."

const wchar_t *gDefaultMsgInstCloseIE = CLOSEMSG(L"installation");
const wchar_t *gDefaultMsgUninstCloseIE = CLOSEMSG(L"uninstallation");
const wchar_t *gDefaultMsgInstCloseIEManually = MANUALCLOSEMSG(L"installation");
const wchar_t *gDefaultMsgUninstCloseIEManually = MANUALCLOSEMSG(L"uninstallation");

const wchar_t *gMsgInstCloseIE = gDefaultMsgInstCloseIE;
const wchar_t *gMsgUninstCloseIE = gDefaultMsgUninstCloseIE;
const wchar_t *gMsgInstCloseIEManually = gDefaultMsgInstCloseIEManually;
const wchar_t *gMsgUninstCloseIEManually = gDefaultMsgUninstCloseIEManually;

/**
 * Overrides default messages asking user to close IE automatically.
 * NULLs are replaced with default strings.
 * @param msgInstCloseIE prompt to automatically close IE when installing
 * @param msgUninstCloseIE prompt to automatically close IE when uninstalling
 */
EXPORTED(void, SetMessagesCloseIEAutomatically)(const wchar_t *msgInstCloseIE, const wchar_t * msgUninstCloseIE)
{
  gMsgInstCloseIE = msgInstCloseIE ? msgInstCloseIE : gDefaultMsgInstCloseIE;
  gMsgUninstCloseIE = msgUninstCloseIE ? msgUninstCloseIE : gDefaultMsgUninstCloseIE;
}

/**
 * Overrides default messages asking user to close IE manually.
 * NULLs are replaced with default strings.
 * @param msgInstCloseIEManually prompt to manually close IE when installing
 * @param msgUninstCloseIEManually prompt to manually close IE when uninstalling
 */
EXPORTED(void, SetMessagesCloseIEManually)(const wchar_t *msgInstCloseIEManually, const wchar_t * msgUninstCloseIEManually)
{
  gMsgInstCloseIEManually = msgInstCloseIEManually ? msgInstCloseIEManually : gDefaultMsgInstCloseIEManually;
  gMsgUninstCloseIEManually = msgUninstCloseIEManually ? msgUninstCloseIEManually : gDefaultMsgUninstCloseIEManually;
}

static BOOL AskUser(BOOL manually, BOOL isInstalling)
{
  const wchar_t * msg;
  if (manually)
  {
    msg = isInstalling ? gMsgInstCloseIEManually : gMsgUninstCloseIEManually;
  } else {
    msg = isInstalling ? gMsgInstCloseIE : gMsgUninstCloseIE;
  }

  return (MessageBoxW(NULL, msg, L"Internet Explorer must be closed", MB_ICONINFORMATION | MB_OKCANCEL) == IDOK);
}

/**
 * Finds a handle of any IE window.
 */
EXPORTED(HWND, GetIEFrameHandle)()
{
  return FindWindowW(L"IEFrame", NULL);
}

static BOOL WaitForManualIEClose(BOOL isInstalling, BOOL silent)
{
  while (GetIEFrameHandle())
  {
    if (isInstalling && silent)
    {
      return FALSE;
    } else if (!AskUser(TRUE, isInstalling)) {
      return FALSE;
    }
  }
  return TRUE;
}

/**
 * Checks if IE is not running asking user to close it automatically/manually if it is.
 * @returns TRUE iff IE is no longer running.
 */
EXPORTED(BOOL, EnsureIENotRunning)(BOOL isInstalling, BOOL silent)
{
  DWORD startTime;

  if (GetIEFrameHandle())
  {
    // Prompt the user to let the IE be closed automatically.
    if (!isInstalling || !silent)
    {
      if (!AskUser(FALSE, isInstalling))
      {
        return FALSE;
      }
    }

    // Keep trying to close IE for limited amount of milliseconds only.
    startTime = GetTickCount();
    HWND hwnd;
    while ((GetTickCount() - startTime < 10000) && (hwnd = GetIEFrameHandle()))
    {
      PostMessage(hwnd, WM_CLOSE, 0, 0);
      Sleep(100);
    }
  }

  // Still no success? Prompt the user to do it manually then.
  if (GetIEFrameHandle())
  {
    return WaitForManualIEClose(isInstalling, silent);
  } else {
    return TRUE;
  }
}
