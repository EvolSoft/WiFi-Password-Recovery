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
#include "WPRMain.h"

#include "AboutDialog.h"
#include "LanguageHelper.h"
#include "NetworkInfoDialog.h"
#include "NetworkWizard.h"
#include "Version.h"
#include "WelcomeDialog.h"
#include "WlanInfo.h"

#include <wx/icon.h>
#include <wx/intl.h>
#include <wx/msgdlg.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/statusbr.h>
#include <wx/string.h>

bool WPRMain::sortDesc = false;
int WPRMain::lastCol = 0;

BEGIN_EVENT_TABLE(WPRMain, wxFrame)
END_EVENT_TABLE()

WPRMain::WPRMain(wxWindow* parent, wxFileConfig* config){
    wxBoxSizer* BoxSizerV, *BoxSizerH;
    Create(parent, wxID_ANY, wxT("WiFi Password Recovery"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxMINIMIZE_BOX | wxSTATIC_BORDER);
    this->config = config;
    SetIcon(wxICON(APP_ICON));
    SetClientSize(wxSize(475, 350));
    wxFont mainFont = wxFont();
    mainFont.SetPointSize(8);
	SetFont(mainFont);
    SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));
    MenuBar = new wxMenuBar();
    MenuFile = new wxMenu();
    MenuHelp = new wxMenu();
    MenuLanguage = new wxMenu();
    MenuItemWebsite = new wxMenuItem(MenuHelp, wxID_ANY, wxString::Format(_("Visit %s Website"), wxT("EvolSoft")), wxEmptyString, wxITEM_NORMAL);
    MenuItemBug = new wxMenuItem(MenuHelp, wxID_ANY, _("Report a bug..."), wxEmptyString, wxITEM_NORMAL);
    MenuItemDonate = new wxMenuItem(MenuHelp, wxID_ANY, _("Donate"), wxEmptyString, wxITEM_NORMAL);
    MenuItemAbout = new wxMenuItem(MenuHelp, wxID_ANY, wxString::Format(_("About %s..."), wxT("WiFi Password Recovery")), wxEmptyString, wxITEM_NORMAL);
    MenuItemScan = new wxMenuItem(MenuFile, wxID_ANY, wxString::Format(wxT("%s\tF1"), _("Scan")), wxEmptyString, wxITEM_NORMAL);
    MenuItemExportNetworks = new wxMenuItem(MenuFile, wxID_ANY, wxString::Format(wxT("%s\tF3"), _("Export networks...")), wxEmptyString, wxITEM_NORMAL);
    MenuItemImportNetworks = new wxMenuItem(MenuFile, wxID_ANY, wxString::Format(wxT("%s\tF2"), _("Import networks...")), wxEmptyString, wxITEM_NORMAL);
    MenuItemQuit = new wxMenuItem(MenuFile, wxID_ANY, wxString::Format(wxT("%s\tCtrl+Q"), _("Quit")), wxEmptyString, wxITEM_NORMAL);
    MenuHelp->Append(MenuItemWebsite);
    MenuHelp->Append(MenuItemBug);
    MenuHelp->Append(MenuItemDonate);
    MenuHelp->AppendSeparator();
    MenuHelp->Append(MenuItemAbout);
    MenuFile->Append(MenuItemScan);
    MenuFile->Append(MenuItemImportNetworks);
    MenuFile->Append(MenuItemExportNetworks);
    MenuFile->AppendSeparator();
    MenuFile->Append(MenuItemQuit);
    MenuBar->Append(MenuFile, _("File"));
    MenuBar->Append(MenuLanguage, _("Language"));
    MenuBar->Append(MenuHelp, _("Help"));
    InitLanguageMenu(MenuLanguage);
    SetMenuBar(MenuBar);
    ButtonRecover = new wxButton(this, wxID_ANY, _("Recover"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
    ButtonRecover->SetDefault();
    ButtonRecover->Disable();
    ButtonScan = new wxButton(this, wxID_ANY, _("Scan"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
    Label1 = new wxStaticText(this, wxID_ANY, _("Select a network from the list to show its info:"), wxDefaultPosition, wxDefaultSize);
    ListViewNetworks = new wxListView(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_SINGLE_SEL | wxTAB_TRAVERSAL, wxDefaultValidator);
    ListViewNetworks->InsertColumn(0, _("Name"), wxLIST_FORMAT_LEFT, wxLIST_AUTOSIZE);
    ListViewNetworks->InsertColumn(1, _("Security"), wxLIST_FORMAT_LEFT, wxLIST_AUTOSIZE);
    ListViewNetworks->InsertColumn(2, _("Encryption"), wxLIST_FORMAT_LEFT, wxLIST_AUTOSIZE);
    ListViewNetworks->InsertColumn(3, _("Interface"), wxLIST_FORMAT_LEFT, wxLIST_AUTOSIZE);
    //Alignment
    BoxSizerV = new wxBoxSizer(wxVERTICAL);
    BoxSizerH = new wxBoxSizer(wxHORIZONTAL);
    BoxSizerH->Add(ButtonScan, 0, wxRIGHT, 6);
    BoxSizerH->Add(ButtonRecover, 0);
    BoxSizerV->Add(Label1, 0, wxALL, 8);
    BoxSizerV->Add(ListViewNetworks, 1, wxEXPAND | wxALIGN_BOTTOM | wxLEFT | wxRIGHT | wxBOTTOM, 8);
    BoxSizerV->Add(BoxSizerH, 0, wxALIGN_RIGHT | wxALIGN_BOTTOM | wxRIGHT | wxBOTTOM, 8);
    SetSizer(BoxSizerV);
    SetStatusBarPane(-1);   //Remove status bar help text when using menu
    CreateStatusBar(1, wxSB_NORMAL);
    SetStatusText(wxT(" EvolSoft WiFi Password Recovery " WPR_VERSION " | " WPR_WEBSITE));
    //End alignment
    ButtonRecover->Connect(wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction) &WPRMain::OnButtonRecoverClick, NULL, this);
    ButtonScan->Connect(wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction) &WPRMain::OnButtonScanClick, NULL, this);
    ListViewNetworks->Connect(wxEVT_COMMAND_LIST_COL_CLICK, (wxObjectEventFunction) &WPRMain::OnListViewNetworksColumnClick, NULL, this);
    ListViewNetworks->Connect(wxEVT_COMMAND_LIST_ITEM_ACTIVATED, (wxObjectEventFunction) &WPRMain::OnListViewNetworksItemActivated, NULL, this);
    ListViewNetworks->Connect(wxEVT_COMMAND_LIST_ITEM_DESELECTED, (wxObjectEventFunction) &WPRMain::OnListViewNetworksItemDeselect, NULL, this);
    ListViewNetworks->Connect(wxEVT_COMMAND_LIST_ITEM_SELECTED, (wxObjectEventFunction) &WPRMain::OnListViewNetworksItemSelect, NULL, this);
    Connect(MenuItemAbout->GetId(), wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) &WPRMain::OnMenuItemAboutSelected);
    Connect(MenuItemBug->GetId(), wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) &WPRMain::OnMenuItemBugSelected);
    Connect(MenuItemDonate->GetId(), wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) &WPRMain::OnMenuItemDonateSelected);
    Connect(MenuItemExportNetworks->GetId(), wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) &WPRMain::OnMenuItemExportNetworksSelected);
    Connect(MenuItemImportNetworks->GetId(), wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) &WPRMain::OnMenuItemImportNetworksSelected);
    Connect(MenuItemQuit->GetId(), wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) &WPRMain::OnQuit);
    Connect(MenuItemScan->GetId(), wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) &WPRMain::OnButtonScanClick);
    Connect(MenuItemWebsite->GetId(), wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) &WPRMain::OnMenuItemWebsiteSelected);
    UpdateNetworksList();
}

