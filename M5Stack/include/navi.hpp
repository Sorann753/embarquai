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
#include <NMEA2000_CAN.h>  // This will automatically choose right CAN library and create suitable NMEA2000 object
#include <N2kMsg.h>
#include <NMEA2000.h>
#include <N2kMessages.h>



namespace navi{

    /*
    KNOWN PGN LIST : 
    PGN : 60928  -> ISO Address Claim
    PGN : 130306 -> wind data
    PGN : 127251 -> rate of turn
    PGN : 127250 -> magnetic heading
    PGN : 129025 -> position GPS
    PGN : 129026 -> data GPS (direction + speed)
    */

    /* @brief fr */
    class Navi{

    public:

        Navi() = default;
        ~Navi() = default;
        
        Navi(Navi&&) = delete;
        Navi(const Navi&) = delete;
        Navi& operator=(Navi&&) = delete;
        Navi& operator=(const Navi&) = delete;

        void begin();
        bool handle(const tN2kMsg &N2kMsg);
        void get_nmea_data();

    private:

        void handleHeading(const tN2kMsg& N2kMsg) const noexcept; // 127250L
        void handleRateOfTurn(const tN2kMsg& N2kMsg) const noexcept; // 127251L
        void handlePosition(const tN2kMsg& N2kMsg) const noexcept; // 129025L
        void handleCogSog(const tN2kMsg& N2kMsg) const noexcept; // 129026L
        void handleWind(const tN2kMsg& N2kMsg) const noexcept; // 130306L
    } Navi;



    //NMEA 2000 message handler
    void HandleNMEA2000Msg(const tN2kMsg &N2kMsg) {

        Navi.handle(N2kMsg);
    }
}

using navi::Navi;

#endif //NAVI_HPP