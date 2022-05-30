#include "ihm.hpp"

namespace ihm{

    image img_config{jpg_config, 5986};
    image img_config_touched{jpg_config_touched, 5115};
    image img_boat{jpg_boat, 5523};
    image img_boat_touched{jpg_boat_touched, 5247};



    Button boutonConfig(0, 0, 50, 50, false, "CONFIG",
                INVISIBLE, INVISIBLE);

    Button boutonHome(TOUCH_W - 50, 0, 50, 50, false, "HOME",
                INVISIBLE, INVISIBLE);

    Button boutonIdBateau(TOUCH_W - 250, TOUCH_H - 135, 200, 50, false, "ID BATEAU",
                {BLACK, MY_ORANGE, MY_ORANGE}, {MY_ORANGE, BLACK, MY_ORANGE});

    Button boutonIdCourse(TOUCH_W - 250, TOUCH_H - 215, 200, 50, false, "ID COURSE",
                {BLACK, MY_ORANGE, MY_ORANGE}, {MY_ORANGE, BLACK, MY_ORANGE});

    Button boutonConfirmIdBateau(TOUCH_W - 50, 0, 50, 50, false, "CONFIRM BATEAU",
                    {BLACK, MY_ORANGE, MY_ORANGE}, {MY_ORANGE, BLACK, MY_ORANGE});

    Button boutonConfirmIdCourse(TOUCH_W - 50, 0, 50, 50, false, "CONFIRM COURSE",
                    {BLACK, MY_ORANGE, MY_ORANGE}, {MY_ORANGE, BLACK, MY_ORANGE});

    Button boutonCancelId(0, 0, 50, 50, false, "CANCEL ID",
                    {BLACK, MY_ORANGE, MY_ORANGE}, {MY_ORANGE, BLACK, MY_ORANGE});



    //? optimiser : mettre les 10 boutons de chiffres dans un tableau?
    /** @brief boutons du pavé numérique tactile */
    Button boutonNumber0(TOUCH_W/2 - 145, TOUCH_H - 150, 50, 50, false, "0",
                {BLACK, MY_ORANGE, MY_ORANGE}, {MY_ORANGE, BLACK, MY_ORANGE});

    /** @brief boutons du pavé numérique tactile */
    Button boutonNumber1(TOUCH_W/2 - 85, TOUCH_H - 150, 50, 50, false, "1",
                {BLACK, MY_ORANGE, MY_ORANGE}, {MY_ORANGE, BLACK, MY_ORANGE});

    /** @brief boutons du pavé numérique tactile */
    Button boutonNumber2(TOUCH_W/2 - 25, TOUCH_H - 150, 50, 50, false, "2",
                {BLACK, MY_ORANGE, MY_ORANGE}, {MY_ORANGE, BLACK, MY_ORANGE});

    /** @brief boutons du pavé numérique tactile */
    Button boutonNumber3(TOUCH_W/2 + 35, TOUCH_H - 150, 50, 50, false, "3",
                {BLACK, MY_ORANGE, MY_ORANGE}, {MY_ORANGE, BLACK, MY_ORANGE});

    /** @brief boutons du pavé numérique tactile */
    Button boutonNumber4(TOUCH_W/2 + 95, TOUCH_H - 150, 50, 50, false, "4",
                {BLACK, MY_ORANGE, MY_ORANGE}, {MY_ORANGE, BLACK, MY_ORANGE});

    /** @brief boutons du pavé numérique tactile */
    Button boutonNumber5(TOUCH_W/2 - 145, TOUCH_H - 90, 50, 50, false, "5",
                {BLACK, MY_ORANGE, MY_ORANGE}, {MY_ORANGE, BLACK, MY_ORANGE});

    /** @brief boutons du pavé numérique tactile */
    Button boutonNumber6(TOUCH_W/2 - 85, TOUCH_H - 90, 50, 50, false, "6",
                {BLACK, MY_ORANGE, MY_ORANGE}, {MY_ORANGE, BLACK, MY_ORANGE});

    /** @brief boutons du pavé numérique tactile */
    Button boutonNumber7(TOUCH_W/2 - 25, TOUCH_H - 90, 50, 50, false, "7",
                {BLACK, MY_ORANGE, MY_ORANGE}, {MY_ORANGE, BLACK, MY_ORANGE});

