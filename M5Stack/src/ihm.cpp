#include "ihm.hpp"

namespace ihm{

    image img_config{jpg_config, 5986};
    image img_config_touched{jpg_config_touched, 5115};

    image img_boat{jpg_boat, 5523};
    image img_boat_touched{jpg_boat_touched, 5247};

    image img_confirm{jpg_confirm, 4281};
    image img_confirm_touched{jpg_confirm_touched, 3690};

    image img_cancel{jpg_cancel, 4381};
    image img_cancel_touched{jpg_cancel_touched, 3881};



    Button boutonConfig(0, 0, 50, 50, false, "CONFIG",
                INVISIBLE, INVISIBLE);

    Button boutonHome(TOUCH_W - 50, 0, 50, 50, false, "HOME",
                INVISIBLE, INVISIBLE);

    Button boutonIdBateau(TOUCH_W - 250, TOUCH_H - 215, 200, 50, false, "ID BATEAU",
                {BLACK, MY_ORANGE, MY_ORANGE}, {MY_ORANGE, BLACK, MY_ORANGE});

    Button boutonIdCourse(TOUCH_W - 250, TOUCH_H - 135, 200, 50, false, "ID COURSE",
                {BLACK, MY_ORANGE, MY_ORANGE}, {MY_ORANGE, BLACK, MY_ORANGE});

    Button boutonConfirmIdBateau(TOUCH_W - 50, 0, 50, 50, false, "CONFIRM BATEAU",
                    INVISIBLE, INVISIBLE);

    Button boutonConfirmIdCourse(TOUCH_W - 50, 0, 50, 50, false, "CONFIRM COURSE",
                    INVISIBLE, INVISIBLE);

    Button boutonCancelId(0, 0, 50, 50, false, "CANCEL ID",
                    INVISIBLE, INVISIBLE);



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



    //une variable pour stocker l'écran actuel
    uint8_t currentScreen;


    bool M5LCDGuard::isInit = false;
    size_t M5LCDGuard::nbInstances = 0;
    pthread_mutex_t M5LCDGuard::lcdMutex;
    pthread_mutex_t M5LCDGuard::counterMutex;

    M5LCDGuard::M5LCDGuard() : M5Lcd(M5.Lcd) {
        if(!M5LCDGuard::isInit) {
            Serial.println("ERROR : LCD LOCK NOT INITIALIZED BUT CONSTRUCTED, UNDEFINED BEHAVIOR !!!");
        }

        pthread_mutex_lock(&this->counterMutex);
        this->nbInstances++;
        pthread_mutex_unlock(&this->counterMutex);
        pthread_mutex_lock(&this->lcdMutex); // stop other thread from fully instantiating this class
    }

    // calling this function in a concurrent context is UB
    // creating an instance of M5LCDGuard before calling this function is also UB
    void M5LCDGuard::init() {
        if(!M5LCDGuard::isInit){
            pthread_mutex_init(&lcdMutex, NULL);
            pthread_mutex_init(&counterMutex, NULL);
            
            nbInstances = 0;
            M5LCDGuard::isInit = true;
        }
    }

    void M5LCDGuard::destroy() {
        pthread_mutex_lock(&counterMutex);

        if(nbInstances > 0) {
            // abort destroy if there are still instances of M5LCDGuard alive
            pthread_mutex_unlock(&counterMutex);
            return;
        }

        pthread_mutex_lock(&lcdMutex);
        pthread_mutex_unlock(&lcdMutex);
        pthread_mutex_destroy(&lcdMutex);
        
        pthread_mutex_unlock(&counterMutex);
        pthread_mutex_destroy(&counterMutex);
        M5LCDGuard::isInit = false;
    }

    M5LCDGuard::~M5LCDGuard() {
        pthread_mutex_lock(&this->counterMutex);
        pthread_mutex_unlock(&this->lcdMutex);
        this->nbInstances--;
        pthread_mutex_unlock(&this->counterMutex);
    }

    M5Display& M5LCDGuard::getLcd() {
        // to run this method we need an object M5LCDGuard
        // and to construct this object we have to lock the mutex
        // thus using this reference to access the lcd makes it thread safe
        return this->M5Lcd;
    }



    /**
     * @brief fonction pour cacher l'écran HOME
     * @param rien
     * @return rien
     */
    void hideHomeScreen(){
        currentScreen = screens::NONE;
        { //lifetime for the lcd lock
            M5LCDGuard LcdGuard;
            M5Display& LcdDisplay = LcdGuard.getLcd();
            boutonConfig.hide();
            LcdDisplay.clear();
        }
        M5LCDGuard::destroy();
    }

