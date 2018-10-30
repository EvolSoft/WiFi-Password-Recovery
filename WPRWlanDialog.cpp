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

#include "WPRWlanDialog.h"

#include <wx/intl.h>
#include <wx/font.h>
#include <wx/msgdlg.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/string.h>
#include <wx/icon.h>

const long WPRWlanDialog::ID_BUTTONCLOSE = wxNewId();
const long WPRWlanDialog::ID_LABELENCRYPT = wxNewId();
const long WPRWlanDialog::ID_LABELENCRYPTVAL = wxNewId();
const long WPRWlanDialog::ID_LABELINTERFACE = wxNewId();
const long WPRWlanDialog::ID_LABELINTERFACEVAL = wxNewId();
const long WPRWlanDialog::ID_LABELPASSWORD = wxNewId();
const long WPRWlanDialog::ID_LABELSECURITY = wxNewId();
const long WPRWlanDialog::ID_LABELSECURITYVAL = wxNewId();
const long WPRWlanDialog::ID_LABELSSID = wxNewId();
const long WPRWlanDialog::ID_LABELSSIDVAL = wxNewId();
const long WPRWlanDialog::ID_TEXTBOXPASSWORDVAL = wxNewId();

BEGIN_EVENT_TABLE(WPRWlanDialog,wxDialog)
END_EVENT_TABLE()

WPRWlanDialog::WPRWlanDialog(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size){
    wxBoxSizer* BoxSizer;
    wxFlexGridSizer* GridSizer;
	Create(parent, wxID_ANY, _("Unknown"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
	SetIcon(wxICON(APP_ICON));
	SetClientSize(wxSize(375, 225));
	SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));
    wxFont mainFont = wxFont();
    mainFont.SetPointSize(9);
    wxFont mainFontBold = mainFont;
    mainFontBold.SetWeight(wxFONTWEIGHT_BOLD);
    SetFont(mainFont);
    BoxSizer = new wxBoxSizer(wxVERTICAL);
    GridSizer = new wxFlexGridSizer(5, 2, 12, 20);
	ButtonClose = new wxButton(this, ID_BUTTONCLOSE, _("Close"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONCLOSE"));
	LabelEncrypt = new wxStaticText(this, ID_LABELENCRYPT, _("Encryption:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_LABELENCRYPT"));
	LabelEncrypt->SetFont(mainFontBold);
    LabelEncryptVal = new wxStaticText(this, ID_LABELENCRYPTVAL, _("Unknown"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_LABELENCRYPTVAL"));
    LabelInterface = new wxStaticText(this, ID_LABELINTERFACE, _("Interface:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_LABELINTERFACE"));
    LabelInterface->SetFont(mainFontBold);
	LabelInterfaceVal = new wxStaticText(this, ID_LABELINTERFACEVAL, _("Unknown"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_LABELINTERFACEVAL"));
	LabelPassword = new wxStaticText(this, ID_LABELPASSWORD, _("Password:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_LABELPASSWORD"));
	LabelPassword->SetFont(mainFontBold);
    LabelSecurity = new wxStaticText(this, ID_LABELSECURITY, _("Security:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_LABELSECURITY"));
	LabelSecurity->SetFont(mainFontBold);
    LabelSecurityVal = new wxStaticText(this, ID_LABELSECURITYVAL, _("Unknown"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_LABELSECURITYVAL"));
	LabelSSID = new wxStaticText(this, ID_LABELSSID, _("SSID:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_LABELSSID"));
	LabelSSID->SetFont(mainFontBold);
	LabelSSIDVal = new wxStaticText(this, ID_LABELSSIDVAL, _("Unknown"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_LABELSSIDVAL"));
	TextBoxPasswordVal = new wxTextCtrl(this, ID_TEXTBOXPASSWORDVAL, _(""), wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_TEXTBOXPASSWORDVAL"));
	TextBoxPasswordVal->SetFocus();
	//Alignment
    GridSizer->Add(LabelSSID, 0, wxALIGN_CENTER_VERTICAL | wxTOP | wxBOTTOM, 2);
	GridSizer->Add(LabelSSIDVal, 0, wxEXPAND | wxTOP | wxBOTTOM, 1);
    GridSizer->Add(LabelInterface, 0, wxALIGN_CENTER_VERTICAL | wxTOP | wxBOTTOM, 2);
	GridSizer->Add(LabelInterfaceVal, 0, wxEXPAND | wxTOP | wxBOTTOM, 1);
    GridSizer->Add(LabelSecurity, 0, wxALIGN_CENTER_VERTICAL | wxTOP | wxBOTTOM, 2);
	GridSizer->Add(LabelSecurityVal, 0, wxEXPAND | wxTOP | wxBOTTOM, 2);
	GridSizer->Add(LabelEncrypt, 0, wxALIGN_CENTER_VERTICAL | wxTOP, 2);
	GridSizer->Add(LabelEncryptVal, 0, wxEXPAND | wxTOP, 2);
	GridSizer->Add(LabelPassword, 0, wxALIGN_CENTER_VERTICAL);
	GridSizer->Add(TextBoxPasswordVal, 0, wxEXPAND);
	GridSizer->AddGrowableRow(1);
    GridSizer->AddGrowableCol(1);
    BoxSizer->AddSpacer(8);
	BoxSizer->Add(GridSizer, 0, wxALL | wxEXPAND, 8);
	BoxSizer->AddStretchSpacer();
    BoxSizer->Add(ButtonClose, 0, wxALIGN_RIGHT | wxALIGN_BOTTOM | wxRIGHT | wxBOTTOM, 8);
	SetSizer(BoxSizer);
	//End alignment
	Center();
	Connect(ID_BUTTONCLOSE, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction) &WPRWlanDialog::OnButtonCloseClick);
	Connect(wxID_ANY, wxEVT_INIT_DIALOG, (wxObjectEventFunction) &WPRWlanDialog::OnInit);
}

WPRWlanDialog::~WPRWlanDialog(){}

void WPRWlanDialog::OnInit(wxInitDialogEvent& event){
    SetTitle(wxString::Format(wxT("Network: %s"), wn->name, wxConvUTF8));
    LabelInterfaceVal->SetLabel(wxString(wn->intrface, wxConvUTF8));
    LabelEncryptVal->SetLabel(wxString(WlanInfo::WlanEncryptionToString(wn->encryption), wxConvUTF8));
    LabelSecurityVal->SetLabel(wxString(WlanInfo::WlanAuthToString(wn->auth), wxConvUTF8));
    LabelSSIDVal->SetLabel(wxString(wn->name, wxConvUTF8));
    if(wn->dcerror){
        TextBoxPasswordVal->Enable();
        wxMessageBox(wxT("Failed to retrieve network password."), wxT("Error"), wxOK | wxICON_ERROR);
    }else{
        if(wn->encryption != WCRYPT_NONE){
            TextBoxPasswordVal->Enable();
            TextBoxPasswordVal->SetLabel(wxString(wn->password, wxConvUTF8));
            TextBoxPasswordVal->SetSelection(-1,-1);
        }else{
            TextBoxPasswordVal->Disable();
            TextBoxPasswordVal->SetLabel(wxT("None"));
        }
    }
}

void WPRWlanDialog::OnButtonCloseClick(wxCommandEvent& event){
    Hide();
}
