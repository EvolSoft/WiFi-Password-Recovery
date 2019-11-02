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

#include "XMLHelper.h"

#include <wx/mstream.h>

XmlHelper::XmlHelper(){}

XmlHelper::XmlHelper(const wxXmlDocument& doc){
    this->doc = doc;
    root = new XmlHelperNode(this->doc.GetRoot());
}

XmlHelper::XmlHelper(wxInputStream& stream, const wxString& encoding){
    doc = wxXmlDocument(stream, encoding);
    root = new XmlHelperNode(doc.GetRoot());
}

XmlHelper::XmlHelper(const wxString& filename, const wxString& encoding){
    doc = wxXmlDocument(filename, encoding);
    root = new XmlHelperNode(doc.GetRoot());
}

bool XmlHelper::LoadFromFile(const wxString& filename, const wxString& encoding, int flags){
    bool status;
    wxLogNull disableLogging;
    status = doc.Load(filename, encoding, flags);
    disableLogging.~wxLogNull();
    if(status){
        root = new XmlHelperNode(doc.GetRoot());
    }
    return status;
}

bool XmlHelper::LoadFromStream(wxInputStream& stream, const wxString& encoding, int flags){
    bool status;
    wxLogNull disableLogging;
    status = doc.Load(stream, encoding, flags);
    disableLogging.~wxLogNull();
    if(status){
        root = new XmlHelperNode(doc.GetRoot());
    }
    return status;
}

bool XmlHelper::LoadFromString(const wxString& str, const wxString& encoding, int flags){
    wxCharBuffer buffer;
    if(encoding.CmpNoCase(wxT("ASCII"))){
        buffer = str.ToAscii();
    }else if(encoding.CmpNoCase(wxT("UTF-8"))){
        buffer = str.ToUTF8();
    }
    wxMemoryInputStream stream(buffer.data(), strlen(buffer.data()));
    return LoadFromStream(stream, encoding, flags);
}

XmlHelperNode* XmlHelper::GetRoot(){
    return root;
}

wxXmlDocument XmlHelper::GetDocument(){
    return doc;
}

XmlHelperNode::XmlHelperNode(){
    this->node = NULL;
}

XmlHelperNode::XmlHelperNode(wxXmlNode* node){
    this->node = node;
}

bool XmlHelperNode::ChildExists(wxString key){
    wxXmlNode* child;
    if(node == NULL){
        return false;
    }
    child = node->GetChildren();
    while(child){
        if(child->GetName().CmpNoCase(key)){
            return true;
        }
        child->GetNext();
    }
    return false;
}

bool XmlHelperNode::Exists(){
    return node != NULL;
}

XmlHelperNode* XmlHelperNode::GetChild(wxString key){
    wxXmlNode* child;
    if(node == NULL){
        return &XmlHelperNullNode;
    }
    child = node->GetChildren();
    while(child){
        if(child->GetName().CmpNoCase(key) == 0){
            return new XmlHelperNode(child);
        }
        child = child->GetNext();
    }
    return &XmlHelperNullNode;
}

wxString XmlHelperNode::GetContent(){
    if(node == NULL){
        return wxT("");
    }
    return node->GetNodeContent();
}

wxXmlNode* XmlHelperNode::GetNode(){
    return node;
}
