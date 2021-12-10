![start2](https://cloud.githubusercontent.com/assets/10303538/6315586/9463fa5c-ba06-11e4-8f30-ce7d8219c27d.png)

# WiFi Password Recovery

[![Download WiFi Password Recovery](https://img.shields.io/sourceforge/dt/wifi-password-recovery.svg)](https://sourceforge.net/projects/wifi-password-recovery/files/latest/download)

WiFi Password Recovery is a free utility to recover the passwords of the WiFi networks saved on your computer. For example if you forgot the password of a WiFi network (which you have entered in the past), you can easily recover it thanks to this tool. WiFi Password Recovery provides a very simple user interface which shows also other informations (SSID, Interface, Security type, Encryption algorithm) for each wireless network. The program is able to recover the passwords of wireless networks which use the following security types: WEP, WPA, WPA-PSK, WPA2, WPA2-PSK.*

**Note: On Windows XP the passwords of WiFi networks using WPA security will be shown as 64 HEX digits. You can use these characters as password to connect to the wireless network anyway.**

## Requirements

- Windows XP SP3, Vista, 7, 8.x, 10
- A wireless network card of course

## Donate

If you want you can support this project with a small donation by clicking [:dollar: here](https://www.paypal.me/Flavius12). 
Your generosity will help us paying web hosting, domains, buying programs (such as IDEs, debuggers, etc...) and new hardware to improve software development. Thank you :smile:

## Precompiled binaries

Precompiled binaries of WiFi Password Recovery are available for download as installer version or portable version.
Click the button below to download the installer:

[![Download WiFi Password Recovery](https://a.fsdn.com/con/app/sf-download-button)](https://sourceforge.net/projects/wifi-password-recovery/files/latest/download)

or visit [this page](https://www.evolsoft.org/wifi-password-recovery/download/) for more download options.

## Translations

| **Language** | **Translated by** | **Version** | **Download** |
| --- | --- | --- | --- |
| 🇫🇷 French | Rico.sos | 2.0 | [WiFiPasswordRecovery2.0_French.zip](https://github.com/EvolSoft/WiFi-Password-Recovery/files/7685713/WiFiPasswordRecovery2.0_French.zip) |

## Screenshots

![1](https://user-images.githubusercontent.com/10297075/68071383-4fe8b900-fd7a-11e9-8e75-24a38be27a50.png)
<br>*WiFi Password Recovery main screen*<br><br>

![2](https://user-images.githubusercontent.com/10297075/68071389-5840f400-fd7a-11e9-8d1f-205adf83a29c.png)
<br>*WiFi Password Recovery main screen 2*<br><br>

![3](https://user-images.githubusercontent.com/10297075/68071393-62fb8900-fd7a-11e9-96e0-9fb0781b37dc.png)
<br>*WiFi Password Recovery wireless network info screen*<br><br>

![4](https://user-images.githubusercontent.com/10297075/68071419-7dcdfd80-fd7a-11e9-88f8-965b99fe3ea8.png)
<br>*WiFi Password Recovery Wireless Networks Import Wizard*<br><br>

## Compiling source code

To compile WiFi Password Recovery source code you need:
- *MinGW-w64* compiler 
- *wxWidgets* 2.8

Before compiling source code you may need to modify the *Makefile* in order to set the right compiler and libraries paths. After doing that, run:

```
mingw32-make clean
```

and

```
mingw32-make
```

If the compilation succeeds, you will find the generated object files into *obj/* folder and the main executable into *bin/* folder.

Don't forget to add the runtime libraries inside the executable path before running it otherwise it will not work.

## License

```
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
```

## 3rd-party libraries used

- [wxWidgets](https://www.wxwidgets.org)

## Contributing

If you want to contribute to this project please follow the [Contribution Guidelines](https://github.com/EvolSoft/WiFi-Password-Recovery/blob/master/CONTRIBUTING.md).
