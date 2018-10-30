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

#include "WlanInfo.h"

//For a future cross-platform support
//#ifdef WINDOWS
#pragma comment(lib, "wlanapi.lib")
#pragma comment(lib, "crypt32.lib")
//#endif WINDOWS

bool WlanInfo::init = false;
HANDLE WlanInfo::hWlan = NULL;
WLAN_INTERFACE_INFO_LIST* WlanInfo::interfaces = NULL;
int WlanInfo::wnCount = 0;
WlanNetwork* WlanInfo::wnItems = NULL;

int WlanInfo::InitWlanInterface(){
    DWORD supportedVer = 0, osVer = IsWindowsVistaOrGreater() ? 2 : 1;
    if(WlanOpenHandle(osVer, NULL, &supportedVer, &hWlan) != ERROR_SUCCESS){
        return ERR_WI_WLAN_INIT;
    }
    interfaces = (WLAN_INTERFACE_INFO_LIST*) WlanAllocateMemory(sizeof(WLAN_INTERFACE_INFO_LIST));
    if(WlanEnumInterfaces(hWlan, NULL, &interfaces) != ERROR_SUCCESS){
        WlanFreeMemory(interfaces);
        return ERR_WI_ENUM_INIT;
    }
    if(interfaces->dwNumberOfItems == 0){
        return ERR_WI_NO_INT;
    }
    init = true;
    return ERR_WI_SUCCESS;
}

bool WlanInfo::IsInitialized(){
    return init;
}

int WlanInfo::GetStatus(){
    if(!IsInitialized()){
        return ERR_WI_NOT_INIT;
    }
}

int WlanInfo::GetNetworksCount(){
    if(!IsInitialized()){
        return 0;
    }
    return wnCount;
}

WlanNetwork* WlanInfo::GetNetworksList(){
    if(!IsInitialized()){
        return NULL;
    }
    return wnItems;
}

