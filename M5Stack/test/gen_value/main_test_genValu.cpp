#include <unity.h>
#include "test_gen_value.h"

//Activations des tests
#define lorawan_send
#define gener_value

//-----------------------------------------------------
//Main de test pour la generation des donnee capteurs + test de la lib std::array
void setup()
{
    UNITY_BEGIN();
    randomSeed(analogRead(0));
}

void loop()
{
    #ifdef gener_value 
        RUN_TEST(test_gen_number);
        delay(500);
        RUN_TEST(test_gen_letter);
        delay(500);
        RUN_TEST(test_gen_string);
    #endif

    #ifdef lorawan_send
        //... 
    #endif

    UNITY_END();
}
//------------------------------------------------------