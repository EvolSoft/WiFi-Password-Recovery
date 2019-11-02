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

#include "NetworkInfoDialog.h"

#include <wx/icon.h>
#include <wx/intl.h>
#include <wx/filedlg.h>
#include <wx/font.h>
#include <wx/msgdlg.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/string.h>
#include <wx/wfstream.h>

BEGIN_EVENT_TABLE(NetworkInfoDialog, wxDialog)
END_EVENT_TABLE()

NetworkInfoDialog::NetworkInfoDialog(wxWindow* parent, WlanNetwork* networkInfo){
    wxBoxSizer* ButtonSizer, *BoxSizer;
    wxFlexGridSizer* GridSizer;
	Create(parent, wxID_ANY, _("Unknown"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE);
	this->networkInfo = networkInfo;
	SetIcon(wxICON(APP_ICON));
	SetClientSize(wxSize(375, 225));
	SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));
    wxFont mainFont = wxFont();
    mainFont.SetPointSize(8);
    wxFont mainFontBold = mainFont;
    mainFontBold.SetWeight(wxFONTWEIGHT_BOLD);
    SetFont(mainFont);
    BoxSizer = new wxBoxSizer(wxVERTICAL);
    ButtonSizer = new wxBoxSizer(wxHORIZONTAL);
    GridSizer = new wxFlexGridSizer(5, 2, 8, 20);
	ButtonClose = new wxButton(this, wxID_ANY, _("Close"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
	ButtonSave = new wxButton(this, wxID_ANY, _("Save"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
	LabelEncrypt = new wxStaticText(this, wxID_ANY, _("Encryption:"), wxDefaultPosition, wxDefaultSize);
	LabelEncrypt->SetFont(mainFontBold);
    LabelEncryptVal = new wxStaticText(this, wxID_ANY, _("Unknown"), wxDefaultPosition, wxDefaultSize);
    LabelInterface = new wxStaticText(this, wxID_ANY, _("Interface:"), wxDefaultPosition, wxDefaultSize);
    LabelInterface->SetFont(mainFontBold);
	LabelInterfaceVal = new wxStaticText(this, wxID_ANY, _("Unknown"), wxDefaultPosition, wxDefaultSize, 0);
	LabelPassword = new wxStaticText(this, wxID_ANY, _("Password:"), wxDefaultPosition, wxDefaultSize);
	LabelPassword->SetFont(mainFontBold);
    LabelSecurity = new wxStaticText(this, wxID_ANY, _("Security:"), wxDefaultPosition, wxDefaultSize);
	LabelSecurity->SetFont(mainFontBold);
    LabelSecurityVal = new wxStaticText(this, wxID_ANY, _("Unknown"), wxDefaultPosition, wxDefaultSize, 0);
	LabelSSID = new wxStaticText(this, wxID_ANY, _("SSID:"), wxDefaultPosition, wxDefaultSize);
	LabelSSID->SetFont(mainFontBold);
	LabelSSIDVal = new wxStaticText(this, wxID_ANY, _("Unknown"), wxDefaultPosition, wxDefaultSize, 0);
	TextBoxPasswordVal = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxTE_MULTILINE, wxDefaultValidator);
	TextBoxPasswordVal->SetFocus();
	//Alignment
    GridSizer->Add(LabelSSID, 1, wxALIGN_CENTER_VERTICAL);
	GridSizer->Add(LabelSSIDVal, 1, wxEXPAND);
    GridSizer->Add(LabelInterface, 1, wxALIGN_CENTER_VERTICAL);
	GridSizer->Add(LabelInterfaceVal, 1, wxEXPAND);
    GridSizer->Add(LabelSecurity, 1, wxALIGN_CENTER_VERTICAL);
	GridSizer->Add(LabelSecurityVal, 1, wxEXPAND);
	GridSizer->Add(LabelEncrypt, 1, wxALIGN_CENTER_VERTICAL);
	GridSizer->Add(LabelEncryptVal, 1, wxEXPAND);
	GridSizer->Add(LabelPassword, 1, wxEXPAND | wxALIGN_TOP);
	GridSizer->Add(TextBoxPasswordVal, 1, wxEXPAND | wxALL);
	GridSizer->AddGrowableRow(4, 1);
    GridSizer->AddGrowableCol(1);
    ButtonSizer->Add(ButtonSave, 0, wxRIGHT, 6);
    ButtonSizer->Add(ButtonClose, 0);
    BoxSizer->AddSpacer(8);
	BoxSizer->Add(GridSizer, 1, wxALL | wxEXPAND, 8);
	//BoxSizer->AddStretchSpacer();
    BoxSizer->Add(ButtonSizer, 0, wxALIGN_RIGHT | wxALIGN_BOTTOM | wxRIGHT | wxBOTTOM, 8);
	SetSizer(BoxSizer);
	//End alignment
	Center();
	SetTitle(wxString::Format(_("Network: %s"), networkInfo->name, wxConvUTF8));
    LabelInterfaceVal->SetLabel(wxString(networkInfo->intrface, wxConvUTF8));
    LabelEncryptVal->SetLabel(WlanInfo::WlanEncryptionToString(networkInfo->encryption));
    LabelSecurityVal->SetLabel(WlanInfo::WlanAuthToString(networkInfo->auth));
    LabelSSIDVal->SetLabel(wxString(networkInfo->name, wxConvUTF8));
    if(networkInfo->dcerror){
        TextBoxPasswordVal->Enable();
        wxMessageBox(_("Failed to retrieve network password."), wxT("WiFi Password Recovery"), wxOK | wxICON_ERROR);
    }else{
        if(networkInfo->encryption != WCRYPT_NONE){
            TextBoxPasswordVal->Enable();
            TextBoxPasswordVal->SetLabel(wxString(networkInfo->password, wxConvUTF8));
            TextBoxPasswordVal->SetSelection(-1,-1);
        }else{
            TextBoxPasswordVal->Disable();
            TextBoxPasswordVal->SetLabel(_("No password."));
        }
    }
	ButtonClose->Connect(wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction) &NetworkInfoDialog::OnButtonCloseClick, NULL, this);
	ButtonSave->Connect(wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction) &NetworkInfoDialog::OnButtonSaveClick, NULL, this); //Class instance must be passed in order to access class variables
}

