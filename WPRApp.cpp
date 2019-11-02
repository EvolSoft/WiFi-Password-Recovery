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

#include "WPRApp.h"

#include "LanguageHelper.h"
#include "NetworkWizard.h"
#include "WPRMain.h"
#include "WelcomeDialog.h"
#include "WlanInfo.h"

//For a future cross-platform support
//#ifdef WINDOWS
#include <windows.h>
#include <VersionHelpers.h>
//#endif

#include <wx/fileconf.h>
#include <wx/filename.h>
#include <wx/image.h>
#include <wx/msgdlg.h>
#include <wx/stdpaths.h>

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

bool WPRApp::RunAsAdmin(wxString arg){
    wchar_t curPath[MAX_PATH];
    if(GetModuleFileName(NULL, curPath, sizeof(curPath))){
        SHELLEXECUTEINFO shExecInfo;
        shExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
        shExecInfo.fMask = NULL;
        shExecInfo.hwnd = NULL;
        shExecInfo.lpVerb = L"runas";
        shExecInfo.lpFile = curPath;
        if(arg.IsEmpty()){
            shExecInfo.lpParameters = NULL;
        }else{
            shExecInfo.lpParameters = (wchar_t*) arg.wchar_str();
        }
        shExecInfo.lpDirectory = NULL;
        shExecInfo.nShow = SW_SHOW;
        shExecInfo.hInstApp = NULL;
        return ShellExecuteEx(&shExecInfo);
    }
    return false;
}

void WPRApp::InitConfig(){
    int localeCfg;
    bool welcomeMessage;
    if(config->Read(wxT("locale"), &localeCfg)){ //Language configuration
        if(!LanguageHelper::GetInstance()->ApplyLanguage(localeCfg)){
            config->Write(wxT("locale"), LanguageHelper::GetDefaultLanguage());
        }
    }else if(LanguageHelper::GetInstance()->ApplyLanguage(localeCfg)){
        config->Write(wxT("locale"), localeCfg);
    }else{
        config->Write(wxT("locale"), LanguageHelper::GetDefaultLanguage());
    }
    if(!config->Read(wxT("welcome_message"), &welcomeMessage)){
        config->Write(wxT("welcome_message"), true);
    }
}

bool WPRApp::OnInit(){
    bool showWelcomeDialog = true;
    wxString arg = wxEmptyString;
    wxInitAllImageHandlers();
    wxFileName configPath = wxStandardPaths::Get().GetExecutablePath();
    config = new wxFileConfig(wxEmptyString, wxEmptyString, configPath.GetPath().Append(wxT("/config.ini")));
    InitConfig();
    if(!IsWindowsXPSP3OrGreater()){
        wxMessageBox(_("This program can only run on Windows XP SP3 or later."), wxT("WiFi Password Recovery"), wxOK | wxICON_ERROR);
        return false;
    }
    if(argc >= 2){
        arg = argv[1];
    }
    if(IsWindowsVistaOrGreater() && !IsElevated()){ //Windows XP handles Admin permissions differently
        RunAsAdmin(arg);
        return false;
    }
    switch(WlanInfo::InitWlanInterface()){
        case ERR_WI_WLAN_INIT:
            wxMessageBox(_("Failed to initialize Wlan Interfaces.\n\nTry to run the program as administrator."), wxT("WiFi Password Recovery"), wxOK | wxICON_ERROR);
            return false;
        case ERR_WI_ENUM_INIT:
            wxMessageBox(_("Failed to enumerate Wlan Interfaces.\n\nTry to run the program as administrator."), wxT("WiFi Password Recovery"), wxOK | wxICON_ERROR);
            return false;
        case ERR_WI_NO_INT:
            wxMessageBox(_("No Wlan interfaces found."), wxT("WiFi Password Recovery"), wxOK | wxICON_WARNING);
            return false;
    }
    if(arg.IsEmpty()){ //If argument is empty, show default windows
        WPRMain* Frame = new WPRMain(0, config);
        Frame->Show();
        SetTopWindow(Frame);
        Frame->CenterOnScreen();
        config->Read(wxT("welcome_message"), &showWelcomeDialog); //Show welcome dialog
        if(showWelcomeDialog){
            WelcomeDialog welcomeDialog(Frame, config);
            welcomeDialog.ShowModal();
        }
    }else{
        NetworkWizard* networkWizard = new NetworkWizard(NULL, arg);
        return false;
    }
    return true;
}
