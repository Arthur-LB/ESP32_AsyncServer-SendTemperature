#include <AsyncEventSource.h>
#include <AsyncWebSocket.h>
#include <AsyncWebSynchronization.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFSEditor.h>
#include <StringArray.h>
#include <WebAuthentication.h>
#include <WebHandlerImpl.h>
#include <WebResponseImpl.h>

#include <AsyncTCP.h>

#include <SPIFFS.h>
#include <WiFi.h>

const char* ssid = "ELKHOUKHI"; // Mettre le ssid de votre réseau Wifi
const char* password = "tutur"; // Mettre le mot de passe de votre réseau Wifi
AsyncWebServer server(80); // Creation d’un objet de type serveur HTTP


// the number of the LED pin
const int ledPin = 23;  // 16 corresponds to GPIO16

// setting PWM properties
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;

float valT, T;
  
float convert2Temp(int val) {
  valT = (float) (val*3.3)/4095;
  T = (valT-0.25)/0.028;
  return T;
}

void setup() {
  Serial.begin(115200);
  analogSetPinAttenuation(A3, ADC_0db); // eventuellement, pour le capteur de luminosite
  // ne rien faire sur l’entrée A4 : 1V = valeur 1088

  
  Serial.begin(115200);
  int ma_pause = 1000;
  delay(1000);
  Serial.print("Setting AP (Access Point)...");
  WiFi.softAP(ssid, password); // Remove the password parameter, if you want the AP (Access Point) to be open
  //WiFi.softAP(ssid);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  
  // put your setup code here, to run once:
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    delay(100);
    return;
  }
  
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/analog.html", String());
  });
  server.on("/getValeurs", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Requete Async Temperature");
    int lum = analogRead(A3);
    float temp = (float) analogRead(A4);
    String resp = String(temp)+"_"+String(lum);
    Serial.println(resp);
    request->send(200, "text/plain", resp);
  });
  server.begin(); // Initialisation du serveur web
  Serial.println("Serveur web actif");
  
}
void loop() {
  int valA3 = analogRead(A3); // resultat de la conversion A->N, correspondant a la tension sur A3
  int valA4 = analogRead(A4); // resultat de la conversion A->N, correspondant a la tension sur A4
  float temp = convert2Temp(valA4);
  Serial.print("Temperature :");
  Serial.print(temp);
  Serial.print(" Luminosite : ");
  Serial.println(valA3);
  delay(1000);

}
