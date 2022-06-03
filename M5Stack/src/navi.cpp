/**
 * @brief 
 * @author DORIATH ARTHUS
 * @date 05/05/2022
 * @version 0.1
 */

#include "navi.hpp"
#include <NMEA2000_CAN.h>  // choisi automatiquement la bonne librairie CAN et construit le bon objet NMEA2000

namespace navi{

    bool started = false;

    /**
    * @brief méthode qui extrait le cap du bateau
    * @param N2kMsg un message NMEA2000
    * @return rien
    */
    void c_Navi::handleHeading(const tN2kMsg& N2kMsg) noexcept{
    
        unsigned char messageIdentifier; // == SID
        double heading = 0;
        double deviation = 0; //unused
        double variation = 0; //unused
        tN2kHeadingReference headingType;

        if(ParseN2kHeading(N2kMsg, messageIdentifier, heading, deviation, variation, headingType) ){

            if(!N2kIsNA(heading)){

                this->_data.heading = heading;
                this->_data.data_content |= navi::data_navi_content::HEADING;
            }
        }
    }



    /**
    * @brief méthode qui extrait la vitesse du bateau
    * @param N2kMsg un message NMEA2000
    * @return rien
    */
    void c_Navi::handleBoatSpeed(const tN2kMsg& N2kMsg) noexcept{

        unsigned char SID = 0;
        double speed_to_water = 0;
        double speed_to_ground = 0;
        tN2kSpeedWaterReferenceType sensorType{};
        if(ParseN2kBoatSpeed(N2kMsg, SID, speed_to_water, speed_to_ground, sensorType)){

            if(!N2kIsNA(speed_to_water)){
                this->_data.speed = speed_to_water;
                this->_data.data_content |= navi::data_navi_content::SPEED;
            }
            else if(!N2kIsNA(speed_to_ground)){
                this->_data.speed = speed_to_ground;
                this->_data.data_content |= navi::data_navi_content::SPEED;
            }
        }
    }



    /**
    * @brief méthode qui extrait les information de position et les enregistre
    * @param N2kMsg un message NMEA2000
    * @return rien
    */
    void c_Navi::handlePosition(const tN2kMsg& N2kMsg) noexcept{
    
        double latitude = 0;
        double longitude = 0;

        if(ParseN2kPositionRapid(N2kMsg, latitude, longitude) ){

            if(!N2kIsNA(latitude)){
                this->_data.latitude = latitude;
                this->_data.data_content |= navi::data_navi_content::LATITUDE;
            }

            if(!N2kIsNA(longitude)){
                this->_data.longitude = longitude;
                this->_data.data_content |= navi::data_navi_content::LONGITUDE;
            }
        }
    }



    /**
    * @brief méthode qui utilise le COG SOG pour récupéré les informations de vitesse et de cap si celles-ci ne sont
    * pas récupéré autrement, permet de quand même avoir une vitesse même si le capteur de vitesse n'est pas installé
    * @param N2kMsg un message NMEA2000
    * @return rien
    */
    void c_Navi::handleCogSog(const tN2kMsg& N2kMsg) noexcept{
    
        unsigned char messageIdentifier; // == SID
        tN2kHeadingReference headingType;
        double COG = 0; // Cource Over Ground (peut remplacé le heading)
        double SOG = 0; // Speed Over Ground (peut remplacé la speed_to_water)
        if(ParseN2kCOGSOGRapid(N2kMsg, messageIdentifier, headingType, COG, SOG) ){

            //on utilise le COG pour remplacer le heading si celui-ci est manquant
            if(!(_data.data_content & navi::data_navi_content::HEADING)){            
                if(!N2kIsNA(COG)){
                    this->_data.heading = COG;
                    this->_data.data_content |= navi::data_navi_content::HEADING;
                }
            }

            //on utilise la SOG pour remplacer la speed si celle-ci est manquante
            if(!(_data.data_content & navi::data_navi_content::SPEED)){
                if(!N2kIsNA(SOG)){
                    this->_data.speed = SOG;
                    this->_data.data_content |= navi::data_navi_content::SPEED;
                }
            }
        }
    }



    /**
    * @brief méthode qui extrait les données du vent
    * @param N2kMsg un message NMEA2000
    * @return rien
    */
    void c_Navi::handleWind(const tN2kMsg& N2kMsg) noexcept{
    
        unsigned char messageIdentifier; // == SID
        double windSpeed = 0;
        double windAngle = 0;
        tN2kWindReference windType;

        if(ParseN2kWindSpeed(N2kMsg, messageIdentifier, windSpeed, windAngle, windType) ){

            if(!N2kIsNA(windSpeed)){
                this->_data.windSpeed = windSpeed;
                this->_data.data_content |= data_navi_content::WINDSPEED;
            }

            if(!N2kIsNA(windAngle)){
                this->_data.windAngle = windAngle;
                this->_data.data_content |= data_navi_content::WINDANGLE;
            }
        }
    }



