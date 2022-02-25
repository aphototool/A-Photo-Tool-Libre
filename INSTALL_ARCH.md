### How to Install (Arch User Repository (AUR) for 64 bit Intel / AMD)

A Photo Tool (Libre) is in the AUR repository https://aur.archlinux.org/packages/aphototoollibre.

Instructions for AUR are here https://wiki.archlinux.org/title/Arch_User_Repository#Installing_and_upgrading_packages. 
You should be familiar with the instructions and security advice. AUR packages are not verified by Arch Linux.
Also see what is instructed in the **Prerequisites** to be ready for installing AUR packages if this is your first use of AUR.

Installation from AUR is multi step procedure but instructions below are easy to follow.
Arch Linux does not recommend use of helpper utilities that simplify installation 
as they usually skip manual verification of the application (like something that is done in step 3 below).

Steps to install A Photo Tool (Libre) from AUR.

1. Go to a folder where you process AUR installations. Make one if you don't have one ready.
2. Enter command
```
git clone https://aur.archlinux.org/aphototoollibre.git
```

- If git command fails with **git: command not found** you need to install it with `sudo pacman -S git` comand.

3. Enter command
```
cd aphototoollibre
```

4. This step may be skipped but to always follow security guidelines inspect PKGBUILD file carefully. For example you may enter
```
less PKGBUILD
```
- Inspect also evertyhting in src folder. This needs C++ skils so you may skip if you trust the source of this app.

5. Build package
```
makepkg -src
```
- if build fails with **makepkg: command not found** you need to install it with `sudo pacman -S --needed base-devel` comand.
- if build fails with **ERROR: One or more PGP signatures could not be verified!** then install public GPG signature for "aphototool". You can search for key with command `gpg --search-keys aphototool` but to install use
```
gpg --recv-keys A970F7E40CB64F0D5B9FC516AFE56C2DF614820C
```
- then enter `makepkg` again to rebuild.

6. Install build application.
```
sudo pacman -U aphototoollibre-1.0.1-2-x86_64.pkg.tar.zst
```

#### Tested environments
- Arch Linux (KDE Plasma and Gnome)

Absolutely **no** guarantee is given that this app will work on any tested or untested environment.

**This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.**

