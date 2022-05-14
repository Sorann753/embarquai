# Librairies utilisées
from socket import timeout, gaierror

import paho.mqtt.client as mqtt
import re
import base64
import sqlite3
import configparser
import sys
import logging
from time import sleep, perf_counter
from threading import Thread

ip = ""
port = 0
nom_bdd = ""
chemin_fichier_conf = ""
chemin_fichier_log = ""
verif_message = False # test car pas use
client = 0 # test car pas use

def log_init():
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
    sql = Name_commande + " " + Name_table + " (id_course, id_bateau, latitude, longitude, cap, vitesse, vitesse_vent, direction_vent) VALUES (?, ?, ?, ?, ?, ?, ?, ?)"
    val = list_data

    # Connexion base de donnée et fermeture quand la commande a été effectué...
    Name_Base_donnee = Nom_bdd
    try:
        connection = sqlite3.connect(Name_Base_donnee)
        Activ_1 = True
        print(f'ok_1')
        cursor = connection.cursor()
        Activ_2 = True
        print(f'ok_2')
        cursor.execute(sql, val)
        Activ_3 = True
        print(f'ok_3')
        cursor.close()
        Activ_4 = True
        print(f'ok_4')
        connection.commit()
        Activ_5 = True
        print(f'ok_5')
        connection.close()
        Activ_6 = True
        print(f'ok_6')
    except Exception as e:
        print(str(e))
        if Activ_1 is False:
            print(f'Impossible de se connecter à la base de donnée, nom : {Name_Base_donnee}')
        elif Activ_2 is False:
            print(f'Impossible de cree un curseur pour interagir sur la base de donnée, nom : {Name_Base_donnee}')
        elif Activ_3 is False:
            print(f'Impossible d exécuter la commande : {Name_commande} sur la base de donnée, nom : {Name_Base_donnee}')
        elif Activ_4 is False:
            print(f'Impossible de fermer le curseur sur la base de donnée, nom : {Name_Base_donnee}')
        elif Activ_5 is False:
            print(f'Impossible de commit sur la base de donnée, nom : {Name_Base_donnee}')
        elif Activ_6 is False:
            print(f'Impossible de fermer la connection sur la base de donnée, nom : {Name_Base_donnee}')
    else:
        print(f'Operations effectuées correctement sur la base de donnée, nom : {Name_Base_donnee}')

# Fonction pour une transformation binaire
#---------------------------------------
def strToBinary(s):
    bin_conv = []

    for c in s:
        # convert each char to
        # ASCII value
        ascii_val = ord(c)

        # Convert ASCII value to binary
        binary_val = bin(ascii_val)
        bin_conv.append(binary_val[2:])

    return (' '.join(bin_conv))
#---------------------------------------

# Fonctions pour verifier le CRC
#--------------------------------------------------------
def xor(a, b):
    # initialize result
    result = []

    # Traverse all bits, if bits are
    # same, then XOR is 0, else 1
    for i in range(1, len(b)):
        if a[i] == b[i]:
            result.append('0')
        else:
            result.append('1')

    return ''.join(result)

# Performs Modulo-2 division
def mod2div(divident, divisor):
    # Number of bits to be XORed at a time.
    pick = len(divisor)

    # Slicing the divident to appropriate
    # length for particular step
    tmp = divident[0: pick]

    while pick < len(divident):

        if tmp[0] == '1':

            # replace the divident by the result
            # of XOR and pull 1 bit down
            tmp = xor(divisor, tmp) + divident[pick]

        else:  # If leftmost bit is '0'

            # If the leftmost bit of the dividend (or the
            # part used in each step) is 0, the step cannot
            # use the regular divisor; we need to use an
            # all-0s divisor.
            tmp = xor('0' * pick, tmp) + divident[pick]

        # increment pick to move further
        pick += 1

    # For the last n bits, we have to carry it out
    # normally as increased value of pick will cause
    # Index Out of Bounds.
    if tmp[0] == '1':
        tmp = xor(divisor, tmp)
    else:
        tmp = xor('0' * pick, tmp)

    checkword = tmp
    return checkword

# Function used at the sender side to encode
# data by appending remainder of modular division
# at the end of data.
def encodeData(data, key):
    l_key = len(key)

    # Appends n-1 zeroes at end of data
    appended_data = data + '0' * (l_key - 1)
    remainder = mod2div(appended_data, key)

    return remainder
#----------------------------------------------------------------

