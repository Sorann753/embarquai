/**
 * @brief fichier principal du M5Stack
 * @author DORIATH ARTHUS
 * @date 05/05/2022
 * @version 0.2
 */

#include <M5Core2.h>
#include "navi.hpp"
#include "fonction_send_lora.h"



void setup() {

    M5.begin();
    Serial.begin(115200);

    Navi.begin();
}



static uint64_t loop_counter = 0;
void loop() {

    Navi.get_nmea_data();

    //affiche un . de temps en temps pour être sure que le M5stack fonctionne bien
    if(loop_counter % 5000000 == 0){
        Serial.println(".");
    }
    loop_counter++;
}