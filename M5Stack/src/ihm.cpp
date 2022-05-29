#include "ihm.hpp"

image img_config{dat_config, 5986};
image img_config_touched{dat_config_touched, 5115};
image img_boat{dat_boat, 5523};
image img_boat_touched{dat_boat_touched, 5247};

Button boutonConfig(0, 0, 50, 50, false, "CONFIG",
               INVISIBLE, INVISIBLE);

Button boutonHome(TOUCH_W - 50, 0, 50, 50, false, "HOME",
               INVISIBLE, INVISIBLE);

Button boutonIdBateau(TOUCH_W - 250, TOUCH_H - 135, 200, 50, false, "ID BATEAU",
               {BLACK, MY_ORANGE, MY_ORANGE}, {MY_ORANGE, BLACK, MY_ORANGE});

Button boutonIdCourse(TOUCH_W - 250, TOUCH_H - 215, 200, 50, false, "ID COURSE",
               {BLACK, MY_ORANGE, MY_ORANGE}, {MY_ORANGE, BLACK, MY_ORANGE});



Button boutonNumber0(TOUCH_W/2 - 50, TOUCH_H/2 - 50, 50, 50, false, "NUMBER 1",
               {BLACK, MY_ORANGE, MY_ORANGE}, {MY_ORANGE, BLACK, MY_ORANGE});

Button boutonNumber1(TOUCH_W/2 - 50, TOUCH_H/2 - 50, 50, 50, false, "NUMBER 1",
               {BLACK, MY_ORANGE, MY_ORANGE}, {MY_ORANGE, BLACK, MY_ORANGE});

Button boutonNumber2(TOUCH_W/2 - 50, TOUCH_H/2 - 50, 50, 50, false, "NUMBER 1",
               {BLACK, MY_ORANGE, MY_ORANGE}, {MY_ORANGE, BLACK, MY_ORANGE});

Button boutonNumber3(TOUCH_W/2 - 50, TOUCH_H/2 - 50, 50, 50, false, "NUMBER 1",
               {BLACK, MY_ORANGE, MY_ORANGE}, {MY_ORANGE, BLACK, MY_ORANGE});

Button boutonNumber4(TOUCH_W/2 - 50, TOUCH_H/2 - 50, 50, 50, false, "NUMBER 1",
               {BLACK, MY_ORANGE, MY_ORANGE}, {MY_ORANGE, BLACK, MY_ORANGE});

Button boutonNumber5(TOUCH_W/2 - 50, TOUCH_H/2 - 50, 50, 50, false, "NUMBER 1",
               {BLACK, MY_ORANGE, MY_ORANGE}, {MY_ORANGE, BLACK, MY_ORANGE});

Button boutonNumber6(TOUCH_W/2 - 50, TOUCH_H/2 - 50, 50, 50, false, "NUMBER 1",
               {BLACK, MY_ORANGE, MY_ORANGE}, {MY_ORANGE, BLACK, MY_ORANGE});

Button boutonNumber7(TOUCH_W/2 - 50, TOUCH_H/2 - 50, 50, 50, false, "NUMBER 1",
               {BLACK, MY_ORANGE, MY_ORANGE}, {MY_ORANGE, BLACK, MY_ORANGE});

Button boutonNumber8(TOUCH_W/2 - 50, TOUCH_H/2 - 50, 50, 50, false, "NUMBER 1",
               {BLACK, MY_ORANGE, MY_ORANGE}, {MY_ORANGE, BLACK, MY_ORANGE});

Button boutonNumber9(TOUCH_W/2 - 50, TOUCH_H/2 - 50, 50, 50, false, "NUMBER 1",
               {BLACK, MY_ORANGE, MY_ORANGE}, {MY_ORANGE, BLACK, MY_ORANGE});

uint8_t currentScreen;



void hideHomeScreen(){

    boutonConfig.hide();
    M5.lcd.clear();
}

void drawHomeScreen(){

    boutonConfig.draw();
    M5.lcd.drawJpg(img_config.img, img_config.size, 0, 0, 0,0,0,0, JPEG_DIV_2);
    // M5.Lcd.drawString("PROGRESS BAR", TOUCH_W / 2, TOUCH_H/2 - 30);
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

void init_IHM(){

    boutonConfig.addHandler([](Event& e){
        Serial.println("A");
        M5.lcd.drawJpg(img_config_touched.img, img_config_touched.size, boutonConfig.x, boutonConfig.y, 0,0,0,0, JPEG_DIV_2);
    }, E_TOUCH);
    boutonConfig.addHandler([](Event& e){
        hideHomeScreen();
        drawConfigScreen();
    }, E_RELEASE);


    // boutonTEST.setLabel("3");
    // boutonTEST.setTextSize(2);
    // boutonTEST.addHandler([](Event& e){
    //     Serial.println("1");
    // }, E_TOUCH);


    boutonIdBateau.setLabel("CHANGER ID BATEAU");
    boutonIdBateau.addHandler([](Event& e){
        Serial.println("B");
    }, E_TOUCH);


    boutonIdCourse.setLabel("CHANGER ID COURSE");
    boutonIdCourse.addHandler([](Event& e){
        Serial.println("C");
    }, E_TOUCH);


    boutonHome.addHandler([](Event& e){
        M5.Lcd.drawJpg(img_boat_touched.img, img_boat_touched.size, boutonHome.x, boutonHome.y, 0,0,0,0, JPEG_DIV_2);
    }, E_TOUCH);
    boutonHome.addHandler([](Event& e){
        hideConfigScreen();
        drawHomeScreen();
    }, E_RELEASE);



    //par défault les autres boutons ne sont pas désactivé,
    //du coup il faut les désactivé a l'initialisation
    hideConfigScreen();

    //on démare le programme sur l'écran principale
    drawHomeScreen();

    Serial.println("IHM STARTED");
}