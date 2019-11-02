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

#include "NetworkDataFormat1.h"

#include <stdint.h>
#include <wx/mstream.h>

int NetworkDataFormat1::Export(wxFileOutputStream* fileStream, vector<NetworkDataItem> networks, bool encrypt){
    uint8_t numNetworks = networks.size(), encryptFile = (uint8_t) encrypt;
    char* outBuffer;
    uint32_t crc;
    wxMemoryOutputStream* outStream = new wxMemoryOutputStream();
    fileStream->Write(&encryptFile, sizeof(uint8_t));
    outStream->Write(&numNetworks, sizeof(uint8_t));
    for(unsigned int i = 0; i < networks.size(); i++){
        outStream->Write(&networks[i], sizeof(NetworkDataItem));
    }
    outBuffer = (char*) outStream->GetOutputStreamBuffer()->GetBufferStart();
    if(encrypt){
        crc = NetworkData::Encrypt(outBuffer, outStream->GetOutputStreamBuffer()->GetBufferSize(), CRYPT_KEY);
    }else{
        crc = NetworkData::Crc32(0, outBuffer, outStream->GetOutputStreamBuffer()->GetBufferSize());
    }
    fileStream->Write(&crc, sizeof(uint32_t));
    fileStream->Write(outBuffer, outStream->GetOutputStreamBuffer()->GetBufferSize());
    outStream->Close();
    return SUCCESS;
}

vector<NetworkDataItem> NetworkDataFormat1::Import(wxFileInputStream* fileStream, int* status){
    wxMemoryInputStream* inStream;
    NetworkDataItem networkDataItem;
    vector<NetworkDataItem> networks = vector<NetworkDataItem>();
    uint8_t numNetworks;
    uint8_t encrypted;
    char* buffer;
    size_t buflen;
    uint32_t crc;
    fileStream->Read(&encrypted, sizeof(uint8_t));
    fileStream->Read(&crc, sizeof(uint32_t));
    buflen = fileStream->GetSize() - fileStream->TellI(); //Total file length - size of encrypted and crc
    buffer = (char*) malloc(buflen);
    fileStream->Read(buffer, buflen);
    if(encrypted){
        if(!NetworkData::Decrypt(buffer, buflen, CRYPT_KEY, crc)){
            free(buffer);
            *status = ERR_FILE_DECRYPT;
            return networks;
        }
    }else if(crc != NetworkData::Crc32(0, buffer, buflen)){
        free(buffer);
        *status = ERR_FILE_CORRUPT;
        return networks;
    }
    inStream = new wxMemoryInputStream(buffer, buflen);
    inStream->Read(&numNetworks, sizeof(uint8_t));
    for(int i = 0; i < numNetworks; i++){
        inStream->Read(&networkDataItem, sizeof(NetworkDataItem));
        networks.push_back(networkDataItem);
    }
    free(buffer);
    *status = SUCCESS;
    return networks;
}

int NetworkDataFormat1::GetVersion(){
    return 1;
}
