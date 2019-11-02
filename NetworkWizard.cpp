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

#include "NetworkWizard.h"

#include <wx/filedlg.h>
#include <wx/filename.h>
#include <wx/font.h>
#include <wx/icon.h>
#include <wx/intl.h>
#include <wx/listbox.h>
#include <wx/msgdlg.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/string.h>
#include <wx/wfstream.h>

BEGIN_EVENT_TABLE(NetworkWizard, wxWizard)
END_EVENT_TABLE()

NetworkWizard::NetworkWizard(wxWindow* parent, bool import){
    Create(parent, wxID_ANY, wxEmptyString, wxBitmap(wxT("WIZARD_BITMAP"), wxBITMAP_TYPE_BMP_RESOURCE));
    if(import){
        SetTitle(_("Wireless Networks Import Wizard"));
    }else{
        SetTitle(_("Wireless Networks Export Wizard"));
    }
    SetIcon(wxICON(APP_ICON));
    SetPageSize(wxSize(334, 316));
    data.import = import;
    data.canExit = true;
    data.askExit = false;
    WelcomePage();
    pathPage = new NetworkWizardPathPage(this, &data);
    progressPage = new NetworkWizardProgressPage(this, &data, WlanInfo::GetNetworksList());
    customPage = new NetworkWizardCustomPage(this, &data);
    chooserPage = new NetworkWizardChooserPage(this, pathPage, progressPage, customPage, &data);
    welcomePage->SetNext(pathPage);
    pathPage->SetPrev(welcomePage);
    pathPage->SetNext(chooserPage);
    customPage->SetPrev(chooserPage);
    customPage->SetNext(progressPage);
    Connect(wxID_ANY, wxEVT_WIZARD_CANCEL, (wxObjectEventFunction) &NetworkWizard::OnWizardCancel);
	RunWizard(welcomePage);
}

NetworkWizard::NetworkWizard(wxWindow* parent, wxString path){
    Create(parent, wxID_ANY, _("Wireless Networks Import Wizard"), wxBitmap(wxT("WIZARD_BITMAP"), wxBITMAP_TYPE_BMP_RESOURCE));
    SetIcon(wxICON(APP_ICON));
    data.import = true;
    data.path = path;
    progressPage = new NetworkWizardProgressPage(this, &data, WlanInfo::GetNetworksList());
    customPage = new NetworkWizardCustomPage(this, &data);
    chooserPage = new NetworkWizardChooserPage(this, NULL, progressPage, customPage, &data);
    customPage->SetPrev(chooserPage);
    customPage->SetNext(progressPage);
    Connect(wxID_ANY, wxEVT_WIZARD_CANCEL, (wxObjectEventFunction) &NetworkWizard::OnWizardCancel);
	RunWizard(chooserPage);
}

NetworkWizard::~NetworkWizard(){}

void NetworkWizard::OnWizardCancel(wxWizardEvent& event){
    if(data.canExit){
        if(data.askExit){
            int response;
            response = wxMessageBox(_("Are you sure you want to quit?"), _("Exit Wizard"), wxYES_NO | wxICON_WARNING);
            if(response == wxNO){
                event.Veto();
            }
        }
    }else{
        wxMessageBox(_("Could not cancel wizard when operation is in progress."), wxT("WiFi Password Recovery"), wxOK | wxICON_ERROR);
        event.Veto();
    }
}