# Client python
# Fonction de "callback" quand la connexion est établie
def on_connect(client, userdata, flags, rc):
    print(f"Connexion réussies au broker (code={rc})")

    # Abonnement aux messages du topic "general"
    #client.subscribe("lora/70-b3-d5-9b-a0-00-65-9e/up",) # Vrai donnees
    client.subscribe("lora/#") # donnees test
    #client.subscribe("lora/00-00-00-00-00-00-00-00/70-b3-d5-7e-d0-04-de-9e/up")

def on_message_test(client, userdata, msg):
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
        print(f" crc = {crc}")
        if crc == found_3:
            """Ajout BDD..."""
            print(f'Données lisible et enregistre dans la base de donnee(crc valide)...!')
            list_data = (1, 1, 10.0, 20.0, 15.0, 16.5, 17.5, 18.5)
            Ajout_Base_Donnee(list_data, "bdd_embarquai.sqlite", "data_bateau")
        else:
            print(f'Données lisible mais corrompu(crc invalide)...!')


# Fonction de "callback" quand un message est publié
def on_message(client, userdata, msg):
    categorie = msg.topic
    message = msg.payload.decode("utf8")

    # Decode message base64 ->text et filtrage pour récupérer seulement la donnée
    string = message
    found = re.search('"data":"(.+?)"', string).group(1)
    print(f"base64 = {found}")
    message_bytes = base64.b64decode(found)
    print(f"text = {message_bytes}")

    # Verifier CRC pour verifier si data n'est pas corrompu
    message_bytes_str = str(message_bytes)
    try:
        found_2 = re.search('@(.+?)!', message_bytes_str).group(1) # Récupération des données utiles
        found_3 = re.search('!(.+?)~', message_bytes_str).group(1) # Récupération du CRC
    except:
        print(f'Données corrompu(illisible)!')
    else:
        print(f'Données lisible en attente de verification du crc!')
        print(f'message = {found_2}')
        binary_data_Temp = strToBinary(found_2) # Donnée avec espace entre chaque groupes de bits
        binary_data = binary_data_Temp.replace(" ","") # Espace supprimer entre chaque groupes de bits
        crc = encodeData(binary_data, "101010001")
        print(f" crc = {crc}")
        if crc == found_3 :
            """Ajout BDD..."""
            print(f'Données lisible et enregistre dans la base de donnee(crc valide)...!')
            list_data = (1, 1, 10.0, 20.0, 15.0, 16.5, 17.5, 18.5)
            Ajout_Base_Donnee(list_data, "bdd_embarquai.sqlite", "data_bateau")
        else:
            print(f'Données lisible mais corrompu(crc invalide)...!')

def recuperer_parametres(compteur = 1):
    global ip
    global port
    global nom_bdd
    global chemin_fichier_conf
    read_file = []

    try:
        if compteur == 1 :
            print(f'Récuperation configuation personnalise...')
            config = configparser.ConfigParser()
            config.sections()

            # lecture fichier
            read_file = config.read(chemin_fichier_conf)

            # Rentrer paramètre réseau (ip et port)
            param_reseau = config['RESEAU']
            ip = param_reseau.get('ip')
            port = param_reseau.get('port')

            # Rentrer paramètre bdd (nom)
            param_bdd = config['BDD']
            nom_bdd = param_bdd.get('nom_bdd')
        else:
            print(f'Récuperation configuation par default...')
            config = configparser.ConfigParser()
            config.sections()

            # lecture fichier
            config.read(chemin_fichier_conf)

            # Rentrer paramètre réseau (ip et port)
            param_def = config['DEFAULT']
            ip = param_def.get('ip')
            port = param_def.get('port')
            nom_bdd = param_def.get('nom_bdd')

    except KeyError as name_file: # Si on ne peut plus acceder a une des section(reseau...),on tente le default
        if compteur == 1:
            print(f'Configuration personnalise invalide(voir fichier param.ini...)')
            recuperer_parametres(compteur - 1) # Rappel fonction pour tester les parametres par defaults...
        else:
            sys.exit("Impossible d'acceder au fichier de configuration!")

    except Exception as e: # Si les clés des champs sont vides alors on teste le default, si il n y a plus de champs, alors on prend ceux de defaults
        print(f'erreur : {type(e).__name__}')
        if compteur == 1:
            print(f'Configuration personnalise invalide(voir fichier param.ini...)')
            recuperer_parametres(compteur - 1) # Rappel fonction pour tester les parametres par defaults...
        else:
            sys.exit("Le fichier de configuration par default n'est pas dans les normes!\nVerifier le contenu du fichier...")

    else:
        if ip == "" or ip is None or port == 0 or port == '' or port is None or nom_bdd is None or nom_bdd == "": # Verifier si champs sont vide
            print(f'Le champs ip et ou le port sont vide(s)...\nVerifier le contenu du fichier')
            if compteur == 1:
                print(f'Configuration personnalise invalide(voir fichier param.ini...)')
                recuperer_parametres(compteur - 1)  # Rappel fonction pour tester les parametres par defaults...
            else:
                sys.exit("Le fichier de configuration par default n'est pas dans les normes!\nVerifier le contenu du fichier...")
        else:
            print(f'Fichier de configuration dans les normes!')

