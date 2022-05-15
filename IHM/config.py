"""
@brief Fichier de configuration de l'application
@author David SALLE
@date 29/03/2017
@version 0.1
@licence GPL3
"""


class Config(object):
    """Classe pour centraliser différents paramètres de configuration
    """

    # Position de départ et niveau de zoom de la carte
    CARTE_LATITUDE = 46.321788
    CARTE_LONGITUDE = -0.466915
    CARTE_ZOOM = 13

    # Chemin vers la base de données
    CHEMIN_BDD = "./bdd/embarquai.sqlite"

    # Id de la course à afficher
    COURSE_ID = 1

    # Position du départ de la course
    COURSE_DEPART_LATITUDE = 46.32
    COURSE_DEPART_LONGITUDE = -0.46



class DevelopmentConfig(Config):
    """Classe pour centraliser différents paramètres de configuration
    lors de la phase de développement
    """
    pass