NetworkInfoDialog::~NetworkInfoDialog(){}

void NetworkInfoDialog::OnButtonCloseClick(wxCommandEvent& event){
    Close();
}

void NetworkInfoDialog::OnButtonSaveClick(wxCommandEvent& event){
    wxFileDialog* saveDlg;
    wxFileOutputStream* fileStr;
    wxString content;
    saveDlg = new wxFileDialog(this, _("Save as"), wxT(""), networkInfo->name, wxString::Format(wxT("%s (*.txt)|*.txt|%s (*.*)|*.*"), _("Text files"), _("All files")), wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
    if(saveDlg->ShowModal() == wxID_CANCEL){
        return;
    }
    fileStr = new wxFileOutputStream(saveDlg->GetPath());
    if(!fileStr->IsOk()){
        wxMessageBox(_("Failed to save file."), wxT("WiFi Password Recovery"), wxOK | wxICON_ERROR);
        return;
    }
    content = wxString::Format(wxT("== %s: %s ==\r\n\r\n"
                                   "%s %s\r\n"
                                   "%s %s\r\n"
                                   "%s %s\r\n"
                                   "%s %s\r\n"), _("Network:"), networkInfo->name, _("SSID:"), networkInfo->name, _("Interface:"), networkInfo->intrface, _("Security:"), WlanInfo::WlanAuthToString(networkInfo->auth).wc_str(), _("Encryption:"), WlanInfo::WlanEncryptionToString(networkInfo->encryption).wc_str());
    if(networkInfo->encryption != WCRYPT_NONE){
        content.Append(wxString::Format(wxT("%s %s"), _("Password:"), networkInfo->password));
    }else{
        content.Append(_("No password."));
    }
    fileStr->Write(content.ToAscii(), content.Length());
    wxMessageBox(_("File saved successfully!"), wxT("WiFi Password Recovery"), wxOK | wxICON_INFORMATION);
    fileStr->Close();
}
