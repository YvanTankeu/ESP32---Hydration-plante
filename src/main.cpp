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

#include <Arduino.h>

// 03 Includes pour faire fonctionner le rtc
#include <SPI.h>
#include <Wire.h>
#include "RTClib.h" 

// 02 Includes pour la connexion wifi
#include <WiFi.h>
#include <WiFiClient.h>

// 03 Includes pour le serveur web, dns et mise à jour
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>

// Identifiants du wifi
const char* host = "esp32";
const char* ssid = "Galaxi";
const char* password = "12345678";

const int relaiLED = 33;
const unsigned long DELAI = 3000;
const double SEUIL_HUMIDITE = 300.0;


//Déclaration de variables
double soilHumidity = 0.0;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday",
                             "Thursday", "Friday", "Saturday"};
RTC_DS3231 rtc;

WebServer server(80);

const char* startIndex =
    "<!doctype html>"
    "<html lang='en'>"

    "<head>"
    "<!-- Required meta tags -->"
    "<meta charset='utf-8'>"
    "<meta name='viewport' content='width=device-width, initial-scale=1'>"

    "<!-- Bootstrap CSS -->"
    "<link href='https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/css/bootstrap.min.css' rel='stylesheet'>"

    "<!-- Font awesome ico CSS -->"
    "<link href='https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.0.0/css/all.min.css' rel='stylesheet'>"

    "<style>"
    ".img-fluid {"
    "max-width: 70%;"
    "}"

    ".chat {"
    "max-width: 20%;"
    "margin-left: 540px;"
    "margin-top: -100px;"
    "/* Une ombre noire avec un rayon de flou de 10px */"
    "filter: drop-shadow(16px 16px 10px rgb(1, 73, 31))"
    "}"

    ".textChat {"
    "font-size: 25px;"
    "color: black;"
    "max-width: 20%;"
    "margin-left: 550px;"
    "margin-top: -60px;"

    "/* Une ombre noire avec un rayon de flou de 10px */"
    "filter: drop-shadow(16px 16px 10px rgb(255, 255, 255))"
    "}"

    ".info {"
    "margin-left: 12px;"
    "height: 180px;"
    "width: 400px;"
    "background-color: rgb(255, 255, 255);"
    "filter: drop-shadow(16px 10px 5px rgb(1, 73, 31))"
    " }"

    "#barBlanche {"
    "height: 1px;"
    "background-color: rgb(0, 0, 0);"
    "left: 5px;"
    "bottom: 0.5em;"
    "position: relative;"
    "}"
    "</style>"

    "<title>ma plante</title>"
    "</head>"

    "<body>"
    "<div class='container bg-success text-white'>"
    "<h1 class = 'row justify-content-center text-white  mb-4 shadow-lg p-3 mb-2 rounded'>PLANTE CONNECTE</h1>"
    "<div class='row'> "
    "<div class='row mt-5'></div>"
    "<div class='row mt-5'></div>"

    "<div class='row position-relative'>"
    "<img src='https://i.ibb.co/5nz5Rsw/chat.png' class='chat  float-end img-fluid position-absolute bottom-90 end-10 ' alt='chat'>"
    "<span class='textChat'>Je suis rassasié :)</span>"
    "</div>"
    "</div>"
    "<div class='row'>"
    "<div class='row mb-5'></div>"
    "<div class='row mb-5'></div>"
    "</div>"
    "<div class='row position-relative mt-5'>"
    "<div class='col position-absolute bottom-25 start-0 info'>"
    "<h1 class='text-dark'> Mes informations <span class='fa-solid fa-circle-info text-primary h3 text-center'></span></h1>"
    "<div id='barBlanche'></div>"
    "<div class='ow text-dark p-3'>"
    "Je suis actu à 100% d'humidité, il est important que je reste desechée"
    "</div>"
    "</div>"
    "<div class='col'>"
    "<img src='https://i.ibb.co/WFGtN1L/plante.png' class='float-end img-fluid position-absolute bottom-0 end-0' alt='image png plante'>"
    "</div>"
    "<!--JavaScript-->"
    "<img src='https://i.ibb.co/t4nsJfR/table1.png' id='table' class='img' alt='La table'>"
    "</div>"
    "</div>"
    "</body>"

    "</html>";

