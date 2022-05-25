#include "fonction_send_lora.h"

namespace lora{
    bool is_init = false;
}

/**
 * @brief Fonction qui va inverser une chaine de caractère
 * @param flag Chaine de caractère contenant la chaine à inverser
 * @return String : Chaine de caractère inversé
 */
String lora::Reversed_data(String flag)
{
    String Data_reversed {""};
    for (int i = flag.length() - 1; i >= 0; i--)
    {
        Data_reversed += flag[i];
    }
    return Data_reversed; 
}

/**
* @brief Fonction qui va remplir une chaine de caractère contenant les infos mise à jour en fonction de la valeur du flag.
* @param data structure qui contiend les différents champs qui contiennent les données des capteurs
* @return String, Chaine de caractère contenant la data (flag + donnée(s) capteur(s)) 
*/
String lora::Traitement_flag_data(const navi::data_navi& data)
{
    // Variable locale
    String message{""};
    String flag{""};
    
    //Traitement avec un 'ET logique' pour voir si il y a un bit à 1 ou pas sur chaque bit du flag
    if(data.data_content & navi::data_navi_content::HEADING)
    {
        flag += "1";
        message += String(data.heading, 2) + ";";
    }
    else{flag += "0";}

    if(data.data_content & navi::data_navi_content::SPEED)
    {
        flag += "1";
        message += String(data.speed, 2) + ";";
    }
    else{flag += "0";}

    if(data.data_content & navi::data_navi_content::LATITUDE)
    {
        flag += "1";
        message += String(data.latitude, 5) + ";";
    }
    else{flag += "0";}

    if(data.data_content & navi::data_navi_content::LONGITUDE)
    {
        flag += "1";
        message += String(data.longitude, 5) + ";";
    }
    else{flag += "0";}

    if(data.data_content & navi::data_navi_content::WINDSPEED)
    {
        flag += "1";
        message += String(data.WindSpeed, 2) + ";";
    }
    else{flag += "0";}

    if(data.data_content & navi::data_navi_content::WINDANGLE)
    {
        flag += "1";
        message += String(data.WindAngle, 2) + ";";
    }
    else{flag += "0";}


    // Formation du flag au complet
    flag += "00B";
    flag = Reversed_data(flag);

    //Formation de la data
    String config = String(data.id_course, 10) + ";" + String(data.id_bateau, 10);
    String trame = flag + ";" + config + ";" + message;

    return trame;
}

/**
* Fonction qui va envoyer une chaine de caractère sur un module lorawan.
* @param struct donnee structure ui contiendra les différents champs qui contiendront les données des capteurs
* @return void 
*/
void lora::Send_Message_Lorawan(const navi::data_navi& data)
{
    //si le serial2 n'a pas été initialisé on ne fait rien
    if(!is_init) return;

    String message{""};
    String Borne{""}; 
    String key = "101010001"; // Choisir cle en binaire

    // Concatenation des differentes valeurs du capteurs
    Borne += "b'@"; // Debut message(obligatoire pour l envoie)
    message = Traitement_flag_data(data);
    //  message += data.data + ";";
    //  message += data.data_1 + ";";
    //  message += data.data_2; // Ne pas mettre de caractere de separation pour la dernière data
    // message += data.param4 + "/";
    // message += data.param5 + "/";
    // message += data.param6 + "/";
    
    //Convertir le message en binaire
    String value_bin = conversion_binaire(message);

    //Generation CRC
    String crc_bin = generation_CRC(value_bin, key);

    //Concatener le crc à la fin du message
    Borne += message + "!";
    Borne += crc_bin + "~'"; // Fin message
    
    Serial.println(Borne); // DEBUG
    Serial2.println(Borne); // Trame message -> Serial2.println("b'&Loris;Loris&'");
    delay(2000);
}

/**
* @brief Fonction qui va initialiser le port serial 2 pour faire fonctionner le module lorawan
* @param none
* @return void
*/
void lora::init()
{
    //si le Serial2 a déjà été initialiser sa sert a rien de le refaire
    if(is_init) return;
    
    Serial2.begin(19200, SERIAL_8N1, 13, 14);//RX13 et tx14
    Serial2.flush();
    delay(100);
    is_init = true;
}

/**
 * @brief Cette fonction permet d'ajouter des bits sur une chaine de caractère
 * 
 * @param limite 
 * @return String 
 */
String lora::add_bit(int limite)
{
    String variable = "";
    for(auto i = 0; i < limite; i++){variable += '0';}

    return variable;
}

/**
 * @brief Conversion d'une chaine de caractère en binaire 
 * @return String 
 */
String lora::conversion_binaire(const String& data)
{
    int N = data.length();
    String bin = "";

    for(auto i = 0; i <= N; i++)
    {
        //Convertir chaque char en ASCII
        int val = int(data[i]);

        //Convertir ASCII en binaire
        String value_Bin = "";
        while(val > 0)
        {
            (val % 2)? value_Bin += '1':
                       value_Bin += '0';
            val /= 2;
        }
        std::reverse(value_Bin.begin(), value_Bin.end());
        bin += value_Bin;
    }
    return bin;
}

/**
 * @brief Generation d'un string contenant le message combinant une clé pour donner une valeur d'identification en cas de perte de données 
 * @param key 
 * @param data 
 * @return String 
 */
String lora::generation_CRC(const String& data, const String& key)
{
    int l_key = key.length();

	// Ajoute n-1 zéros à la fin des données
	String appended_data = data + add_bit(l_key - 1);

	String remainder = mod2div(appended_data, key);
    
    return remainder;
}

/**
 * @brief Fonction qui va faire un xor(ou exclusif) entre deux String
 * @param a 
 * @param b 
 * @return String 
 */
String lora::xor1(const String& a, const String& b)
{
    // Initialise le resultat
	String result = "";

	int n = b.length();

	// Parcourt tous les bits, si les bits sont
    // pareil, alors XOR vaut 0, sinon 1
	for(int i = 1; i < n; i++)
	{
		if (a[i] == b[i])
			result += "0";
		else
			result += "1";
	}
	return result;
}

/**
 * @brief Fonction qui va diviser les données binaires par la clé et stocker le reste de la division
 * @param divident 
 * @param divisor 
 */
String lora::mod2div(const String& divident, const String& divisor)
{
    // Nombre de bits à XORer à la fois.
	int i = divisor.length();

	// Découpage du diviseur pour s'approprier
    // longueur pour une étape particulière
	String tmp = divident.substring(0, i);

	int length = divident.length();

	while (i < length)
	{
		if (tmp[0] == '1')

			// Remplace le diviseur par le résultat
            // de XOR et tirez 1 bit vers le bas
			tmp = xor1(divisor, tmp) + divident[i];
		else

			// Si le bit le plus à gauche est '0'.
            // Si le bit le plus à gauche du dividende (ou le
            // partie utilisée à chaque étape) est 0, l'étape ne peut pas
            // utilise le diviseur normal ; nous devons utiliser un
            // diviseur composé uniquement de 0.
			tmp = xor1(add_bit(i), tmp) + divident[i];

		// Incrémenter la sélection pour aller plus loin
		i += 1;
	}

	// Pour les n derniers bits, il faut l'exécuter
    // normalement, car l'augmentation de la valeur de i entraînera
    // Index hors limites.
	if (tmp[0] == '1')
		tmp = xor1(divisor, tmp);
	else
		tmp = xor1(add_bit(i), tmp);

	return tmp;
}