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
#include <Preferences.h>



namespace navi{

    /**
     *  @brief classe qui gère la communication avec le matériel NMEA2000
     */
    class c_Navi{

    public:

        /**
         * @brief constructeur de la classe
         * @param void
         * @return rien
         */
        c_Navi() = default;

        /**
         * @brief destructeur de la classe
         * @param void
         * @return rien
         */
        ~c_Navi() = default;
        
        //on empêche la copie ou le move de cette classe
        c_Navi(c_Navi&&) = delete;
        c_Navi(const c_Navi&) = delete;
        c_Navi& operator=(c_Navi&&) = delete;
        c_Navi& operator=(const c_Navi&) = delete;



        /**
         * @brief fonction qui initialise le module navi
         * @param void
         * @return rien
         */
        void begin();

        /**
        * @brief méthode qui décide quel handler utiliser pour parser le message NMEA2000
        * @param NmeaMessage un message NMEA2000
        * @return rien
        */
        void handle(const tN2kMsg &N2kMsg);

        /**
        * @brief méthode qui demande a l'objet NMEA2000 de parser les messages NMEA2000
        * @param rien
        * @return rien
        */
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
         * @brief récupère l'identifiant du bateau
         * @param rien
         * @return l'identifiant du bateau
         */
        int32_t get_id_bateau();

        /**
         * @brief récupère l'identifiant de la course
         * @param rien
         * @return l'identifiant de la course
         */
        int32_t get_id_course();



        /**
         * @brief extrait les données de _data et réinitialise data_content
         * @param rien
         * @return un objet data_content
         */
        [[nodiscard]] data_navi pop_data();

        /**
         * @brief récupère les données du bateau
         * @param rien
         * @return un objet data_content
         */
        data_navi get_data();

    private:

        //données qui n'ont pas encore été envoyées
        data_navi _data{};

        //interface avec la mémoire flash
        Preferences _preferences;

        

        //méthodes qui parse les messages NMEA2000

        /**
         * @brief méthode qui extrait le cap du bateau
         * @param N2kMsg un message NMEA2000
         * @return rien
         */
        void handleHeading(const tN2kMsg& N2kMsg) noexcept; // 127250L

        /**
         * @brief méthode qui extrait la vitesse du bateau
         * @param N2kMsg un message NMEA2000
         * @return rien
         */
        void handleBoatSpeed(const tN2kMsg& N2kMsg) noexcept; // 128259L

        /**
         * @brief méthode qui extrait les information de position et les enregistre
         * @param N2kMsg un message NMEA2000
         * @return rien
         */
        void handlePosition(const tN2kMsg& N2kMsg) noexcept; // 129025L

        /**
         * @brief méthode qui utilise le COG SOG pour récupéré les informations de vitesse et de cap si celles-ci ne sont
         * pas récupéré autrement, permet de quand même avoir une vitesse même si le capteur de vitesse n'est pas installé
         * @param N2kMsg un message NMEA2000
         * @return rien
         */
        void handleCogSog(const tN2kMsg& N2kMsg) noexcept; // 129026L

        /**
         * @brief méthode qui extrait les données du vent
         * @param N2kMsg un message NMEA2000
         * @return rien
         */
        void handleWind(const tN2kMsg& N2kMsg) noexcept; // 130306L
    };

    extern c_Navi Navi;
}

using navi::Navi;

#endif //NAVI_HPP