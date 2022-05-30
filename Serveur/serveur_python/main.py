"""Serveur python qui récupèrent les messages de type lora/up, décodent les données utilent, utilisent et enregistrent les données dans une base de donnée.
Usage:
======
    python main.py 

"""

__authors__ = ("Loris Benaitier")
__copyright__ = "MIT"
__date__ = "2022-05-01"
__version__= "1.0"




# Librairies utilisées
from socket import timeout, gaierror
from threading import Thread
import paho.mqtt.client as mqtt
import re
import base64
import sqlite3
import configparser
import sys
import logging
import datetime

# Variables globales
ip = ""
port = 0
nom_bdd = ""
chemin_fichier_conf = ""
list_data_bdd = [-1, -1, 0, 0, 0, 0, 0, 0, 0]

# Fonction pour générerer fichier historique système
def log_init():
    """! Fonction qui va générer un fichier log pour la sauvegarde des messages d'erreurs ou de debug pour chaque sessions (Si le fichier existe deja, alors il ecrit dedans). 
    @param aucun
    @return rien
    """
    try:
        logging.basicConfig(level=logging.DEBUG,
                            filename="historique.log",
                            filemode="a",
                            format='%(asctime)s - %(levelname)s - %(message)s')
    except Exception as e:
        print(f'log : {type(e).__name__}')
        print("Erreur inatendue, impossible de cree le fichier log...")
    else:
        print("Fichier log pret!")

# Fonction pour ajouter données dans une BDD
def Ajout_Base_Donnee(list_data, Nom_bdd, Nom_table):
    """! Fonctions qui va enregistrer les données capteurs contenues dans les trames lora.
    @param list_data Liste de données contenant les informations à enregistrer
    @param Nom_bdd Chaine de caractère contenant le nom de la base de donnée
    @param Nom_table Chaine de caractère contenant le nom de la table
    @return rien
    """

    # Variable activation pour gérer les bugs...
    Activ_1 = False
    Activ_2 = False
    Activ_3 = False
    Activ_4 = False
    Activ_5 = False
    Activ_6 = False

    # Preparation commande pour l'ajout de données
    Name_commande = "INSERT INTO"
    Name_table = Nom_table
    sql = Name_commande + " " + Name_table + " (id_course, id_bateau, latitude, longitude, cap, vitesse, vitesse_vent, direction_vent, date) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)"
    val = list_data

    # Connexion base de donnée et fermeture quand la commande a été effectué...
    Name_Base_donnee = Nom_bdd
    try:
        connection = sqlite3.connect(Name_Base_donnee)
        Activ_1 = True
        cursor = connection.cursor()
        Activ_2 = True
        cursor.execute(sql, val)
        Activ_3 = True
        cursor.close()
        Activ_4 = True
        connection.commit()
        Activ_5 = True
        connection.close()
        Activ_6 = True
    except Exception as e:
        print(str(e))
        if Activ_1 is False:
            print(f'Impossible de se connecter à la base de donnée, nom : {Name_Base_donnee}')
            logging.debug(f'Impossible de se connecter à la base de donnée, nom : {Name_Base_donnee}')
        elif Activ_2 is False:
            print(f'Impossible de cree un curseur pour interagir sur la base de donnée, nom : {Name_Base_donnee}')
            logging.debug(f'Impossible de cree un curseur pour interagir sur la base de donnée, nom : {Name_Base_donnee}')
        elif Activ_3 is False:
            print(f'Impossible d exécuter la commande : {Name_commande} sur la base de donnée, nom : {Name_Base_donnee}')
            logging.debug(f'Impossible d exécuter la commande : {Name_commande} sur la base de donnée, nom : {Name_Base_donnee}')
        elif Activ_4 is False:
            print(f'Impossible de fermer le curseur sur la base de donnée, nom : {Name_Base_donnee}')
            logging.debug(f'Impossible de fermer le curseur sur la base de donnée, nom : {Name_Base_donnee}')
        elif Activ_5 is False:
            print(f'Impossible de commit sur la base de donnée, nom : {Name_Base_donnee}')
            logging.debug(f'Impossible de commit sur la base de donnée, nom : {Name_Base_donnee}')
        elif Activ_6 is False:
            print(f'Impossible de fermer la connection sur la base de donnée, nom : {Name_Base_donnee}')
            logging.debug(f'Impossible de fermer la connection sur la base de donnée, nom : {Name_Base_donnee}')
    else:
        print(f'Operations effectuées correctement sur la base de donnée, nom : {Name_Base_donnee}')
        logging.debug(f'Operations effectuées correctement sur la base de donnée, nom : {Name_Base_donnee}')

