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

#include "LanguageHelper.h"

#include <wx/intl.h>
#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/msgdlg.h>
#include <wx/stdpaths.h>
#include <wx/string.h>

vector<Language> LanguageHelper::languages{
    {wxT("Aҧсуа бызшәа [Abkhazian]"), wxLANGUAGE_ABKHAZIAN},
    {wxT("Qafár af [Afar]"), wxLANGUAGE_AFAR},
    {wxT("Afrikaans [Afrikaans]"), wxLANGUAGE_AFRIKAANS},
    {wxT("Shqip [Albanian]"), wxLANGUAGE_ALBANIAN},
    {wxT("ኣማርኛ [Amharic]"), wxLANGUAGE_AMHARIC},
    {wxT("عربى [Arabic]"), wxLANGUAGE_ARABIC},
#ifdef EXTRA_LANGUAGES
    {wxT("[Arabic (Algeria)]"), wxLANGUAGE_ARABIC_ALGERIA},
    {wxT("[Arabic (Bahrain)]"), wxLANGUAGE_ARABIC_BAHRAIN},
    {wxT("[Arabic (Egypt)]"), wxLANGUAGE_ARABIC_EGYPT},
    {wxT("[Arabic (Iraq)]"), wxLANGUAGE_ARABIC_IRAQ},
    {wxT("[Arabic (Jordan)]"), wxLANGUAGE_ARABIC_JORDAN},
    {wxT("[Arabic (Kuwait)]"), wxLANGUAGE_ARABIC_KUWAIT},
    {wxT("[Arabic (Lebanon)]"), wxLANGUAGE_ARABIC_LEBANON},
    {wxT("[Arabic (Libya)]"), wxLANGUAGE_ARABIC_LIBYA},
    {wxT("[Arabic (Morocco)]"), wxLANGUAGE_ARABIC_MOROCCO},
    {wxT("[Arabic (Oman)]"), wxLANGUAGE_ARABIC_OMAN},
    {wxT("[Arabic (Qatar)]"), wxLANGUAGE_ARABIC_QATAR},
    {wxT("[Arabic (Saudi Arabia)]"), wxLANGUAGE_ARABIC_SAUDI_ARABIA},
    {wxT("[Arabic (Sudan)]"), wxLANGUAGE_ARABIC_SUDAN},
    {wxT("[Arabic (Syria)]"), wxLANGUAGE_ARABIC_SYRIA},
    {wxT("[Arabic (Tunisia)]"), wxLANGUAGE_ARABIC_TUNISIA},
    {wxT("[Arabic (United Arab Emirates)]"), wxLANGUAGE_ARABIC_UAE},
    {wxT("[Arabic (Yemen)]"), wxLANGUAGE_ARABIC_YEMEN},
#endif
    {wxT("Հայերեն [Armenian]"), wxLANGUAGE_ARMENIAN},
    {wxT("অসমীয়া [Assamese]"), wxLANGUAGE_ASSAMESE},
    {wxT("Aymar Aru [Aymara]"), wxLANGUAGE_AYMARA},
    {wxT("آذربايجانجا ديلي [Azerbaijani]"), wxLANGUAGE_AZERI},
    {wxT("Азәрбајҹан дили (Cyrillic) [Azerbaijani]"), wxLANGUAGE_AZERI_CYRILLIC},
    {wxT("Azərbaycan dili (Latin) [Azerbaijani]"), wxLANGUAGE_AZERI_LATIN },
    {wxT("башҡорт Теле [Bashkir]"), wxLANGUAGE_BASHKIR},
    {wxT("Euskara [Basque]"), wxLANGUAGE_BASQUE},
    {wxT("Беларуская [Belarusian]"), wxLANGUAGE_BELARUSIAN},
    {wxT("বাংলা [Bengali]"), wxLANGUAGE_BENGALI},
//    {wxT("Bhutani [Bhutani]"), wxLANGUAGE_BHUTANI},
//    {wxT("Bihari [Bihari]"), wxLANGUAGE_BIHARI},
    {wxT("Bislama [Bislama]"), wxLANGUAGE_BISLAMA},
    {wxT("Brezhoneg [Breton]"), wxLANGUAGE_BRETON},
    {wxT("български език [Bulgarian]"), wxLANGUAGE_BULGARIAN},
    {wxT("မြန်မာစာ or မြန်မာစကား [Burmese]"), wxLANGUAGE_BURMESE},
    {wxT("Català [Catalan]"), wxLANGUAGE_CATALAN},
    {wxT("中文 [Chinese]"), wxLANGUAGE_CHINESE},
#ifdef EXTRA_LANGUAGES
    {wxT("简体中文 [Chinese (Simplified)]"), wxLANGUAGE_CHINESE_SIMPLIFIED},
    {wxT("繁体中文 [Chinese (Traditional)]"), wxLANGUAGE_CHINESE_TRADITIONAL},
    {wxT("中文 （香港） [Chinese]"), wxLANGUAGE_CHINESE_HONGKONG},
    {wxT("中文 (澳门) [Chinese]"), wxLANGUAGE_CHINESE_MACAU},
    {wxT("中文 (新加坡) [Chinese]"), wxLANGUAGE_CHINESE_SINGAPORE},
    {wxT("中文 (台湾) [Chinese]"), wxLANGUAGE_CHINESE_TAIWAN},
#endif
    {wxT("Corsu [Corsican]"), wxLANGUAGE_CORSICAN},
    {wxT("Hrvatski [Croatian]"), wxLANGUAGE_CROATIAN},
    {wxT("Čeština [Czech]"), wxLANGUAGE_CZECH},
    {wxT("Dansk [Danish]"), wxLANGUAGE_DANISH},
    {wxT("Nederlands [Dutch]"), wxLANGUAGE_DUTCH},
#ifdef EXTRA_LANGUAGES
    {wxT("Nederlands (België) [Dutch]"), wxLANGUAGE_DUTCH_BELGIAN},
#endif
    {wxT("English [English]"), wxLANGUAGE_ENGLISH},    //Default language
#ifdef EXTRA_LANGUAGES
    {wxT("English (UK) [English]"), wxLANGUAGE_ENGLISH_UK},
    {wxT("English (US) [English]"), wxLANGUAGE_ENGLISH_US},
    {wxT("English (Australia) [English]"), wxLANGUAGE_ENGLISH_AUSTRALIA},
    {wxT("English (Belize) [English]"), wxLANGUAGE_ENGLISH_BELIZE},
    {wxT("English (Botswana) [English]"), wxLANGUAGE_ENGLISH_BOTSWANA},
    {wxT("English (Canada) [English]"), wxLANGUAGE_ENGLISH_CANADA},
    {wxT("English (Caribbean) [English]"), wxLANGUAGE_ENGLISH_CARIBBEAN},
    {wxT("English (Denmark) [English]"), wxLANGUAGE_ENGLISH_DENMARK},
    {wxT("English (Ireland) [English]"), wxLANGUAGE_ENGLISH_EIRE},
    {wxT("English (Jamaica) [English]"), wxLANGUAGE_ENGLISH_JAMAICA},
    {wxT("English (New Zealand) [English]"), wxLANGUAGE_ENGLISH_NEW_ZEALAND},
    {wxT("English (Philippines) [English]"), wxLANGUAGE_ENGLISH_PHILIPPINES},
    {wxT("English (South Africa) [English]"), wxLANGUAGE_ENGLISH_SOUTH_AFRICA},
    {wxT("English (Trinidad) [English]"), wxLANGUAGE_ENGLISH_TRINIDAD},
    {wxT("English (Zimbabwe) [English]"), wxLANGUAGE_ENGLISH_ZIMBABWE},
#endif
    {wxT("Esperanto [Esperanto]"), wxLANGUAGE_ESPERANTO},
    {wxT("Eesti [Estonian]"), wxLANGUAGE_ESTONIAN},
    {wxT("Føroyskt [Faeroese]"), wxLANGUAGE_FAEROESE},
    {wxT("فارسی [Farsi]"), wxLANGUAGE_FARSI},
    {wxT("Vakaviti [Fiji]"), wxLANGUAGE_FIJI},
    {wxT("Suomi [Finnish]"), wxLANGUAGE_FINNISH},
    {wxT("Français [French]"), wxLANGUAGE_FRENCH},
#ifdef EXTRA_LANGUAGES
    {wxT("Français (Belgique) [French]"), wxLANGUAGE_FRENCH_BELGIAN},
    {wxT("Français (Canada) [French]"), wxLANGUAGE_FRENCH_CANADIAN},
    {wxT("Français (Luxembourg) [French]"), wxLANGUAGE_FRENCH_LUXEMBOURG},
    {wxT("Français (Monaco) [French]"), wxLANGUAGE_FRENCH_MONACO},
    {wxT("Français (Suisse) [French]"), wxLANGUAGE_FRENCH_SWISS},
#endif
    {wxT("Frysk [Frisian]"), wxLANGUAGE_FRISIAN},
    {wxT("Galego [Galician]"), wxLANGUAGE_GALICIAN},
    {wxT("ქართული [Georgian]"), wxLANGUAGE_GEORGIAN},
    {wxT("Deutsch [German]"), wxLANGUAGE_GERMAN},
#ifdef EXTRA_LANGUAGES
    {wxT("Deutsch (Österreich) [German]"), wxLANGUAGE_GERMAN_AUSTRIAN},
    {wxT("Deutsch (Belgien) [German]"), wxLANGUAGE_GERMAN_BELGIUM},
    {wxT("Deutsch (Liechtenstein) [German]"), wxLANGUAGE_GERMAN_LIECHTENSTEIN},
    {wxT("Deutsch (Luxemburg) [German]"), wxLANGUAGE_GERMAN_LUXEMBOURG},
    {wxT("Deutsch (Schweiz) [German]"), wxLANGUAGE_GERMAN_SWISS},
#endif
    {wxT("Ελληνικά [Greek]"), wxLANGUAGE_GREEK},
    {wxT("Kalaallisut [Greenlandic]"), wxLANGUAGE_GREENLANDIC},
    {wxT("Avañe’ẽ [Guarani]"), wxLANGUAGE_GUARANI},
    {wxT("ગુજરાતી [Gujarati]"), wxLANGUAGE_GUJARATI},
    {wxT("حَوْسَ [Hausa]"), wxLANGUAGE_HAUSA},
    {wxT("עברית [Hebrew]"), wxLANGUAGE_HEBREW},
    {wxT("हिन्दी [Hindi]"), wxLANGUAGE_HINDI},
    {wxT("Magyar [Hungarian]"), wxLANGUAGE_HUNGARIAN},
    {wxT("Íslenska [Icelandic]"), wxLANGUAGE_ICELANDIC},
    {wxT("Bahasa Indonesia [Indonesian]"), wxLANGUAGE_INDONESIAN},
    {wxT("Interlingua [Interlingua]"), wxLANGUAGE_INTERLINGUA},
    {wxT("Interlingue [Occidental]"), wxLANGUAGE_INTERLINGUE},
    {wxT("ᐃᓄᒃᑎᑐᑦ [Inuktitut]"), wxLANGUAGE_INUKTITUT},
    {wxT("Iñupiatun [Inupiaq]"), wxLANGUAGE_INUPIAK},
    {wxT("Gaeilge [Irish]"), wxLANGUAGE_IRISH},
    {wxT("Italiano [Italian]"), wxLANGUAGE_ITALIAN},
#ifdef EXTRA_LANGUAGES
    {wxT("Italiano (Svizzera) [Italian]"), wxLANGUAGE_ITALIAN_SWISS},
#endif
    {wxT("日本語 [Japanese]"), wxLANGUAGE_JAPANESE},
    {wxT("ꦧꦱꦗꦮ [Javanese]"), wxLANGUAGE_JAVANESE},
    {wxT("ಕನ್ನಡ [Kannada]"), wxLANGUAGE_KANNADA},
    {wxT("कॉशुर [Kashmiri]"), wxLANGUAGE_KASHMIRI},
#ifdef EXTRA_LANGUAGES
    {wxT("[Kashmiri (India)]"), wxLANGUAGE_KASHMIRI_INDIA},
#endif
    {wxT("Қазақ Tілі [Kazakh]"), wxLANGUAGE_KAZAKH},
    {wxT("Kernewek Kemmyn [Kernewek]"), wxLANGUAGE_KERNEWEK},
    {wxT("Ikinyarwanda [Kinyarwanda]"), wxLANGUAGE_KINYARWANDA},
    {wxT("кыргызча [Kirghiz]"), wxLANGUAGE_KIRGHIZ},
    {wxT("Ikirundi [Kirundi]"), wxLANGUAGE_KIRUNDI},
    {wxT("कोंकणी [Konkani]"), wxLANGUAGE_KONKANI},
    {wxT("한국어 [Korean]"), wxLANGUAGE_KOREAN},
    {wxT("Kurdí [Kurdish]"), wxLANGUAGE_KURDISH},
    {wxT("ພາສາລາວ [Lao]"), wxLANGUAGE_LAOTHIAN},
    {wxT("Lingua Latina [Latin]"), wxLANGUAGE_LATIN},
    {wxT("Latviešu [Latvian]"), wxLANGUAGE_LATVIAN},
    {wxT("Lingála [Lingala]"), wxLANGUAGE_LINGALA},
    {wxT("Lietuvių [Lithuanian]"), wxLANGUAGE_LITHUANIAN},
    {wxT("Mакедонски [Macedonian]"), wxLANGUAGE_MACEDONIAN},
    {wxT("Malagasy [Malagasy]"), wxLANGUAGE_MALAGASY},
    {wxT("Bahasa Melayu [Malay]"), wxLANGUAGE_MALAY},
#ifdef EXTRA_LANGUAGES
    {wxT("Bahasa Melayu (Brunei Darussalam) [Malay (Brunei-Darussalam)]"), wxLANGUAGE_MALAY_BRUNEI_DARUSSALAM},
    {wxT("Bahasa Melayu (Malaysia) [Malay]"), wxLANGUAGE_MALAY_MALAYSIA},
#endif
    {wxT("മലയാളം [Malayalam]"), wxLANGUAGE_MALAYALAM},
    {wxT("Malti [Maltese]"), wxLANGUAGE_MALTESE},
    {wxT("মৈইতৈইলোন [Manipuri]"), wxLANGUAGE_MANIPURI},
    {wxT("Te Reo Māori [Maori]"), wxLANGUAGE_MAORI},
    {wxT("मराठी [Marathi]"), wxLANGUAGE_MARATHI},
    {wxT("лимба молдовеняскэ [Moldavian]"), wxLANGUAGE_MOLDAVIAN},
    {wxT("Монгол Хэл [Mongolian]"), wxLANGUAGE_MONGOLIAN},
    {wxT("Ekakairũ Naoero [Nauru]"), wxLANGUAGE_NAURU},
    {wxT("नेपाली [Nepali]"), wxLANGUAGE_NEPALI},
#ifdef EXTRA_LANGUAGES
    {wxT("नेपाली (भारत) [Nepali]"), wxLANGUAGE_NEPALI_INDIA},
#endif
    {wxT("Norsk (Bokmål) [Norwegian]"), wxLANGUAGE_NORWEGIAN_BOKMAL},
    {wxT("Norsk (Nynorsk) [Norwegian]"), wxLANGUAGE_NORWEGIAN_NYNORSK},
    {wxT("Occitan [Occitan]"), wxLANGUAGE_OCCITAN},
    {wxT("ଓଡ଼ିଆ [Oriya]"), wxLANGUAGE_ORIYA},
    {wxT("Afaan Oromo [Oromo]"), wxLANGUAGE_OROMO},
    {wxT("پښتو [Pashto]"), wxLANGUAGE_PASHTO},
    {wxT("Polski [Polish]"), wxLANGUAGE_POLISH},
    {wxT("Português [Portuguese]"), wxLANGUAGE_PORTUGUESE},
#ifdef EXTRA_LANGUAGES
    {wxT("Português (Brasil) [Portuguese]"), wxLANGUAGE_PORTUGUESE_BRAZILIAN},
#endif
    {wxT("ਪੰਜਾਬੀ [Punjabi]"), wxLANGUAGE_PUNJABI},
    {wxT("Qhichwa [Quechua]"), wxLANGUAGE_QUECHUA},
//    {wxT("Lingue Romanze [Rhaeto Romance]"), wxLANGUAGE_RHAETO_ROMANCE},
    {wxT("Română [Romanian]"), wxLANGUAGE_ROMANIAN},
    {wxT("Русский [Russian]"), wxLANGUAGE_RUSSIAN},
#ifdef EXTRA_LANGUAGES
    {wxT("Русский (Украина) [Russian]"), wxLANGUAGE_RUSSIAN_UKRAINE},
#endif
//    {wxT("Sami [Sami]"), wxLANGUAGE_SAMI},
    {wxT("Gagana Sāmoa [Samoan]"), wxLANGUAGE_SAMOAN},
    {wxT("Yângâ tî Sängö [Sangho]"), wxLANGUAGE_SANGHO},
    {wxT("संस्कृतम् [Sanskrit]"), wxLANGUAGE_SANSKRIT},
    {wxT("Gàidhlig [Scottish Gaelic]"), wxLANGUAGE_SCOTS_GAELIC},
    {wxT("Српски [Serbian]"), wxLANGUAGE_SERBIAN},
#ifdef EXTRA_LANGUAGES
    {wxT("Српски [Serbian (Cyrillic)]"), wxLANGUAGE_SERBIAN_CYRILLIC},
    {wxT("Srpski [Serbian (Latin)]"), wxLANGUAGE_SERBIAN_LATIN},
#endif
    {wxT("Srpskohrvatski [Serbo-Croatian]"), wxLANGUAGE_SERBO_CROATIAN},
    {wxT("Sesotho [Sesotho]"), wxLANGUAGE_SESOTHO},
    {wxT("Setswana [Tswana]"), wxLANGUAGE_SETSWANA},
    {wxT("Shona [Shona]"), wxLANGUAGE_SHONA},
    {wxT("سنڌي [Sindhi]"), wxLANGUAGE_SINDHI},
    {wxT("සිංහල [Sinhala]"), wxLANGUAGE_SINHALESE},
    {wxT("siSwati [Siswati]"), wxLANGUAGE_SISWATI},
    {wxT("Slovenčina [Slovak]"), wxLANGUAGE_SLOVAK},
    {wxT("Slovenščina [Slovenian]"), wxLANGUAGE_SLOVENIAN},
    {wxT("Af-Soomaali [Somali]"), wxLANGUAGE_SOMALI},
    {wxT("Español [Spanish]"), wxLANGUAGE_SPANISH},
#ifdef EXTRA_LANGUAGES
    {wxT("Español (Argentina) [Spanish]"), wxLANGUAGE_SPANISH_ARGENTINA},
    {wxT("Español (Bolivia) [Spanish]"), wxLANGUAGE_SPANISH_BOLIVIA},
    {wxT("Español (Chile) [Spanish]"), wxLANGUAGE_SPANISH_CHILE},
    {wxT("Español (Colombia) [Spanish]"), wxLANGUAGE_SPANISH_COLOMBIA},
    {wxT("Español (Costa Rica) [Spanish]"), wxLANGUAGE_SPANISH_COSTA_RICA},
    {wxT("Español (Republica Dominicana) [Spanish]"), wxLANGUAGE_SPANISH_DOMINICAN_REPUBLIC},
    {wxT("Español (Ecuador) [Spanish]"), wxLANGUAGE_SPANISH_ECUADOR},
    {wxT("Español (El Salvador) [Spanish]"), wxLANGUAGE_SPANISH_EL_SALVADOR},
    {wxT("Español (Guatemala) [Spanish]"), wxLANGUAGE_SPANISH_GUATEMALA},
    {wxT("Español (Honduras) [Spanish]"), wxLANGUAGE_SPANISH_HONDURAS},
    {wxT("Español (Mexicano) [Spanish]"), wxLANGUAGE_SPANISH_MEXICAN},
//    {wxT("Español (Modern) [Spanish]"), wxLANGUAGE_SPANISH_MODERN},
    {wxT("Español (Nicaragua) [Spanish]"), wxLANGUAGE_SPANISH_NICARAGUA},
    {wxT("Español (Panama) [Spanish]"), wxLANGUAGE_SPANISH_PANAMA},
    {wxT("Español (Paraguay) [Spanish]"), wxLANGUAGE_SPANISH_PARAGUAY},
    {wxT("Español (Peru) [Spanish]"), wxLANGUAGE_SPANISH_PERU},
    {wxT("Español (Puerto Rico) [Spanish]"), wxLANGUAGE_SPANISH_PUERTO_RICO},
    {wxT("Español (Uruguay) [Spanish]"), wxLANGUAGE_SPANISH_URUGUAY},
    {wxT("Español (US) [Spanish]"), wxLANGUAGE_SPANISH_US},
    {wxT("Español (Venezuela) [Spanish]"), wxLANGUAGE_SPANISH_VENEZUELA},
#endif
    {wxT("Basa Sunda [Sundanese]"), wxLANGUAGE_SUNDANESE},
    {wxT("Kiswahili [Swahili]"), wxLANGUAGE_SWAHILI},
    {wxT("Svenska [Swedish]"), wxLANGUAGE_SWEDISH},
#ifdef EXTRA_LANGUAGES
    {wxT("Svenska (Finland) [Swedish]"), wxLANGUAGE_SWEDISH_FINLAND},
#endif
    {wxT("Tagalog [Tagalog]"), wxLANGUAGE_TAGALOG},
    {wxT("تاجيک [Tajik]"), wxLANGUAGE_TAJIK},
    {wxT("தமிழ் [Tamil]"), wxLANGUAGE_TAMIL},
    {wxT("Tатарча [Tatar]"), wxLANGUAGE_TATAR},
    {wxT("తెలుగు [Telugu]"), wxLANGUAGE_TELUGU},
    {wxT("ภาษาไทย [Thai]"), wxLANGUAGE_THAI},
    {wxT("དབུས་སྐད་ [Tibetan]"), wxLANGUAGE_TIBETAN},
    {wxT("ትግርኛ [Tigrinya]"), wxLANGUAGE_TIGRINYA},
    {wxT("chiTonga [Tonga]"), wxLANGUAGE_TONGA},
    {wxT("xiTsonga [Tsonga]"), wxLANGUAGE_TSONGA},
    {wxT("Türkçe [Turkish]"), wxLANGUAGE_TURKISH},
    {wxT("түркmенче [Turkmen]"), wxLANGUAGE_TURKMEN},
    {wxT("Twi [Twi]"), wxLANGUAGE_TWI},
    {wxT("ئۇيغۇر تىلى [Uyghur]"), wxLANGUAGE_UIGHUR},
    {wxT("Українська [Ukrainian]"), wxLANGUAGE_UKRAINIAN},
    {wxT("اُردُو [Urdu]"), wxLANGUAGE_URDU},
#ifdef EXTRA_LANGUAGES
    {wxT("اُردُو (بھارت) [Urdu]"), wxLANGUAGE_URDU_INDIA},
    {wxT("اُردُو (پاکستان) [Urdu]"), wxLANGUAGE_URDU_PAKISTAN},
#endif
    {wxT("Oʻzbek [Uzbek]"), wxLANGUAGE_UZBEK},
#ifdef EXTRA_LANGUAGES
    {wxT("[Uzbek (Cyrillic)]"), wxLANGUAGE_UZBEK_CYRILLIC},
    {wxT("[Uzbek (Latin)]"), wxLANGUAGE_UZBEK_LATIN},
#endif
    {wxT("Valencià [Valencian]"), wxLANGUAGE_VALENCIAN},
    {wxT("Tiếng Việt Nam [Vietnamese]"), wxLANGUAGE_VIETNAMESE},
    {wxT("Volapük [Volapuk]"), wxLANGUAGE_VOLAPUK},
    {wxT("Cymraeg [Welsh]"), wxLANGUAGE_WELSH},
    {wxT("Wolof [Wolof]"), wxLANGUAGE_WOLOF},
    {wxT("Xhosa [Xhosa]"), wxLANGUAGE_XHOSA},
    {wxT("ײִדיש [Yiddish]"), wxLANGUAGE_YIDDISH},
    {wxT("Yorùbá [Yoruba]"), wxLANGUAGE_YORUBA},
    {wxT("話僮 [Zhuang]"), wxLANGUAGE_ZHUANG},
    {wxT("Zulu [Zulu]"), wxLANGUAGE_ZULU}
};

