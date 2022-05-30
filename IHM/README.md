# Projet EMBARQUAI

## 1 - Introduction
Le projet **EMBARQUAI** consiste à suivre en temps réel l'évolution d'une
course de bateaux.

Ce sous-ensemble récupère les données des bateaux dans une base de données,
et les affiche sur une carte


## 2 - Outils utilisés
Liste des outils pour la partie backend :
- [Flask](https://flask.palletsprojects.com/en/2.1.x/)
- [SQLite](https://www.sqlite.org/index.html)

Liste des outils pour la partie frontend :
- [Leaflet](https://leafletjs.com/)
- [jQuery](https://jquery.com/)


## 3 - Fonctionnement
### 3.1 - Manuel utilisateur
Configuration
```
nano config.py
```

Lancement de l'application web Flask
```python
python3 ./serveur.py
```

Ouverture dans un navigateur de l'URL
```
http://localhost:1664/carte
```

### 3.2 - ALgorithme
Les grandes étapes du fonctionnement de l'application web
1. préparation et téléchargement de la carte à partir des paramètres de configuration

2. récupération du nombre de bateaux participants au départ de la course (il faut qu'il y ait au moins une ligne dans la table Position par bateau)

3. en continu les positions sont récupérées depuis la bdd et utilisées pour mettre à jour les positions des bateaux sur la carte


## 4 - Ce qu'il reste à faire
Liste de ce qu'il reste à faire :
- [ ] vérifier que le code est compatible avec la bdd existante
- [ ] téléverser le code sur le github du projet
- [ ] créer un token d'accès à l'API mapbox
- [ ] remplacer le bouton par un timer