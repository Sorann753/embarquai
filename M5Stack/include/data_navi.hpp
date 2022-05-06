#ifndef DATA_NAVI_HPP
#define DATA_NAVI_HPP

#include <Arduino.h>

namespace navi{
    
    /*@brief une structure qui stoque les données des capteurs d'un bateau */
    struct data_navi{

        String data = "10.0";
        String data_1 = "15.0";
        String data_2 = "20.5";
    };
}

#endif