#ifndef FONCTION_SEND_LORA_H 
#define FONCTION_SEND_LORA_H 

#include <M5Core2.h>
#include <algorithm>
#include "data_navi.hpp"

namespace lora{

    /**
     * @brief Fonction qui va inverser une chaine de caractère
     * @param flag Chaine de caractère contenant la chaine à inverser
     * @return String : Chaine de caractère inversé
     */
    String Reversed_data(String flag);

    /**
    * @brief Fonction qui va remplir une chaine de caractère contenant les infos mise à jour en fonction de la valeur du flag.
    * @param data structure qui contiend les différents champs qui contiennent les données des capteurs
    * @return String, Chaine de caractère contenant la data (flag + donnée(s) capteur(s)) 
    */
    String Traitement_flag_data(const navi::data_navi& data);

    /**
    * @brief Fonction qui va envoyer une chaine de caractère sur un module lorawan.
    * @param data donnee structure qui contiendra les différents champs qui contiendront les données des capteurs
    * @return void 
    */
    void Send_Message_Lorawan(const navi::data_navi& data);

    /**
    * @brief Fonction qui va initialiser le port serial 2 pour faire fonctionner le module lorawan
    * @param none
    * @return void
    */
    void init();

    /**
     * @brief Cette fonction permet d'ajouter des bits sur une chaine de caractère
     * @param limite 
     * @return String 
     */
    String add_bit(int limite);

    /**
     * @brief Generation d'un string contenant le message combinant une clé pour donner une valeur d'identification en cas de perte de données 
     * @param key 
     * @param data 
     * @return String 
     */
    String generation_CRC(const String& data, const String& key);

    /**
     * @brief Conversion d'une chaine de caractère en binaire 
     * @return String 
     */
    String conversion_binaire(const String& data);

    /**
     * @brief Fonction qui va faire un xor(ou exclusif) entre deux chaine de carectere
     * @param a 
     * @param b 
     * @return String 
     */
    String xor1(const String& a, const String& b);

    /**
     * @brief Fonction qui va diviser les données binaires par la clé et stocker le reste de la division
     * @param divident 
     * @param divisor 
     */
    String mod2div(const String& divident, const String& divisor);

    extern bool is_init;
}

#endif