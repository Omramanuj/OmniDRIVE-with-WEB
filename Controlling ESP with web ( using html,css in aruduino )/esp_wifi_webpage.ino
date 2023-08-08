#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#elif defined(ESP32)
#include <WiFi.h>
#include <ESPmDNS.h>
#else
#error "Board not found"
#endif


#define LED1 2
#define LED2 12

char webpage[] PROGMEM = R"=====(

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>OmniDrive</title>
    <!-- <link rel="stylesheet" href="style.css"> -->
    <style>
        body{
            background-color: rgba(0, 0, 0, 0.844);
            color: white;
        }
        
        .font_center{
           text-align: center;
        }
        
        .center{
            width: fit-content;
            margin: auto;
        }
        .flex{
            display: flex;
        }
        
        .btn{
            border-radius: 0.6rem;
            padding: 0.5rem;
            margin: 0.6rem;
        }
        
        .on:hover{
            background-color: red;
            color:white;
            cursor: pointer;
        }
        .off:hover{
            background-color: blue;
            color:white;
            cursor: pointer;
        }
        
        .on{
            border:2px solid red;
        }
        
        .off{
            border:2px solid blue;
        }
    </style>
</head>
<body>
    <h1 class="font_center">
        OmniDrive with Web Application
    </h1>

    <div class="center flex">
        <div class="btn on center" onclick="window.location = 'http://'+location.hostname+'/led1/on'">ON</div>
        <div class="btn off center" onclick="window.location = 'http://'+location.hostname+'/led1/off'">OFF</div>
    </div>
    

</body>
</html>

)=====";

// ipaddress/led1/on
//ipaddress/led1/off

// ipaddress/led2/on
//ipaddress/led2/off
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80); // server port 80

void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Page Not found");
}

void setup(void)
{
  
  Serial.begin(115200);
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
  
  WiFi.softAP("techiesms", "");
  Serial.println("softap");
  Serial.println("");
  Serial.println(WiFi.softAPIP());


  if (MDNS.begin("ESP")) { //esp.local/
    Serial.println("MDNS responder started");
  }



  server.on("/", [](AsyncWebServerRequest * request)
  { 
   
  request->send_P(200, "text/html", webpage);
  });

   server.on("/led1/on", HTTP_GET, [](AsyncWebServerRequest * request)
  { 
    digitalWrite(LED1,HIGH);
  request->send_P(200, "text/html", webpage);
  });

  server.on("/led1/off", HTTP_GET, [](AsyncWebServerRequest * request)
  { 
    digitalWrite(LED1,LOW);
  request->send_P(200, "text/html", webpage);
  });
  server.onNotFound(notFound);

  server.begin();  // it will start webserver
}


void loop(void)
{
}