# Fonction pour transformer une décimale en binaire
def int2bin(integer, digits): # 1: valeur décimale / 2:  nb bit(s)
    """! Fonction qui transforme une décimale en binaire.
    @param integer Variable contenant la valeur décimale à transformer
    @param digits Variable qui définit sur combien de bit on veut que le resultat de la transformation soit faite
    @return bin Valeur transforme en binaire
    """
    if integer >= 0:
        return bin(integer)[2:].zfill(digits)
    else:
        return bin(2**digits + integer)[2:]

# Fonction pour une transformation binaire
def strToBinary(s):
    """! Fonction qui effectue une transformation binaire.
    @param s Chaine de caractère qu'on souhaite transformer
    @return Variable Resultat de la transformation (valeur binaire)
    """
    bin_conv = []

    for c in s:
        # Convertie chaque char en
        # valeur ASCCI
        ascii_val = ord(c)

        # Coversion de la valeur ASCCI en binaire
        binary_val = bin(ascii_val)
        bin_conv.append(binary_val[2:])

    return (' '.join(bin_conv))

# Fonctions pour verifier le CRC
# Fonction pour effectuer un 'ou' exclusif(xor)
def xor(a, b):
    """! Fonction qui effectue un ou exclusif (XOR).
    @param a Variable qui est une des composantes sur laquelle on veut effectuer un XOR
    @param b Variable qui est une des composantes sur laquelle on veut effectuer un XOR
    @return Variable Valeur modifier apres le XOR
    """
    # Initialisation du résultat
    result = []

    # Traverse tous les bits, si les bits sont
    # pareil, alors XOR vaut 0, sinon 1
    for i in range(1, len(b)):
        if a[i] == b[i]:
            result.append('0')
        else:
            result.append('1')

    return ''.join(result)

# Effectue la division Modulo-2
def mod2div(divident, divisor):
    """! Fonction qui Effectue la division Modulo-2.
    @param divident Une des composantes sur la quelle on va faire un Modulo-2 
    @param divisor Une des composantes sur la quelle on va faire un Modulo-2
    @return Variable Resultat du Modulo-2
    """
    # Nombre de bits ou on doit appliquer le XOR.
    pick = len(divisor)

    # Couper le diviseur pour s'approprier
    # longueur pour une étape particulière
    tmp = divident[0: pick]

    while pick < len(divident):

        if tmp[0] == '1':

            # Remplacer le diviseur par le résultat
            # de XOR et tirez 1 bit vers le bas
            tmp = xor(divisor, tmp) + divident[pick]

        else:  # Si le bit le plus à gauche est '0'

            # Si le bit le plus à gauche du dividende (ou le
            # partie utilisée dans chaque étape) est 0, l'étape ne peut pas
            # utilise le diviseur normal ; nous devons utiliser un
            # diviseur de tous les 0.
            tmp = xor('0' * pick, tmp) + divident[pick]

        # incrémenter pick pour aller plus loin
        pick += 1

    # Pour les n derniers bits, il faut le réaliser
    # normalement, car l'augmentation de la valeur de pick entraînera
    # Index hors limites.
    if tmp[0] == '1':
        tmp = xor(divisor, tmp)
    else:
        tmp = xor('0' * pick, tmp)

    checkword = tmp
    return checkword

# Fonction utilisée pour encoder les
# données en ajoutant le reste de la division modulaire
# à la fin des données.
def encodeData(data, key):
    """! Fonction qui sera utilisée pour encoder les données en ajoutant le reste de la division modulaire à la fin des données (CRC).
    @param data Donnée sur laquelle on souhaite effectuer l'opération
    @param key Clé pour le CRC définit au préalable
    @return remainder CRC
    """
    l_key = len(key)

    # Ajoute n-1 0 à la fin de la data
    appended_data = data + '0' * (l_key - 1)
    remainder = mod2div(appended_data, key)

    return remainder

