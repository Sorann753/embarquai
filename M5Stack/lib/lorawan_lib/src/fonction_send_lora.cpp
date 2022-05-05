#include "fonction_send_lora.h"

/**
* Fonction qui va envoyer une chaine de caractère sur un module lorawan.
* @param struct donnee structure ui contiendra les différents champs qui contiendront les données des capteurs
* @return void 
*/
void lora::Send_Message_Lorawan(const data_bateau& data)
{
    // Initialisation de Serial 2
    //init();

    String message{""};
    String Borne{""}; 
    String key = "10101"; // Choisir cle en binaire

    // Concatenation des differentes valeurs du capteurs
     Borne += "b'&"; // Debut message(obligatoire pour l envoie)
     message += data.data + "/";
     message += data.data_1 + "/";
     message += data.data_2 + "/";
    // message += data.param4 + "/";
    // message += data.param5 + "/";
    // message += data.param6 + "/";
    
    //Convertir le message en binaire
    String value_bin = conversion_binaire(message);

    //Generation CRC
    String crc_bin = generation_CRC(value_bin, key);

    //Concatener le crc à la fin du message
    Borne += message;
    Borne += "/" + crc_bin + "&'"; // Fin message
    
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
    Serial2.begin(19200, SERIAL_8N1, 13, 14);//RX13 et tx14
    Serial2.flush();
    delay(100);
}

/**
 * @brief Conversion d'une chaine de caractère en binaire 
 * @return std::string 
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
 * @return std::string 
 */
String lora::generation_CRC(const String& data, const String& key)
{
    int l_key = key.length();

	// Ajoute n-1 zéros à la fin des données
	String appended_data = (data + String(l_key - 1, '0'));

	String remainder = mod2div(appended_data, key);
    
    return remainder;
}

/**
 * @brief Fonction qui va faire un xor(ou exclusif) entre deux chaine de carectere
 * @param a 
 * @param b 
 * @return std::string 
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
	int pick = divisor.length();

	// Découpage du diviseur pour s'approprier
    // longueur pour une étape particulière
	String tmp = divident.substring(0, pick);

	int n = divident.length();

	while (pick < n)
	{
		if (tmp[0] == '1')

			// Remplace le diviseur par le résultat
            // de XOR et tirez 1 bit vers le bas
			tmp = xor1(divisor, tmp) + divident[pick];
		else

			// Si le bit le plus à gauche est '0'.
            // Si le bit le plus à gauche du dividende (ou le
            // partie utilisée à chaque étape) est 0, l'étape ne peut pas
            // utilise le diviseur normal ; nous devons utiliser un
            // diviseur composé uniquement de 0.
			tmp = xor1(String(pick, '0'), tmp) +
				divident[pick];

		// Incrémenter la sélection pour aller plus loin
		pick += 1;
	}

	// Pour les n derniers bits, il faut l'exécuter
    // normalement, car l'augmentation de la valeur de pick entraînera
    // Index hors limites.
	if (tmp[0] == '1')
		tmp = xor1(divisor, tmp);
	else
		tmp = xor1(String(pick, '0'), tmp);

	return tmp;
}