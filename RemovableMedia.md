**Removable Media Access and Other Not Home Locations**

A Photo Tool (Libre) has normally full system wide disk access. But containerized versions of the app have limited access, restricted by the container to the home folder.

To allow access to external drives they need to be mounted to /media (or /mnt). This normally happens automatically but may require some action by the user.

Then the user must allow the A Photo Tool (Libre) to have access to this folder. Some examples are shown below. More information can be found on the container systems own documentation.

***Snap***

``sudo snap connect aphototoollibre:removable-media``

***Flatpak***

``flatpak override --user --filesystem=/media me.ahola.aphototoollibre``

***General workaround***

Create a folder for photos under the Pictures folder

``mkdir -p ~/Pictures/external-system-folder``

- **external-system-folder** is what ever name you want to use

Mount your pictures from your source to this location

``sudo mount --bind /var/data/images ~/Pictures/external-system-folder``

- **/var/data/images** is an example of your photos actual location
- all changes made in external-system-folder are immediately made to original source files also (they are the same files) 
- this is a temporary mounting and will not survive reboot

If you want to unmount

``sudo umount ~/Pictures/external-system-folder``
