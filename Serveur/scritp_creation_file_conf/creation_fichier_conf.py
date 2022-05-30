"""Programme qui va servir à générer un fichier de configuration avec le champ : ip, port, nom_bdd.
Usage:
======
    python creation_fichier_conf.py
"""

__authors__ = ("Loris Benaitier")
__copyright__ = "MIT"
__date__ = "2022-05-01"
__version__= "1.0"

# Bibliotèque
import configparser

config = configparser.ConfigParser()
config['RESEAU'] = {'port': 'Remplacer champ(uniquement chiffres ou nombres)',
                     'ip': 'Remplacer champ(uniquement chiffres ou nombres)'}
config['BDD'] = {'nom_bdd': 'Remplacer champ(uniquement chiffres ou nombres)'}
with open(r"param.ini", 'w') as configfile: # Creation fichier et si il existe deja, supprimmer et creation nouveau fichier
  config.write(configfile)
