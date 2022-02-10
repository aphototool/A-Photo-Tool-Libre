# A Photo Tool (Libre)

## Photo editing app for Linux

Easy to use photo editing software.

![A Photo Tool Libre on Debian desktop](/pics/APhotoTool(Libre)Debian.png)
   
Change exposure, contrast and color of your jpeg photos.
Turn color phototos in to black and white images with control how color is handled.
Rotate, crop, straighten and resize photos.

Copyright © 2021-2022 Jari Ahola

A Photo Tool (Libre[^1] version) is free image editor published under GNU Public Lisence version 3 (GPLv3).

[^1]: See explanation of Libre in https://en.wikipedia.org/wiki/Free_software

You are free to use it, free to download source code and use it in accordance of the GPLv3 and basically
you are free to do what you like. If you publish modified app you need to publish the source code also
(as dictated by GPLv3). Original author Jari Ahola will remain as the copyright holder of A Photo Tool
(Libre).

See LICENSE file or https://www.gnu.org/licenses/gpl-3.0.txt for GPLv3.

### How to Install (.deb for 64 bit Intel / AMD)

1. Download aphototoollibre_1.0-1.deb (see releases)

2. You may check the file integrity (*this step is not required but is good for your safety*). 
- Enter command
```
sha256sum aphototoollibre_1.0-1.deb
```
- The result should be (*if not, do not install*)
```
f589315a34e1184dea357c1e683de61355f9efb81d817a84cbf199f3d0596e64  aphototoollibre_1.0-1.deb
```
- You can compare the SHA256 message digest checksum at https://www.ahola.me/aphototoollibre.html to get additional confirmation.

3. Use your favorite graphical package manager to install. For command line use
```
sudo apt install ./aphototoollibre_1.0-1.deb
```
   - You may see error message at the end of otherwise succesfull installation about permission denied and performin as root. That is ok.
   - Package managers may show license as "proprietary". The license is actually GPLv3.

#### Tested environments
- Debian 11.2 (Gnome)
- Ubuntu Desktop 20.04 LTS
- Ubuntu Desktop 21.10
- Ubuntu MATE 20.04 LTS
- Kubuntu 21.10 

Absolutely **no** quarantee is given that this app will work on any tested or untested environment.

**This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.**


### How to Run

<img align="left" src="https://github.com/aphototool/APhotoToolLibre/blob/main/pics/APhotoToolLibreIcon.png?raw=true">

**A Photo Tool (Libre)** is shown on your applications menu. Depending on your environment it may be under category **Graphics** or you may search it by entering **A Photo Tool** on your app search.

On command line you can enter **APhotoToolLibre** to launch A Photo Tool (Libre) application.

### How to Uninstall

Use your favorite graphical package manager. For command line use
```
sudo apt remove aphototoollibre
```


### BUILD INSTRUCTIONS

*If you just want to use A Photo Tool (Libre) app you can ignore build instructions and just follow instructions at **How to Install**. These build instructions are here only to comply with GPLv3 source code requirements.*

#### Technical
- Programming language C++
- Framework Qt 5.15.2 (see https://www.qt.io/ and https://doc.qt.io/qt-5/lgpl.html and https://www.qt.io/licensing/open-source-lgpl-obligations)

#### Prerequisites
- Basic knowledge of C++, Qt, QMake, GitHub and Linux development.
- Advanced knowledge of Debian package management (see https://www.debian.org/doc/manuals/maint-guide/index.en.html and other relevant documentation).

#### Steps to build
- Install necessary Debian developer tools
- Create and configure .pbuilderrc
```
BASETGZ=$HOME/pbuilder/base.tgz
BUILDPLACE=$HOME/pbuilder/build/
BUILDRESULT=$HOME/pbuilder/result/
DEBEMAIL='Your Name <your@emaild.id>'
BUILDUSERNAME=username
AUTO_DEBSIGN=yes
APTCACHEHARDLINK=no
DEBBUILDOPTS="-sa"
```
- Run
```
pbuilder --create
```
- Get project from GitHub into aphototoolibre-x.x.x folder (x.x.x is current version number)
- Compress aphototoolibre-x.x.x folder to aphototoolibre_x.x.x.tar.xz (filemanager Compress... works usually)
- Go to aphototoolibre-x.x.x folder and run
```
dh_make -e your_email -f ../aphototoollibre_x.x.x.tar.xz -s -c gpl3
pdebuild --debbuildopts -sa
```
- **aphototoollibre_x.x.x-y_amd64.deb** file is created on **pbuilder/result** folder

#### Special note

Prebuild .deb file was built on Debian 11.2 with Qt 5.15.2



### CONTRIBUTING TO THE PROJECT

At this time source code contributions to A Photo Tool (Libre) project are not accepted. This is to avoid any copyright issues with any of the commercial
products of Ahola Development (https://www.ahola.dev) and the need to require Contributor License Agreement (CLA) or a Copyright Transfer Agreement (CTA).
This does not in anyway limit the rights you have under GPLv3.
