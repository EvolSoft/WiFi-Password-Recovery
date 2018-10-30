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

#ifndef WPRINFODIALOG_H
#define WPRINFODIALOG_H

#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/hyperlink.h>
#include <wx/statbmp.h>
#include <wx/stattext.h>

class WPRInfoDialog : public wxDialog {
	public:
        wxStaticBitmap* BitmapIcon;
		wxButton* ButtonClose;
		wxButton* ButtonDonate;
        wxHyperlinkCtrl* Hyperlink1;
		wxStaticText* Label1;
		wxStaticText* Label2;
		wxStaticText* Label3;
		wxStaticText* Label4;
		WPRInfoDialog(wxWindow* parent, wxWindowID id = wxID_ANY);
		virtual ~WPRInfoDialog();
	protected:
        static const long ID_BITMAPICON;
		static const long ID_BUTTONCLOSE;
        static const long ID_BUTTONDONATE;
        static const long ID_HYPERLINK1;
		static const long ID_LABEL1;
		static const long ID_LABEL2;
		static const long ID_LABEL3;
		static const long ID_LABEL4;
	private:
		void OnInit(wxInitDialogEvent& event);
		void OnButtonCloseClick(wxCommandEvent& event);
		void OnButtonDonateClick(wxCommandEvent& event);
		DECLARE_EVENT_TABLE()
};
#endif //WPRINFODIALOG_H
