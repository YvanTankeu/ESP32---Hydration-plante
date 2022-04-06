#include <Arduino.h>
#include <WebServer.h>
#include <Update.h>

#include "serveur.hpp"

WebServer server(80);

// -----------------------------------------------------------------------------------------------------------------------
// -------------------------- Implementation des Fonctions de Serveur ----------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------

/**
 * @brief Construit un nouveau objet de la classe Serveur
 * 
 * @param startIndex page d'accueil
 * @param updatePage page de mise à jour du firmware
 */
Serveur::Serveur(String startIndex, String updatePage)
                :   startIndex_(startIndex), updatePage_(updatePage){}

/**
 * Retourne la page d'accueil
 * @return startIndex_
 */
String Serveur::startIndex() const{
    return startIndex_;
}

/**
 * Retourne la page de mise à jour
 * @return Paged de mise à jour
 */
String Serveur::updatePage() const{
    return updatePage_;
}

void Serveur::sendPage(String pageToSend) const{
    if(pageToSend = "/"){
        server.on(pageToSend, HTTP_GET, []() {
            server.sendHeader("Connection", "close");
            server.send(200, "text/html", pageToSend);
        });
    }else if(pageToSend = "/updatePage")
    {
        server.on("/updatePage", HTTP_GET, [](){
            server.sendHeader("Connection", "close");
            server.send(200, "text/html", updatePage_);
        });
    }else if(pageToSend = "/dateValue"){
        server.on("/dateValue", HTTP_GET, [](){
            server.sendHeader("Connection", "close");
            server.send(200, "text/plain", actualDate);
        });
    }else if(pageToSend = "/dateValue"){
         server.on("/readHumVal", HTTP_GET, [](){
            server.sendHeader("Connection", "close");
            server.send(200, "text/plain", humide);
        });
    }else if(pageToSend = "/ota"){
        /* Gestion du téléchargement du fichier du firmware */
  server.on("/update", HTTP_POST, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", (Update.hasError()) ? "ECHEC" : "OK");
    ESP.restart();
  }, []() {
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
      Serial.printf("Mise à jour: %s\n", upload.filename.c_str());
      if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //Commencez par la taille maximale disponible
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {

      /* flashage du firmware sur ESP */
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      if (Update.end(true)) { //Vrai pour définir la taille de la progression actuelle
        Serial.printf("Succès de la mise à jour: %u\nRebooting...\n", upload.totalSize);
      } else {
        Update.printError(Serial);
      }
    }
  });
    }
    
}
