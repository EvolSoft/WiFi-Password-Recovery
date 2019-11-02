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

#ifndef NETWORKINFODIALOG_H
#define NETWORKINFODIALOG_H

#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>

#include "WlanInfo.h"

class NetworkInfoDialog : public wxDialog {
	public:
        wxButton* ButtonClose;
        wxButton* ButtonSave;
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
		NetworkInfoDialog(wxWindow* parent, WlanNetwork* networkInfo);
		virtual ~NetworkInfoDialog();
	private:
	    WlanNetwork* networkInfo;
		void OnButtonCloseClick(wxCommandEvent& event);
		void OnButtonSaveClick(wxCommandEvent& event);
		DECLARE_EVENT_TABLE()
};
#endif //NETWORKINFODIALOG_H