    /** @brief boutons du pavé numérique tactile */
    Button boutonNumber8(TOUCH_W/2 + 35, TOUCH_H - 90, 50, 50, false, "8",
                {BLACK, MY_ORANGE, MY_ORANGE}, {MY_ORANGE, BLACK, MY_ORANGE});

    /** @brief boutons du pavé numérique tactile */
    Button boutonNumber9(TOUCH_W/2 + 95, TOUCH_H - 90, 50, 50, false, "9",
                {BLACK, MY_ORANGE, MY_ORANGE}, {MY_ORANGE, BLACK, MY_ORANGE});





    uint8_t currentScreen;

    void hideHomeScreen(){

        boutonConfig.hide();
        M5.lcd.clear();
    }

    void drawHomeScreen(){

        boutonConfig.draw();
        M5.lcd.drawJpg(img_config.img, img_config.size, 0, 0, 0,0,0,0, JPEG_DIV_2);

        M5.Lcd.setTextSize(1);
        M5.Lcd.drawString("ID BATEAU", 100, 10);
        M5.Lcd.drawNumber(Navi.get_id_bateau(), 200, 10);

        M5.Lcd.drawString("ID COURSE", 102, 30);
        M5.Lcd.drawNumber(Navi.get_id_course(), 200, 30);

        M5.Lcd.drawString("Direction du vent", TOUCH_W/2, TOUCH_H/2 - 90);
        M5.Lcd.drawString("Vitesse du vent", TOUCH_W/2, TOUCH_H/2 - 60);
        M5.Lcd.drawString("Vitesse", TOUCH_W/2, TOUCH_H/2 - 30);
        M5.Lcd.drawString("Latitude", TOUCH_W/2, TOUCH_H/2);
        M5.Lcd.drawString("Longitude", TOUCH_W/2, TOUCH_H/2 + 30);
        M5.Lcd.drawString("Cap", TOUCH_W/2, TOUCH_H/2 + 60);

        currentScreen = screens::HOME;
    }

    void hideConfigScreen(){

        boutonHome.hide();
        boutonIdBateau.hide();
        boutonIdCourse.hide();
        M5.Lcd.clear();
    }

    void drawConfigScreen(){

        boutonHome.draw();
        boutonIdBateau.draw();
        boutonIdCourse.draw();
        M5.Lcd.drawJpg(img_boat.img, img_boat.size, boutonHome.x, boutonHome.y, 0,0,0,0, JPEG_DIV_2);
        currentScreen = screens::CONFIG;
    }

    void hideIdBateauScreen(){

        boutonCancelId.hide();
        boutonConfirmIdBateau.hide();

        boutonNumber0.hide();
        boutonNumber1.hide();
        boutonNumber2.hide();
        boutonNumber3.hide();
        boutonNumber4.hide();
        boutonNumber5.hide();
        boutonNumber6.hide();
        boutonNumber7.hide();
        boutonNumber8.hide();
        boutonNumber9.hide();

        M5.Lcd.clear();
    }

    void drawIdBateauScreen(){

        boutonCancelId.draw();
        boutonConfirmIdBateau.draw();

        M5.Lcd.drawRoundRect(TOUCH_W/2 - 55, TOUCH_H/2 - 75, 200, 50, 10, MY_ORANGE);
        M5.Lcd.drawString("NOUVEL ID", TOUCH_W/2 - 110, TOUCH_H/2 - 50);

        boutonNumber0.draw();
        boutonNumber1.draw();
        boutonNumber2.draw();
        boutonNumber3.draw();
        boutonNumber4.draw();
        boutonNumber5.draw();
        boutonNumber6.draw();
        boutonNumber7.draw();
        boutonNumber8.draw();
        boutonNumber9.draw();

        currentScreen = screens::SET_ID_BATEAU;
    }

    void hideIdCourseScreen(){

        boutonCancelId.hide();
        boutonConfirmIdCourse.hide();

        boutonNumber0.hide();
        boutonNumber1.hide();
        boutonNumber2.hide();
        boutonNumber3.hide();
        boutonNumber4.hide();
        boutonNumber5.hide();
        boutonNumber6.hide();
        boutonNumber7.hide();
        boutonNumber8.hide();
        boutonNumber9.hide();

        M5.Lcd.clear();
    }

