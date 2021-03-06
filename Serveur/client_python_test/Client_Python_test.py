"""Client de test pour se connecter en mqtt et publier des messages.

Usage:
======
    python Client_Python_test.py 

"""

__authors__ = ("Loris Benaitier")
__copyright__ = "MIT"
__date__ = "2022-05-01"
__version__= "1.0"

# Bibliotèques
import time
import paho.mqtt.client as mqtt

# Le rappel lorsque le client reçoit une réponse lora du serveur.
def on_connect(client, userdata, flags, rc):
    """! Fonction qu'on appelle lorsque le client reçoit une réponse lora du serveur
    @param client Identité du client
    @param userdata Données définies par l'utilisateur de tout type qui sont transmises en tant que paramètre userdata aux rappels
    @param flag objet dictionnaire utilisé pour vérifier si vous avez défini une session propre sur True ou False pour l'objet client
    @param rc code de résultat , est utilisé pour vérifier l'état de la connexion

    @return  rien
    """
    print("Connected with result code " + str(rc))
    # S'abonner à on_connect() signifie que si nous perdons la connexion et
    # se reconnecter puis les abonnements seront renouvelés.
    client.subscribe("lora/#")

# Le rappel lorsqu'un message PUBLISH est reçu du serveur.
def on_message(client, userdata, msg):
    """! Fonction qu'on appelle lorsqu'un message PUBLISH est reçu du serveur
    @param client Identité du client
    @param userdata Données définies par l'utilisateur de tout type qui sont transmises en tant que paramètre userdata aux rappels
    @param msg Contient la donnée

    @return rien
    """
    topic = str(msg.topic)
    message = str(msg.payload.decode("utf-8"))
    print(topic + " " + message)


# Appel pour voir si il y a des messages publié ou non
client = mqtt.Client()

# Rentrer l'IP et le port ou le client doit ecouter
client.connect("192.168.1.156", 1883, 60)

# Appel bloquant qui traite le trafic réseau, distribue les rappels et
# gère la reconnexion.
# D'autres fonctions loop*() sont disponibles qui donnent une interface threadée et un
# interface manuelle.
"""Boucle qui envoi à l'infini la data suivante avec un delai de 10 seconde"""
while True:
    your_data = "/YidAMTAuMDsxNS4wOzIwLjUhMTAxMDEwMDB+Jw==/up"
    client.publish("lora/",your_data,)
    time.sleep(10)
client.loop_forever()     # (le programme ne continuera pas au-delà d'ici).

