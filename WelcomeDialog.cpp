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

#include "WelcomeDialog.h"

#include "Version.h"

#include <wx/icon.h>
#include <wx/intl.h>
#include <wx/sizer.h>
#include <wx/statline.h>
#include <wx/string.h>

BEGIN_EVENT_TABLE(WelcomeDialog, wxDialog)
    EVT_CLOSE(WelcomeDialog::OnClose)
END_EVENT_TABLE()

WelcomeDialog::WelcomeDialog(wxWindow* parent, wxFileConfig* config){
    wxBoxSizer* boxsizer, *descBoxSizer, *buttonBoxSizer;
    wxButton* buttonClose;
    wxButton* buttonDonate;
    wxStaticLine* separator;
    wxStaticText* title, *description;
    Create(parent, wxID_ANY, wxT("WiFi Password Recovery"), wxDefaultPosition, wxSize(425, 305), wxDEFAULT_DIALOG_STYLE);
    this->config = config;
	SetIcon(wxICON(APP_ICON));
    wxFont mainFont = wxFont();
    mainFont.SetPointSize(8);
	SetFont(mainFont);
    title = new wxStaticText(this, wxID_ANY, wxString::Format(_("Thank you for using %s!"), wxT("WiFi Password Recovery")), wxDefaultPosition, wxDefaultSize, 0);
    title->SetFont(wxFont(10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    separator = new wxStaticLine(this, wxID_ANY);
    description = new wxStaticText(this, wxID_ANY, wxString::Format(_("Thank you for using %s. %s is a small utility to recover the passwords of the WiFi networks saved on your computer.\n\n"
                                                                       "Unlike many other alterative programs, %s is totally free and even open source. Please consider "
                                                                       "supporting this program with a small donation. Thank you!"), wxT("WiFi Password Recovery"), wxT("WiFi Password Recovery"), wxT("WiFi Password Recovery")), wxDefaultPosition, wxDefaultSize, 0);
    buttonClose = new wxButton(this, wxID_ANY, _("Close"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
    buttonDonate = new wxButton(this, wxID_ANY, _("Donate"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
    showAgainCheckBox = new wxCheckBox(this, wxID_ANY, _("Don't show this message again"));
    boxsizer = new wxBoxSizer(wxVERTICAL);
    descBoxSizer = new wxBoxSizer(wxVERTICAL);
    buttonBoxSizer = new wxBoxSizer(wxHORIZONTAL);
    title->Wrap(GetSize().GetWidth());
    boxsizer->Add(title, 0, wxLEFT | wxTOP | wxRIGHT, 8);
    boxsizer->Add(separator, 0, wxEXPAND | wxALL, 10);
    descBoxSizer->Add(description, 1, wxLEFT | wxRIGHT | wxBOTTOM | wxEXPAND, 2);
    descBoxSizer->Add(showAgainCheckBox, 0, wxALL | wxALIGN_BOTTOM, 2);
    boxsizer->Add(descBoxSizer, 1, wxLEFT | wxRIGHT | wxBOTTOM | wxEXPAND, 8);
    buttonBoxSizer->Add(buttonDonate, 1, wxRIGHT | wxEXPAND, 6);
    buttonBoxSizer->Add(buttonClose, 1, wxEXPAND, 0);
    boxsizer->Add(buttonBoxSizer, 0, wxALL | wxALIGN_BOTTOM | wxALIGN_RIGHT, 8);
    SetSizer(boxsizer);
    Center();
    buttonClose->Connect(wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction) &OnButtonCloseClick, NULL, this);
    buttonDonate->Connect(wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction) &OnButtonDonateClick, NULL, this);
}

WelcomeDialog::~WelcomeDialog(){}

void WelcomeDialog::OnClose(wxCloseEvent& event){
    config->Write(wxT("welcome_message"), !showAgainCheckBox->GetValue());
    config->Flush();
    event.Skip();
}

void WelcomeDialog::OnButtonCloseClick(wxCommandEvent& event){
    Close();
}

void WelcomeDialog::OnButtonDonateClick(wxCommandEvent& event){
    wxLaunchDefaultBrowser(wxT(WPR_DONATION_URL));
}
