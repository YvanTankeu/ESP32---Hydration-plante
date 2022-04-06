// 03 Includes pour faire fonctionner le rtc
#include <SPI.h>
#include <Wire.h>
#include "RTClib.h" 
#include "plante.hpp"

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday",
                             "Thursday", "Friday", "Saturday"};
RTC_DS3231 rtc;
DateTime now = rtc.now();

// -----------------------------------------------------------------------------------------------------------------------
// -------------------------- Implémentation des Fonctions de la plante --------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------

/**
 * @brief Construit un nouveau objet de la classe Plante
 * 
 * @param humidite valeur d'humidité de la plante
 * @param date date du jour 
 * @param heure  heure du jour
 */
Plante::Plante(unsigned int humidite, String date, String heure)
                :   humidite_(humidite), date_(date), heure_(heure){}

/**
 * Retourne le taux d'humidité de la plante
 * @return humidite_
 */
unsigned int Plante::humidite() const{
    return humidite_;
}

/**
 * Retourne la date du jour
 * @return mot de passe
 */
String Plante::date() const{

    return  String(now.year(), DEC) + "/" 
            + String(now.month(), DEC) + "/" 
            + String(now.day(), DEC) + "(" 
            + String(daysOfTheWeek[now.dayOfTheWeek()]) + ") " ;
}

/**
 * Retourne l'heure
 * @return heure
 */
String Plante::heure() const{
    return String(now.hour() +1 , DEC) + "H:" + String(now.minute() + 4, DEC) + " min" ;
}

/**
 * @brief set la valeur d'humidité
 * 
 * @param humidite 
 */
void Plante::humidite(unsigned int humidite){
    humidite_ = humidite;
}

/**
 * @brief set l'heure
 * 
 * @param heure 
 */
void Plante::heure(String heure){
    heure_ = heure;
}

/**
 * @brief set la date
 * 
 * @param date 
 */
void Plante::date(String date){
    date_ = date;
}

int getSoilHumidityVal(int pinCapteur){
    return analogRead(pinCapteur);
}

/**
 * @brief Donner de l'eau à la plante en ouvrant vane d'eau
 * 
 */
void Plante::hydrater(const int pinRelai){
   digitalWrite(pinRelai, HIGH); 
}

/**
 * @brief Arreter de donner de l'eau à la plante en fermant vane d'eau
 * 
 */
void Plante::deshydrater(const int pinRelai){
   digitalWrite(pinRelai, LOW); 
   
}

void Plante::initDateTime(){
      if (! rtc.begin()) {
    Serial.println("N'a pas pu trouver le RTC");
    Serial.flush();
    while (1) delay(10);
  }

  if (rtc.lostPower()) {
    Serial.println("Le RTC a perdu de l'énergie, remettons l'heure à jour !");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

}