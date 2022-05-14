/**
 * @brief 
 * @author DORIATH ARTHUS
 * @date 05/05/2022
 * @version 0.1
 */

#include "navi.hpp"
#include <NMEA2000_CAN.h>  // This will automatically choose right CAN library and create suitable NMEA2000 object

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
        double deviation = 0;
        double variation = 0;
        tN2kHeadingReference headingType;

        if(ParseN2kHeading(N2kMsg, messageIdentifier, heading, deviation, variation, headingType) ){

            switch (headingType){
                case tN2kHeadingReference::N2khr_error :
                    Serial.print("Error : heading type error");
                break;

                case tN2kHeadingReference::N2khr_magnetic :
                    Serial.print("orientation based on the magnetic north");
                break;

                case tN2kHeadingReference::N2khr_true :
                    Serial.print("orientation based on the true north");
                break;

                case tN2kHeadingReference::N2khr_Unavailable :
                    Serial.print("can't get heading type");
                break;
            }

            Serial.print("heading toward : ");
            Serial.print(heading);
            Serial.print(" radians");
            Serial.println();

            Serial.print("deviation : ");
            Serial.print(deviation);
            Serial.print(" radians");
            Serial.println();

            Serial.print("variation : ");
            Serial.print(variation);
            Serial.print(" radians");
            Serial.println();

            Serial.println();
        }
    }



    /**
    * @brief 
    * @param 
    * @return 
    */
    void c_Navi::handleBoatSpeed(const tN2kMsg& N2kMsg) noexcept{

        constexpr double NoGroundSpeedValue = -1000000000.00;
        unsigned char SID = 0;
        double speed_to_water = 0;
        double speed_to_ground = 0;
        tN2kSpeedWaterReferenceType sensorType{};
        if(ParseN2kBoatSpeed(N2kMsg, SID, speed_to_water, speed_to_ground, sensorType)){

            Serial.print("speed to water : ");
            Serial.println(speed_to_water);

            if(speed_to_ground != NoGroundSpeedValue){ //si la vitesse par rapport au sol est connue
                Serial.print("speed to ground : ");
                Serial.println(speed_to_ground);
            }

            Serial.print("type de capteur : ");
            switch(sensorType){

                case N2kSWRT_Paddle_wheel:
                    Serial.println("N2kSWRT_Paddle_wheel");
                break;

                case N2kSWRT_Pitot_tube:
                    Serial.println("N2kSWRT_Pitot_tube");
                break;

                case N2kSWRT_Doppler_log:
                    Serial.println("N2kSWRT_Doppler_log");
                break;

                case N2kSWRT_Ultra_Sound:
                    Serial.println("N2kSWRT_Ultra_Sound");
                break;

                case N2kSWRT_Electro_magnetic:
                    Serial.println("N2kSWRT_Electro_magnetic");
                break;

                case N2kSWRT_Error:
                    Serial.println("N2kSWRT_Error");
                break;

                case N2kSWRT_Unavailable:
                    Serial.println("N2kSWRT_Unavailable");
                break;

                default:
                    Serial.println("???");
                break;
            }

            Serial.println();
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

            Serial.print("latitude : ");
            Serial.println(latitude);

            Serial.print("longitude : ");
            Serial.println(longitude);

            Serial.println();
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
        double COG = 0; // Cource Over Ground
        double SOG = 0; // Speed Over Ground
        if(ParseN2kCOGSOGRapid(N2kMsg, messageIdentifier, headingType, COG, SOG) ){

            switch (headingType){
                case tN2kHeadingReference::N2khr_error :
                    Serial.print("Error : heading type error");
                break;

                case tN2kHeadingReference::N2khr_magnetic :
                    Serial.print("orientation based on the magnetic north");
                break;

                case tN2kHeadingReference::N2khr_true :
                    Serial.print("orientation based on the true north");
                break;

                case tN2kHeadingReference::N2khr_Unavailable :
                    Serial.print("can't get heading type");
                break;
            }

            Serial.print("Course over ground : ");
            Serial.println(COG);

            Serial.print("Speed over ground : ");
            Serial.println(SOG);
        }
    }



    /**
    * @brief 
    * @param 
    * @return 
    */
    void c_Navi::handleWind(const tN2kMsg& N2kMsg) noexcept{
    
        unsigned char messageIdentifier; // == SID
        double WindSpeed = 0;
        double WindAngle = 0;
        tN2kWindReference windType;

        if(ParseN2kWindSpeed(N2kMsg, messageIdentifier, WindSpeed, WindAngle, windType) ){

            Serial.print("wind speed : ");
            Serial.println(WindSpeed);

            Serial.print("wind angle : ");
            Serial.println(WindAngle);

            Serial.print("wind type : ");
            Serial.println(windType);

            Serial.println();
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

        // Do not forward bus messages at all
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
    void c_Navi::get_nmea_data(){

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
        KNOWN PGN LIST : 
        PGN : 60928  -> ISO Address Claim (unused)
        PGN : 127250 -> magnetic heading
        PGN : 127251 -> rate of turn (unused)
        PGN : 128259 -> boat speed (speed of the boat on the water)
        PGN : 128267 -> water depth (unused)
        PGN : 129025 -> position GPS
        PGN : 129026 -> COG SOG (speed combared to a fixed object on ground)
        PGN : 130306 -> wind data
        PGN : 130311 -> temperatures (unused)
        */

        switch(NmeaMessage.PGN){

            case 127250L:
                this->handleHeading(NmeaMessage);
            break;

            case 128259:
                handleBoatSpeed(NmeaMessage);
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

                Serial.print("Unknown message from : ");
                Serial.println((int)NmeaMessage.Source);

                Serial.print("PGN : ");
                Serial.println(NmeaMessage.PGN);

                Serial.print("destination : ");
                Serial.println((int)NmeaMessage.Destination);

                Serial.print("priority : ");
                Serial.println((int)NmeaMessage.Priority);

                Serial.print("nb bytes : ");
                Serial.println(NmeaMessage.DataLen);
                
                Serial.print("msg time : ");
                Serial.println(NmeaMessage.MsgTime);

                Serial.println( (NmeaMessage.IsValid())? "message is valid" : "message is invalid");

                Serial.println();
            break;
        }
    }

    //on construit un objet Navi pour pouvoir l'utilisé de manière globale
    c_Navi Navi;
}
