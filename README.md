![start2](https://cloud.githubusercontent.com/assets/10303538/6315586/9463fa5c-ba06-11e4-8f30-ce7d8219c27d.png)

# WiFi Password Recovery

[![Download WiFi Password Recovery](https://img.shields.io/sourceforge/dt/wifi-password-recovery.svg)](https://sourceforge.net/projects/wifi-password-recovery/files/latest/download)

WiFi Password Recovery is a free utility to recover the passwords of the WiFi networks saved on your computer. For example if you forgot the password of a WiFi network (which you have entered in the past), you can easily recover it thanks to this tool. WiFi Password Recovery provides a very simple user interface which shows also other informations (SSID, Interface, Security type, Encryption algorithm) for each wireless network. The program is able to recover the passwords of wireless networks which use the following security types: WEP, WPA, WPA-PSK, WPA2, WPA2-PSK.*

**Note: On Windows XP the passwords of WiFi networks using WPA security will be shown as 64 HEX digits. You can use these characters as password to connect to the wireless network anyway.**

## Requirements

- Windows XP SP3, Vista, 7, 8.x, 10
- A wireless network card of course

## Precompiled binaries

Precompiled binaries of WiFi Password Recovery are available for download as installer version or portable version.
Click the button below to download the installer:

[![Download WiFi Password Recovery](https://a.fsdn.com/con/app/sf-download-button)](https://sourceforge.net/projects/wifi-password-recovery/files/latest/download)

or visit [this page](https://www.evolsoft.tk/wifi-password-recovery/download/) for more download options.

## Screenshots

![1](https://user-images.githubusercontent.com/10297075/47753540-30670e00-dc98-11e8-9250-ba034244c5bc.png)
<br>*WiFi Password Recovery main screen*<br><br>

![2](https://user-images.githubusercontent.com/10297075/47753659-9e133a00-dc98-11e8-91d7-4bd9e1be6e0c.png)
<br>*WiFi Password Recovery main screen 2*<br><br>

![3](https://user-images.githubusercontent.com/10297075/47753682-b08d7380-dc98-11e8-8acb-adb9bd103ad4.png)
<br>*WiFi Password Recovery wireless network info screen*<br><br>

## Compiling source code

To compile WiFi Password Recovery source code you need:
- *MinGW-w64* compiler 
- *wxWidgets* 2.8
- *pugixml* library

Before compiling source code you may need to modify the *Makefile* in order to set the right compiler and libraries paths. After doing that, run:

```
mingw32-make clean
```

and

```
mingw32-make
```

If compilation succeeds, you will find the generated object files into *obj/* folder and the main executable into *bin/* folder.

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

- wxWidgets
- pugixml

## Donate

If you want you can support this project with a small donation by clicking [:dollar: here](https://www.paypal.com/cgi-bin/webscr?cmd=_donations&business=flavius.c.1999@gmail.com&lc=US&item_name=www.evolsoft.tk&no_note=0&cn=&curency_code=EUR&bn=PP-DonationsBF:btn_donateCC_LG.gif:NonHosted). 
Your generosity will help us paying web hosting, domains, buying programs (such as IDEs, debuggers, etc...) and new hardware to improve software development. Thank you :smile:

## Contributing

If you want to contribute to this project please follow the [Contribution Guidelines](https://github.com/EvolSoft/WiFi-Password-Recovery/blob/master/CONTRIBUTING.md).