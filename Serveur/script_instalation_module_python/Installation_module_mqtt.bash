#!/bin/bash
# -*- tab-width: 4; encoding: utf-8 -*-
#
## @file
## @author Loris Benaitier
## @brief Bash pour installer paho-mqtt
## @copyright MIT
## @version 1
##

# Lancer commande pour installer paho-mqtt
# Mode d'emploi : 
# Commande de lancement d'un script bash : 
# 1/ sudo chmod +x nom_fichier.bash 
# 2/ sudo sed -i -e 's/\r$//' nom_fichier.bash (Lancer cette commande si on a l'erreur suivant : bad interpreter : No such file or directory)
# 3/ sudo ./nom_fichier.bash (Il faut se trouver dans le dossier ou se trouve le script)
pip3 install paho-mqtt
# Si il manque d'autres modules (erreur quand on lance le serveur python : No module named 'nom_module_manquant')
# installer module manquant : pip3 install nom_module_manquant