    void drawIdCourseScreen(){

        boutonCancelId.draw();
        boutonConfirmIdCourse.draw();

        M5.Lcd.drawRoundRect(TOUCH_W/2 - 55, TOUCH_H/2 - 75, 200, 50, 10, MY_ORANGE);
        M5.Lcd.drawString("NOUVEL ID", TOUCH_W/2 - 110, TOUCH_H/2 - 50);

        boutonNumber0.draw();
        boutonNumber1.draw();
        boutonNumber2.draw();
        boutonNumber3.draw();
        boutonNumber4.draw();
        boutonNumber5.draw();
        boutonNumber6.draw();
        boutonNumber7.draw();
        boutonNumber8.draw();
        boutonNumber9.draw();

        currentScreen = screens::SET_ID_COURSE;
    }





    static String inputBuffer = "";
    void drawBuffer(){

        M5.Lcd.setTextSize(2);
        M5.Lcd.fillRect(TOUCH_W/2 - 50, TOUCH_H/2 - 70, 190, 40, 0);
        M5.Lcd.drawNumber(inputBuffer.toInt(), TOUCH_W/2 + 50, TOUCH_H/2 - 50);
        M5.Lcd.setTextSize(1);
    }

    static bool isInit = false;
    void init(){

        if(isInit) return;

        M5.Lcd.setTextColor(MY_ORANGE);

        boutonConfig.addHandler([](Event& e){
            M5.lcd.drawJpg(img_config_touched.img, img_config_touched.size, boutonConfig.x, boutonConfig.y, 0,0,0,0, JPEG_DIV_2);
        }, E_TOUCH);
        boutonConfig.addHandler([](Event& e){
            hideHomeScreen();
            drawConfigScreen();
        }, E_RELEASE);


        boutonIdBateau.setLabel("CHANGER ID BATEAU");
        boutonIdBateau.addHandler([](Event& e){
            hideConfigScreen();
            drawIdBateauScreen();
        }, E_RELEASE);


        boutonIdCourse.setLabel("CHANGER ID COURSE");
        boutonIdCourse.addHandler([](Event& e){
            hideConfigScreen();
            drawIdCourseScreen();
        }, E_RELEASE);


        boutonHome.addHandler([](Event& e){
            M5.Lcd.drawJpg(img_boat_touched.img, img_boat_touched.size, boutonHome.x, boutonHome.y, 0,0,0,0, JPEG_DIV_2);
        }, E_TOUCH);
        boutonHome.addHandler([](Event& e){
            hideConfigScreen();
            drawHomeScreen();
        }, E_RELEASE);


        boutonConfirmIdBateau.addHandler([](Event& e){
                if(!inputBuffer.isEmpty()){
                    Navi.set_id_bateau(inputBuffer.toInt());
                    inputBuffer = "";
                }
            
                hideIdBateauScreen();
                drawConfigScreen();
        }, E_RELEASE);


        boutonConfirmIdCourse.addHandler([](Event& e){
            if(!inputBuffer.isEmpty()){
                Navi.set_id_course(inputBuffer.toInt());
                inputBuffer = "";
            }

            hideIdCourseScreen();
            drawConfigScreen();
        }, E_RELEASE);


        boutonCancelId.addHandler([](Event& e){
            inputBuffer = "";

            hideIdBateauScreen();
            hideIdCourseScreen();
            drawConfigScreen();
        }, E_RELEASE);





        boutonNumber0.setTextSize(2);
        boutonNumber0.addHandler([](Event& e){
            if(inputBuffer.length() < 8){
                inputBuffer += '0';
                drawBuffer();
            }
        }, E_TOUCH);

        boutonNumber1.setTextSize(2);
        boutonNumber1.addHandler([](Event& e){
            if(inputBuffer.length() < 8){
                inputBuffer += '1';
                drawBuffer();
            }
        }, E_TOUCH);

        boutonNumber2.setTextSize(2);
        boutonNumber2.addHandler([](Event& e){
            if(inputBuffer.length() < 8){
                inputBuffer += '2';
                drawBuffer();
            }
        }, E_TOUCH);

        boutonNumber3.setTextSize(2);
        boutonNumber3.addHandler([](Event& e){
            if(inputBuffer.length() < 8){
                inputBuffer += '3';
                drawBuffer();
            }
        }, E_TOUCH);

        boutonNumber4.setTextSize(2);
        boutonNumber4.addHandler([](Event& e){
            if(inputBuffer.length() < 8){
                inputBuffer += '4';
                drawBuffer();
            }
        }, E_TOUCH);

        boutonNumber5.setTextSize(2);
        boutonNumber5.addHandler([](Event& e){
            if(inputBuffer.length() < 8){
                inputBuffer += '5';
                drawBuffer();
            }
        }, E_TOUCH);

        boutonNumber6.setTextSize(2);
        boutonNumber6.addHandler([](Event& e){
            if(inputBuffer.length() < 8){
                inputBuffer += '6';
                drawBuffer();
            }
        }, E_TOUCH);

        boutonNumber7.setTextSize(2);
        boutonNumber7.addHandler([](Event& e){
            if(inputBuffer.length() < 8){
                inputBuffer += '7';
                drawBuffer();
            }
        }, E_TOUCH);

        boutonNumber8.setTextSize(2);
        boutonNumber8.addHandler([](Event& e){
            if(inputBuffer.length() < 8){
                inputBuffer += '8';
                drawBuffer();
            }
        }, E_TOUCH);

        boutonNumber9.setTextSize(2);
        boutonNumber9.addHandler([](Event& e){
            if(inputBuffer.length() < 8){
                inputBuffer += '9';
                drawBuffer();
            }
        }, E_TOUCH);



        //par défault les autres boutons ne sont pas désactivé,
        //du coup il faut les désactivé a l'initialisation
        hideConfigScreen();
        hideIdBateauScreen();
        hideIdCourseScreen();

        //on démare l'ihm sur l'écran principale
        drawHomeScreen();

        xTaskCreate(updateHomeScreen, "UPDATE DATA", 4096, NULL, 0, NULL);

        isInit = true;
        Serial.println("IHM STARTED");
    }



