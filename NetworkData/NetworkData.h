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

#ifndef NETWORKDATA_H
#define NETWORKDATA_H

#include "NetworkDataFormat.h"
#include "../WlanInfo.h"

#include <map>
#include <stdint.h>
#include <vector>

#define SUCCESS 0
#define ERR_FILE_OPEN 1
#define ERR_FILE_CLOSE 2
#define ERR_FILE_INVALID_VERSION 3
#define ERR_FILE_CORRUPT 4
#define ERR_FILE_DECRYPT 5

#define NETWORKDATA_CURRENT_VERSION 1
#define CRYPT_KEY "A9Ci3%oB3_hn"

using namespace std;

struct NetworkDataItem {
    wchar_t name[WLAN_MAX_NAME_LENGTH];
    wchar_t password[MAX_PASSWORD_LENGTH];
    WlanAuth auth;
    WlanEncryption encryption;
};

class NetworkData {
public:
    NetworkData();
    void Add(WlanNetwork network);
    int Export(wxString filename, bool encrypt);
    int Import(wxString filename);
    vector<NetworkDataItem> GetNetworksList();
    bool GetNetworkByName(wchar_t* name, NetworkDataItem* network);
    static uint32_t Encrypt(char* buf, int len, char* key);
    static bool Decrypt(char* buf, int len, char* key, uint32_t verify);
    static uint32_t Crc32(uint32_t crc, const char *buf, size_t len);
    static NetworkDataItem ToNetworkDataItem(WlanNetwork network);
    static WlanNetwork ToWlanNetwork(NetworkDataItem profileData);
private:
    map<uint8_t, NetworkDataFormat*> networkDataFormats;
    vector<NetworkDataItem> networks;
};

#endif // NETWORKDATA_H