# Client python
# Fonction de "callback" quand la connexion est établie
def on_connect(client, userdata, flags, rc):
    """! Fonction qu'on appelle lorsque le client reçoit une réponse lora du serveur.
    @param client Identité du client
    @param userdata Données définies par l'utilisateur de tout type qui sont transmises en tant que paramètre userdata aux rappels
    @param flag objet dictionnaire utilisé pour vérifier si vous avez défini une session propre sur True ou False pour l'objet client
    @param rc code de résultat , est utilisé pour vérifier l'état de la connexion
    @return  rien
    """
    print(f"Connexion réussies au broker (code={rc})")
    logging.debug(f"Connexion réussies au broker (code={rc})")

    # Abonnement aux messages du topic "general"
    """S'abonner à d'autres appareils:
    lora/identifiant_lora_appareil/up,
    Cet identifiant sera visible sur le serveur qui recoit les données"""
    client.subscribe("lora/70-b3-d5-9b-a0-00-65-9e/up",) # Vrai donnees
    #client.subscribe("lora/#") # donnees test

# Traiter données avec la gestion du flag pour l'enregistrement dans une BDD
def traitement_donnee(list):
    """! Fonction qui va Traiter les données avec la gestion du flag pour l'enregistrement dans une .
    @param list Liste contenant les valeurs à enregistrers
    @return rien
    """

    # Liste qui contiendra les donnée à enregistrer dans la BDD
    global list_data_bdd # id_course, id_bateau, latitude, longitude, cap, vitesse, vitesse_vent, direction_vent, date
    list_data_bdd[0] = list[1] # id_course
    list_data_bdd[1] = list[2] # id_bateau

    # Récupération du flag
    flag = list[0][::-1]

    # Parcours du flag pour enregistrer données en conséquence...
    # Principe de l'algorythme :
    """Je prend la première valeur de ma liste 'list' contenant les données que j'ai recu,
    puis je parcours mon flag et des que je tombe sur un '1', alors en connaissant l'ordre prédefinis de mon flag et l'ordre de mon tableau 'list_data_bdd',
    je peux donc savoir par correspondance que la première valeur recu correspond à tel champs."""
    compteur = 0
    for key in list[3:len(list)]: # champs obligatoire : flag, id_course, id_bateau
        for key_2 in flag[compteur:len(flag)]: # je commence à parcourir mon str à partir de 'l'itération + 1' ou je me suis arreter précédement...
            if key_2 == "1":
                if compteur == 0: # cap
                    list_data_bdd[4] = key
                    compteur += 1
                    break
                elif compteur == 1: # vitesse
                    list_data_bdd[5] = key
                    compteur += 1
                    break
                elif compteur == 2: # latitude
                    list_data_bdd[2] = key
                    compteur += 1
                    break
                elif compteur == 3: # longitude
                    list_data_bdd[3] = key
                    compteur += 1
                    break
                elif compteur == 4: # vitesse vent
                    list_data_bdd[6] = key
                    compteur += 1
                    break
                elif compteur == 5: # direction vent
                    list_data_bdd[7] = key
                    compteur += 1
                    break
            compteur += 1

# Decrypter le message et le traiter
def on_message_test(client, userdata, msg):
    """! Fonction qu'on appelle lorsqu'un message PUBLISH est reçu du serveur (Utilisé seulement pour les tests).
         Elle va extraire et decoder la donnée puis l'enregistrer dans une base de donnée.
    @param client Identité du client
    @param userdata Données définies par l'utilisateur de tout type qui sont transmises en tant que paramètre userdata aux rappels
    @param msg Contient la donnée
    @return rien
    """
    categorie = msg.topic
    message = msg.payload.decode("utf8")

    # Decode message base64 ->text et filtrage pour récupérer seulement la donnée
    string = message
    found = re.search('/(.+?)/', string).group(1)
    print(f"base64 = {found}")
    message_bytes = base64.b64decode(found)
    print(f"text = {message_bytes}")

    # Verifier CRC pour verifier si data n'est pas corrompu
    message_bytes_str = str(message_bytes)
    try:
        found_2 = re.search('@(.+?)!', message_bytes_str).group(1)  # Récupération des données utiles
        found_3 = re.search('!(.+?)~', message_bytes_str).group(1)  # Récupération du CRC
    except:
        print(f'Données corrompu(illisible)!')
    else:
        print(f'Données lisible en attente de verification du crc!')
        print(f'message = {found_2}')
        binary_data_Temp = strToBinary(found_2)  # Donnée avec espace entre chaque groupes de bits
        binary_data = binary_data_Temp.replace(" ", "")  # Espace supprimer entre chaque groupes de bits
        crc = encodeData(binary_data, "101010001")
        print(f"crc = {crc}")
        if crc == found_3:
            """Ajout BDD..."""
            print(f'Données lisible en cours d enregistrement dans la base de donnee(crc valide)...!')
            logging.debug(f'Données lisible et enregistre dans la base de donnee(crc valide)...!')

            # Séparation des données dans une liste pour le traitement
            List_donnee_ = found_2.split(";")
            List_donnee_.remove('')  # Supprimer l'élément vide

            # Transformation binaire pour traitement du flag
            List_donnee_[0] = str(int2bin(int(List_donnee_[0]), 6))

            # Ajout donnée pour traitement avec le flag...
            traitement_donnee(List_donnee_)

            # Liste qui contiendra les donnée à enregistrer dans la BDD
            global list_data_bdd  # id_course, id_bateau, latitude, longitude, cap, vitesse, vitesse_vent, direction_vent, date

            # Récupérer heure et date du moment
            curent_time = datetime.datetime.now()
            list_data_bdd[8] = curent_time

            # Ajout bdd
            Ajout_Base_Donnee(list_data_bdd, nom_bdd, "data_bateau") # data, nom bdd, nom table

        else:
            print(f'Données lisible mais corrompu(crc invalide)...!')
            logging.debug(f'Données lisible mais corrompu(crc invalide)...!')

