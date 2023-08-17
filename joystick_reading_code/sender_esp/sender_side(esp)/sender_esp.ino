// tx code/ esp code

#include <EasyTransfer.h>
#include <Ps3Controller.h>
#define esptodue Serial
Ps3Controller ps3;
//create object
EasyTransfer ET; 

struct SEND_DATA_STRUCTURE{
  //put your variable definitions here for the data you want to send
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  int16_t rx;
  int16_t ry;
};



//give a name to the group of data
SEND_DATA_STRUCTURE mydata;

void setup(){
  Serial.begin(9600);
  esptodue.begin(9600, SERIAL_8N1,3,1);
  //start the library, pass in the data details and the name of the serial port. Can be Serial, Serial1, Serial2, etc.
  ET.begin(details(mydata), &esptodue);
  // Ps3.attach(notify);
  ps3.begin("00:1a:7d:da:71:10");
//   Serial.println("Ready.");

}

void loop()
{
  // ps3.read();
  int16_t rx = ps3.data.analog.stick.rx;
  int16_t ry = ps3.data.analog.stick.ry;
  if (abs(rx) < 10) {
    rx = 0;
  }
  if (abs(ry) < 10) {
    ry = 0;
  }
  mydata.rx = rx;
  mydata.ry = ry;
// send the data 
  ET.sendData();  
  delay(50);
}