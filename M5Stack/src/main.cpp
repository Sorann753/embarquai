/**
 * @brief fichier principal du M5Stack
 * @author DORIATH ARTHUS
 * @date 05/05/2022
 * @version 0.2
 */

//on précise a platformio qu'il ne faut pas compilé ce fichier quand on fait des test unitaires
//si non sa provoque un bug de multiple definition de loop() et de setup()
#ifndef UNIT_TEST

#include <M5Core2.h>
#include "navi.hpp"
#include "fonction_send_lora.h"
#include "ihm.hpp"

//TODO : finir les commentaires doxygen 

/**
 * @brief tache freeRTOS qui sert a faire clignoter la led
 * @param void* un faux paramètre pour pouvoir ce servir de cette fonction dans une task freeRTOS
 * @return rien
 */ 
void loop_led(void*){

    static bool led_state = 1;
    while(true){
        M5.Axp.SetLed(led_state);
        led_state = !led_state;
        vTaskDelay(500 / portTICK_PERIOD_MS); //met en pause la tache et laisse le processeur pour d'autres taches
        M5.Axp.SetLed(led_state);
        led_state = !led_state;

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

/**
 * @brief tache pour freeRTOS qui sert a envoyé les données Navi avec le module Lorawan
 * @param void* un faux paramètre pour pouvoir ce servir de cette fonction dans une task freeRTOS
 * @return rien
 */
void send_with_lora(void*){

    while(true){

        navi::data_navi data = Navi.pop_data();
        if(data.id_bateau != -1 && data.id_course != -1){
            if(data.data_content != 0){
                lora::Send_Message_Lorawan(data);
            }
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}



/**
 * @brief la fonction d'initialisation du programme
 * @param rien
 * @return rien
 */
void setup() {

    M5.begin(true, false);
    Serial.begin(115200);

    Navi.begin();
    lora::init();
    ihm::init();

    //demarre des taches FreeRTOS
    xTaskCreate(loop_led, "BLINKING", 1300, NULL, 1, NULL);
    xTaskCreate(send_with_lora, "SEND LORA", 4096, NULL, 5, NULL);
}

static uint64_t loop_counter = 0;

/**
 * @brief la boucle d'execution du programme
 * @param rien
 * @return rien
 */
void loop() {

    Navi.fetch_nmea_data();
    M5.update();

    //affiche un . de temps en temps pour être sure que le M5stack fonctionne bien
    if(loop_counter % 1000 == 0){
        Serial.println(".");
    }
    loop_counter++;
    vTaskDelay(10 / portTICK_PERIOD_MS);
}

#endif