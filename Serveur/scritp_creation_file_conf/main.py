import configparser
config = configparser.ConfigParser()
config['RESEAU'] = {'port': 'Remplacer champ(uniquement chiffres ou nombres)',
                     'ip': 'Remplacer champ(uniquement chiffres ou nombres)'}
config['BDD'] = {'nom_bdd': 'Remplacer champ(uniquement chiffres ou nombres)'}
with open(r"param.ini", 'w') as configfile: # Creation fichier et si il existe deja, supprimmer et creation nouveau fichier
  config.write(configfile)
