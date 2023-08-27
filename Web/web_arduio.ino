#include <WiFi.h>
#include <ESPmDNS.h>

#include <WebSocketsServer.h>
#include "controlpage.h"
#include "devpage.h"
#include <ESPAsyncWebServer.h>
//#include <SPIFFS.h>
#include<ArduinoJson.h>
#include <EasyTransfer.h>
//#include <Ps3Controller.h>
#define esptodue Serial


//create object
EasyTransfer ET; 

struct SEND_DATA_STRUCTURE{
  //put your variable definitions here for the data you want to send
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  int16_t rx;
  int16_t ry;
  int16_t l1;
  int16_t r1;
  int16_t l2;
  int16_t r2;
};

SEND_DATA_STRUCTURE mydata;
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
//      Serial.printf("[%u] get Text: %s\n", num, payload);
//      String message = String((char*)( payload));
//      Serial.println(message);

       // Parse the JSON data
        DynamicJsonDocument jsonDocument(512);
        deserializeJson(jsonDocument, payload);
        int joystickX = jsonDocument["x"];
        int joystickY = jsonDocument["y"];

        if(jsonDocument["state"]==true)
        {
          int16_t rx;
          int16_t ry;

          rx=jsonDocument["x"];
          ry=jsonDocument["y"];
          
          mydata.rx = rx;
          mydata.ry = ry;
          

        
          Serial.print("Joystick X: ");
          Serial.println(mydata.rx);
          Serial.print("Joystick Y: ");
          Serial.println(mydata.ry);

          ET.sendData();  
          delay(20);
        }
  }
}

void setup(void)
{
   
  Serial.begin(115200);
  esptodue.begin(115200, SERIAL_8N1,3,1);
  //start the library, pass in the data details and the name of the serial port. Can be Serial, Serial1, Serial2, etc.
  ET.begin(details(mydata), &esptodue);
  // Ps3.attach(notify);
//  Ps3.begin("00:1a:7d:da:71:10");
//   Serial.println("Ready.");

  
  WiFi.softAP("techiesms", "");
  Serial.println("softap");
  Serial.println("");
  Serial.println(WiFi.softAPIP());


  if (MDNS.begin("ESP")) { //esp.local/
    Serial.println("MDNS responder started");
  }
  
//  if (!SPIFFS.begin(true)) {
//        Serial.println("Failed to mount SPIFFS");
//    }
//  else{
//    Serial.println("Mounted SPIFFS succesfully.");
//  }
//
//    server.on("/bg13_1.png", HTTP_GET, [](AsyncWebServerRequest *request){
//          request->send(SPIFFS,"/data/bg13_1.png", "image/png");
//  });
//
//    server.on("/svnit_logo.png", HTTP_GET, [](AsyncWebServerRequest *request){
//            request->send(SPIFFS,"/data/svnit_logo.png", "image/png");
//            Serial.println("svnit loaded");
//    });
//
//    server.on("/drishti_logo_white.png", HTTP_GET, [](AsyncWebServerRequest *request){
//            request->send(SPIFFS,"/data/drishti_logo_white.png", "image/png");
//    });
    
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
