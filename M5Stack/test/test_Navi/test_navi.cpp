/**
 * @brief fichier test du M5Stack pour module Navi
 * @author Doriath Arthus
 * @date 11/05/2022
 * @version 0.1
 */

#include <M5Core2.h>
#include <unity.h>
#include "navi.hpp"
#include "data_navi.hpp"

/**
 * @brief fonction d'initialisation du programme
 * @param none
 * @return none
 */
void setup() {
    M5.begin();
    Serial.begin(115200);

    UNITY_BEGIN();
}


/**
 * @brief boucle d'exécution du programme
 * @param none
 * @return none
 */
void loop() {
    navi::data_navi data;
        
    delay(100);
    UNITY_END(); // termine le test unitaire
}