# Fonction de "callback" quand un message est publié
def on_message(client, userdata, msg):
    """! Fonction qu'on appelle lorsqu'un message PUBLISH est reçu du serveur (Utilisé pour une utlisation réelle).
        Elle va extraire et decoder la donnée puis l'enregistrer dans une base de donnée.
    @param client Identité du client
    @param userdata Données définies par l'utilisateur de tout type qui sont transmises en tant que paramètre userdata aux rappels
    @param msg Contient la donnée
    @return rien
    """
    categorie = msg.topic
    message = msg.payload.decode("utf8")

    # Decode message base64 ->text et filtrage pour récupérer seulement la donnée
    string = message
    found = re.search('"data":"(.+?)"', string).group(1)
    print(f"base64 = {found}")
    logging.debug(f"base64 = {found}")
    message_bytes = base64.b64decode(found)
    print(f"text = {message_bytes}")
    logging.debug(f"text = {message_bytes}")

    # Verifier CRC pour verifier si data n'est pas corrompu
    message_bytes_str = str(message_bytes)
    try:
        found_2 = re.search('@(.+?)!', message_bytes_str).group(1) # Récupération des données utiles
        found_3 = re.search('!(.+?)~', message_bytes_str).group(1) # Récupération du CRC
    except:
        print(f'Données corrompu(illisible)!')
        logging.debug(f'Données corrompu(illisible)!')
    else:
        print(f'Données lisible en attente de verification du crc!')
        logging.debug(f'Données lisible en attente de verification du crc!')
        print(f'message = {found_2}')
        logging.debug(f'message = {found_2}')
        binary_data_Temp = strToBinary(found_2) # Donnée avec espace entre chaque groupes de bits
        binary_data = binary_data_Temp.replace(" ","") # Espace supprimer entre chaque groupes de bits
        crc = encodeData(binary_data, "101010001")
        print(f" crc = {crc}")
        logging.debug(f" crc = {crc}")
        if crc == found_3 :
            """Ajout BDD..."""
            print(f'Données lisible en cours d enregistrement dans la base de donnee(crc valide)...!')
            logging.debug(f'Données lisible et enregistre dans la base de donnee(crc valide)...!')

            # Séparation des données dans une liste pour le traitement
            List_donnee_ = found_2.split(";")
            List_donnee_.remove('')  # Supprimer l'élément vide

            # Transformation binaire pour traitement du flag
            List_donnee_[0] = str(int2bin(int(List_donnee_[0]), 6))

            # Ajout donnée pour traitement avec le flag...
            traitement_donnee(List_donnee_)

            # Liste qui contiendra les donnée à enregistrer dans la BDD
            global list_data_bdd  # id_course, id_bateau, latitude, longitude, cap, vitesse, vitesse_vent, direction_vent, date

            # Récupérer heure et date du moment
            curent_time = datetime.datetime.now()
            list_data_bdd[8] = curent_time

            # Ajout bdd
            Ajout_Base_Donnee(list_data_bdd, nom_bdd, "data_bateau")  # data, nom bdd, nom table
        else:
            print(f'Données lisible mais corrompu(crc invalide)...!')
            logging.debug(f'Données lisible mais corrompu(crc invalide)...!')

