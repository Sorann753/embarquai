/**
 * @brief 
 * @author DORIATH ARTHUS
 * @date 05/05/2022
 * @version 0.1
 */

#include "navi.hpp"

namespace navi{

    /**
    * @brief 
    * @param 
    * @return 
    */
    void Navi::handleHeading(const tN2kMsg& N2kMsg) const noexcept{
    
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
    void Navi::handleRateOfTurn(const tN2kMsg& N2kMsg) const noexcept{
    
        unsigned char messageIdentifier; // == SID
        double RateOfTurn = 0;

        if(ParseN2kRateOfTurn(N2kMsg, messageIdentifier, RateOfTurn) ){

            Serial.print("Rate of turn : ");
            Serial.println(RateOfTurn);

            Serial.println();
        }
    }



    /**
    * @brief 
    * @param 
    * @return 
    */
    void Navi::handlePosition(const tN2kMsg& N2kMsg) const noexcept{
    
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
    void Navi::handleCogSog(const tN2kMsg& N2kMsg) const noexcept{
    
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
    void Navi::handleWind(const tN2kMsg& N2kMsg) const noexcept{
    
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
    void Navi::begin(){
    
        // Do not forward bus messages at all
        NMEA2000.EnableForward(false);
        NMEA2000.SetMsgHandler(HandleNMEA2000Msg);

        Serial.println();
        while(!NMEA2000.Open()){
            Serial.println("CAN ERROR : can't Open");
            delay(1000);
        }
        Serial.println("CAN OPEN");
    }


    /**
    * @brief 
    * @param 
    * @return 
    */
    void Navi::get_nmea_data(){

        NMEA2000.ParseMessages();
    }



    /**
    * @brief 
    * @param 
    * @return 
    */
    bool Navi::handle(const tN2kMsg &N2kMsg){
    
        static constexpr long PGN_list[] {
            127250L,
            127251L,
            129025L,
            129026L,
            130306L
        };

        if(false){
            Serial.print("Unknown message from : ");
            Serial.println((int)N2kMsg.Source);

            Serial.print("PGN : ");
            Serial.println(N2kMsg.PGN);

            Serial.print("destination : ");
            Serial.println((int)N2kMsg.Destination);

            Serial.print("priority : ");
            Serial.println((int)N2kMsg.Priority);

            Serial.print("nb bytes : ");
            Serial.println(N2kMsg.DataLen);
            
            Serial.print("msg time : ");
            Serial.println(N2kMsg.MsgTime);

            Serial.println( (N2kMsg.IsValid())? "message is valid" : "message is invalid");
            


            Serial.println();
        }
    }
}