void NetworkWizard::WelcomePage(){
    welcomePage = new wxWizardPageSimple(this);
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    wxStaticText* title = new wxStaticText(welcomePage, wxID_ANY, wxT(""));
    title->SetFont(wxFont(14, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    wxStaticText* text = new wxStaticText(welcomePage, wxID_ANY, wxT(""));
    if(data.import){
        title->SetLabel(_("Welcome to the Wireless Networks Import Wizard"));
        text->SetLabel(wxString::Format(_("This wizard will guide you through the import of wireless networks to your computer.\n"
                         "You can import the networks exported through %s.\n"
                         "Warning: the program may hang during the import process.\n\n\n"
                         "Click Next to continue."), wxT("WiFi Password Recovery")));
    }else{
        title->SetLabel(_("Welcome to the Wireless Networks Export Wizard"));
        text->SetLabel(wxString::Format(_("This wizard will guide you through the export of wireless networks stored on your computer.\n"
                         "You can then import the exported networks to other devices via %s.\n"
                         "Warning: the program may hang during the export process.\n\n\n"
                         "Click Next to continue."),wxT("WiFi Password Recovery")));
    }
    title->Wrap(GetPageSize().GetWidth());
    text->Wrap(GetPageSize().GetWidth() - 1);
    sizer->Add(title, 0, wxALL, 5);
    sizer->AddSpacer(5);
    sizer->Add(text, 0, wxALL, 5);
    //sizer->Fit(welcomePage);
    welcomePage->SetSizer(sizer);
    welcomePage->Fit();
    //GetPageAreaSizer()->Add(welcomePage);
}

void NetworkWizard::SetButtonStatus(int id, bool status){
    wxWindow* windowObj = FindWindowById(id);
    if(windowObj){
        windowObj->Enable(status);
    }
}

NetworkWizardPathPage::NetworkWizardPathPage(wxWizard* parent, NetworkWizardData* data){
    wxBoxSizer* boxSizer;
    wxStaticBox* staticBox;
    wxStaticBoxSizer* staticBoxSizer;
    wxStaticText* labelDesc;
    Create(parent);
    this->data = data;
    data->networkData = NetworkData();
    data->selectedNetworks = wxArrayString();
    boxSizer = new wxBoxSizer(wxVERTICAL);
    if(data->import){
        labelDesc = new wxStaticText(this, wxID_ANY, wxString::Format(wxT("%s\n"), _("Select the path where you want to import WiFi networks from. Click Next to continue.")));
        filePickerDest = new wxFilePickerCtrl(this, wxID_ANY, wxEmptyString, wxEmptyString, wxT("WiFi Password Recovery network data (*.wprnd)|*.wprnd"), wxDefaultPosition, wxDefaultSize, wxFLP_OPEN | wxFLP_FILE_MUST_EXIST | wxFLP_USE_TEXTCTRL);
        staticBox = new wxStaticBox(this, wxID_ANY, _("Source file"));
    }else{
        labelDesc = new wxStaticText(this, wxID_ANY, wxString::Format(wxT("%s\n"), _("Select the path where you want to export WiFi networks. Click Next to continue.")));
        checkBoxEncryptData = new wxCheckBox(this, wxID_ANY, _("Encrypt exported data"));
        filePickerDest = new wxFilePickerCtrl(this, wxID_ANY, wxEmptyString, wxEmptyString, wxT("WiFi Password Recovery network data (*.wprnd)|*.wprnd"), wxDefaultPosition, wxDefaultSize, wxFLP_SAVE | wxFLP_OVERWRITE_PROMPT | wxFLP_USE_TEXTCTRL);
        staticBox = new wxStaticBox(this, wxID_ANY, _("Destination file"));
    }
    filePickerDest->GetTextCtrl()->SetEditable(false); //Make file picker control textbox not editable
    staticBoxSizer = new wxStaticBoxSizer(staticBox, wxVERTICAL);
    staticBoxSizer->Add(filePickerDest, 0, wxGROW | wxALIGN_LEFT | wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, 5);
    labelDesc->Wrap(parent->GetPageSize().GetWidth());
    boxSizer->Add(labelDesc, 0, wxALL, 5);
    boxSizer->Add(staticBoxSizer, 0, wxGROW | wxALL, 5);
    boxSizer->AddSpacer(5);
    if(!data->import){
        boxSizer->Add(checkBoxEncryptData, 0, wxLEFT, 15);
    }
    SetSizer(boxSizer);
    boxSizer->Fit(this);
    Connect(wxID_ANY, wxEVT_WIZARD_PAGE_SHOWN, (wxObjectEventFunction) &NetworkWizardPathPage::OnShow);
    Connect(wxID_ANY, wxEVT_WIZARD_PAGE_CHANGING, (wxObjectEventFunction) &NetworkWizardPathPage::OnPageChange);
}

void NetworkWizardPathPage::OnShow(wxWizardEvent& event){
    data->askExit = true;
}

void NetworkWizardPathPage::OnPageChange(wxWizardEvent& event){
    if(event.GetDirection()){
        if(!filePickerDest->GetPath()){
            wxMessageBox(_("Please specify a valid path."), wxT("WiFi Password Recovery"), wxOK | wxICON_ERROR);
            event.Veto();
            return;
        }else{
            wxFileName path = wxFileName(filePickerDest->GetPath());
            if(path.GetExt().CmpNoCase(wxT("wprnd")) != 0){
                data->path = path.GetFullPath().Append(wxT(".wprnd"));
            }else{
                data->path = path.GetFullPath();
            }
            if(data->import){
                data->encrypt = false; //Default value for import (since this field is unused when importing networks)
                switch(data->networkData.Import(data->path)){
                case ERR_FILE_OPEN:
                    wxMessageBox(wxString::Format(_("Could not open \"%s\"."), data->path), wxT("WiFi Password Recovery"), wxOK | wxICON_ERROR);
                    event.Veto();
                    break;
                case ERR_FILE_INVALID_VERSION:
                    wxMessageBox(wxString::Format(_("Could not open \"%s\".\n\nInvalid file version."), data->path), wxT("WiFi Password Recovery"), wxOK | wxICON_ERROR);
                    event.Veto();
                    break;
                case ERR_FILE_CORRUPT:
                    wxMessageBox(wxString::Format(_("Could not open \"%s\".\n\nFile corrupted."), data->path), wxT("WiFi Password Recovery"), wxOK | wxICON_ERROR);
                    event.Veto();
                    break;
                case ERR_FILE_DECRYPT:
                    wxMessageBox(wxString::Format(_("Could not open \"%s\".\n\nFile decryption failed."), data->path), wxT("WiFi Password Recovery"), wxOK | wxICON_ERROR);
                    event.Veto();
                    break;
                default:
                case SUCCESS:
                    break;
                }
            }else{
                data->encrypt = checkBoxEncryptData->GetValue();
            }
        }
    }
}

NetworkWizardChooserPage::NetworkWizardChooserPage(wxWizard* parent, wxWizardPage* previousPage, wxWizardPage* progressPage, wxWizardPage* customPage, NetworkWizardData* data){
    wxBoxSizer* boxSizer;
    wxRadioButton* radioButtonCustom;
    wxStaticText* labelDesc, *labelDescAll, *labelDescCustom;
    Create(parent);
    this->data = data;
    this->previousPage = previousPage;
    this->progressPage = progressPage;
    this->customPage = customPage;
    boxSizer = new wxBoxSizer(wxVERTICAL);
    radioButtonAll = new wxRadioButton(this, wxID_ANY, _("All"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
    radioButtonCustom = new wxRadioButton(this, wxID_ANY, _("Custom"), wxDefaultPosition, wxDefaultSize);
    if(data->import || previousPage == NULL){
        labelDesc = new wxStaticText(this, wxID_ANY, wxString::Format(wxT("%s\n"), _("Please select an import type.")));
        labelDescAll = new wxStaticText(this, wxID_ANY, _("All wireless networks stored in the file will be imported."));
        labelDescCustom = new wxStaticText(this, wxID_ANY, _("Choose which wireless networks you want to import."));
    }else{
        labelDesc = new wxStaticText(this, wxID_ANY, wxString::Format(wxT("%s\n"), _("Please select an export type.")));
        labelDescAll = new wxStaticText(this, wxID_ANY, _("All wireless networks available on this computer will be exported."));
        labelDescCustom = new wxStaticText(this, wxID_ANY, _("Choose which wireless networks you want to export."));
    }
    radioButtonAll->SetValue(true);
    labelDesc->Wrap(parent->GetPageSize().GetWidth());
    labelDescAll->Wrap(parent->GetPageSize().GetWidth());
    labelDescCustom->Wrap(parent->GetPageSize().GetWidth());
    boxSizer->Add(labelDesc, 0, wxALL, 5);
    boxSizer->Add(radioButtonAll, 0, wxALL, 5);
    boxSizer->Add(labelDescAll, 0, wxALL, 5);
    boxSizer->AddSpacer(5);
    boxSizer->Add(radioButtonCustom, 0, wxALL, 5);
    boxSizer->Add(labelDescCustom, 0, wxALL, 5);
    SetSizer(boxSizer);
    boxSizer->Fit(this);
    Connect(wxID_ANY, wxEVT_WIZARD_PAGE_SHOWN, (wxObjectEventFunction) &NetworkWizardChooserPage::OnShow);
}

void NetworkWizardChooserPage::OnShow(wxWizardEvent& event){
    if(this->previousPage == NULL && event.GetDirection()){ //Show message boxes and import data only when showing the page for the first time
        wxFileName path = wxFileName(data->path);
        data->canExit = true;
        data->askExit = false;
        if(!path.FileExists()){
            wxMessageBox(wxString::Format(_("Could not open \"%s\".\n\nFile not found."), (wchar_t*) data->path.wchar_str()), wxT("WiFi Password Recovery"), wxOK | wxICON_ERROR);
            GetParent()->Close();
            GetParent()->Destroy();
            return;
        }else if(path.GetExt().CmpNoCase(wxT("wprnd")) != 0){
            wxMessageBox(wxString::Format(_("Could not open \"%s\".\n\nInvalid file format."), (wchar_t*) data->path.wchar_str()), wxT("WiFi Password Recovery"), wxOK | wxICON_ERROR);
            GetParent()->Close();
            GetParent()->Destroy();
            return;
        }else{
            int response;
            response = wxMessageBox(wxString::Format(_("Do you want to import networks from file \"%s\"?"), (wchar_t*) path.GetFullPath().wchar_str()), wxT("WiFi Password Recovery"), wxYES_NO | wxICON_INFORMATION);
            if(response == wxNO){
                GetParent()->Close();
                GetParent()->Destroy();
                return;
            }else{
                switch(data->networkData.Import(data->path)){
                    case ERR_FILE_OPEN:
                        wxMessageBox(wxString::Format(_("Could not open \"%s\"."), (wchar_t*) data->path.wchar_str()), wxT("WiFi Password Recovery"), wxOK | wxICON_ERROR);
                        GetParent()->Close();
                        GetParent()->Destroy();
                        return;
                    case ERR_FILE_INVALID_VERSION:
                        wxMessageBox(wxString::Format(_("Could not open \"%s\".\n\nInvalid file version."), (wchar_t*) data->path.wchar_str()), wxT("WiFi Password Recovery"), wxOK | wxICON_ERROR);
                        GetParent()->Close();
                        GetParent()->Destroy();
                        return;
                    case ERR_FILE_CORRUPT:
                        wxMessageBox(wxString::Format(_("Could not open \"%s\".\n\nFile corrupted."), (wchar_t*) data->path.wchar_str()), wxT("WiFi Password Recovery"), wxOK | wxICON_ERROR);
                        GetParent()->Close();
                        GetParent()->Destroy();
                        return;
                    case ERR_FILE_DECRYPT:
                        wxMessageBox(wxString::Format(_("Could not open \"%s\".\n\nFile decryption failed."), (wchar_t*) data->path.wchar_str()), wxT("WiFi Password Recovery"), wxOK | wxICON_ERROR);
                        GetParent()->Close();
                        GetParent()->Destroy();
                        return;
                    default:
                    case SUCCESS:
                        data->askExit = true;
                        break;
                }
            }
        }
    }
}

wxWizardPage* NetworkWizardChooserPage::GetPrev() const {
    return previousPage;
}

wxWizardPage* NetworkWizardChooserPage::GetNext() const {
    if((data->selectAll = radioButtonAll->GetValue())){
        return progressPage;
    }
    return customPage;
}

NetworkWizardCustomPage::NetworkWizardCustomPage(wxWizard* parent, NetworkWizardData* data){
    Create(parent);
    this->data = data;
    wxBoxSizer* boxSizer;
    wxStaticText* label;
    boxSizer = new wxBoxSizer(wxVERTICAL);
    label = new wxStaticText(this, wxID_ANY, wxT(""));
    listBoxNetworks = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, NULL, wxLB_EXTENDED);
    if(data->import){
        label->SetLabel(_("Choose the wireless networks you want to import:"));
    }else{
        label->SetLabel(_("Choose the wireless networks you want to export:"));
    }
    label->Wrap(parent->GetPageSize().GetWidth());
    boxSizer->Add(label, 0, wxALL, 5);
    boxSizer->Add(listBoxNetworks, 1, wxALL | wxEXPAND, 5);
    SetSizer(boxSizer);
    boxSizer->Fit(this);
    Connect(wxID_ANY, wxEVT_WIZARD_PAGE_SHOWN, (wxObjectEventFunction) &NetworkWizardCustomPage::OnShow);
    Connect(wxID_ANY, wxEVT_COMMAND_LISTBOX_SELECTED, (wxObjectEventFunction) &NetworkWizardCustomPage::OnListBoxSelect);
    Connect(wxID_ANY, wxEVT_WIZARD_PAGE_CHANGING, (wxObjectEventFunction) &NetworkWizardCustomPage::OnPageChange);
}

void NetworkWizardCustomPage::OnShow(wxCommandEvent& event){
    listBoxNetworks->Clear();
    if(data->import){
        vector<NetworkDataItem> networks = data->networkData.GetNetworksList();
        for(int i = 0; i < networks.size(); i++){
            listBoxNetworks->Append(networks[i].name);
        }
    }else{
        WlanNetwork* networks = WlanInfo::GetNetworksList();
        for(int i = 0; i < WlanInfo::GetNetworksCount(); i++){
            listBoxNetworks->Append(networks[i].name);
        }
    }
    if(listBoxNetworks->GetCount() > 0){
        listBoxNetworks->Select(0); //Set default choice
    }
}

void NetworkWizardCustomPage::OnListBoxSelect(wxCommandEvent& event){
    wxArrayInt selections;
    listBoxNetworks->GetSelections(selections);
    if(selections.IsEmpty()){
        NetworkWizard::SetButtonStatus(wxID_FORWARD, false);
    }else{
        NetworkWizard::SetButtonStatus(wxID_FORWARD, true);
    }
}

void NetworkWizardCustomPage::OnPageChange(wxWizardEvent& event){
    if(event.GetDirection()){
        wxArrayInt selections;
        listBoxNetworks->GetSelections(selections);
        if(!selections.IsEmpty()){
            data->selectedNetworks.Empty();
            for(int i = 0; i < selections.GetCount(); i++){
                data->selectedNetworks.Add(listBoxNetworks->GetString(selections[i]));
            }
        }
    }
}

NetworkWizardProgressPage::NetworkWizardProgressPage(wxWizard* parent, NetworkWizardData* data, WlanNetwork* wlanNetworks){
    wxBoxSizer* boxSizer;
    Create(parent);
    this->data = data;
    this->wlanNetworks = wlanNetworks;
    boxSizer = new wxBoxSizer(wxVERTICAL);
    if(data->import){
        labelProgressDesc = new wxStaticText(this, wxID_ANY, wxT("Initializing import"));
    }else{
        labelProgressDesc = new wxStaticText(this, wxID_ANY, wxT("Initializing export"));
    }
    progressBar = new wxGauge(this, wxID_ANY, 0, wxDefaultPosition, wxSize(-1, 20)); //Add + 1 to the range because of save file step
    boxSizer->Add(labelProgressDesc, 0, wxLEFT | wxTOP | wxRIGHT, 5);
    boxSizer->Add(progressBar, 0, wxEXPAND | wxALL, 5);
    SetSizer(boxSizer);
    boxSizer->Fit(this);
    Connect(wxID_ANY, wxEVT_WIZARD_PAGE_SHOWN, (wxObjectEventFunction) &NetworkWizardProgressPage::OnShow);
}

void NetworkWizardProgressPage::OnShow(wxCommandEvent& event){
    int i;
    vector<NetworkDataItem> importedNetworks = data->networkData.GetNetworksList();
    data->canExit = false;
    NetworkWizard::SetButtonStatus(wxID_FORWARD, false);
    NetworkWizard::SetButtonStatus(wxID_CANCEL, false);
    if(data->import){
        if(data->selectAll){
            progressBar->SetRange(importedNetworks.size());
            for(i = 0; i < importedNetworks.size(); i++){
                int importStatus, response;
                labelProgressDesc->SetLabel(wxString::Format(_("Importing network: %s"), importedNetworks[i].name));
                importStatus = WlanInfo::Import(NetworkData::ToWlanNetwork(importedNetworks[i]));
                if(importStatus == ERR_WI_ALREADY_EXISTS){
                    response = wxMessageBox(wxString::Format(_("The profile \"%s\" already exists on this computer.\nDo you want to overwrite it?"), importedNetworks[i].name), wxT("WiFi Password Recovery"), wxYES_NO | wxICON_WARNING);
                    if(response == wxYES){
                        importStatus = WlanInfo::Import(NetworkData::ToWlanNetwork(importedNetworks[i]), true);
                    }
                }
                if(importStatus != ERR_WI_SUCCESS && importStatus != ERR_WI_ALREADY_EXISTS){
                    wxMessageBox(wxString::Format(_("An error has occurred while importing profile \"%s\"."), importedNetworks[i].name), wxT("WiFi Password Recovery"), wxOK | wxICON_ERROR);
                }
                progressBar->SetValue(i + 1);
            }
        }else{
            progressBar->SetRange(data->selectedNetworks.GetCount());
            for(i = 0; i < data->selectedNetworks.GetCount(); i++){
                NetworkDataItem importedNetwork;
                bool status;
                labelProgressDesc->SetLabel(wxString::Format(_("Importing network: %s"), data->selectedNetworks[i].wc_str()));
                status = data->networkData.GetNetworkByName((wchar_t*) data->selectedNetworks[i].wc_str(), &importedNetwork);
                if(status){
                    int importStatus, response;
                       importStatus = WlanInfo::Import(NetworkData::ToWlanNetwork(importedNetwork));
                        if(importStatus == ERR_WI_ALREADY_EXISTS){
                            response = wxMessageBox(wxString::Format(_("The profile \"%s\" already exists on this computer.\nDo you want to overwrite it?"), importedNetwork.name), wxT("WiFi Password Recovery"), wxYES_NO | wxICON_WARNING);
                            if(response == wxYES){
                                importStatus = WlanInfo::Import(NetworkData::ToWlanNetwork(importedNetwork), true);
                            }
                        }
                        if(importStatus != ERR_WI_SUCCESS && importStatus != ERR_WI_ALREADY_EXISTS){
                            wxMessageBox(wxString::Format(_("An error has occurred while importing profile \"%s\"."), importedNetwork.name), wxT("WiFi Password Recovery"), wxOK | wxICON_ERROR);
                        }
                }else{
                    wxMessageBox(wxString::Format(_("Failed to import \"%s\".\n\nThe specified network could not be found."), data->selectedNetworks[i].wc_str()), wxT("WiFi Password Recovery"), wxOK | wxICON_ERROR);
                }
                progressBar->SetValue(i + 1);
            }
        }
        labelProgressDesc->SetLabel(_("Import finished."));
    }else{
        if(data->selectAll){
            progressBar->SetRange(WlanInfo::GetNetworksCount() + 1);
            for(i = 0; i < WlanInfo::GetNetworksCount(); i++){
                labelProgressDesc->SetLabel(wxString::Format(_("Exporting network: %s"), wlanNetworks[i].name));
                data->networkData.Add(wlanNetworks[i]);
                progressBar->SetValue(i + 1);
            }
        }else{
            progressBar->SetRange(data->selectedNetworks.GetCount() + 1);
            for(i = 0; i < data->selectedNetworks.GetCount(); i++){
                WlanNetwork* network;
                labelProgressDesc->SetLabel(wxString::Format(_("Exporting network: %s"), data->selectedNetworks[i]));
                network = WlanInfo::GetNetworkInfo((wchar_t*) data->selectedNetworks[i].wc_str());
                if(network == NULL){
                    wxMessageBox(wxString::Format(_("Failed to export \"%s\".\n\nThe specified network could not be found."), data->selectedNetworks[i].wc_str()), wxT("WiFi Password Recovery"), wxOK | wxICON_ERROR);
                }else{
                    data->networkData.Add(*network);
                }
                progressBar->SetValue(i + 1);
            }
        }
        labelProgressDesc->SetLabel(_("Export finished."));
        if(data->networkData.Export(data->path, data->encrypt) != SUCCESS){
            wxMessageBox(_("Failed to save file.\n\nTry to change the export path and run the wizard again."), wxT("WiFi Password Recovery"), wxOK | wxICON_ERROR);
        }/*else{
            wxMessageBox(_("%d networks imported with %d errors");
        }*/
        progressBar->SetValue(i + 1);
    }
    data->canExit = true;
    data->askExit = false;
    NetworkWizard::SetButtonStatus(wxID_FORWARD, true);
}
