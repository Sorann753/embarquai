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
#include "data_navi.hpp"



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
        void fetch_nmea_data();

        /**
         * @brief change l'identifiant du bateau
         * @param newId le nouvel identifiant du bateau
         * @return rien
         * @throw throw std::invalid_argument si newId < 0
         */
        void set_id_bateau(int32_t newId);

        /**
         * @brief change l'identifiant de la course a la quel participe le bateau
         * @param newId le nouvel identifiant de la course
         * @return rien
         * @throw throw std::invalid_argument si newId < 0
         */
        void set_id_course(int32_t newId);

        /**
         * @brief
         * @param
         * @return
         */
        int32_t get_id_bateau();

        /**
         * @brief
         * @param
         * @return 
         */
        int32_t get_id_course();



        /**
         * @brief
         * @param
         * @return
         */
        [[nodiscard]] data_navi pop_data();

        /**
         * @brief
         * @param
         * @return
         */
        data_navi get_data();

    private:

        //dernières données qui n'ont pas encore été envoyées
        data_navi _data{};

        //fonctions privés
        void handleHeading(const tN2kMsg& N2kMsg) noexcept; // 127250L
        void handleBoatSpeed(const tN2kMsg& N2kMsg) noexcept; // 128259L
        void handlePosition(const tN2kMsg& N2kMsg) noexcept; // 129025L
        void handleCogSog(const tN2kMsg& N2kMsg) noexcept; // 129026L
        void handleWind(const tN2kMsg& N2kMsg) noexcept; // 130306L
    };

    extern c_Navi Navi;
}

using navi::Navi;

#endif //NAVI_HPP