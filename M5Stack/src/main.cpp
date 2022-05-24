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

extern const unsigned char img_menu[2335];
extern const unsigned char img_menu_touched[2156];

constexpr uint16_t MY_ORANGE = 0xFC00; //RGB 16bit -> 0xFF8100 RGB 24bit
constexpr ButtonColors INVISIBLE = {NODRAW, NODRAW, NODRAW};

Button Bouton1(0, 0, 50, 50, false, "TEST A",
               INVISIBLE, INVISIBLE);

Button Bouton2(TOUCH_W - 100, TOUCH_H - 145, 100, 100, false, "TEST B",
               {BLACK, MY_ORANGE, MY_ORANGE}, {MY_ORANGE, BLACK, MY_ORANGE});

void init_IHM(){

    M5.lcd.drawJpg(img_menu, 2335, 0, 0);

    Bouton1.addHandler([](Event& e){
        Serial.println("A");
        M5.Axp.SetLed(0);
        M5.lcd.drawJpg(img_menu_touched, 2156, 0, 0);
    }, E_TOUCH);
    Bouton1.addHandler([](Event& e){
        M5.lcd.drawJpg(img_menu, 2335, 0, 0);
        M5.Axp.SetLed(1);
    }, E_RELEASE);

    Bouton2.addHandler([](Event& e){
        Serial.println("B");
    }, E_TOUCH);

    M5.Lcd.drawString("PROGRESS BAR", TOUCH_W / 2, TOUCH_H/2 - 30);

    M5.Buttons.draw();

    Serial.println("IHM STARTED");
}

//TODO : faire fonction affichage texte

//TODO : faire fonction affichage image (if needed)



void loop_led(void*){

    static bool led_state = 0;
    while(true){
        M5.Axp.SetLed(led_state);
        led_state = !led_state;
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void setup() {

    M5.begin(true, false);
    Serial.begin(115200);

    Navi.begin();

    init_IHM();
    xTaskCreate(loop_led, "clignotement", 2048, NULL, 1, NULL);
}



static uint64_t loop_counter = 0;
void loop() {

    uint32_t start = xTaskGetTickCountFromISR();
    Navi.fetch_nmea_data();
    M5.update();

    //TODO faire IHM

    if(loop_counter % 100 == 0){
       M5.Lcd.fillRect(TOUCH_W / 2 - 49, TOUCH_H/2 - 6, 100, 14, 0);
       M5.Lcd.drawString(String((int)loop_counter/100).c_str(), TOUCH_W / 2, TOUCH_H/2 + 30);
    }
    M5.Lcd.progressBar(TOUCH_W / 2 - 50, TOUCH_H/2 - 7, 100, 15, loop_counter%100);

    

    //affiche un . de temps en temps pour être sure que le M5stack fonctionne bien
    if(loop_counter % 1000 == 0){
        Serial.println(".");
    }
    loop_counter++;
    vTaskDelay(10 / portTICK_PERIOD_MS);
}

#endif