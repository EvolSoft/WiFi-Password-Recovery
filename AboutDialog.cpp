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

#include "AboutDialog.h"

#include "Version.h"

#include <wx/icon.h>
#include <wx/intl.h>
#include <wx/notebook.h>
#include <wx/sizer.h>
#include <wx/statline.h>
#include <wx/string.h>

BEGIN_EVENT_TABLE(AboutDialog, wxDialog)
END_EVENT_TABLE()

AboutDialog::AboutDialog(wxWindow* parent){
	wxStaticBitmap* logo;
    wxBoxSizer* mainSizer, *buttonSizer, *creditsSizer, *licenseSizer, *changelogSizer;
    wxButton* buttonClose, *buttonDonate;
    wxStaticText* title, *build, *copyright;
    wxHyperlinkCtrl* websiteLink, *translateLink;
    wxStaticLine* separator;
    wxNotebook* tabControl;
    wxPanel* creditsPanel, *licensePanel, *changelogPanel;
    wxTextCtrl* creditsTextBox, *licenseTextBox, *changelogTextBox;
    Create(parent, wxID_ANY, wxString::Format(_("About %s"), wxT("WiFi Password Recovery")), wxDefaultPosition, wxSize(475, 600), wxDEFAULT_DIALOG_STYLE);
	SetIcon(wxICON(APP_ICON));
    wxFont mainFont = wxFont();
    mainFont.SetPointSize(8);
	SetFont(mainFont);
    mainSizer = new wxBoxSizer(wxVERTICAL);
    buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonClose = new wxButton(this, wxID_ANY, _("Close"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
    buttonDonate = new wxButton(this, wxID_ANY, _("Donate"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
    logo = new wxStaticBitmap(this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize(96, 96), 0);
	logo->SetIcon(wxIcon(wxT("APP_ICON"), wxBITMAP_TYPE_ICO_RESOURCE, 96, 96));
	title = new wxStaticText(this, wxID_ANY, wxT("WiFi Password Recovery "WPR_VERSION""), wxDefaultPosition, wxDefaultSize, 0);
    title->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
	build = new wxStaticText(this, wxID_ANY, wxT("Build: "__DATE__", "__TIME__""), wxDefaultPosition, wxDefaultSize, 0);
    build->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_GRAYTEXT));
    copyright = new wxStaticText(this, wxID_ANY, wxT("\xA9 2018-2019 EvolSoft"), wxDefaultPosition, wxDefaultSize, 0);
    websiteLink = new wxHyperlinkCtrl(this, wxID_ANY, wxT(WPR_WEBSITE), wxT(WPR_WEBSITE_URL), wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE);
    separator = new wxStaticLine(this, wxID_ANY);
    tabControl = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
    //Credits tab
    creditsPanel = new wxPanel(tabControl, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    creditsTextBox = new wxTextCtrl(creditsPanel, wxID_ANY, wxString::Format(wxT("%s\n--------------\nFlavio Collocola\n\n%s\n-------------------\nFlavio Collocola\n\n%s\n---------------\nEnglish translation - Flavio Collocola <mail@flavius12.net>\nItalian translation - Flavio Collocola <mail@flavius12.net>\n\n%s\n---------------------\nwxWidgets (https://www.wxwidgets.org)"), _("Developers:"), _("Graphics and UI:"), _("Translations:"), _("3rd-party libraries:")), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
    translateLink = new wxHyperlinkCtrl(creditsPanel, wxID_ANY, _("Translate into your language"), wxT(WPR_TRANSLATIONS_URL), wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE);
    creditsSizer = new wxBoxSizer(wxVERTICAL);
    creditsSizer->Add(creditsTextBox, 1, wxALL | wxEXPAND, 8);
    creditsSizer->Add(translateLink, 0, wxALL, 8);
    creditsPanel->SetSizer(creditsSizer);
    //License tab
    licensePanel = new wxPanel(tabControl, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    licenseTextBox = new wxTextCtrl(licensePanel, wxID_ANY, wxT("This program is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.\nThis program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.\nYou should have received a copy of the GNU Lesser General Public License\nalong with this program.  If not, see <http://www.gnu.org/licenses/>."), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
    licenseSizer = new wxBoxSizer(wxVERTICAL);
    licenseSizer->Add(licenseTextBox, 1, wxALL | wxEXPAND, 8);
    licensePanel->SetSizer(licenseSizer);
    //Changelog tab
    changelogPanel = new wxPanel(tabControl, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    changelogTextBox = new wxTextCtrl(changelogPanel, wxID_ANY, wxT("Added:\n"
                                                                    "- Added multi-language support\n"
                                                                    "- Added welcome window\n"
                                                                    "- Added wireless networks data export wizard\n"
                                                                    "- Added wireless networks data import wizard\n"
                                                                    "- Added feature to save wireless network data to text file\n"
                                                                    "- Added support for opening exported wireless networks files from shell\n"
                                                                    "- General UI improvements\n"
                                                                    "- Improved password text box UI\n"
                                                                    "- Improved main window UI\n"
                                                                    "- Improved main menu\n"
                                                                    "- Improved about window UI\n"
                                                                    "- Implemented encryption on exported wireless networks\n\n"
                                                                    "Fixed:\n"
                                                                    "- Fixed program loading bug on Windows XP (with Service Pack 3)\n"
                                                                    "- Fixed a bug when recovering network passwords on Windows Vista"), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
    changelogSizer = new wxBoxSizer(wxVERTICAL);
    changelogSizer->Add(changelogTextBox, 1, wxALL | wxEXPAND, 8);
    changelogPanel->SetSizer(changelogSizer);
    tabControl->AddPage(creditsPanel, _("Credits"), true);
    tabControl->AddPage(licensePanel, _("License"), false);
    tabControl->AddPage(changelogPanel, _("Changelog"), false);
    mainSizer->AddSpacer(8);
	mainSizer->Add(logo, 0, wxALIGN_CENTER | wxALL, 4);
	mainSizer->Add(title, 0, wxALIGN_CENTER | wxALL, 4);
	mainSizer->Add(build, 0, wxALIGN_CENTER | wxALL, 4);
	mainSizer->Add(copyright, 0, wxALIGN_CENTER | wxALL, 4);
	mainSizer->Add(websiteLink, 0, wxALIGN_CENTER | wxALL, 3);
	mainSizer->AddSpacer(5);
    mainSizer->Add(separator, 0, wxEXPAND | wxLEFT | wxRIGHT, 8);
    mainSizer->Add(tabControl, 1, wxEXPAND | wxALL, 8);
    buttonSizer->Add(buttonDonate, 1, wxRIGHT | wxEXPAND, 6);
    buttonSizer->Add(buttonClose, 1, wxEXPAND, 0);
    mainSizer->Add(buttonSizer, 0, wxALL | wxALIGN_CENTER | wxALIGN_BOTTOM | wxEXPAND, 8);
	SetSizer(mainSizer);
	Center();
    buttonClose->Connect(wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction) &OnButtonCloseClick, NULL, this);
    buttonDonate->Connect(wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction) &OnButtonDonateClick, NULL, this);
}

AboutDialog::~AboutDialog(){}

void AboutDialog::OnButtonCloseClick(wxCommandEvent& event){
    Close();
}

void AboutDialog::OnButtonDonateClick(wxCommandEvent& event){
    wxLaunchDefaultBrowser(wxT(WPR_DONATION_URL));
}
