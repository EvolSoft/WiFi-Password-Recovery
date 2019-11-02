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

#ifndef NETWORKDATAFORMAT1_H
#define NETWORKDATAFORMAT1_H

#include "NetworkDataFormat.h"
#include "NetworkData.h"

#include <wx/string.h>
#include <wx/wfstream.h>

class NetworkDataFormat1 : public NetworkDataFormat {
public:
    int Export(wxFileOutputStream* fileStream, vector<NetworkDataItem> networks, bool encrypt);
    vector<NetworkDataItem> Import(wxFileInputStream* fileStream, int* status);
    static int GetVersion();
};
#endif //NETWORKDATAFORMAT1_H