LanguageHelper* LanguageHelper::instance = NULL;

LanguageHelper::LanguageHelper(){
    wxLocale::AddCatalogLookupPathPrefix(wxT("locale"));
    curLang = wxLANGUAGE_DEFAULT;
    locale = new wxLocale(curLang, wxLOCALE_CONV_ENCODING);
    InitAvaliableLanguages();
}

void LanguageHelper::InitAvaliableLanguages(){
    int i;
    for(i = 0; i < GetSupportedLanguagesCount(); i++){
        if(IsInstalled(languages[i].langId)){
            avLanguages.push_back(languages[i]);
        }
    }
}

LanguageHelper* LanguageHelper::GetInstance(){
    if(!instance){
       instance = new LanguageHelper();
    }
    return instance;
}

bool LanguageHelper::ApplyLanguage(int lang){
    if(!IsInstalled(lang)){
        return false;
    }
    delete locale;
    locale = new wxLocale;
    locale->Init(lang);
    locale->AddCatalog(wxLocale::GetLanguageInfo(lang)->CanonicalName);
    if(!locale->IsOk()){            //Restore default language
        delete locale;
        curLang = GetDefaultLanguage();
        locale = new wxLocale(curLang);
        return false;
    }
    curLang = lang;
    return true;
}

bool LanguageHelper::IsInstalled(int lang){
    wxString localePath;
    if(lang == GetDefaultLanguage()){
        return true;
    }else if(!wxLocale::IsAvailable(lang)){
        return false;
    }
    localePath = GetLocalePath();
    return wxFileExists(localePath + '/' << wxLocale::GetLanguageInfo(lang)->CanonicalName + '/' << wxLocale::GetLanguageInfo(lang)->CanonicalName << wxT(".mo"));
}

wxString LanguageHelper::GetLocalePath(){
    wxFileName exeDir = wxStandardPaths::Get().GetExecutablePath();
    return exeDir.GetPath().Append(wxT("/locale"));
}

vector<Language> LanguageHelper::GetSupportedLanguages(){
    return languages;
}

int LanguageHelper::GetSupportedLanguagesCount(){
    return languages.size();
}

vector<Language> LanguageHelper::GetAvailableLanguages(){
    return avLanguages;
}

int LanguageHelper::GetAvailableLanguagesCount(){
    return avLanguages.size();
}

int LanguageHelper::GetCurrentLanguage(){
    return curLang;
}

int LanguageHelper::GetDefaultLanguage(){
    return wxLANGUAGE_ENGLISH;
}
