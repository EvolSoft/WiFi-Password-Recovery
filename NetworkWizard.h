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

#ifndef NETWORKWIZARD_H
#define NETWORKWIZARD_H

#include <wx/arrstr.h>
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/dialog.h>
#include <wx/filepicker.h>
#include <wx/gauge.h>
#include <wx/listbox.h>
#include <wx/radiobut.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/wizard.h>

#include "WlanInfo.h"
#include "NetworkData/NetworkData.h"

struct NetworkWizardData {
    wxString path;
    bool import;
    bool encrypt;
    bool selectAll;
    bool canExit;
    bool askExit;
    wxArrayString selectedNetworks;
    NetworkData networkData;
};

class NetworkWizardPathPage : public wxWizardPageSimple {
public:
    NetworkWizardPathPage(wxWizard* parent, NetworkWizardData* data);
private:
    NetworkWizardData* data;
    wxFilePickerCtrl* filePickerDest;
    wxCheckBox* checkBoxEncryptData;
    void OnShow(wxWizardEvent& event);
    void OnPageChange(wxWizardEvent& event);
};

class NetworkWizardChooserPage : public wxWizardPage {
public:
    NetworkWizardChooserPage(wxWizard* parent, wxWizardPage* previousPage, wxWizardPage* progressPage, wxWizardPage* customPage, NetworkWizardData* data);
    virtual wxWizardPage* GetPrev() const;
    virtual wxWizardPage* GetNext() const;
private:
    NetworkWizardData* data;
    wxWizardPage* previousPage;
    wxWizardPage* progressPage;
    wxWizardPage* customPage;
    wxRadioButton* radioButtonAll;
    void OnShow(wxWizardEvent& event);
};

class NetworkWizardCustomPage : public wxWizardPageSimple {
public:
    NetworkWizardCustomPage(wxWizard* parent, NetworkWizardData* data);
private:
    NetworkWizardData* data;
    wxListBox* listBoxNetworks;
    void OnShow(wxCommandEvent& event);
    void OnListBoxSelect(wxCommandEvent& event);
    void OnPageChange(wxWizardEvent& event);
};

class NetworkWizardProgressPage : public wxWizardPageSimple {
public:
    NetworkWizardProgressPage(wxWizard* parent, NetworkWizardData* data, WlanNetwork* wlanNetworks);
private:
    NetworkWizardData* data;
    WlanNetwork* wlanNetworks;
    wxStaticText* labelProgressDesc;
    wxGauge* progressBar;
    void OnShow(wxCommandEvent& event);
};

class NetworkWizard : public wxWizard {
	public:
		NetworkWizard(wxWindow* parent, bool import);
		NetworkWizard(wxWindow* parent, wxString path);
		static void SetButtonStatus(int id, bool status);
		virtual ~NetworkWizard();
		NetworkWizardData data;
	private:
	    wxWizardPageSimple* welcomePage;
	    NetworkWizardPathPage* pathPage;
	    NetworkWizardChooserPage* chooserPage;
	    NetworkWizardCustomPage* customPage;
	    NetworkWizardProgressPage* progressPage;
	    void OnWizardCancel(wxWizardEvent& event);
	    void WelcomePage();
		DECLARE_EVENT_TABLE()
};
#endif //NETWORKWIZARD_H
