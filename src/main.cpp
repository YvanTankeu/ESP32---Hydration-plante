/*
  Titre      : Hydratation d'une plante
  Auteur     : Yvan Tankeu
  Date       : 27/03/2022
  Description: Faire arroser une plante en utilisant un soil moisture sensor
    # Description de la valeur du capteur
    # 00~300 sol sec
    # 300~700 sol humide
    # 700~950 dans l'eau
  Version    : 0.0.1
*/

// 02 Includes pour le serveur web et mise à jour
#include <WebServer.h>
#include <Update.h>

#include "connect.hpp"
#include "plante.hpp"
#include "serveur.hpp"
#include "pageweb.hpp"

// Identifiants du wifi
char* ssid = "Galaxi";
char* password = "12345678";

const int relaiLED = 33;
const unsigned long DELAI = 1000;
const unsigned int SEUIL_HUMIDITE = 300;

String actualDate = "";

Plante unePlante;

//Déclaration de variables
unsigned int soilHumidity = 0;

WebServer server(80);
Serveur unServeur;

/*
 * setup function
 */

void setup () {
  Serial.begin(9600);

  pinMode(relaiLED, OUTPUT); // Relai en mode output

  unePlante.initDateTime();

  Connect connect(ssid, password);
  // Attendre la connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connecter à ");
  Serial.println(ssid);
  Serial.print("Address IP: ");
  Serial.println(WiFi.localIP());

  // Renvoie la page d'acceuil
  unServeur.sendPage("/");

  // requete ajax pour la valeur de l'humidité
  unServeur.sendPage("/readHumVal");

  // requete ajax pour la date et l'heure du jour
  unServeur.sendPage("/dateValue");

  unServeur.sendPage("/updatePage");

  unServeur.sendPage("/ota");

  server.begin();

  Serial.println("HTTP server started");
}

void loop () {
  // Recupère la date et l'heure
  actualDate = unePlante.date() + unePlante.heure();

  server.handleClient();

  // Code valeur du capteur d'humidité du sol
  soilHumidity = unePlante.getSoilHumidityVal(A0);
  Serial.println(soilHumidity);

  // La plante se fait arroser automatiquement
  if (soilHumidity <= SEUIL_HUMIDITE){
    Serial.println("Sol sec, plante doit être arrosée");
    Serial.println();
    unePlante.hydrater(relaiLED);
  }
  else if (soilHumidity > SEUIL_HUMIDITE)
  {
    Serial.println("Sol humide, bien pour la plante");
    Serial.println();
    unePlante.deshydrater(relaiLED);
  }

  delay(DELAI);

}