def recuperer_parametres(compteur = 1):
    """! Fonction qui va récupérer les paramètres du fichier de conf pour tenter de les exploiters.Si il y a le moindre problèmes, il prendra la config par default.
    @param compteur Variable qui possède une valeur par default et est utilisé pour faire de la recursion(Redemander à l'utlisateur de rentrer des valeurs correctes)
    @return rien
    """
    global ip
    global port
    global nom_bdd
    global chemin_fichier_conf

    try:
        if compteur == 1 :
            print(f'Récuperation configuation personnalise...')
            logging.debug(f'Récuperation configuation personnalise...')
            config = configparser.ConfigParser()
            config.sections()

            # lecture fichier
            config.read(chemin_fichier_conf)

            # Rentrer paramètre réseau (ip et port)
            param_reseau = config['RESEAU']
            ip = param_reseau.get('ip')
            port = param_reseau.get('port')

            # Rentrer paramètre bdd (nom)
            param_bdd = config['BDD']
            nom_bdd = param_bdd.get('nom_bdd')
        else:
            print(f'Récuperation configuation par default...')
            logging.debug(f'Récuperation configuation par default...')
            ip = "127.0.0.1"
            port = 1883
            nom_bdd = "bdd_embarquai.sqlite"

    except Exception as e: # Si les clés des champs sont vides alors on teste le default, si il n y a plus de champs, alors on prend ceux de defaults, si nom fichier incorrecte...
        print(f'erreur : {type(e).__name__}')
        logging.debug(f'erreur : {type(e).__name__}')
        if compteur == 1:
            print(f'Configuration personnalise invalide(voir fichier param.ini...)')
            logging.debug(f'Configuration personnalise invalide(voir fichier param.ini...)')
            recuperer_parametres(compteur - 1) # Rappel fonction pour tester les parametres par defaults...
        else:
            logging.debug("Le fichier de configuration par default n'est pas dans les normes!\nVerifier le contenu du fichier...\nProgramme fermé...")
            sys.exit("Le fichier de configuration par default n'est pas dans les normes!\nVerifier le contenu du fichier...")

    else:
        if ip == "" or ip is None or port == 0 or port == '' or port is None or nom_bdd is None or nom_bdd == "": # Verifier si champs sont vide
            print(f'Le champs ip, nom_bdd et ou le port sont vide(s)...\nVerifier le contenu du fichier')
            logging.debug(f'Le champs ip, nom_bdd et ou le port sont vide(s)...\nVerifier le contenu du fichier')
            if compteur == 1:
                print(f'Configuration personnalise invalide(voir fichier param.ini...)')
                logging.debug(f'Configuration personnalise invalide(voir fichier param.ini...)')
                recuperer_parametres(compteur - 1)  # Rappel fonction pour tester les parametres par defaults...
            else:
                logging.debug("Le fichier de configuration par default n'est pas dans les normes!\nVerifier le contenu du fichier...\nProgramme fermé...")
                sys.exit("Le fichier de configuration par default n'est pas dans les normes!\nVerifier le contenu du fichier...")
        else:
            print(f'Fichier de configuration dans les normes!')
            logging.debug(f'Fichier de configuration dans les normes!')

def reception_trame(client):
    """! Fonction qui va attendre qu'il y a un message de publier sur le serveur.
    @param client Récupère l'identifiant du client qui envoi un message sur le serveur
    @return rien
    """
    client.on_connect = on_connect

def reception_message(client):
    """! Fonction qui va receptionner le message dans le paquet das qu'il y aura une activitée.
    @param Identifiant client
    @return rien
    """
    client.on_message = on_message # vrai donnees
    # client.on_message = on_message_test # fausses donnees(test..)

# Recuperation argument pour travailler sur le fichier de configuration
def demande_argument_conf():
    """! Fonction qui va s'occuper de la Recuperation des arguments rentrer par l'utlisateur pour travailler sur le fichier de configuration.
    @param aucun
    @return rien
    """
    global chemin_fichier_conf
    chemin_fichier_conf = input(f'Rentrer le chemin complet du fichier de configuration\n')
    if len(chemin_fichier_conf) != 0 and chemin_fichier_conf.strip():
        print("Argument valide!")
        logging.debug("Argument valide!")
    else:
        demande_argument_conf()

