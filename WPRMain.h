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

#ifndef WPRMAIN_H
#define WPRMAIN_H

#include <wx/button.h>
#include <wx/frame.h>
#include <wx/listctrl.h>
#include <wx/menu.h>
#include <wx/stattext.h>

class WPRMain : public wxFrame {
    public:
        WPRMain(wxWindow* parent, wxWindowID id = -1);
        virtual ~WPRMain();
    private:
        static bool sortDesc;
        static const long ID_BUTTONRECOVER;
        static const long ID_BUTTONSCAN;
        static const long ID_LABEL1;
        static const long ID_LISTVIEWNETWORKS;
        static const long ID_MENUITEMABOUT;
        static const long ID_MENUITEMQUIT;
        static int wxCALLBACK ListViewCompare(wxIntPtr item1, wxIntPtr item2, wxIntPtr sortData);
        static int lastCol;
        wxButton* ButtonRecover;
        wxButton* ButtonScan;
        wxStaticText* Label1;
        wxListView* ListViewNetworks;
        wxMenu* Menu1;
        wxMenuBar* MenuBar1;
        wxMenuItem* MenuItemAbout;
        wxMenuItem* MenuItemQuit;
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnButtonRecoverClick(wxCommandEvent& event);
        void OnButtonScanClick(wxCommandEvent& event);
        void OnListViewNetworksColumnClick(wxListEvent& event);
        void OnListViewNetworksItemActivated(wxListEvent& event);
        void OnListViewNetworksItemDeselect(wxListEvent& event);
        void OnListViewNetworksItemSelect(wxListEvent& event);
        void OnMenuItemAboutSelected(wxCommandEvent& event);
        void ShowNetworkInfo();
        void SortItems(int col);
        void UpdateNetworksList();
        DECLARE_EVENT_TABLE()
};
#endif //WPRMAIN_H
