/**
 * @brief fichier test du M5Stack pour module Navi
 * @author Doriath Arthus
 * @date 11/05/2022
 * @version 0.1
 */

#include <M5Core2.h>
#include <unity.h>
#include "navi.hpp"
#include "data_navi.hpp"

/**
 * @brief un test unitaire pour la classe data_navi
 * @param rien
 * @return rien
 */
void test_data_navi(){
    navi::data_navi data;
    data.id_bateau = 1;
    data.id_course = 2;
    data.data_content = 3;
    TEST_ASSERT_EQUAL(1, data.id_bateau);
    TEST_ASSERT_EQUAL(2, data.id_course);
    TEST_ASSERT_EQUAL(3, data.data_content);
}

/**
 * @brief test unitaire du flag de data_navi
 * @param rien
 * @return rien
 */
void test_flag_navi(){
    navi::data_navi data;
    data.data_content |= navi::data_navi_content::HEADING;
    data.data_content |= navi::data_navi_content::SPEED;
    data.data_content |= navi::data_navi_content::LATITUDE;
    data.data_content |= navi::data_navi_content::LONGITUDE;

    TEST_ASSERT(data.data_content & navi::data_navi_content::HEADING);
    TEST_ASSERT(data.data_content & navi::data_navi_content::SPEED);
    TEST_ASSERT(data.data_content & navi::data_navi_content::LATITUDE);
    TEST_ASSERT(data.data_content & navi::data_navi_content::LONGITUDE);

    TEST_ASSERT_UNLESS(data.data_content & navi::data_navi_content::WINDANGLE);
    TEST_ASSERT_UNLESS(data.data_content & navi::data_navi_content::WINDSPEED);
    TEST_ASSERT_UNLESS(data.data_content & 0b01000000);
    TEST_ASSERT_UNLESS(data.data_content & 0b10000000);
}

/**
 * @brief fonction d'initialisation des tests unitaires
 * @param none
 * @return none
 */
void setup() {
    M5.begin();
    Serial.begin(115200);

    UNITY_BEGIN();
}


/**
 * @brief boucle d'exécution des tests unitaires
 * @param none
 * @return none
 */
void loop() {
    navi::data_navi data;

    RUN_TEST(test_data_navi);

    delay(100);

    RUN_TEST(test_flag_navi);
        
    delay(100);
    UNITY_END(); // termine le test unitaire
}