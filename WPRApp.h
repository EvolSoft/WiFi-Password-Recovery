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

#ifndef WPRAPP_H
#define WPRAPP_H

#include <wx/app.h>
#include <wx/fileconf.h>
#include <wx/string.h>

class WPRApp : public wxApp {
    public:
        virtual bool OnInit();
    private:
        wxFileConfig* config;
        void InitConfig();
        bool IsElevated();
        bool RunAsAdmin(wxString arg);
};
#endif //WPRAPP_H