WPRMain::~WPRMain(){}

/*** Multilanguage support ***/

void WPRMain::InitLanguageMenu(wxMenu* languageMenu){
    int i;
    long id;
    wxMenuItem* languageItem;
    Language tempLang;
    LanguageHelper* langHlpInst = LanguageHelper::GetInstance();
    for(i = 0; i < langHlpInst->GetAvailableLanguagesCount(); i++){
        tempLang = langHlpInst->GetAvailableLanguages()[i];
        id = wxNewId();
        menuLanguageItemLink[id] = i;
        languageItem = new wxMenuItem(languageMenu, id, tempLang.name, wxEmptyString, wxITEM_RADIO);
        languageMenu->Append(languageItem);
        if(tempLang.langId == langHlpInst->GetCurrentLanguage()){
            languageItem->Check();
        }
        Connect(id, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) &WPRMain::MenuLanguageHandler);
    }
}

void WPRMain::MenuLanguageHandler(wxCommandEvent& event){
    Language selLang;
    LanguageHelper* langHlpInst = LanguageHelper::GetInstance();
    if(menuLanguageItemLink.find(event.GetId()) != menuLanguageItemLink.end()){
        selLang = langHlpInst->GetAvailableLanguages()[menuLanguageItemLink[event.GetId()]];
        if(langHlpInst->ApplyLanguage(selLang.langId)){
            UpdateUITexts();
            config->Write(wxT("locale"), selLang.langId);
            config->Flush();
        }else{
            wxMessageBox(_("Failed to set the specified language."), wxT("WiFi Password Recovery"), wxOK | wxICON_ERROR);
        }
    }
}

