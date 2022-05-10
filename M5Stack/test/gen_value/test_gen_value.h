#ifndef TEST_GEN_VALUE_H
#define TEST_GEN_VALUE_H

#include <unity.h>
#include "fonction_gener_value.h"

/*Fonction pour tester si la generation de valeur est correcte
*@param : void
*@return: void
*/
void test_gen_number()
{
    //Ajout valeurs generées par les fonctions dans tableau pour le test
    std::array<float, 10> tab;
    std::array<float, 10> tab1;
    tab = generate_test_values_sensors_number<float, 10>();
    tab1 = generate_test_values_sensors_number<float, 10>();//2/

    //Boucle de test
    for(uint8_t i = 0; i<tab.size(); i++)
    {
        //Si first == secondla valeur est fausse, le test échouera.
        TEST_ASSERT_NOT_EQUAL(tab[i], tab1[i]);

        Serial.print(tab[i]);
        Serial.print(" ");
    }

    //Test pour savoir si la taille demande est correcte
    //Si first != second la valeur est fausse, le test échouera
    TEST_ASSERT_EQUAL(tab.size(), tab1.size());

    //Test pour savoir si le type est correcte
    bool type = false;
    if(sizeof(tab[1]) == sizeof(float) && sizeof(tab1[1]) == sizeof(float)){type = true;}
    TEST_ASSERT_TRUE_MESSAGE(type, "Type correcte");
}

/*Fonction pour tester si la generation de lettre est correcte
*@param : void
*@return: void
*/
void test_gen_letter()
{
    //Ajout valeurs generées par les fonctions dans tableau pour le test
    std::array<String, 10> tab;
    std::array<String, 10> tab1;
    tab = generate_test_values_sensors_word<10>(true);
    tab1 = generate_test_values_sensors_word<10>(true);//l
    bool egalite = false;

    //Boucle de test
    for(uint8_t i = 0; i<tab.size(); i++)
    {
        if(tab[i] == "N" || tab[i] == "O" || tab[i] == "E" || tab[i] == "S")
        {
            if(tab1[i] == "N" || tab1[i] == "O" || tab1[i] == "E" || tab1[i] == "S")
            {
                //Activation de la condition
                egalite = true;

                Serial.print(tab[i]);
                Serial.print(" ");
            } 
        }
        
        //Si egalite est à false, le test échouera.
        TEST_ASSERT_TRUE(egalite);
        
        //Desactivation de la condition
        egalite = false; 
    }

    //Test pour savoir si la taille demande est correcte
    //Si first != second la valeur est fausse, le test échouera
    TEST_ASSERT_EQUAL(tab.size(), tab1.size());

    //Test pour savoir si le type est correcte
    bool type = false;
    if(sizeof(tab[1]) == sizeof(String) && sizeof(tab1[1]) == sizeof(String)){type = true;}
    TEST_ASSERT_TRUE_MESSAGE(type, "Type incorrecte");
}

/*Fonction pour tester si la generation du message est correcte
*@param : void
*@return: void
*/
void test_gen_string()
{
    //Creation tableau
    std::array<float, 10> tab;
    std::array<String, 10> tab1;
    bool activation = false;
    
    //Copie contenue tableau
    tab = generate_test_values_sensors_number<float, 10>();
    tab1 = generate_test_values_sensors_word<10>(true);
    
    //Processus de formattage message
    String message = formatting_value_lorawan<float, 10, 10>(tab, tab1);
    //Serial.print(message);

    //Concatenation + affichage des donnees
    String part = static_cast<String>(message[0]) + static_cast<String>(message[1]) + static_cast<String>(message[2]) + static_cast<String>(message[3]) + static_cast<String>(message[4]);
    Serial.print(part);
    Serial.print("!");
    Serial.print(tab[0]);
    Serial.print("!");
    Serial.print(message[60]);
    Serial.print("!");
    Serial.print(tab1[0]);
    Serial.print("!!");
    Serial.print(message);

    //Verification egalite entre valeur tableau et message
    if(static_cast<String>(tab[0]) == part && tab1[0] == static_cast<String>(message[60]))
    {
        activation = true;
    }
    
    //Si activation est a false alors le test echou
    TEST_ASSERT_TRUE_MESSAGE(activation, "Contenu du message incorrecte");

    //Test pour savoir si la taille demande est correcte
    //Si first != second la valeur est fausse, le test échouera
    TEST_ASSERT_EQUAL(message.length(), 80);

    //Test pour savoir si le type est correcte
    bool type = false;
    if(sizeof(message) == sizeof(String)){type = true;}
    TEST_ASSERT_TRUE_MESSAGE(type, "Type incorrecte");
}
#endif