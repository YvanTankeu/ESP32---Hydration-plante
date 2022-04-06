#include <Arduino.h>
// 02 Includes pour la connexion wifi
#include <WiFi.h>
#include <WiFiClient.h>

#include "connect.hpp"

// -----------------------------------------------------------------------------------------------------------------------
// -------------------------- Implementation des Fonctions de connect ----------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------

/**
 * @brief Construit un nouveau objet de la classe Connect
 *
 * @param nomWifi const char* Pointeur vers la chaîne SSID.
 * @param motDePasse const char* Optionel vers la chaîne motDePasse compris entre ASCII 32 et 126 (décimal)..
 */
Connect::Connect(char* nomWifi, char* motDePasse)
                :   nomWifi_(nomWifi), motDePasse_(motDePasse){}


/**
 * Retourne le nom actuel associé au réseau
 * @return nomWifi_
 */
char* Connect::nomWifi() const{
    return nomWifi_;
}

/**
 * Retourne le password actuel associé au réseau
 * @return mot de passe
 */
char* Connect::motDePasse() const{
    return motDePasse_;
}

/**
 * @brief set nom du wifi pour le device se connecte
 * 
 * @param nomWifi 
 */
void Connect::nomWifi(char* nomWifi){
    nomWifi_ = nomWifi;
}

/**
 * @brief set le password du wifi pour le device se connecte
 * 
 * @param motDePasse 
 */
void Connect::motDePasse(char* motDePasse){
    motDePasse_ = motDePasse;
}

/**
 * @brief Utiliser pour se connecter au wifi
 * 
 * @param nomWifi ssid du wifi  
 * @param motDePasse  mot de passe du wifi
 */
void Connect::connecter(char* nomWifi, char* motDePasse){
    WiFi.begin(nomWifi, motDePasse);
}