void WPRMain::UpdateUITexts(){
    wxListItem listViewNetworksItem;
    long sel = ListViewNetworks->GetFirstSelected();
    UpdateNetworksList();
    if(sel < ListViewNetworks->GetItemCount()){
        ListViewNetworks->Select(sel);
    }else{
        ListViewNetworks->Select(ListViewNetworks->GetItemCount() - 1);
    }
    ListViewNetworks->GetColumn(0, listViewNetworksItem);
    listViewNetworksItem.SetText(_("Name"));
    ListViewNetworks->SetColumn(0, listViewNetworksItem);
    ListViewNetworks->GetColumn(1, listViewNetworksItem);
    listViewNetworksItem.SetText(_("Security"));
    ListViewNetworks->SetColumn(1, listViewNetworksItem);
    ListViewNetworks->GetColumn(2, listViewNetworksItem);
    listViewNetworksItem.SetText(_("Encryption"));
    ListViewNetworks->SetColumn(2, listViewNetworksItem);
    ListViewNetworks->GetColumn(3, listViewNetworksItem);
    listViewNetworksItem.SetText(_("Interface"));
    ListViewNetworks->SetColumn(3, listViewNetworksItem);
    MenuItemWebsite->SetText(wxString::Format(_("Visit %s Website"), wxT("EvolSoft")));
    MenuItemBug->SetText(_("Report a bug..."));
    MenuItemDonate->SetText(_("Donate"));
    MenuItemAbout->SetText(wxString::Format(_("About %s..."), wxT("WiFi Password Recovery")));
    MenuItemScan->SetText(wxString::Format(wxT("%s\tF1"), _("Scan")));
    MenuItemExportNetworks->SetText(wxString::Format(wxT("%s\tF3"), _("Export networks...")));
    MenuItemImportNetworks->SetText(wxString::Format(wxT("%s\tF2"), _("Import networks...")));
    MenuItemQuit->SetText(wxString::Format(wxT("%s\tCtrl+Q"), _("Quit")));
    MenuBar->SetLabelTop(0, _("File"));
    MenuBar->SetLabelTop(1, _("Language"));
    MenuBar->SetLabelTop(2, _("Help"));
    ButtonRecover->SetLabel(_("Recover"));
    ButtonRecover->SetSize(wxDefaultSize);
    ButtonScan->SetLabel(_("Scan"));
    ButtonScan->SetSize(wxDefaultSize);
    Label1->SetLabel(_("Select a network from the list to show its info:"));
}

/*** End multilanguage support ***/

int wxCALLBACK WPRMain::ListViewCompare(wxIntPtr item1, wxIntPtr item2, wxIntPtr sortData){
    wxListItem l1, l2;
    int order = sortDesc ? -1 : 1;
    SortInfo* sInf = (SortInfo*) sortData;
    l1.SetColumn(sInf->col);
    l1.SetId(sInf->listView->FindItem(-1, item1));
    l1.SetMask(wxLIST_MASK_TEXT);
    sInf->listView->GetItem(l1);
    l2.SetColumn(sInf->col);
    l2.SetId(sInf->listView->FindItem(-1, item2));
    l2.SetMask(wxLIST_MASK_TEXT);
    sInf->listView->GetItem(l2);
    return order * l1.GetText().CmpNoCase(l2.GetText());
}

void WPRMain::OnAbout(wxCommandEvent& event){}

void WPRMain::OnQuit(wxCommandEvent& event){
    WlanInfo::Destroy();
    Close();
}