int WlanInfo::UpdateNetworksList(){
    WLAN_PROFILE_INFO_LIST* networks;
    GUID gInterface;
    if(!IsInitialized()){
        return ERR_WI_NOT_INIT;
    }
    networks = (WLAN_PROFILE_INFO_LIST*) WlanAllocateMemory(sizeof(WLAN_PROFILE_INFO_LIST));
    wnCount = 0;
    if(wnItems){
        free((void*) wnItems);
    }
    for(int i = 0; i < interfaces->dwNumberOfItems; i++){    //Count networks for each interface
        if(WlanGetProfileList(hWlan, &interfaces->InterfaceInfo[i].InterfaceGuid, NULL, &networks) != ERROR_SUCCESS){
                return ERR_WI_GET_NETWORKS;
        }
        wnCount += networks->dwNumberOfItems;
    }
    wnItems = (WlanNetwork*) calloc(wnCount, sizeof(WlanNetwork));
    for(int l = 0; l < interfaces->dwNumberOfItems; l++){
        if(WlanGetProfileList(hWlan, &interfaces->InterfaceInfo[l].InterfaceGuid, NULL, &networks) != ERROR_SUCCESS){
                return ERR_WI_GET_NETWORKS;
        }
        for(int i = 0; i < networks->dwNumberOfItems; i++){
            DWORD dwFlags = 4, dwAccess = 0;
            LPWSTR networkXML;
            wcscpy(wnItems[i].name, networks->ProfileInfo[i].strProfileName);
            wcscpy(wnItems[i].intrface, interfaces->InterfaceInfo[l].strInterfaceDescription);
            wnItems[i].dcerror = false;
            if(WlanGetProfile(hWlan, &interfaces->InterfaceInfo[l].InterfaceGuid, networks->ProfileInfo[i].strProfileName, NULL, &networkXML, &dwFlags, &dwAccess) == ERROR_SUCCESS){
                pugi::xml_document doc;
                wchar_t* auth;
                wchar_t* encryption;
                char* password;
                doc.load_string(networkXML);
                auth = (wchar_t*) doc.child(L"WLANProfile").child(L"MSM").child(L"security").child(L"authEncryption").child(L"authentication").text().get();
                if(wcsicmp(auth, L"open") == 0){
                    wnItems[i].auth = WAUTH_OPEN;
                }else if(wcsicmp(auth, L"shared") == 0){
                    wnItems[i].auth = WAUTH_WEP;
                }else if(wcsicmp(auth, L"WPA") == 0){
                    wnItems[i].auth = WAUTH_WPA;
                }else if(wcsicmp(auth, L"WPAPSK") == 0){
                    wnItems[i].auth = WAUTH_WPA_PSK;
                }else if(wcsicmp(auth, L"WPA2") == 0){
                    wnItems[i].auth = WAUTH_WPA2;
                }else if(wcsicmp(auth, L"WPA2PSK") == 0){
                    wnItems[i].auth = WAUTH_WPA2_PSK;
                }else{
                    wnItems[i].auth = WAUTH_UNKNOWN;
                }
                encryption = (wchar_t*) doc.child(L"WLANProfile").child(L"MSM").child(L"security").child(L"authEncryption").child(L"encryption").text().get();
                if(wcsicmp(encryption, L"none") == 0){
                    wnItems[i].encryption = WCRYPT_NONE;
                }else if(wcsicmp(encryption, L"WEP") == 0){
                    wnItems[i].encryption = WCRYPT_WEP;
                }else if(wcsicmp(encryption, L"TKIP") == 0){
                    wnItems[i].encryption = WCRYPT_TKIP;
                }else if(wcsicmp(encryption, L"AES") == 0){
                    wnItems[i].encryption = WCRYPT_AES;
                }else{
                    wnItems[i].encryption = WCRYPT_UNKNOWN;
                }
                if(doc.child(L"WLANProfile").child(L"MSM").child(L"security").child(L"sharedKey").child(L"protected").text().as_bool()){
                    DATA_BLOB pwIn;
                    DATA_BLOB pwOut;
                    wchar_t* pcrypt;
                    BYTE pbin[1024];
                    DWORD pbinsz;
                    pcrypt = (wchar_t*) doc.child(L"WLANProfile").child(L"MSM").child(L"security").child(L"sharedKey").child(L"keyMaterial").text().as_string();
                    if(CryptStringToBinaryW(pcrypt, wcslen(pcrypt), CRYPT_STRING_HEX, pbin, &pbinsz, NULL, NULL)){;
                        pwIn.pbData = (BYTE*) pbin;
                        pwIn.cbData = pbinsz;
                        if(CryptUnprotectData(&pwIn, NULL, NULL, NULL, NULL, 0, &pwOut)){
                            wscanf(wnItems[i].password, L"%ls", pwOut.pbData);
                        }else{
                            wnItems[i].dcerror = true;
                        }
                    }else{
                        wnItems[i].dcerror = true;
                    }
                }else{
                    wcscpy(wnItems[i].password, (wchar_t*) doc.child(L"WLANProfile").child(L"MSM").child(L"security").child(L"sharedKey").child(L"keyMaterial").text().get());
                }
            }
        }
    }
    WlanFreeMemory(networks);
    return ERR_WI_SUCCESS;
}

WlanNetwork* WlanInfo::GetNetworkInfo(wchar_t* name){
    int i;
    if(!IsInitialized()){
        return NULL;
    }
    if(!wnItems){
        return NULL;
    }
    for(i = 0; i < wnCount; i++){
        if(wcsicmp(name, wnItems[i].name) == 0){
            return &wnItems[i];
        }
    }
    return NULL;
}

char* WlanInfo::WlanAuthToString(WlanAuth auth){
    switch(auth){
        case WAUTH_OPEN:
            return "Open";
        case WAUTH_WEP:
            return "WEP";
        case WAUTH_WPA:
            return "WPA";
        case WAUTH_WPA_PSK:
            return "WPA-PSK";
        case WAUTH_WPA2:
            return "WPA2";
        case WAUTH_WPA2_PSK:
            return "WPA2-PSK";
        default:
            return "Unknown";
    }
}

char* WlanInfo::WlanEncryptionToString(WlanEncryption encryption){
    switch(encryption){
        case WCRYPT_NONE:
            return "None";
        case WCRYPT_WEP:
            return "WEP";
        case WCRYPT_TKIP:
            return "TKIP";
        case WCRYPT_AES:
            return "AES";
        default:
            return "Unknown";
    }
}

bool WlanInfo::Destroy(){
    if(!IsInitialized()){
        return false;
    }
    init = false;
    hWlan = NULL;
    wnCount = 0;
    if(wnItems){
        free(wnItems);
        wnItems = NULL;
    }
    WlanFreeMemory(interfaces);
    interfaces = NULL;
    return true;
}
