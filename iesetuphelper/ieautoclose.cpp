#include "stdafx.h"

#define CLOSEMSG(op) "Internet Explorer must be closed for "##op##" to continue.\nClick OK to automatically close all open Internet Explorer windows.\nClick Cancel to abort "##op##"."
#define MANUALCLOSEMSG(op) "Internet Explorer must be closed for "##op##" to continue.\nPlease close all open Internet Explorer windows and click OK to continue.\nClick Cancel to abort "##op##"."

const char *gDefaultMsgInstCloseIE = CLOSEMSG("installation");
const char *gDefaultMsgUninstCloseIE = CLOSEMSG("uninstallation");
const char *gDefaultMsgInstCloseIEManually = MANUALCLOSEMSG("installation");
const char *gDefaultMsgUninstCloseIEManually = MANUALCLOSEMSG("uninstallation");

const char *gMsgInstCloseIE = gDefaultMsgInstCloseIE;
const char *gMsgUninstCloseIE = gDefaultMsgUninstCloseIE;
const char *gMsgInstCloseIEManually = gDefaultMsgInstCloseIEManually;
const char *gMsgUninstCloseIEManually = gDefaultMsgUninstCloseIEManually;

/**
 * Overrides default messages asking user to close IE automatically.
 * NULLs are replaced with default strings.
 * @param msgInstCloseIE prompt to automatically close IE when installing
 * @param msgUninstCloseIE prompt to automatically close IE when uninstalling
 */
EXPORTED(void, SetMessagesCloseIEAutomatically)(const char *msgInstCloseIE, const char * msgUninstCloseIE)
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
EXPORTED(void, SetMessagesCloseIEManually)(const char *msgInstCloseIEManually, const char * msgUninstCloseIEManually)
{
  gMsgInstCloseIEManually = msgInstCloseIEManually ? msgInstCloseIEManually : gDefaultMsgInstCloseIEManually;
  gMsgUninstCloseIEManually = msgUninstCloseIEManually ? msgUninstCloseIEManually : gDefaultMsgUninstCloseIEManually;
}

static BOOL AskUser(BOOL manually, BOOL isInstalling)
{
  const char * msg;
  if (manually)
  {
    msg = isInstalling ? gMsgInstCloseIEManually : gMsgUninstCloseIEManually;
  } else {
    msg = isInstalling ? gMsgInstCloseIE : gMsgUninstCloseIE;
  }

  return (MessageBox(NULL, msg, "Internet Explorer must be closed", MB_ICONINFORMATION | MB_OKCANCEL) == IDOK);
}

/**
 * Finds a handle of any IE window.
 */
EXPORTED(HWND, GetIEFrameHandle)()
{
  return FindWindow("IEFrame", NULL);
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
