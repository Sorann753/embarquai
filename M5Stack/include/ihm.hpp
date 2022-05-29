#ifndef IHM_HPP
#define IHM_HPP

#include <M5Core2.h>
#include "navi.hpp"

extern const unsigned char dat_config[5986];
extern const unsigned char dat_config_touched[5115];
extern const unsigned char dat_boat[5523];
extern const unsigned char dat_boat_touched[5247];

constexpr uint16_t MY_ORANGE = 0xFC00; //RGB 16bit -> 0xFF8100 RGB 24bit
constexpr ButtonColors INVISIBLE = {NODRAW, NODRAW, NODRAW};

struct image{
    const unsigned char* img = nullptr;
    size_t size = 0;
};

enum screens{

    HOME,
    CONFIG,
    SET_ID_BATEAU,
    SET_ID_COURSE
};

void init_IHM();


void hideHomeScreen();

void drawHomeScreen();

void hideConfigScreen();

void drawConfigScreen();



extern uint8_t currentScreen;

#endif