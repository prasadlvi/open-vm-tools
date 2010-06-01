/*********************************************************
 * Copyright (C) 1998 VMware, Inc. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation version 2.1 and no later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the Lesser GNU General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA.
 *
 *********************************************************/


/*
 * system.h --
 *
 *    System-specific routines used by the tools daemon.
 *
 */


#ifndef __SYSTEM_H__
#   define __SYSTEM_H__

#include "vm_basic_types.h"
#ifdef _WIN32
#include "dbllnklst.h"
#endif
#include "unicode.h"

uint64 System_Uptime(void);
Bool System_GetCurrentTime(int64 *secs, int64 *usecs);
Bool System_AddToCurrentTime(int64 deltaSecs, int64 deltaUsecs);
Unicode System_GetTimeAsString(void);
Bool System_EnableTimeSlew(int64 delta, int64 timeSyncPeriod);
Bool System_DisableTimeSlew(void);
Bool System_IsTimeSlewEnabled(void);
Bool System_IsACPI(void);
void System_Shutdown(Bool reboot);
Bool System_IsUserAdmin(void);

char *System_GetEnv(Bool global, const char *valueName);
int System_SetEnv(Bool global, const char *valueName, const char *value);

#ifdef _WIN32
typedef enum AeroStateCommand {
   AeroCommand_Enable,
   AeroCommand_Disable,
} AeroStateCommand;

/*
 * Representation of monitors gathered by System_GetMonitors.
 */
typedef struct MonListNode {
   RECT rect;
   RECT work;
   BOOL isPrimary;
   DWORD bpp;
   BOOL isActive;
   uint32 srcId;
   DblLnkLst_Links l;
} MonListNode;

/*
 * The value returned by System_GetServiceState if the current state of the
 * vmtools service can't be determined. We need to use a value that is not
 * already used for a real state. The current values run from 0x1 to 0x7, so
 * 0xffffffff should be fairly safe (cf. winsvc.h).
 */
#define VM_SERVICE_STATE_UNKNOWN 0xffffffff

BOOL System_SetProcessPrivilege(LPCTSTR lpszPrivilege, Bool bEnablePrivilege);
int32 System_GetSPVersion(void);
Bool System_IsLoginScreenActive(void);
Bool System_IsProcessElevated(void);
Bool System_IsScreenSaverActive(void);
Bool System_IsScreenSaverRunning(void);
Bool System_IsSecureDesktopActive(void);
Bool System_DisableAndKillScreenSaver(void);
DWORD System_GetServiceState(LPCWSTR szServiceName);
DblLnkLst_Links *System_GetMonitors();
void System_SetFocusedWindow(HWND windowToFocus, Bool force);
Bool System_SetAeroState(AeroStateCommand command, AeroStateCommand *oldState);
#endif


/*
 * TODO:  Targets' make/SCons files, or perhaps the entire build infrastructure
 *        as a whole, should define a POSIX_LIKE_ENVIRONMENT variable which is
 *        then acted upon and translates to a -DPOSIX_LIKE_ENVIRONMENT
 *        preprocessor option.
 */
#if !defined(_WIN32) && !defined(N_PLAT_NLM)
Bool System_WritePidFile(const char *fileName, pid_t pid);
const char **System_GetNativeEnviron(const char **compatEnviron);
void System_FreeNativeEnviron(const char **nativeEnviron);
int System_UnsetEnv(const char *variableName);
char *System_SetLDPath(const char *path, const Bool native);
#endif

#endif /* __SYSTEM_H__ */