    /**
    * @brief méthode qui démarre la lecture des messages NMEA2000
    * @param rien
    * @return rien
    */
    void c_Navi::begin(){
    
        //s'assure qu'on n'ai pas déjà appelé begin
        if(started) return;  

        //on initialise l'interface avec la mémoire flash
        _preferences.begin("navi", false);

        //récupère les id depuis la mémoire flash
        _data.id_bateau = _preferences.getInt("id_bateau", -1);
        _data.id_course = _preferences.getInt("id_course", -1);

        NMEA2000.EnableForward(false);
        NMEA2000.SetMsgHandler([](const tN2kMsg &N2kMsg){
            Navi.handle(N2kMsg);
        });

        Serial.println();
        while(!NMEA2000.Open()){
            Serial.println("CAN ERROR : can't Open");
            delay(1000);
        }
        Serial.println("CAN OPEN : NAVI STARTED");

        started = true;
    }



    /**
    * @brief méthode qui demande a l'objet NMEA2000 de parser les messages NMEA2000
    * @param rien
    * @return rien
    */
    void c_Navi::fetch_nmea_data(){

        if(!started) return;

        NMEA2000.ParseMessages();
    }



    /**
    * @brief méthode qui décide quel handler utiliser pour parser le message NMEA2000
    * @param NmeaMessage un message NMEA2000
    * @return rien
    */
    void c_Navi::handle(const tN2kMsg &NmeaMessage){
    
        /*
        ** KNOWN PGN LIST : 
        ** PGN : 60928  -> ISO Address Claim (unused)
        ** PGN : 127250 -> magnetic heading
        ** PGN : 127251 -> rate of turn (unused)
        ** PGN : 128259 -> boat speed (vitesse du bateau sur l'eau)
        ** PGN : 128267 -> water depth (unused)
        ** PGN : 129025 -> position GPS
        ** PGN : 129026 -> COG SOG (vitesse et orientation calculé a partir des data GPS)
        ** PGN : 130306 -> wind data
        ** PGN : 130311 -> temperatures (unused)
        **/

        switch(NmeaMessage.PGN){

            case 127250L:
                this->handleHeading(NmeaMessage);
            break;

            case 128259:
                this->handleBoatSpeed(NmeaMessage);
            break;

            case 129025L:
                this->handlePosition(NmeaMessage);
            break;

            case 129026L:
                this->handleCogSog(NmeaMessage);
            break;

            case 130306L:
                this->handleWind(NmeaMessage);
            break;

            default: //si le PGN n'est pas connu

                // Serial.print("Unknown message from : ");
                // Serial.println((int)NmeaMessage.Source);

                // Serial.print("PGN : ");
                // Serial.println(NmeaMessage.PGN);

                // Serial.print("destination : ");
                // Serial.println((int)NmeaMessage.Destination);

                // Serial.print("priority : ");
                // Serial.println((int)NmeaMessage.Priority);

                // Serial.print("nb bytes : ");
                // Serial.println(NmeaMessage.DataLen);
                
                // Serial.print("msg time : ");
                // Serial.println(NmeaMessage.MsgTime);

                // Serial.println( (NmeaMessage.IsValid())? "message is valid" : "message is invalid");

                // Serial.println();
            break;
        }
    }



    /**
     * @brief change l'identifiant du bateau
     * @param newId le nouvel identifiant du bateau
     * @return rien
     * @throw throw std::invalid_argument si newId < 0
     */
    void c_Navi::set_id_bateau(int32_t newId){

        if(newId < 0){
            throw std::invalid_argument("id bateau invalide");
        }

        this->_data.id_bateau = newId;

        //enregistre le nouvel id bateau dans la mémoire flash
        _preferences.putInt("id_bateau", newId);
    }

    /**
     * @brief change l'identifiant de la course a la quel participe le bateau
     * @param newId le nouvel identifiant de la course
     * @return rien
     * @throw throw std::invalid_argument si newId < 0
     */
    void c_Navi::set_id_course(int32_t newId){

        if(newId < 0){
            throw std::invalid_argument("id course invalide");
        }

        this->_data.id_course = newId;

        //enregistre le nouvel id course dans la memoire flash
        _preferences.putInt("id_course", newId);
    }

    /**
     * @brief récupère l'identifiant du bateau
     * @param rien
     * @return l'identifiant du bateau
     */
    int32_t c_Navi::get_id_bateau(){

        if(_data.id_bateau == -1){
            _data.id_bateau = _preferences.getInt("id_bateau", -1);
        }

        return _data.id_bateau;
    }

    /**
     * @brief récupère l'identifiant de la course
     * @param rien
     * @return l'identifiant de la course
     */
    int32_t c_Navi::get_id_course(){

        if(_data.id_course == -1){
            _data.id_course = _preferences.getInt("id_course", -1);
        }

        return _data.id_course;
    }



    [[nodiscard]] data_navi c_Navi::pop_data(){

        //si il n'y a pas de données alors on ne s'embête pas a réinitialisé _data
        if(this->_data.data_content == 0) return this->_data;

        data_navi poped_data = this->_data;
        this->_data.data_content = 0;

        return poped_data;
    }



    data_navi c_Navi::get_data(){
        return _data;
    }

    //on construit un objet Navi pour pouvoir l'utilisé de manière globale
    c_Navi Navi;
}
