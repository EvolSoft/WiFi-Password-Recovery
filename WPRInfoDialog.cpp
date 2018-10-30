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

#include "WPRInfoDialog.h"

#include <wx/intl.h>
#include <wx/icon.h>
#include <wx/sizer.h>
#include <wx/string.h>

const long WPRInfoDialog::ID_BITMAPICON = wxNewId();
const long WPRInfoDialog::ID_BUTTONCLOSE = wxNewId();
const long WPRInfoDialog::ID_BUTTONDONATE = wxNewId();
const long WPRInfoDialog::ID_HYPERLINK1 = wxNewId();
const long WPRInfoDialog::ID_LABEL1 = wxNewId();
const long WPRInfoDialog::ID_LABEL2 = wxNewId();
const long WPRInfoDialog::ID_LABEL3 = wxNewId();
const long WPRInfoDialog::ID_LABEL4 = wxNewId();

BEGIN_EVENT_TABLE(WPRInfoDialog, wxDialog)
END_EVENT_TABLE()

WPRInfoDialog::WPRInfoDialog(wxWindow* parent, wxWindowID id){
	wxBoxSizer* BoxSizerV1, *BoxSizerV2, *BoxSizerH1, *BoxSizerH2;
	Create(parent, wxID_ANY, _("Info"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
	SetIcon(wxICON(APP_ICON));
	SetClientSize(wxSize(350, 350));
    wxFont mainFont = wxFont();
    mainFont.SetPointSize(8);
	SetFont(mainFont);
	BitmapIcon = new wxStaticBitmap(this, ID_BITMAPICON, wxNullBitmap, wxDefaultPosition, wxSize(128, 128), 0, _T("ID_BITMAPICON"));
	BitmapIcon->SetIcon(wxIcon(wxT("APP_ICON"), wxBITMAP_TYPE_ICO_RESOURCE, 128, 128));
	ButtonClose = new wxButton(this, ID_BUTTONCLOSE, _("Close"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONCLOSE"));
    ButtonDonate = new wxButton(this, ID_BUTTONDONATE, _("Donate"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONDONATE"));
    Hyperlink1 = new wxHyperlinkCtrl(this, ID_HYPERLINK1, _("https://www.evolsoft.tk"), wxEmptyString, wxDefaultPosition, wxDefaultSize, wxHL_CONTEXTMENU|wxHL_ALIGN_CENTRE | wxNO_BORDER, _T("ID_HYPERLINK1"));
	Label1 = new wxStaticText(this, ID_LABEL1, _("WiFi Password Recovery\nRelease: v1.0 (Build: "__DATE__", "__TIME__")\n\xA9 2018 EvolSoft"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_LABEL1"));
	Label2 = new wxStaticText(this, ID_LABEL2, _("\nDeveloped by: Flavio Collocola"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_LABEL2"));
	Label3 = new wxStaticText(this, ID_LABEL3, _("3rd-party libraries used:\n\nwxWidgets (https://www.wxwidgets.org)\n\npugixml (http://pugixml.org)\npugixml is Copyright \xA9 2006-2018 Arseny Kapoulkine.\n"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_LABEL3"));
	Label4 = new wxStaticText(this, ID_LABEL4, _("License:\nThis program is free software: you can redistribute it and/or modify\nit under the terms of the GNU Lesser General Public License as published by\nthe Free Software Foundation, either version 3 of the License, or\n(at your option) any later version.\n\nThis program is distributed in the hope that it will be useful,\nbut WITHOUT ANY WARRANTY; without even the implied warranty of\nMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\nGNU Lesser General Public License for more details.\n\nYou should have received a copy of the GNU Lesser General Public License\nalong with this program.  If not, see <http://www.gnu.org/licenses/>."), wxDefaultPosition, wxDefaultSize, 0, _T("ID_LABEL4"));
	//Alignment
	BoxSizerV1 = new wxBoxSizer(wxVERTICAL);
    BoxSizerV2 = new wxBoxSizer(wxVERTICAL);
    BoxSizerH1 = new wxBoxSizer(wxHORIZONTAL);
    BoxSizerH2 = new wxBoxSizer(wxHORIZONTAL);
    BoxSizerH1->Add(BitmapIcon, 0, wxRIGHT | wxBOTTOM, 3);
    BoxSizerV1->Add(Label1, 0, wxLEFT, 5);
    BoxSizerV1->Add(Hyperlink1, 0, wxLEFT, 5);
    BoxSizerV1->Add(Label2, 0, wxLEFT, 5);
    BoxSizerH1->Add(BoxSizerV1, 0, wxLEFT | wxALIGN_CENTER, 3);
    BoxSizerH2->Add(ButtonDonate, 1, wxRIGHT, 3);
    BoxSizerH2->Add(ButtonClose, 1, wxLEFT, 3);
    BoxSizerV2->Add(BoxSizerH1, 0, wxLEFT | wxTOP | wxRIGHT | wxEXPAND, 8);
    BoxSizerV2->Add(Label3, 0, wxLEFT | wxTOP | wxRIGHT, 8);
    BoxSizerV2->Add(Label4, 0, wxLEFT | wxRIGHT, 8);
    BoxSizerV2->AddSpacer(20);
    BoxSizerV2->AddStretchSpacer();
    BoxSizerV2->Add(BoxSizerH2, 0, wxALL | wxEXPAND | wxALIGN_BOTTOM | wxALIGN_CENTER, 8);
    SetSizerAndFit(BoxSizerV2);
	//End alignment
	Center();
	Connect(ID_BUTTONCLOSE, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction) &WPRInfoDialog::OnButtonCloseClick);
	Connect(ID_BUTTONDONATE, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction) &WPRInfoDialog::OnButtonDonateClick);
	Connect(wxID_ANY, wxEVT_INIT_DIALOG, (wxObjectEventFunction) &WPRInfoDialog::OnInit);
}

WPRInfoDialog::~WPRInfoDialog(){}

void WPRInfoDialog::OnInit(wxInitDialogEvent& event){}

void WPRInfoDialog::OnButtonCloseClick(wxCommandEvent& event){
    Hide();
}

void WPRInfoDialog::OnButtonDonateClick(wxCommandEvent& event){
    wxLaunchDefaultBrowser(wxT("https://www.paypal.com/cgi-bin/webscr?cmd=_donations&business=flavius.c.1999@gmail.com&lc=US&item_name=www.evolsoft.tk&no_note=0&cn=&curency_code=EUR&bn=PP-DonationsBF:btn_donateCC_LG.gif:NonHosted"));
}
