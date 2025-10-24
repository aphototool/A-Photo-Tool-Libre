### How to Install (.deb for 64 bit Intel / AMD)

1. Download aphototoollibre_X.X.X-Y_amd64.deb (where X.X.X-Y is the latest version number - see [Releases](../../releases))

2. You may check the file integrity (*this step is not required but is good for your safety*). 
- Enter command
```
sha256sum aphototoollibre_X.X.X-Y_amd64.deb
```
- The result should be what is shown on the release page (*if not, do not install*)

- You can compare the SHA256 message digest checksum at https://www.ahola.me/aphototoollibre.html to get additional confirmation.

3. Use your favorite graphical package manager to install. For command line use
```
sudo apt install ./aphototoollibre_X.X.X-Y_amd64.deb
```
   - You may see error message at the end of otherwise successful installation about permission denied and performing as root. That is ok.
   - Package managers may show license as "proprietary". The license is actually GPLv3.

#### Tested environments (version 1.0.7)
- Debian 13.1 (Gnome)

#### Old versions vere tested on (does not apply to 1.0.7 or later)
- Ubuntu Desktop 21.10
- Kubuntu 21.10
- Ubuntu MATE 21.10

Does **not** work on
- Ubuntu 20.04.3 LTS (too old libraries, get Snap instead at https://snapcraft.io/install/aphototoollibre/ubuntu)


### How to Install (.deb for 64 bit ARM)

1. Download aphototoollibre_X.X.X-Y_arm64.deb (where X.X.X-Y is the latest version number - see [Releases](../../releases))

2. You may check the file integrity (*this step is not required but is good for your safety*). 
- Enter command
```
sha256sum aphototoollibre_X.X.X-Y_arm64.deb
```
- The result should be what is shown on the release page (*if not, do not install*)

- You can compare the SHA256 message digest checksum at https://www.ahola.me/aphototoollibre.html to get additional confirmation.

3. Use your favorite graphical package manager to install. For command line use
```
sudo apt install ./aphototoollibre_X.X.X-Y_arm64.deb
```
   - You may see error message at the end of otherwise successful installation about permission denied and performing as root. That is ok.
   - Package managers may show license as "proprietary". The license is actually GPLv3.

#### Tested environments (version 1.0.7)
- Ubuntu Desktop 25.04 running on Raspberry Pi 400
- Raspberry Pi OS ("port of Debian Trixie")


Absolutely **no** guarantee is given that this app will work on any tested or untested environment.

**This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.**