# Création d'un client MQTT et attachement des fonctions de callback
def main():
    """! Fonction qui va lancer va créer un client mqtt et lancer le programme, Si jamais il ya la moindre erreurs au debut, il demandera à chaque fois de rentrer à nouveau le fichier de conf.
    @param aucun
    @return aucun
    """
    try:
        logging.debug("INITIALISATION LANCE(En attente de connexion au broker...)")
        print(f'INITIALISATION LANCE(En attente de connexion au brocker...)')

        # Recupération paramètre fichier de conf
        demande_argument_conf()
        recuperer_parametres()

        print("--------------Attention, apres s'etre connecte avec succes sur le serveur mosquitto, si il n'y a aucune nouvelles données arrivant sur le serveur Python:------------------\n-1/ Serveur mosquito déconnecte ou dysfonctionnelle\n-2/ Probleme reseau ou materiel au niveau du routeur et ou m5stack\n-----------------------------------------------------------------------------------------------------")
        logging.debug("--------------Attention, apres s'etre connecte avec succes sur le serveur mosquitto, si il n'y a aucune nouvelles données arrivant sur le serveur Python:------------------\n-1/ Serveur mosquito déconnecte ou dysfonctionnelle\n-2/ Probleme reseau ou materiel au niveau du routeur et ou m5stack\n------------------------")

        # Création client pour la connexion...
        client = mqtt.Client()

        # Créations des deux threads pour la réception et le traitement des données(Modèle 'producteur consommateur')..
        t1 = Thread(target=reception_trame, args=(client,))
        t2 = Thread(target=reception_message, args=(client,))

        # Lancements des deux threads pour la réception et le traitement des données(Modèle 'producteur consommateur')..
        t1.start()
        t2.start()

        # Valeur des différents paramètre utilisés pour la connexion
        print(f'ip : {ip} port : {port} nom_bdd : {nom_bdd} fichier : {chemin_fichier_conf}')
        logging.debug(f'ip : {ip} port : {port} nom_bdd : {nom_bdd} fichier : {chemin_fichier_conf}')

        # Connexion au broker
        client.connect(ip, int(port), 60) # add de mon pc ou est le serveur mosquitto qui recoit les paquets lora

        # Ecoute permanente des messages
        client.loop_forever()
        client.disconnect()

    except timeout as time: # Trop de temps ecoule sans aucune action(valeurs champs reseau incorrecte...)
        print(
            "@Connexion impossible!@\nVerifier les différents paramètres dans le fichier(param.ini)...")
        logging.debug(
            "@Connexion impossible!@\nVerifier les différents paramètres dans le fichier(param.ini)...")
        main()
        sys.exit(0)  # Fin programme a cause de mauvais parametre de base

    except ValueError as init: # si les types dans les arguments sont mauvais
        print("@Lecture du fichier de configuration impossible!@\nVerifier les différents paramètres dans le fichier(param.ini)...")
        logging.debug("@Lecture du fichier de configuration impossible!@\nVerifier les différents paramètres dans le fichier(param.ini)...")
        main()
        sys.exit(-1)  # Fin programme a cause de mauvais parametre de base

    except ConnectionRefusedError as refused: # type bon mais pas les bonnes valeurs
        print("@Connexion impossible!@\nVerifier les différents paramètres dans le fichier(param.ini)...")
        logging.debug("@Connexion impossible!@\nVerifier les différents paramètres dans le fichier(param.ini)...")
        main()
        sys.exit(-2)  # Fin programme a cause de mauvais parametre de base

    except gaierror: # Si les valeur données au niveaux des valeurs par defaults sont incorrectes...
        print(
            "@Connexion impossible!@\nVerifier les différents paramètres dans le fichier(param.ini)...")
        logging.debug(
            "@Connexion impossible!@\nVerifier les différents paramètres dans le fichier(param.ini)...")
        main()
        sys.exit(-3)  # Fin programme a cause de mauvais parametre de

    except KeyboardInterrupt as stop_pg: # Terminer programme normalement
        print("@Connexion termine!@")
        logging.debug("@Connexion termine!@")
        sys.exit(-4) # Fin programme en cas d arret volantaire(controle c...)

    except Exception as General: # erreur non prise en compte
        print(f'type = {type(General).__name__}')
        print("@Une erreur inatendu s'est produite!@")
        logging.debug(f'type = {type(General).__name__}')
        logging.debug("@Une erreur inatendu s'est produite!@")
        sys.exit(-5)  # Fin programme a cause de mauvais parametre de base

# Programme principale
log_init()
logging.debug("|||||||||||||||||||Nouvelles valeurs de log ci dessous|||||||||||||||||||||||")
main()