    void updateHomeScreen(void*){

        while(true){
            if(currentScreen == screens::HOME){

                navi::data_navi data = Navi.get_data();

                M5.Lcd.fillRect(TOUCH_W - 90, TOUCH_H/2-96, 90, 15, 0);
                M5.Lcd.drawFloat(data.WindAngle, 5, TOUCH_W - 50, TOUCH_H/2 - 90);
                vTaskDelay(25 / portTICK_PERIOD_MS);

                M5.Lcd.fillRect(TOUCH_W - 90, TOUCH_H/2-66, 90, 15, 0);
                M5.Lcd.drawNumber(data.WindSpeed, TOUCH_W - 50, TOUCH_H/2 - 60);
                vTaskDelay(25 / portTICK_PERIOD_MS);

                M5.Lcd.fillRect(TOUCH_W - 90, TOUCH_H/2-36, 90, 15, 0);
                M5.Lcd.drawNumber(data.speed, TOUCH_W - 50, TOUCH_H/2 - 30);
                vTaskDelay(25 / portTICK_PERIOD_MS);

                M5.Lcd.fillRect(TOUCH_W - 90, TOUCH_H/2-6, 90, 15, 0);
                M5.Lcd.drawFloat(data.latitude, 5, TOUCH_W - 50, TOUCH_H/2);
                vTaskDelay(25 / portTICK_PERIOD_MS);

                M5.Lcd.fillRect(TOUCH_W - 90, TOUCH_H/2+24, 90, 15, 0);
                M5.Lcd.drawFloat(data.longitude, 5, TOUCH_W - 50, TOUCH_H/2 + 30);
                vTaskDelay(25 / portTICK_PERIOD_MS);


                M5.Lcd.fillRect(TOUCH_W - 90, TOUCH_H/2+54, 90, 15, 0);
                M5.Lcd.drawFloat(data.heading, 5, TOUCH_W - 50, TOUCH_H/2 + 60);
                vTaskDelay(25 / portTICK_PERIOD_MS);
            }

            vTaskDelay(500 / portTICK_PERIOD_MS);
        }
    }

}