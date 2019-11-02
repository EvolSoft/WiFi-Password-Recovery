/***************************************************************
 *
 * WiFi Password Recovery
 *
 * Author:    Flavio Collocola
 * Copyright: (C) 2018-2019 EvolSoft (www.evolsoft.org)
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

#include <wx/mstream.h>
#include <wx/sstream.h>
#include <wx/string.h>
#include "XMLHelper.h"

//For a future cross-platform support
//#ifdef WINDOWS
#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "crypt32.lib")
#pragma comment(lib, "wlanapi.lib")
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

DWORD WlanInfo::GetProcessID(wchar_t* szName){
    DWORD dwId = 0;
    HANDLE hSnap;
    BOOL bResult;
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);
    hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if(hSnap != INVALID_HANDLE_VALUE){
        bResult = Process32First(hSnap, &pe32);
        while(bResult){
            if(lstrcmpi(pe32.szExeFile, szName) == 0) {
                dwId = pe32.th32ProcessID;
                break;
            }
            bResult = Process32Next(hSnap, &pe32);
        }
        CloseHandle(hSnap);
    }
    return dwId;
}

bool WlanInfo::SetPrivilege(wchar_t* szPrivilege){
    HANDLE hToken;
    BOOL bResult;
    LUID luid;
    TOKEN_PRIVILEGES tp;
    bResult = OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken);
    if(bResult){
        bResult = LookupPrivilegeValue(NULL, szPrivilege, &luid);
        if(bResult){
            tp.PrivilegeCount = 1;
            tp.Privileges[0].Luid = luid;
            tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
            bResult = AdjustTokenPrivileges(hToken, FALSE, &tp, 0, NULL, NULL);
        }
        CloseHandle(hToken);
    }
    return bResult;
}

bool WlanInfo::ImpersonateLoginUser(){
    HANDLE hToken, hProcess;
    DWORD  dwId = GetProcessID(L"winlogon.exe");
    if(dwId){
        if(SetPrivilege(SE_DEBUG_NAME)){
            hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwId);
            if(hProcess != NULL){
                if(OpenProcessToken(hProcess, TOKEN_IMPERSONATE | TOKEN_DUPLICATE | TOKEN_QUERY, &hToken)){
                    if(ImpersonateLoggedOnUser(hToken)){
                        return true;
                    }
                    CloseHandle(hToken);
                }
                CloseHandle(hProcess);
            }
        }
    }
    return false;
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
                XmlHelper doc;
                wchar_t* auth;
                wchar_t* encryption;
                char* password;
                doc.LoadFromString(wxString(networkXML), wxT("UTF-8"), wxXMLDOC_KEEP_WHITESPACE_NODES);
                auth = (wchar_t*) doc.GetRoot()->GetChild(wxT("MSM"))->GetChild(wxT("security"))->GetChild(wxT("authEncryption"))->GetChild(wxT("authentication"))->GetContent().wc_str();
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
                encryption = (wchar_t*) doc.GetRoot()->GetChild(wxT("MSM"))->GetChild(wxT("security"))->GetChild(wxT("authEncryption"))->GetChild(wxT("encryption"))->GetContent().wc_str();
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
                if(doc.GetRoot()->GetChild(wxT("MSM"))->GetChild(wxT("security"))->GetChild(wxT("sharedKey"))->GetChild(wxT("protected"))->GetContent().CmpNoCase(wxT("true")) == 0){
                    BYTE pwdBin[1024] = {0};
                    DWORD pwdBinSz = 1024;
                    DATA_BLOB pwdIn, pwdOut;
                    wchar_t* pcrypt;
                    pcrypt = (wchar_t*) doc.GetRoot()->GetChild(wxT("MSM"))->GetChild(wxT("security"))->GetChild(wxT("sharedKey"))->GetChild(wxT("keyMaterial"))->GetContent().wc_str();
                    ImpersonateLoginUser();
                    if(CryptStringToBinary(pcrypt, wcslen(pcrypt), CRYPT_STRING_HEX, pwdBin, &pwdBinSz, NULL, NULL)){
                        pwdIn.pbData = (BYTE*) pwdBin;
                        pwdIn.cbData = pwdBinSz;
                        if(CryptUnprotectData(&pwdIn, NULL, NULL, NULL, NULL, 0, &pwdOut)){
                            mbstowcs(wnItems[i].password, (char*) pwdOut.pbData, MAX_PASSWORD_LENGTH);
                        }else{
                            wnItems[i].dcerror = true;
                        }
                    }else{
                        wnItems[i].dcerror = true;
                    }
                    RevertToSelf();
                }else{
                    wcscpy(wnItems[i].password, (wchar_t*) doc.GetRoot()->GetChild(wxT("MSM"))->GetChild(wxT("security"))->GetChild(wxT("sharedKey"))->GetChild(wxT("keyMaterial"))->GetContent().wc_str());
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

wxString WlanInfo::WlanAuthToString(WlanAuth auth){
    switch(auth){
        case WAUTH_OPEN:
            return _("Open");
        case WAUTH_WEP:
            return wxT("WEP");
        case WAUTH_WPA:
            return wxT("WPA");
        case WAUTH_WPA_PSK:
            return wxT("WPA-PSK");
        case WAUTH_WPA2:
            return wxT("WPA2");
        case WAUTH_WPA2_PSK:
            return wxT("WPA2-PSK");
        default:
            return _("Unknown");
    }
}

wxString WlanInfo::WlanEncryptionToString(WlanEncryption encryption){
    switch(encryption){
        case WCRYPT_NONE:
            return _("None");
        case WCRYPT_WEP:
            return wxT("WEP");
        case WCRYPT_TKIP:
            return wxT("TKIP");
        case WCRYPT_AES:
            return wxT("AES");
        default:
            return _("Unknown");
    }
}

int WlanInfo::Import(WlanNetwork network){
    return Import(network, false);
}

int WlanInfo::Import(WlanNetwork network, bool overwrite){
    DWORD status, reasonCode;
    wxXmlDocument doc = wxXmlDocument();
    wxXmlNode* name,* ssidConfig, *ssid, *ssidName, *connType, *connMode, *msm, *security, *authEncrypt, *auth, *encrypt, *useOneX, *sharedKey, *keyType, *protect, *keyMaterial;
    wxString authVal, encryptVal;
    wxStringOutputStream xmlStream;
    doc.SetRoot(new wxXmlNode(wxXML_ELEMENT_NODE, wxT("WLANProfile")));
    doc.GetRoot()->AddAttribute(wxT("xmlns"), wxT("http://www.microsoft.com/networking/WLAN/profile/v1"));
    name = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("name"));
    name->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxT(""), network.name));
    doc.GetRoot()->AddChild(name);
    ssidConfig = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("SSIDConfig"));
    ssid = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("SSID"));
    ssidName = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("name"));
    ssidName->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxT(""), network.name));
    ssid->AddChild(ssidName);
    ssidConfig->AddChild(ssid);
    doc.GetRoot()->AddChild(ssidConfig);
    connType = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("connectionType"));
    connType->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxT(""), wxT("ESS")));
    doc.GetRoot()->AddChild(connType);
    connMode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("connectionMode"));
    connMode->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxT(""), wxT("manual")));
    doc.GetRoot()->AddChild(connMode);
    msm = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("MSM"));
    security = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("security"));
    authEncrypt = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("authEncryption"));
    auth = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("authentication"));
    switch(network.auth){
    case WAUTH_OPEN:
        authVal = wxT("open");
        break;
    case WAUTH_WEP:
        authVal = wxT("shared");
        break;
    case WAUTH_WPA:
        authVal = wxT("WPA");
        break;
    case WAUTH_WPA_PSK:
        authVal = wxT("WPAPSK");
        break;
    case WAUTH_WPA2:
        authVal = wxT("WPA2");
        break;
    case WAUTH_WPA2_PSK:
        authVal = wxT("WPA2PSK");
        break;
    default:
        authVal = wxT("");
        break;
    }
    auth->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxT(""), authVal));
    encrypt = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("encryption"));
    switch(network.encryption){
    case WCRYPT_NONE:
        encryptVal = wxT("none");
        break;
    case WCRYPT_WEP:
        encryptVal = wxT("WEP");
        break;
    case WCRYPT_TKIP:
        encryptVal = wxT("TKIP");
        break;
    case WCRYPT_AES:
        encryptVal = wxT("AES");
        break;
    default:
        encryptVal = wxT("");
    }
    encrypt->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxT(""), encryptVal));
    useOneX = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("useOneX"));
    useOneX->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxT(""), wxT("false")));
    authEncrypt->AddChild(auth);
    authEncrypt->AddChild(encrypt);
    authEncrypt->AddChild(useOneX);
    security->AddChild(authEncrypt);
    if(network.encryption != WCRYPT_NONE){
        sharedKey = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("sharedKey"));
        keyType = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("keyType"));
        keyType->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxT(""), wxT("passPhrase")));
        sharedKey->AddChild(keyType);
        protect = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("protected"));
        protect->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxT(""), wxT("false")));
        sharedKey->AddChild(protect);
        keyMaterial = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("keyMaterial"));
        keyMaterial->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxT(""), network.password));
        sharedKey->AddChild(keyMaterial);
        security->AddChild(sharedKey);
    }
    msm->AddChild(security);
    doc.GetRoot()->AddChild(msm);
    doc.Save(xmlStream, 2);
    status = WlanSetProfile(hWlan, &interfaces->InterfaceInfo[0].InterfaceGuid, 0, (wchar_t*) xmlStream.GetString().wc_str(), NULL, overwrite, NULL, &reasonCode);
    switch(status){
    case ERROR_SUCCESS:
        return ERR_WI_SUCCESS;
    case ERROR_ALREADY_EXISTS:
        return ERR_WI_ALREADY_EXISTS;
    default:
        return ERR_WI_GENERIC;
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
