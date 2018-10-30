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

#include "WPRApp.h"

#include "WPRMain.h"
#include "WlanInfo.h"

//For a future cross-platform support
//#ifdef WINDOWS
#include <windows.h>
#include <VersionHelpers.h>
//#endif

#include <wx/msgdlg.h>
#include <wx/image.h>

#include <stdio.h>

IMPLEMENT_APP(WPRApp);

bool WPRApp::IsElevated(){
    bool status = false;
    HANDLE hToken = NULL;
    if(OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)){
        TOKEN_ELEVATION tElev;
        DWORD tElevSz = sizeof(TOKEN_ELEVATION);
        if(GetTokenInformation(hToken, TokenElevation, &tElev, sizeof(tElev), &tElevSz)){
            status = tElev.TokenIsElevated;
        }
    }
    if(hToken){
        CloseHandle(hToken);
    }
    return status;
}

bool WPRApp::RunAsAdmin(){
    wchar_t curPath[MAX_PATH];
    if(GetModuleFileName(NULL, curPath, sizeof(curPath))){
        SHELLEXECUTEINFO shExecInfo;
        shExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
        shExecInfo.fMask = NULL;
        shExecInfo.hwnd = NULL;
        shExecInfo.lpVerb = L"runas";
        shExecInfo.lpFile = curPath;
        shExecInfo.lpParameters = NULL;
        shExecInfo.lpDirectory = NULL;
        shExecInfo.nShow = SW_SHOW;
        shExecInfo.hInstApp = NULL;
        return ShellExecuteEx(&shExecInfo);
    }
    return false;
}

bool WPRApp::OnInit(){
    wxInitAllImageHandlers();
    if(!IsWindowsXPSP3OrGreater()){
        wxMessageBox(wxT("This program can only run on Windows XP SP3 or later."), wxT("Error"), wxOK | wxICON_ERROR);
        return false;
    }
    if(!IsElevated()){
        RunAsAdmin();
        return false;
    }
    switch(WlanInfo::InitWlanInterface()){
        case ERR_WI_WLAN_INIT:
            wxMessageBox(wxT("Failed to initialize WLAN Interfaces.\n\nTry to run the program as administrator."), wxT("Error"), wxOK | wxICON_ERROR);
            return false;
        case ERR_WI_ENUM_INIT:
            wxMessageBox(wxT("Failed to enumerate WLAN Interfaces."), wxT("Error"), wxOK | wxICON_ERROR);
            return false;
        case ERR_WI_NO_INT:
            wxMessageBox(wxT("No WLAN interfaces found."), wxT("Warning"), wxOK | wxICON_WARNING);
            return false;
    }
    WPRMain* Frame = new WPRMain(0);
    Frame->Show();
    SetTopWindow(Frame);
    Frame->CenterOnScreen();
    return true;
}
