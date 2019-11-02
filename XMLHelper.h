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

#ifndef XMLHELPER_H
#define XMLHELPER_H

#include <wx/log.h>
#include <wx/stream.h>
#include <wx/xml/xml.h>

class XmlHelperNode {
public:
    XmlHelperNode();
    XmlHelperNode(wxXmlNode* node);
    bool ChildExists(wxString key);
    bool Exists();
    XmlHelperNode* GetChild(wxString key);
    wxString GetContent();
    wxXmlNode* GetNode();
private:
    wxXmlNode* node;
};

static XmlHelperNode XmlHelperNullNode;

class XmlHelper {
public:
    XmlHelper();
    XmlHelper(const wxXmlDocument& doc);
    XmlHelper(const wxString& filename, const wxString& encoding);
    XmlHelper(wxInputStream& stream, const wxString& encoding);
    bool LoadFromFile(const wxString& filename, const wxString& encoding, int flags=wxXMLDOC_NONE);
    bool LoadFromStream(wxInputStream& stream, const wxString& encoding, int flags=wxXMLDOC_NONE);
    bool LoadFromString(const wxString& str, const wxString& encoding, int flags=wxXMLDOC_NONE);
    XmlHelperNode* GetRoot();
    wxXmlDocument GetDocument();
private:
    wxXmlDocument doc;
    XmlHelperNode* root;
};
#endif //XMLHELPER_H
