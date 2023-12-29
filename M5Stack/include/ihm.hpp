#ifndef IHM_HPP
#define IHM_HPP

#include <M5Core2.h>
#include "navi.hpp"

//liste des images pour l'ihm
extern const unsigned char jpg_config[5986];
extern const unsigned char jpg_config_touched[5115];
extern const unsigned char jpg_boat[5523];
extern const unsigned char jpg_boat_touched[5247];
extern const unsigned char jpg_confirm[4281];
extern const unsigned char jpg_confirm_touched[3690];
extern const unsigned char jpg_cancel[4381];
extern const unsigned char jpg_cancel_touched[3881];

namespace ihm{

    constexpr uint16_t MY_ORANGE = 0xFC00; //RGB 16bit -> 0xFF8100 RGB 24bit
    constexpr ButtonColors INVISIBLE = {NODRAW, NODRAW, NODRAW};

    /**
     * @brief une structure pour représenté une image
     */
    struct image{
        const unsigned char* img = nullptr;
        size_t size = 0;
    };

    //l'énumération des différents écrans possibles
    enum screens{
        NONE,
        HOME,
        CONFIG,
        SET_ID_BATEAU,
        SET_ID_COURSE
    };



    /* un warper autour du lcd du M5Stack pour le rendre thread safe */
    class M5LCDGuard {
    public:

        M5LCDGuard();
        static void init();
        static void destroy();

        //on empêche la copie ou le move de cette classe
        M5LCDGuard(M5LCDGuard&&) = delete;
        M5LCDGuard(const M5LCDGuard&) = delete;
        M5LCDGuard& operator=(M5LCDGuard&&) = delete;
        M5LCDGuard& operator=(const M5LCDGuard&) = delete;

        ~M5LCDGuard();

        M5Display& getLcd();

    private:
        static pthread_mutex_t lcdMutex;
        static pthread_mutex_t counterMutex;
        static bool isInit;
        static size_t nbInstances;
        M5Display& M5Lcd;
    };



    /**
     * @brief fonction pour initialiser l'ihm
     * @param rien
     * @return rien
     */
    void init();



    /**
     * @brief fonction pour cacher l'écran HOME
     * @param rien
     * @return rien
     */
    void hideHomeScreen();

    /**
     * @brief fonction pour afficher l'écran HOME
     * @param rien
     * @return rien
     */
    void drawHomeScreen();



    /**
     * @brief fonction pour cacher l'écran CONFIG
     * @param rien
     * @return rien
     */
    void hideConfigScreen();

    /**
     * @brief fonction pour afficher l'écran CONFIG
     * @param rien
     * @return rien
     */
    void drawConfigScreen();



    /**
     * @brief fonction pour cacher l'écran SET_ID_BATEAU
     * @param rien
     * @return rien
     */
    void hideIdBateauScreen();

    /**
     * @brief fonction pour afficher l'écran SET_ID_BATEAU
     * @param rien
     * @return rien
     */
    void drawIdBateauScreen();



    /**
     * @brief fonction pour cacher l'écran SET_ID_COURSE
     * @param rien
     * @return rien
     */
    void hideIdCourseScreen();

    /**
     * @brief fonction pour afficher l'écran SET_ID_COURSE
     * @param rien
     * @return rien
     */
    void drawIdCourseScreen();



    /**
     * @brief fonction pour afficher ce que l'utilisateur est entrain d'écrire
     * @param rien
     * @return rien
     */
    void drawBuffer();



    /**
     * @brief fonction pour mettre a jours les données sur l'écran HOME
     * @param void* un faux paramètre pour pouvoir ce servir de cette fonction dans une task freeRTOS
     * @return rien
     */
    void updateHomeScreen(void*);


    //une variable pour stocker l'écran actuel
    extern uint8_t currentScreen;
}

#endif