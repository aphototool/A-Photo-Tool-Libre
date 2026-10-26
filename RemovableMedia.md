**Removable Media Access**

A Photo Tool (Libre) has normally full system wide disk access. But containerized versions of the app have limited access, restricted by the container to the home folder.

To allow access to external drives they need to be mounted to /media (or /mnt). This normally happens automatically but may require some action by the user.

Then the user must allow the A Photo Tool (Libre) to have access to this folder. Some examples are shown below. More information can be found on the container systems own documentation.

***Snap***

``sudo snap connect aphototoollibre:removable-media``

***Flatpak***

``flatpak override --user --filesystem=/media dev.ahola.aphototoollibre``
