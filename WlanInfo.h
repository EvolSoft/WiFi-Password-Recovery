/***************************************************************
 *
 * WiFi Password Recovery
 *
 * Author:    Flavio Collocola
 * Copyright: (C) 2018 EvolSoft (https://www.evolsoft.tk)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 **************************************************************/

#ifndef WLANINFO_H
#define WLANINFO_H

#include "pugixml.hpp"

#include <stdlib.h>
#include <string.h>

//For a future cross-platform support
//#ifdef WINDOWS
#include <windows.h>
#include <wlanapi.h>
#include <wincrypt.h>
#include <VersionHelpers.h>
//#endif WINDOWS

#define ERR_WI_SUCCESS 0
#define ERR_WI_NOT_INIT 1       //Wlan Info not initialized
#define ERR_WI_WLAN_INIT 2      //Wlan Interface not initialized
#define ERR_WI_ENUM_INIT 3       //WlanEnumInterfaces failed
#define ERR_WI_NO_INT 4         //No Wlan Interfaces found
#define ERR_WI_GET_NETWORKS 5   //WlanGetNetworks failed

//For a future cross-platform support
typedef enum {
    WAUTH_OPEN,
    WAUTH_WEP,
    WAUTH_WPA,
    WAUTH_WPA_PSK,
    WAUTH_WPA2,
    WAUTH_WPA2_PSK,
    WAUTH_UNKNOWN
} WlanAuth;

//For a future cross-platform support
typedef enum {
    WCRYPT_NONE,
    WCRYPT_WEP,
    WCRYPT_TKIP,
    WCRYPT_AES,
    WCRYPT_UNKNOWN
} WlanEncryption;

//For cross-platform support
typedef struct {
    wchar_t name[WLAN_MAX_NAME_LENGTH];
    wchar_t intrface[WLAN_MAX_NAME_LENGTH];
    wchar_t password[64];         //Maximum password length is 64 bytes
    bool dcerror;
    WlanAuth auth;
    WlanEncryption encryption;
} WlanNetwork;

class WlanInfo {
    public:
        static int InitWlanInterface();
        static bool IsInitialized();
        static int GetStatus();
        static int GetNetworksCount();
        static WlanNetwork* GetNetworksList();
        static int UpdateNetworksList();
        static WlanNetwork* GetNetworkInfo(wchar_t* name);
        static char* WlanAuthToString(WlanAuth auth);
        static char* WlanEncryptionToString(WlanEncryption encryption);
        static bool Destroy();
    private:
        static bool init;
        static HANDLE hWlan;
        static WLAN_INTERFACE_INFO_LIST* interfaces;
        static int status;
        static int wnCount;
        static WlanNetwork* wnItems;
};
#endif // WLANINFO_H
