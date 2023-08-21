#include <WiFi.h>
#include <ESPmDNS.h>

#include <WebSocketsServer.h>
#include "devpage.h"
#include "controlpage.h"
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include<ArduinoJson.h>


AsyncWebServer server(80); // server port 80
WebSocketsServer websockets(81);

void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Page Not found");
}


void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

  switch (type) 
  {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED: {
        IPAddress ip = websockets.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

        // send message to client
        websockets.sendTXT(num, "Connected from server");
      }
      break;
    case WStype_TEXT:
      Serial.printf("[%u] get Text: %s\n", num, payload);
      String message = String((char*)( payload));
      Serial.println(message);

       // Parse the JSON data
        DynamicJsonDocument jsonDocument(512);
        deserializeJson(jsonDocument, payload);
        float joystickX = jsonDocument["x"];
        float joystickY = jsonDocument["y"];

        
        // Use joystickX and joystickY in your Arduino code
        Serial.print("Joystick X: ");
        Serial.println(joystickX);
        Serial.print("Joystick Y: ");
        Serial.println(joystickY);
  }
}

void setup(void)
{
   
  Serial.begin(115200);
  Serial.println("Serial began");
  
  WiFi.softAP("techiesms", "");
  Serial.println("softap");
  Serial.println("");
  Serial.println(WiFi.softAPIP());


  if (MDNS.begin("ESP")) { //esp.local/
    Serial.println("MDNS responder started");
  }

  if (!SPIFFS.begin(true)) {
        Serial.println("Failed to mount SPIFFS");
    }
  else{
    Serial.println("Mounted SPIFFS succesfully.");
  }

    server.on("/bg13_1.png", HTTP_GET, [](AsyncWebServerRequest *request){
          request->send(SPIFFS,"/data/bg13_1.png", "image/png");
  });

    server.on("/svnit_logo.png", HTTP_GET, [](AsyncWebServerRequest *request){
            request->send(SPIFFS,"/data/svnit_logo.png", "image/png");
            Serial.println("svnit loaded");
    });

    server.on("/drishti_logo_white.png", HTTP_GET, [](AsyncWebServerRequest *request){
            request->send(SPIFFS,"/data/drishti_logo_white.png", "image/png");
    });
    
    server.on("/",[](AsyncWebServerRequest * request)
    { 
      request->send_P(200, "text/html", controlpage);
    });

   server.on("/developers.html", HTTP_GET, [](AsyncWebServerRequest * request)
  { 
    Serial.println("Devpage loaded");
  request->send_P(200, "text/html", devpage);
  });

  server.on("/r1.html", HTTP_GET, [](AsyncWebServerRequest * request)
  { 
    Serial.println("Controlpage loaded");
  request->send_P(200, "text/html", controlpage);
  });


  server.onNotFound(notFound);

  server.begin();  // it will start webserver
  websockets.begin();
  websockets.onEvent(webSocketEvent);

}


void loop()
{
 websockets.loop();
}
