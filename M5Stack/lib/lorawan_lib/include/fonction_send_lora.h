#ifndef FONCTION_SEND_LORA_H 
#define FONCTION_SEND_LORA_H 

#include <M5Core2.h>
#include <algorithm>

namespace lora{

    /*@brief une structure qui stoque les données des capteurs d'un bateau */
    struct data_bateau{

        String data = "10.0";
        String data_1 = "15.0";
        String data_2 = "20.5";
    };

    /**
    * @brief Fonction qui va envoyer une chaine de caractère sur un module lorawan.
    * @param data donnee structure ui contiendra les différents champs qui contiendront les données des capteurs
    * @return void 
    */
    void Send_Message_Lorawan(const data_bateau& data);

    /**
    * @brief Fonction qui va initialiser le port serial 2 pour faire fonctionner le module lorawan
    * @param none
    * @return void
    */
    void init();

    /**
     * @brief Generation d'un string contenant le message combinant une clé pour donner une valeur d'identification en cas de perte de données 
     * @param key 
     * @param data 
     * @return std::string 
     */
    String generation_CRC(const String& data, const String& key);

    /**
     * @brief Conversion d'une chaine de caractère en binaire 
     * @return std::string 
     */
    String conversion_binaire(const String& data);

    /**
     * @brief Fonction qui va faire un xor(ou exclusif) entre deux chaine de carectere
     * @param a 
     * @param b 
     * @return std::string 
     */
    String xor1(const String& a, const String& b);

    /**
     * @brief Fonction qui va diviser les données binaires par la clé et stocker le reste de la division
     * @param divident 
     * @param divisor 
     */
    String mod2div(const String& divident, const String& divisor);
}

#endif