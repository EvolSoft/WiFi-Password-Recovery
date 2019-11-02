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

#ifndef WPRMAIN_H
#define WPRMAIN_H

#include <map>
#include <wx/button.h>
#include <wx/fileconf.h>
#include <wx/frame.h>
#include <wx/listctrl.h>
#include <wx/menu.h>
#include <wx/stattext.h>

using namespace std;

typedef struct {
    wxListView* listView;
    int col;
} SortInfo;

class WPRMain : public wxFrame {
    public:
        WPRMain(wxWindow* parent, wxFileConfig* config);
        virtual ~WPRMain();
    private:
        static bool sortDesc;
        static int wxCALLBACK ListViewCompare(wxIntPtr item1, wxIntPtr item2, wxIntPtr sortData);
        static int lastCol;
        wxButton* ButtonRecover;
        wxButton* ButtonScan;
        wxFileConfig* config;
        wxStaticText* Label1;
        wxListView* ListViewNetworks;
        wxMenu* MenuFile;
        wxMenu* MenuHelp;
        wxMenu* MenuLanguage;
        wxMenuBar* MenuBar;
        wxMenuItem* MenuItemAbout;
        wxMenuItem* MenuItemBug;
        wxMenuItem* MenuItemDonate;
        wxMenuItem* MenuItemExportNetworks;
        wxMenuItem* MenuItemImportNetworks;
        wxMenuItem* MenuItemQuit;
        wxMenuItem* MenuItemScan;
        wxMenuItem* MenuItemWebsite;
        map<int, int> menuLanguageItemLink;
        void InitLanguageMenu(wxMenu* languageMenu);
        void MenuLanguageHandler(wxCommandEvent& event);
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnButtonRecoverClick(wxCommandEvent& event);
        void OnButtonScanClick(wxCommandEvent& event);
        void OnListViewNetworksColumnClick(wxListEvent& event);
        void OnListViewNetworksItemActivated(wxListEvent& event);
        void OnListViewNetworksItemDeselect(wxListEvent& event);
        void OnListViewNetworksItemSelect(wxListEvent& event);
        void OnMenuItemAboutSelected(wxCommandEvent& event);
        void OnMenuItemBugSelected(wxCommandEvent& event);
        void OnMenuItemDonateSelected(wxCommandEvent& event);
        void OnMenuItemExportNetworksSelected(wxCommandEvent& event);
        void OnMenuItemImportNetworksSelected(wxCommandEvent& event);
        void OnMenuItemWebsiteSelected(wxCommandEvent& event);
        void ShowNetworkInfo();
        void SortItems(int col);
        void UpdateNetworksList();
        void UpdateUITexts();
        DECLARE_EVENT_TABLE()
};
#endif //WPRMAIN_H
