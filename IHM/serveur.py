"""
Projet EMBARQUAI
Script gérant le serveur distribuant les données des bateaux en JSON
"""

# Libraries
from flask import Flask, Response, render_template, jsonify
import sqlite3 


# Création de l'application web et lecture du fichier de configuration
app = Flask(__name__)
app.config.from_object("config.DevelopmentConfig")

# Les différentes routes de l'application web
@app.route("/")
def accueillir():
    """Gère l'affichage de la page d'accueil"""
    return render_template("index.html")

@app.route("/carte/")
def afficher_carte():
    """Gère l'affichage de la carte"""
    # Préparation des paramètres depuis le fichier de configuration
    parametres = {}
    parametres["CARTE_LATITUDE"] = app.config["CARTE_LATITUDE"]
    parametres["CARTE_LONGITUDE"] = app.config["CARTE_LONGITUDE"]
    parametres["CARTE_ZOOM"] = app.config["CARTE_ZOOM"]
    parametres["COURSE_DEPART_LATITUDE"] = app.config["COURSE_DEPART_LATITUDE"]
    parametres["COURSE_DEPART_LONGITUDE"] = app.config["COURSE_DEPART_LONGITUDE"]
    return render_template("carte.html", parametres=parametres)

@app.route("/bateaux/")
def recuperer_bateaux():
    """Récupère les bateaux particiapnts à la course en JSON pour jQuery/Leaflet"""
    # Connexion à la base de données et création d'un curseur
    connexion = sqlite3.connect( app.config["CHEMIN_BDD"] )
    curseur = connexion.cursor()

    # Préparation et exécution de la requête SQL
    resultat = curseur.execute("""
        SELECT DISTINCT Bateau.id, Bateau.nom
        FROM Bateau
        INNER JOIN Position ON Bateau.id = Position.id_bateau_fk
        WHERE Position.id_course_fk = ?
        ORDER BY Bateau.id;""",
        (app.config["COURSE_ID"], )
    )
    
    # Mise en forme des résultats (version 2)
    bateaux = []
    for ligne in resultat:        
        print(ligne)
        bateaux.append( {"id": ligne[0], "nom": ligne[1]} )
    print(bateaux)

    # Déconnexion de la base de données
    connexion.close()

    # Conversion en JSON
    #return Response( jsonify(bateaux), mimetype="application/json") 
    return jsonify(bateaux)

    
@app.route("/positions/")
def recuperer_positions():
    """Récupère les positions des bateaux en JSON pour jQuery/Leaflet"""
    # Connexion à la base de données et création d'un curseur
    connexion = sqlite3.connect( app.config["CHEMIN_BDD"] )
    curseur = connexion.cursor()

    # Préparation et exécution de la requête SQL
    resultat = curseur.execute("""
        SELECT Position.latitude, Position.longitude, Position.date, Bateau.id, Bateau.nom
        FROM Position
        INNER JOIN Bateau ON Position.id_bateau_fk = Bateau.id
        WHERE Position.id_course_fk = ?
        ORDER BY Position.date;""",
        (app.config["COURSE_ID"], )
    )

    # Mise en forme des résultats (version 1)
    """
    bateaux = {}
    for ligne in resultat:        
        print(ligne)
        #print(f"latitude={ligne[1]}, longitude={ligne[2]}")
        if ligne[4] not in bateaux.keys():
            bateaux[ligne[4]] = []
        bateaux[ligne[4]].append( (ligne[1], ligne[2]) )
    print(bateaux)
    """
    # Mise en forme des résultats (version 2)
    bateaux = {}
    for ligne in resultat:        
        print(ligne)
        #print(f"latitude={ligne[1]}, longitude={ligne[2]}")
        if ligne[4] not in bateaux.keys():
            bateaux[ligne[4]] = {}
            bateaux[ligne[4]]["id"] = ligne[3]
            bateaux[ligne[4]]["nom"] = ligne[4]
            bateaux[ligne[4]]["positions"] = []
            
        bateaux[ligne[4]]["positions"].append( (ligne[0], ligne[1]) )
    print(bateaux)


    # Déconnexion de la base de données
    connexion.close()

    # Conversion en JSON
    #return Response( jsonify(bateaux), mimetype="application/json") 
    return jsonify(bateaux)


# Lancement de l'application web Flask
if __name__ == "__main__":
    app.run(host='0.0.0.0', port=1664, debug=True)