    /**
     * @brief fonction pour afficher l'écran HOME
     * @param rien
     * @return rien
     */
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

        M5.Lcd.drawFloat(0.0f, 5, TOUCH_W - 50, TOUCH_H/2 - 90);
        M5.Lcd.drawFloat(0.0f, 5, TOUCH_W - 50, TOUCH_H/2 - 60);
        M5.Lcd.drawFloat(0.0f, 5, TOUCH_W - 50, TOUCH_H/2 - 30);
        M5.Lcd.drawFloat(0.0f, 5, TOUCH_W - 50, TOUCH_H/2);
        M5.Lcd.drawFloat(0.0f, 5, TOUCH_W - 50, TOUCH_H/2 + 30);
        M5.Lcd.drawFloat(0.0f, 5, TOUCH_W - 50, TOUCH_H/2 + 60);

        M5LCDGuard::init();
        currentScreen = screens::HOME;
    }



    /**
     * @brief fonction pour cacher l'écran CONFIG
     * @param rien
     * @return rien
     */
    void hideConfigScreen(){
        currentScreen = screens::NONE;
        boutonHome.hide();
        boutonIdBateau.hide();
        boutonIdCourse.hide();
        M5.Lcd.clear();
    }

    /**
     * @brief fonction pour afficher l'écran CONFIG
     * @param rien
     * @return rien
     */
    void drawConfigScreen(){

        boutonHome.draw();
        boutonIdBateau.draw();
        boutonIdCourse.draw();
        M5.Lcd.drawJpg(img_boat.img, img_boat.size, boutonHome.x, boutonHome.y, 0,0,0,0, JPEG_DIV_2);
        currentScreen = screens::CONFIG;
    }



    /**
     * @brief fonction pour cacher l'écran ID BATEAU
     * @param rien
     * @return rien
     */
    void hideIdBateauScreen(){

        currentScreen = screens::NONE;
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

    /**
     * @brief fonction pour afficher l'écran ID BATEAU
     * @param rien
     * @return rien
     */
    void drawIdBateauScreen(){

        boutonCancelId.draw();
        M5.Lcd.drawJpg(img_cancel.img, img_cancel.size, boutonCancelId.x, boutonCancelId.y, 0,0,0,0, JPEG_DIV_2);

        boutonConfirmIdBateau.draw();
        M5.Lcd.drawJpg(img_confirm.img, img_confirm.size, boutonConfirmIdBateau.x, boutonConfirmIdBateau.y, 0,0,0,0, JPEG_DIV_2);

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



    /**
     * @brief fonction pour cacher l'écran ID COURSE
     * @param rien
     * @return rien
     */
    void hideIdCourseScreen(){

        currentScreen = screens::NONE;
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

    /**
     * @brief fonction pour afficher l'écran ID COURSE
     * @param rien
     * @return rien
     */
    void drawIdCourseScreen(){

        boutonCancelId.draw();
        M5.Lcd.drawJpg(img_cancel.img, img_cancel.size, boutonCancelId.x, boutonCancelId.y, 0,0,0,0, JPEG_DIV_2);

        boutonConfirmIdCourse.draw();
        M5.Lcd.drawJpg(img_confirm.img, img_confirm.size, boutonConfirmIdCourse.x, boutonConfirmIdCourse.y, 0,0,0,0, JPEG_DIV_2);

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
    /**
     * @brief fonction pour afficher ce que l'utilisateur est entrain d'écrire
     * @param rien
     * @return rien
     */
    void drawBuffer(){

        M5.Lcd.setTextSize(2);
        M5.Lcd.fillRect(TOUCH_W/2 - 50, TOUCH_H/2 - 70, 190, 40, 0);
        M5.Lcd.drawNumber(inputBuffer.toInt(), TOUCH_W/2 + 50, TOUCH_H/2 - 50);
        M5.Lcd.setTextSize(1);
    }



    static bool isInit = false;
    /**
     * @brief fonction pour initialiser l'ihm
     * @param rien
     * @return rien
     */
    void init(){

        if(isInit) return;

        M5.Lcd.setTextColor(MY_ORANGE);
        M5LCDGuard::init();

        boutonConfig.addHandler([](Event& e){
            M5LCDGuard LcdGuard;
            M5Display& LcdDisplay = LcdGuard.getLcd();
            LcdDisplay.drawJpg(img_config_touched.img, img_config_touched.size, boutonConfig.x, boutonConfig.y, 0,0,0,0, JPEG_DIV_2);
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


        // TODO : il est possible de fusionner les deux écran de changement d'id en un seul écran générique
        boutonConfirmIdBateau.addHandler([](Event& e){
            M5.Lcd.drawJpg(img_confirm_touched.img, img_confirm_touched.size, boutonConfirmIdBateau.x, boutonConfirmIdBateau.y, 0,0,0,0, JPEG_DIV_2);
        }, E_TOUCH);
        boutonConfirmIdBateau.addHandler([](Event& e){
                if(!inputBuffer.isEmpty()){
                    Navi.set_id_bateau(inputBuffer.toInt());
                    inputBuffer = "";
                }
            
                hideIdBateauScreen();
                drawConfigScreen();
        }, E_RELEASE);


        boutonConfirmIdCourse.addHandler([](Event& e){
            M5.Lcd.drawJpg(img_confirm_touched.img, img_confirm_touched.size, boutonConfirmIdCourse.x, boutonConfirmIdCourse.y, 0,0,0,0, JPEG_DIV_2);
        }, E_TOUCH);
        boutonConfirmIdCourse.addHandler([](Event& e){
            if(!inputBuffer.isEmpty()){
                Navi.set_id_course(inputBuffer.toInt());
                inputBuffer = "";
            }

            hideIdCourseScreen();
            drawConfigScreen();
        }, E_RELEASE);


        boutonCancelId.addHandler([](Event& e){
            M5.lcd.drawJpg(img_cancel_touched.img, img_cancel_touched.size, boutonCancelId.x, boutonCancelId.y, 0,0,0,0, JPEG_DIV_2);
        }, E_TOUCH);
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

        xTaskCreate(updateHomeScreen, "UPDATE DATA", 4096, NULL, 3, NULL);

        isInit = true;
        Serial.println("IHM STARTED");
    }


    /**
     * @brief fonction pour mettre a jours les données sur l'écran HOME
     * @param void* un faux paramètre pour pouvoir ce servir de cette fonction dans une task freeRTOS
     * @return rien
     */
    void updateHomeScreen(void*){

        constexpr int update_delay = 500; // ms
        while(true){
            if(currentScreen == screens::HOME){
                navi::data_navi data = Navi.get_data();
                if(data.data_content == 0) { // no new data to display
                    vTaskDelay(update_delay / portTICK_PERIOD_MS);
                    continue;
                }

                M5LCDGuard lcdGuard;
                M5Display& LcdDisplay = lcdGuard.getLcd();

                if(data.data_content & navi::data_navi_content::WINDANGLE) {
                    M5.Lcd.fillRect(TOUCH_W - 90, TOUCH_H/2-96, 90, 15, 0);
                    LcdDisplay.drawFloat(data.windAngle, 5, TOUCH_W - 50, TOUCH_H/2 - 90);
                }

                if(data.data_content & navi::data_navi_content::WINDSPEED) {
                    M5.Lcd.fillRect(TOUCH_W - 90, TOUCH_H/2-66, 90, 15, 0);
                    LcdDisplay.drawFloat(data.windSpeed, 5, TOUCH_W - 50, TOUCH_H/2 - 60);
                }

                if(data.data_content & navi::data_navi_content::SPEED) {
                    M5.Lcd.fillRect(TOUCH_W - 90, TOUCH_H/2-36, 90, 15, 0);
                    LcdDisplay.drawFloat(data.speed, 5, TOUCH_W - 50, TOUCH_H/2 - 30);
                }

                if(data.data_content & navi::data_navi_content::LATITUDE) {
                    M5.Lcd.fillRect(TOUCH_W - 90, TOUCH_H/2-6, 90, 15, 0);
                    LcdDisplay.drawFloat(data.latitude, 5, TOUCH_W - 50, TOUCH_H/2);
                }

                if(data.data_content & navi::data_navi_content::LONGITUDE) {
                    M5.Lcd.fillRect(TOUCH_W - 90, TOUCH_H/2+24, 90, 15, 0);
                    LcdDisplay.drawFloat(data.longitude, 5, TOUCH_W - 50, TOUCH_H/2 + 30);
                }

                if(data.data_content & navi::data_navi_content::HEADING) {
                    M5.Lcd.fillRect(TOUCH_W - 90, TOUCH_H/2+54, 90, 15, 0);
                    LcdDisplay.drawFloat(data.heading, 5, TOUCH_W - 50, TOUCH_H/2 + 60);
                }
            }

            vTaskDelay(update_delay / portTICK_PERIOD_MS);
        }
    }

}