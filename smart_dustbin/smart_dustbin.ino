#include <ESP8266WiFi.h>;
#include <WiFiClient.h>;
#include <ThingSpeak.h>;
const char* ssid = "Virus infected Wi-Fi!"; //Your Network SSID
const char* password = "WF647241BB1";
WiFiClient client;
// the following is data of the THINGSPEAK cloud
unsigned long myChannelNumber = 906122; //Your Channel Number (Without Brackets)
const char * myWriteAPIKey = "6T9P73H6KOZ3O35A"; //Your Write API Key


//moisture sensor declaration
int moist= A0;
int thresh=3;

//ultra sensor declaration
const int trig1 = 14,echo1 = 12,trig2 = 13,echo2 = 15;  //D5 to D8
long dur;
int dist1,dist2,per1,per2;


//servo and ir sensor declaration
#include<Servo.h>
Servo serv;
int data;
int ir=D0;

void setup()   // this is the setup of all the sensors  and the cloud
{
    //moist
     pinMode(moist, INPUT); 
  
  // ultra
     pinMode(trig1, OUTPUT);  
     pinMode(echo1, INPUT); 
     pinMode(trig2, OUTPUT);  
     pinMode(echo2, INPUT);
  
    // servo and ir
    serv.attach(D4);
    pinMode(ir, INPUT);
  
     //thingspeak
     WiFi.begin(ssid, password);
     ThingSpeak.begin(client);
  
     Serial.begin(9600);
}

void loop() 
{
    // moisture
    float moist_per; 
    int data=digitalRead(ir);  //reading the IR sensor value detecting wheather an object is present or not
    moist_per = ( 100.00 - ( (analogRead(moist)/1023.00) * 100.00 ) );
    // moisture_percentage == moist_per
  //  reading the  sensor value and dividing by total value*100 
  //  to get the percentage of the moisture and subracting it from the total percentage
  
     if(data==0)
       {
         if(moist_per<=thresh)  //threshold value to compare  for checcking the moisture availability
            serv.write(180);      //moving the plate i.e, servo
         else if(moist_per>thresh)  
            serv.write(0);
       }
     else
       serv.write(90);
  
    // ultra-1
    digitalWrite(trig1, LOW);   // Makes trigPin low
    delayMicroseconds(2);       // 2 micro second delay 
    digitalWrite(trig1, HIGH);  // tigPin high
    delayMicroseconds(10);      // trigPin high for 10 micro seconds
    digitalWrite(trig1, LOW);   // trigPin low
    dur = pulseIn(echo1, HIGH); 
    dist1= dur*0.034/2;        //Calculating actual/real distance
    per1= (dist1*100)/20;
    
    // ultra-2
    digitalWrite(trig2, LOW);   // Makes trigPin low
    delayMicroseconds(2);       // 2 micro second delay 
    digitalWrite(trig2, HIGH);  // tigPin high
    delayMicroseconds(10);      // trigPin high for 10 micro seconds
    digitalWrite(trig2, LOW);   // trigPin low
    long dur1 = pulseIn(echo2, HIGH); 
    dist2= dur1*0.034/2;        //Calculating actual/real distance
    per2= (dist2*100)/20;
    
    //serial monitor  i.e, to print on the console
    Serial.print("Distance of 1st = ");  
    Serial.println(dist1);
    Serial.print("Distance of 1st sensor in percentage= ");        //Output distance on arduino serial monitor 
    Serial.println(per1);
    Serial.print("Distance of 2nd = ");        //Output distance on arduino serial monitor 
    Serial.println(dist2);
    Serial.print("Distance of 2nd sensor in percentage= ");
    Serial.println(per2);
    Serial.print(moist_per);

    //to update the value onto the cloud(THINGSPEAK)
    ThingSpeak.writeField(myChannelNumber, 2,per2, myWriteAPIKey); 
    ThingSpeak.writeField(myChannelNumber, 1,per1, myWriteAPIKey); 
    
    delay(100);  //waiting for some time for the nxt iteration 100 milliseconds       
                         
}
