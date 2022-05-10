#ifndef FONCTION_GERER_VALUE_H
#define FONCTION_GERER_VALUE_H

//Biblioteque
#include<M5Core2.h>

/** Cette fonction est generique, va generer des valeurs aleatoire pour simuler les valeurs des capteurs.
 * @param typename-T choix du type pour l'utilisateur
 * @param  size_t-nombre valeur a genener
 * @return std::array<T> tableau contenant les donnees capteurs
 */
template<typename T, size_t capacity>
std::array<T, capacity> generate_test_values_sensors_number()
{
    //Creation tableau
    std::array<T, capacity> tab;

    //Creation valeur random entre 1 et 200(div par 100 == float)
    for(int i = 0; i<capacity; i++)
    {
        
        tab[i] = random(100,1000);
        tab[i] /= 10;
    }

    //Retourner tableau
    return tab;
}

/** Cette fonction est generique, elle va generer des mots ou lettres aleatoire pour simuler les valeurs des capteurs.
 * @param  size_t-nombre valeur a genener
 * @param  bool-let_word choix true : lettre, false: mot
 * @return std::array<T> tableau contenant les donnees capteurs
 */
template<size_t capacity>
std::array<String, capacity> generate_test_values_sensors_word(bool lettre_word)
{
    //Creation tableau
    std::array<String, capacity> tab;

   if(lettre_word == true)
   {
       String pole= "NOES";
       for(int i = 0;i <capacity; i++)
       {
           int indice = random(pole.length());
           tab[i] = pole[indice];
       }
   }
   else
   {
       std::array<String, 4> tab2;
       tab2[0] = "Nord";
       tab2[1] = "Sud";
       tab2[2] = "Ouest";
       tab2[3] = "Est";

       for(int i = 0;i <capacity; i++)
       {
           int indice = random(4);
           tab[i] = tab2[indice];
       }
   }

   return tab;
}

/** Cette fonction est generique, elle va mettre en forme des informations pour envoyer en lorawan(string).
 * @param  typename-T choix du type pour l'utilisateur
 * @param  size_t-capacity valeur du tableau 1
 * @param  size_t-capacity2 valeur du tableau 2
 * @return string chaine de carectere contenant les donnees
 */
template<typename T, size_t capacity, size_t capacity2>
String formatting_value_lorawan(std::array<T, capacity> const & values, std::array<String, capacity2> const & lettre_word)
{
    String message;

    //Mettre valeur dans chaine caractere(float)
    for(int i = 0; i<values.size(); i++)
    {
        message += static_cast<String>(values[i]);
        message += "/";
    }

    //Mettre valeur dans chaine caractere(string)
    for(int j = 0; j<lettre_word.size(); j++)
    {
        message += lettre_word[j] + "/";
    }

    return message;
}
#endif