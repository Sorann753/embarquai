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
        Serial.println("CAN OPEN");

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
        PGN : 129025 -> position GPS
        PGN : 129026 -> data GPS (direction + speed)
        PGN : 130306 -> wind data
        */

        switch(NmeaMessage.PGN){

            case 127250L:
                this->handleHeading(NmeaMessage);
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
