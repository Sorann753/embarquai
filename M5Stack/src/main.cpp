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



void loop_led(void*){

    static bool led_state = 1;
    while(true){
        M5.Axp.SetLed(led_state);
        led_state = !led_state;
        vTaskDelay(500 / portTICK_PERIOD_MS);
        M5.Axp.SetLed(led_state);
        led_state = !led_state;

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void send_with_lora(void*){

    while(true){

        navi::data_navi data = Navi.pop_data();
        if(data.data_content != 0){
            lora::Send_Message_Lorawan(data);
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}



void setup() {

    M5.begin(true, false);
    Serial.begin(115200);

    Navi.begin();
    lora::init();
    init_IHM();

    xTaskCreate(loop_led, "BLINKING", 2048, NULL, 1, NULL);
    xTaskCreate(send_with_lora, "SEND LORA", 4096, NULL, 3, NULL);
}



static uint64_t loop_counter = 0;
void loop() {

    Navi.fetch_nmea_data();
    M5.update();

    // if(currentScreen == screens::HOME){
    //     if(loop_counter % 100 == 0){
    //        M5.Lcd.fillRect(TOUCH_W / 2 - 49, TOUCH_H/2 - 6, 100, 14, 0);
    //        M5.Lcd.drawString(String((int)loop_counter/100).c_str(), TOUCH_W / 2, TOUCH_H/2 + 30);
    //     }
    //     M5.Lcd.progressBar(TOUCH_W / 2 - 50, TOUCH_H/2 - 7, 100, 15, loop_counter%100);
    // }

    

    //affiche un . de temps en temps pour être sure que le M5stack fonctionne bien
    if(loop_counter % 1000 == 0){
        Serial.println(".");
    }
    loop_counter++;
    vTaskDelay(10 / portTICK_PERIOD_MS);
}

#endif