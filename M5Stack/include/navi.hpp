/**
 * @brief 
 * @author DORIATH ARTHUS
 * @date 05/05/2022
 * @version 0.1
 */

#ifndef NAVI_HPP
#define NAVI_HPP

//PORT B
#define ESP32_CAN_TX_PIN GPIO_NUM_26
#define ESP32_CAN_RX_PIN GPIO_NUM_36

#include <M5Core2.h>
#include <N2kMsg.h>
#include <NMEA2000.h>
#include <N2kMessages.h>



namespace navi{

    /**
     *  @brief classe qui gère la communication avec le matériel NMEA2000
     */
    class c_Navi{

    public:

        c_Navi() = default;
        ~c_Navi() = default;
        
        //on empêche la copie ou le move de cette classe
        c_Navi(c_Navi&&) = delete;
        c_Navi(const c_Navi&) = delete;
        c_Navi& operator=(c_Navi&&) = delete;
        c_Navi& operator=(const c_Navi&) = delete;

        void begin();
        void handle(const tN2kMsg &N2kMsg);
        void get_nmea_data();

    private:



        //fonctions privés
        void handleHeading(const tN2kMsg& N2kMsg) noexcept; // 127250L
        void handlePosition(const tN2kMsg& N2kMsg) noexcept; // 129025L
        void handleCogSog(const tN2kMsg& N2kMsg) noexcept; // 129026L
        void handleWind(const tN2kMsg& N2kMsg) noexcept; // 130306L
    };

    extern c_Navi Navi;
}

using navi::Navi;

#endif //NAVI_HPP