void WPRMain::OnButtonRecoverClick(wxCommandEvent& event){
    ShowNetworkInfo();
}

void WPRMain::OnButtonScanClick(wxCommandEvent& event){
    long sel = ListViewNetworks->GetFirstSelected();
    UpdateNetworksList();
    if(sel < ListViewNetworks->GetItemCount()){
        ListViewNetworks->Select(sel);
    }else{
        ListViewNetworks->Select(ListViewNetworks->GetItemCount() - 1);
    }
}

void WPRMain::OnListViewNetworksColumnClick(wxListEvent& event){
    if(lastCol != event.GetColumn()){
        sortDesc = true;
    }
    sortDesc = !sortDesc;
    SortItems(event.GetColumn());
    lastCol = event.GetColumn();
}

void WPRMain::OnListViewNetworksItemActivated(wxListEvent& event){
    ShowNetworkInfo();
}

void WPRMain::OnListViewNetworksItemDeselect(wxListEvent& event){
    ButtonRecover->Disable();
}

void WPRMain::OnListViewNetworksItemSelect(wxListEvent& event){
    ButtonRecover->Enable();
}

void WPRMain::OnMenuItemAboutSelected(wxCommandEvent& event){
    AboutDialog aboutDialog(this);
    aboutDialog.ShowModal();
}

void WPRMain::OnMenuItemBugSelected(wxCommandEvent& event){
    wxLaunchDefaultBrowser(wxT(WPR_ISSUES_URL));
}

void WPRMain::OnMenuItemDonateSelected(wxCommandEvent& event){
    wxLaunchDefaultBrowser(wxT(WPR_DONATION_URL));
}

void WPRMain::OnMenuItemExportNetworksSelected(wxCommandEvent& event){
    NetworkWizard networkWizard(this, false);
    networkWizard.Destroy();
}

void WPRMain::OnMenuItemImportNetworksSelected(wxCommandEvent& event){
    NetworkWizard networkWizard(this, true);
    networkWizard.Destroy();
}

void WPRMain::OnMenuItemWebsiteSelected(wxCommandEvent& event){
    wxLaunchDefaultBrowser(wxT(WPR_WEBSITE_URL));
}

void WPRMain::ShowNetworkInfo(){
    wxListItem sel;
    WlanNetwork* network;
    sel.SetColumn(0);
    sel.SetId(ListViewNetworks->GetFirstSelected());
    sel.SetMask(wxLIST_MASK_TEXT);
    ListViewNetworks->GetItem(sel);
    network = WlanInfo::GetNetworkInfo((wchar_t*) sel.GetText().wc_str());
    if(!network){
        wxMessageBox(_("Failed to retrieve wireless network data."), wxT("WiFi Password Recovery"), wxOK | wxICON_ERROR);
        return;
    }
    NetworkInfoDialog networkinfodlg(this, network);
    networkinfodlg.ShowModal();
}

void WPRMain::SortItems(int col){
    SortInfo sInf;
    sInf.listView = ListViewNetworks;
    sInf.col = col;
    ListViewNetworks->SortItems(WPRMain::ListViewCompare, (wxIntPtr) &sInf);
}

void WPRMain::UpdateNetworksList(){
    WlanNetwork* wn;
    int wnc;
    ListViewNetworks->DeleteAllItems();
    WlanInfo::UpdateNetworksList();
    wnc = WlanInfo::GetNetworksCount();
    if(!wnc){
        wxMessageBox(_("No Wireless networks found."), wxT("WiFi Password Recovery"), wxOK | wxICON_INFORMATION);
        return;
    }
    wn = WlanInfo::GetNetworksList();
    for(int i = 0; i < wnc; i++){
        int l;
        l = ListViewNetworks->InsertItem(0, wxString(wn[i].name, wxConvUTF8));
        ListViewNetworks->SetItem(0, 1, WlanInfo::WlanAuthToString(wn[i].auth));
        ListViewNetworks->SetItem(0, 2, WlanInfo::WlanEncryptionToString(wn[i].encryption));
        ListViewNetworks->SetItem(0, 3, wxString(wn[i].intrface, wxConvUTF8));
        ListViewNetworks->SetItemData(l, i);
        ListViewNetworks->SetColumnWidth(0, wxLIST_AUTOSIZE);
        ListViewNetworks->SetColumnWidth(3, wxLIST_AUTOSIZE);
    }
    SortItems(lastCol);
}
