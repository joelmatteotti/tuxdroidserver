==========================
TuxDroidServer
Joel Matteotti <joel_DOT_matteotti_AT_free_DOT_FR>
http://sourceforge.net/projects/tuxdroidserver/
==========================

1. COMPILATION DU CODE
2. INSTALLATION DE LA REGLE UDEV
3. DESINSTALLATION DE LA REGLE UDEV

===========================


Pour plus d'informations, voir le wiki en ligne :

http://sourceforge.net/p/tuxdroidserver/francais/Home/


1. COMPILATION DU CODE:
------------------------

./configure
make
make clean


2. INSTALLATION DE LA REGLE UDEV:
---------------------------------

!! Cette étape est à faire en root !!

Afin que Fux (le dongle USB de TuxDroid) soit correctement reconnu, il faut pour
cela ajouter une règle à udev et redémarrer udev.

Vous pouvez le faire avec la commande suivante:


make install


Cela aura pour effet de copier la règle udev de TuxDroid dans le répertoire des
règles udev, ainsi que de redémarrer votre service udev.



3. DESINSTALLATION DE LA REGLE UDEV:
------------------------------------

!! Cette étape est à faire en root !!

Si vous souhaitez déinstaller la règle udev vous pouvez le faire avec la
commande suivante:

make uninstall


Cela aura pour effet de supprimer le fichier de règle udev de TuxDroid et
redémarrera votre service udev.