def reception_trame(client):
    client.on_connect = on_connect

def reception_message(client):
    #client.on_message = on_message # vrai donnees
    client.on_message = on_message_test # fausse donnees(test..)

# Recuperation argument pour travailler sur le fichier de configuration
def demande_argument_conf():
    global chemin_fichier_conf
    chemin_fichier_conf = input(f'Rentrer le chemin complet du fichier de configuration\n')
    if len(chemin_fichier_conf) != 0 and chemin_fichier_conf.strip():
        print("Argument valide!")
        logging.debug("Argument valide!")
    else:
        demande_argument_conf()

# Création d'un client MQTT et attachement des fonctions de callback
def main():
    try:
        log_init()
        logging.debug("|||||||||||||||||||Nouvelles valeurs de log ci dessous|||||||||||||||||||||||")
        demande_argument_conf()
        recuperer_parametres()
        print("--------------Attention, apres s'etre connecte avec succes sur le serveur mosquitto, si il n'y a aucune nouvelles données arrivant sur le serveur Python:------------------\n-1/ Serveur mosquito déconnecte ou dysfonctionnelle\n-2/ Probleme reseau ou materiel au niveau du routeur et ou m5stack\n-----------------------------------------------------------------------------------------------------")
        logging.debug("--------------Attention, apres s'etre connecte avec succes sur le serveur mosquitto, si il n'y a aucune nouvelles données arrivant sur le serveur Python:------------------\n-1/ Serveur mosquito déconnecte ou dysfonctionnelle\n-2/ Probleme reseau ou materiel au niveau du routeur et ou m5stack\n------------------------")
        client = mqtt.Client()

        t1 = Thread(target=reception_trame, args=(client,))
        t2 = Thread(target=reception_message, args=(client,))

        t1.start()
        t2.start()

        # Connexion au broker
        print(f'ip : {ip} port : {port} fichier : {chemin_fichier_conf}')
        client.connect(ip, int(port), 60) # add de mon pc ou est le serveur mosquitto qui recoit les paquets lora

        # Ecoute permanente des messages
        client.loop_forever()
        client.disconnect()
    except timeout as time:
        print(
            "------------Connexion impossible!------------\nVerifier les différents paramètres dans le fichier(param.ini)...")
        logging.debug(
            "------------Connexion impossible!------------\nVerifier les différents paramètres dans le fichier(param.ini)...")
        main()
        sys.exit(0)  # Fin programme a cause de mauvais parametre de base

    except ValueError as init: # si le type dans les arguments sont mauvais
        print("------------Lecture du fichier de configuration impossible!------------\nVerifier les différents paramètres dans le fichier(param.ini)...")
        logging.debug("------------Lecture du fichier de configuration impossible!------------\nVerifier les différents paramètres dans le fichier(param.ini)...")
        main()
        sys.exit(-1)  # Fin programme a cause de mauvais parametre de base

    except ConnectionRefusedError as refused: # type bon mais pas les bonne valeurs
        print("------------Connexion impossible!------------\nVerifier les différents paramètres dans le fichier(param.ini)...")
        logging.debug("------------Connexion impossible!------------\nVerifier les différents paramètres dans le fichier(param.ini)...")
        main()
        sys.exit(-2)  # Fin programme a cause de mauvais parametre de base
    except gaierror: # Si les valeur données au niveau de l'ip(default) sont incorrecte...
        print(
            "------------Connexion impossible!------------\nVerifier les différents paramètres dans le fichier(param.ini)...")
        logging.debug(
            "------------Connexion impossible!------------\nVerifier les différents paramètres dans le fichier(param.ini)...")
        main()
        sys.exit(-3)  # Fin programme a cause de mauvais parametre de

    except KeyboardInterrupt as stop_pg: # Terminer programme normalement
        print("------------Connexion termine!------------")
        logging.debug("------------Connexion termine!------------")
        sys.exit(-4) # Fin programme en cas d arret volantaire(controle c...)

    except Exception as General: # erreur non prise en compte
        print(f'type = {type(General).__name__}')
        print("------------Une erreur inatendu s'est produite!------------")
        logging.debug("------------Une erreur inatendu s'est produite!------------")
        sys.exit(-5)  # Fin programme a cause de mauvais parametre de base

# Programme principale
main()