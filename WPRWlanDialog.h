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

#ifndef WPRWLANDIALOG_H
#define WPRWLANDIALOG_H

#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>

#include "WlanInfo.h"

class WPRWlanDialog : public wxDialog {
	public:
	    WlanNetwork* wn;
        wxButton* ButtonClose;
		wxStaticText* LabelEncryptVal;
		wxStaticText* LabelInterfaceVal;
		wxStaticText* LabelSSIDVal;
		wxStaticText* LabelSecurityVal;
		wxStaticText* LabelEncrypt;
		wxStaticText* LabelInterface;
		wxStaticText* LabelPassword;
		wxStaticText* LabelSecurity;
		wxStaticText* LabelSSID;
		wxTextCtrl* TextBoxPasswordVal;
		WPRWlanDialog(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);
		virtual ~WPRWlanDialog();
	protected:
	    static const long ID_BUTTONCLOSE;
        static const long ID_LABELENCRYPT;
		static const long ID_LABELENCRYPTVAL;
        static const long ID_LABELINTERFACE;
		static const long ID_LABELINTERFACEVAL;
        static const long ID_LABELPASSWORD;
		static const long ID_LABELSECURITY;
        static const long ID_LABELSECURITYVAL;
        static const long ID_LABELSSID;
        static const long ID_LABELSSIDVAL;
        static const long ID_TEXTBOXPASSWORDVAL;
	private:
		void OnInit(wxInitDialogEvent& event);
		void OnButtonCloseClick(wxCommandEvent& event);
		DECLARE_EVENT_TABLE()
};
#endif //WPRWLANDIALOG_H