/*
 * Server Index Page
 */
 
const char* updatePage = 
"<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
"<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
   "<input type='file' name='update'>"
        "<input type='submit' value='Update'>"
    "</form>"
 "<div id='prg'>progress: 0%</div>"
 "<script>"
  "$('form').submit(function(e){"
  "e.preventDefault();"
  "var form = $('#upload_form')[0];"
  "var data = new FormData(form);"
  " $.ajax({"
  "url: '/update',"
  "type: 'POST',"
  "data: data,"
  "contentType: false,"
  "processData:false,"
  "xhr: function() {"
  "var xhr = new window.XMLHttpRequest();"
  "xhr.upload.addEventListener('progress', function(evt) {"
  "if (evt.lengthComputable) {"
  "var per = evt.loaded / evt.total;"
  "$('#prg').html('progress: ' + Math.round(per*100) + '%');"
  "}"
  "}, false);"
  "return xhr;"
  "},"
  "success:function(d, s) {"
  "console.log('success!')" 
 "},"
 "error: function (a, b, c) {"
 "}"
 "});"
 "});"
 "</script>";

/*
 * setup function
 */

void setup () {
  Serial.begin(9600);

  pinMode(relaiLED, OUTPUT); // Relai en mode output

  /*
  * Configuration du module RTC
  */
  if (! rtc.begin()) {
    Serial.println("N'a pas pu trouver le RTC");
    Serial.flush();
    while (1) delay(10);
  }

  if (rtc.lostPower()) {
    Serial.println("Le RTC a perdu de l'énergie, remettons l'heure à jour !");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  /*
  * Configuration du server, du wifi et la mise à jour
  */

  // Se connecter au reseau wifi
  WiFi.begin(ssid, password);
  Serial.println("");

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

  /* utilisation du mdns pour la résolution des noms d'hôtes */
  if (!MDNS.begin(host)) { //http://esp32.local
    Serial.println("Erreur dans la configuration du répondeur MDNS !");
    while (1) {             
      delay(1000);
    }
  }
  Serial.println("Démarrage du répondeur mDNS");

  /* Renvoie la page d'index qui est stockée dans updatePage */
  server.on("/", HTTP_GET, []() {
    server.sendHeader("Connection", "fermée");
    server.send(200, "text/html", startIndex);
  });
  server.on("/updatePage", HTTP_GET, []() {
    server.sendHeader("Connection", "fermée");
    server.send(200, "text/html", updatePage);
  });

  /* Gestion du téléchargement du fichier du firmware */
  server.on("/update", HTTP_POST, []() {
    server.sendHeader("Connection", "fermée");
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
  server.begin();
}

void loop () {

  server.handleClient();
  delay(1);

  // Code valeur du capteur d'humidité du sol
  soilHumidity = analogRead(A0);
  Serial.print("Humidité du sol: ");
  Serial.println(soilHumidity);

  if (soilHumidity <= SEUIL_HUMIDITE){
    //Serial.println("val inferieur à 301");
    Serial.println("Sol sec, plante doit être arrosée");
  Serial.println();
    digitalWrite(relaiLED, HIGH);
  }
  else if (soilHumidity > SEUIL_HUMIDITE)
  {
    //Serial.println("val superieur à 300");
    Serial.println("Sol humide, bien pour la plante");
    Serial.println();

    digitalWrite(relaiLED, LOW);
  }

/*  // Code module rtc
  DateTime now = rtc.now();

  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" (");
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  Serial.print(") ");
  Serial.print(now.hour()+1, DEC);
  Serial.print(':');
  Serial.print(now.minute()+5, DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  
  delay(delayTime);
  digitalWrite(relaiLED, HIGH);
  delay(delayTime);
  digitalWrite(relaiLED, LOW);*/
  delay(DELAI);

}
