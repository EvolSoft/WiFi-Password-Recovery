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

#ifndef LANGUAGEHELPER_H
#define LANGUAGEHELPER_H

#include "WPRMain.h"

#include <vector>
#include <wx/menu.h>

using namespace std;

struct Language {
    wxChar* name;
    int langId;
};

class LanguageHelper : public wxObject {
    public:
        static LanguageHelper* GetInstance();
        bool IsInstalled(int language);
        wxString GetLocalePath();
        bool ApplyLanguage(int lang);
        static vector<Language> GetSupportedLanguages();
        static int GetSupportedLanguagesCount();
        vector<Language> GetAvailableLanguages();
        int GetAvailableLanguagesCount();
        int GetCurrentLanguage();
        static int GetDefaultLanguage();
    private:
        static LanguageHelper* instance;
        static vector<Language> languages;
        vector<Language> avLanguages;
        int curLang;
        wxLocale* locale;
        LanguageHelper();
        void InitAvaliableLanguages();
};
#endif
