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
    * @brief 
    * @param 
    * @return 
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
    * @brief 
    * @param 
    * @return 
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
    * @brief 
    * @param 
    * @return 
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
    * @brief 
    * @param 
    * @return 
    */
    void c_Navi::handleCogSog(const tN2kMsg& N2kMsg) noexcept{
    
        unsigned char messageIdentifier; // == SID
        tN2kHeadingReference headingType;
        double COG = 0; // Cource Over Ground (peut remplacé le heading)
        double SOG = 0; // Speed Over Ground (peut remplacé la speed_to_water)
        if(ParseN2kCOGSOGRapid(N2kMsg, messageIdentifier, headingType, COG, SOG) ){

            //TODO faire en sorte que le COG SOG puisse remplacé la vitesse si besoin
            // switch (headingType){
            //     case tN2kHeadingReference::N2khr_error :
            //         Serial.println("Error : heading type error");
            //     break;

            //     case tN2kHeadingReference::N2khr_magnetic :
            //         Serial.println("orientation based on the magnetic north");
            //     break;

            //     case tN2kHeadingReference::N2khr_true :
            //         Serial.println("orientation based on the true north");
            //     break;

            //     case tN2kHeadingReference::N2khr_Unavailable :
            //         Serial.println("can't get heading type");
            //     break;
            // }

            // Serial.print("Course over ground : ");
            // Serial.println(COG);

            // Serial.print("Speed over ground : ");
            // Serial.println(SOG);
        }
        // Serial.println();
    }



    /**
    * @brief 
    * @param 
    * @return 
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
    * @brief 
    * @param 
    * @return 
    */
    void c_Navi::begin(){
    
        //s'assure qu'on n'ai pas déjà appelé begin
        if(started) return;

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
    * @brief 
    * @param 
    * @return 
    */
    void c_Navi::fetch_nmea_data(){

        if(!started) return;

        NMEA2000.ParseMessages();
    }



    /**
    * @brief 
    * @param 
    * @return 
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
    }

    //TODO : utiliser Preferences.h pour enregistré les id_bateau et id_course dans la memoire flash
    //#include <Preferences.h>

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
    }

    /**
     * @brief
     * @param
     * @return
     */
    int32_t c_Navi::get_id_bateau(){

        return this->_data.id_bateau;
    }

    /**
     * @brief
     * @param
     * @return 
     */
    int32_t c_Navi::get_id_course(){

        return this->_data.id_course;
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
