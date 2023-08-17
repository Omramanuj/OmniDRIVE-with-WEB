// rx code/ due code : 
#include <EasyTransfer.h>
//create object
EasyTransfer ET; 


struct RECEIVE_DATA_STRUCTURE{
  //put your variable definitions here for the data you want to receive
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  int16_t rx;
  int16_t ry;

};

//give a name to the group of data
RECEIVE_DATA_STRUCTURE mydata;

void setup(){
  Serial.begin(9600);
  Serial2.begin(9600);

  //start the library, pass in the data details and the name of the serial port. Can be Serial, Serial1, Serial2, etc. 
  ET.begin(details(mydata), &Serial2);  
}

void loop(){
  //check and see if a data packet has come in. 
  if(ET.receiveData()){
    //this is how you access the variables. [name of the group].[variable name]
    //since we have data, we will blink it out.

    Serial.println("Received: ");
    int a = mydata.rx;
    Serial.println(mydata.rx);
    int b = mydata.ry;
    Serial.println(mydata.ry);
    // Serial.println(mydata.ry);
  }
    delay(20);
  
  //you should make this delay shorter then your transmit delay or else messages could be lost
}