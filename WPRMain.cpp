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

#include "WPRMain.h"

#include "WlanInfo.h"
#include "WPRInfoDialog.h"
#include "WPRWlanDialog.h"

#include <wx/intl.h>
#include <wx/icon.h>
#include <wx/msgdlg.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/string.h>

bool WPRMain::sortDesc = false;
const long WPRMain::ID_BUTTONRECOVER = wxNewId();
const long WPRMain::ID_BUTTONSCAN = wxNewId();
const long WPRMain::ID_LABEL1 = wxNewId();
const long WPRMain::ID_LISTVIEWNETWORKS = wxNewId();
const long WPRMain::ID_MENUITEMABOUT = wxNewId();
const long WPRMain::ID_MENUITEMQUIT = wxNewId();
int WPRMain::lastCol = 0;

BEGIN_EVENT_TABLE(WPRMain,wxFrame)
END_EVENT_TABLE()

WPRMain::WPRMain(wxWindow* parent, wxWindowID id){
    wxBoxSizer* BoxSizerV, *BoxSizerH;
    Create(parent, wxID_ANY, _("WiFi Password Recovery [v1.0]"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxMINIMIZE_BOX | wxSTATIC_BORDER, _T("wxID_ANY"));
    SetIcon(wxICON(APP_ICON));
    SetClientSize(wxSize(450, 325));
    wxFont mainFont = wxFont();
    mainFont.SetPointSize(8);
	SetFont(mainFont);
    SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));
    Menu1 = new wxMenu();
    MenuBar1 = new wxMenuBar();
    MenuItemAbout = new wxMenuItem(Menu1, ID_MENUITEMABOUT, _("Info"), wxEmptyString, wxITEM_NORMAL);
    MenuItemQuit = new wxMenuItem(Menu1, ID_MENUITEMQUIT, _("Quit\tCtrl+Q"), wxEmptyString, wxITEM_NORMAL);
    Menu1->Append(MenuItemAbout);
    Menu1->AppendSeparator();
    Menu1->Append(MenuItemQuit);
    MenuBar1->Append(Menu1, _("\?"));
    SetMenuBar(MenuBar1);
    ButtonRecover = new wxButton(this, ID_BUTTONRECOVER, _("Recover"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONRECOVER"));
    ButtonRecover->SetDefault();
    ButtonRecover->Disable();
    ButtonScan = new wxButton(this, ID_BUTTONSCAN, _("Scan"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONSCAN"));
    Label1 = new wxStaticText(this, ID_LABEL1, _("Select a network from the list to show its info:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_LABEL1"));
    ListViewNetworks = new wxListView(this, ID_LISTVIEWNETWORKS, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_SINGLE_SEL | wxTAB_TRAVERSAL, wxDefaultValidator, _T("ID_LISTVIEWNETWORKS"));
    ListViewNetworks->InsertColumn(0, wxT("Name"), wxLIST_FORMAT_LEFT, wxLIST_AUTOSIZE);
    ListViewNetworks->InsertColumn(1, wxT("Security"), wxLIST_FORMAT_LEFT, wxLIST_AUTOSIZE);
    ListViewNetworks->InsertColumn(2, wxT("Encryption"), wxLIST_FORMAT_LEFT, wxLIST_AUTOSIZE);
    ListViewNetworks->InsertColumn(3, wxT("Interface"), wxLIST_FORMAT_LEFT, wxLIST_AUTOSIZE);
    //Alignment
    BoxSizerV = new wxBoxSizer(wxVERTICAL);
    BoxSizerH = new wxBoxSizer(wxHORIZONTAL);
    BoxSizerH->Add(ButtonScan, 0, wxRIGHT, 6);
    BoxSizerH->Add(ButtonRecover, 0);
    BoxSizerV->Add(Label1, 0, wxALL, 8);
    BoxSizerV->Add(ListViewNetworks, 1, wxEXPAND | wxALIGN_BOTTOM | wxLEFT | wxRIGHT | wxBOTTOM, 8);
    BoxSizerV->Add(BoxSizerH, 0, wxALIGN_RIGHT | wxALIGN_BOTTOM | wxRIGHT | wxBOTTOM, 8);
    SetSizer(BoxSizerV);
    //End alignment
    Connect(ID_BUTTONRECOVER, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction) &WPRMain::OnButtonRecoverClick);
    Connect(ID_BUTTONSCAN, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction) &WPRMain::OnButtonScanClick);
    Connect(ID_LISTVIEWNETWORKS, wxEVT_COMMAND_LIST_COL_CLICK, (wxObjectEventFunction) &WPRMain::OnListViewNetworksColumnClick);
    Connect(ID_LISTVIEWNETWORKS, wxEVT_COMMAND_LIST_ITEM_ACTIVATED, (wxObjectEventFunction) &WPRMain::OnListViewNetworksItemActivated);
    Connect(ID_LISTVIEWNETWORKS, wxEVT_COMMAND_LIST_ITEM_DESELECTED, (wxObjectEventFunction) &WPRMain::OnListViewNetworksItemDeselect);
    Connect(ID_LISTVIEWNETWORKS, wxEVT_COMMAND_LIST_ITEM_SELECTED, (wxObjectEventFunction) &WPRMain::OnListViewNetworksItemSelect);
    Connect(ID_MENUITEMABOUT, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) &WPRMain::OnMenuItemAboutSelected);
    Connect(ID_MENUITEMQUIT, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) &WPRMain::OnQuit);
    UpdateNetworksList();
}

WPRMain::~WPRMain(){}

typedef struct {
    wxListView* listView;
    int col;
} SortInfo;

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
    WPRInfoDialog infdlg(this);
    infdlg.ShowModal();
}

void WPRMain::ShowNetworkInfo(){
    wxListItem sel;
    WlanNetwork* wn;
    sel.SetColumn(0);
    sel.SetId(ListViewNetworks->GetFirstSelected());
    sel.SetMask(wxLIST_MASK_TEXT);
    ListViewNetworks->GetItem(sel);
    wn = WlanInfo::GetNetworkInfo(sel.GetText().wchar_str());
    if(!wn){
        wxMessageBox(wxT("Failed to retrieve network informations."), wxT("Error"), wxOK | wxICON_ERROR);
        return;
    }
    WPRWlanDialog wlandlg(this);
    wlandlg.wn = wn;
    wlandlg.ShowModal();
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
        wxMessageBox(wxT("No WiFi networks found."), wxT("Info"), wxOK | wxICON_INFORMATION);
        return;
    }
    wn = WlanInfo::GetNetworksList();
    for(int i = 0; i < wnc; i++){
        int l;
        l = ListViewNetworks->InsertItem(0, wxString(wn[i].name, wxConvUTF8));
        ListViewNetworks->SetItem(0, 1, wxString(WlanInfo::WlanAuthToString(wn[i].auth), wxConvUTF8));
        ListViewNetworks->SetItem(0, 2, wxString(WlanInfo::WlanEncryptionToString(wn[i].encryption), wxConvUTF8));
        ListViewNetworks->SetItem(0, 3, wxString(wn[i].intrface, wxConvUTF8));
        ListViewNetworks->SetItemData(l, i);
        ListViewNetworks->SetColumnWidth(0, wxLIST_AUTOSIZE);
        ListViewNetworks->SetColumnWidth(3, wxLIST_AUTOSIZE);
    }
    SortItems(lastCol);
}
