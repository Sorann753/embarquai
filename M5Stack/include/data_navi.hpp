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

        float   heading   = 0.0f;
        float   speed     = 0.0f;
        float   latitude  = 0.0f;
        float   longitude = 0.0f;
        float   windSpeed = 0.0f;
        float   windAngle = 0.0f;
        

        //flags pour indiquer le contenue valide de la data
        byte data_content = 0b00000000;
    };

    /**
     * @brief enum des différents flags qui indique le contenue de la structure data_navi
     */
    enum data_navi_content{

        HEADING     = 0b00000001,
        SPEED       = 0b00000010,
        LATITUDE    = 0b00000100,
        LONGITUDE   = 0b00001000,
        WINDSPEED   = 0b00010000,
        WINDANGLE   = 0b00100000
    };
}

#endif