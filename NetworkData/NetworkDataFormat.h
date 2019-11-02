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

#ifndef NETWORKDATAFORMAT_H
#define NETWORKDATAFORMAT_H

#include <vector>
#include <wx/string.h>
#include <wx/wfstream.h>

using namespace std;

struct NetworkDataItem;

class NetworkDataFormat {
public:
    virtual int Export(wxFileOutputStream* fileStream, vector<NetworkDataItem> networks, bool encrypt) = 0;
    virtual vector<NetworkDataItem> Import(wxFileInputStream* fileStream, int* status) = 0;
    static int GetVersion();
};
#endif //NETWORKDATAFORMAT_H
