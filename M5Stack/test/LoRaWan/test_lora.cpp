/**
 * @brief fichier test du M5Stack pour module Lora
 * @author Benaitier Loris
 * @date 05/05/2022
 * @version 0.1
 */

#include <M5Core2.h>
#include "fonction_send_lora.h"

/**
 * @brief fonction d'initialisation du programme
 * @param none
 * @return none
 */
void setup() {
    M5.begin();
    Serial.begin(9600);
    lora::init();
}


/**
 * @brief boucle d'exécution du programme
 * @param none
 * @return none
 */
void loop() {
    lora::data_bateau data;
    lora::Send_Message_Lorawan(data);
    
    Serial.println("ok");
}