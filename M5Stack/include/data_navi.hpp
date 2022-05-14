#ifndef DATA_NAVI_HPP
#define DATA_NAVI_HPP

#include <Arduino.h>

namespace navi{
    
    /**
     * @brief une structure qui stoque les données des capteurs d'un bateau
     */
    struct data_navi{

        int32_t id_bateau = -1;
        int32_t id_course = -1;

        float heading = 0;
        float speed = 0;
        float latitude = 0;
        float longitude = 0;
        float WindSpeed = 0;
        float WindAngle = 0;
        

        //flags pour indiquer le contenue valide de la data
        byte data_content = B00000000;
    };

    //fr
    enum data_navi_content{

        HEADING     = B00000001,
        SPEED       = B00000010,
        LATITUDE    = B00000100,
        LONGITUDE   = B00001000,
        WINDSPEED   = B00010000,
        WINDANGLE   = B00100000
    };
}

#endif