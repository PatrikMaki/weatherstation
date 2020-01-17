//!!! HUMIDITY & TEMPERATURE CODE:
#include "DHT.h"
#define DHTPIN 2    // modify to the pin connected
#define DHTTYPE DHT21   // DHT 21 (AM2301)
DHT dht(DHTPIN, DHTTYPE);
//!!! TRANSMITTER:
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";
//!!!PRESSURE
const float SensorOffset = -179.0;
//!!! MY TIME CALCULATOR FOR WIND:
unsigned long time;
float TimeOfConnection = 0;
//!!! AllowRPMcoun -construCt:
int framecount = 0;
bool AllowRPMcount = true;
//!!! return values (STRINGS)
char Humidity[50] = "0 ";
char Temperature[50] = "0 ";
char Pressure[50] = "0";
char WindSpeed[50] = "0 ";
 
void setup() {
  Serial.begin(9600);
  dht.begin(); // !!!HUMIDITY & TEMPERATURE BEGIN
  //!!! TRANSMITTER:
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  //!!! MY TIME CALC FOR WIND:
  pinMode(3, INPUT_PULLUP);
}
 
void loop() {
  delay(10);
  //!!! AllowRPMcoun -construCT:
  if (framecount > 500) {
    framecount = 0;
  }
  framecount = framecount + 1;
  if (framecount < 490) {
    AllowRPMcount = true;
  } else {
    AllowRPMcount = false;
  }
 
  //NOT TIME SENSITIVE PROGRAM:
  if (AllowRPMcount == false) {
    //!!!TRANSMITTER:
    char text[200] = "";
    strcat(text, WindSpeed);
    strcat(text, Temperature);
    strcat(text, Humidity);
    strcat(text, Pressure);
    radio.write(&text, sizeof(text)); //THIS IS THE STRING THAT IS BEING SENT TO THE RECIEVER. STRING'S FOMAT IS ~ "74 260 2690 1010".
    //!!! PRESSURE:
    // read the input on analog pin 0:
    float sensorValue = (analogRead(A0) - SensorOffset) / 10.0; //maths for calibration
    Pressure[0] = '\0';
    char buff5[50];
    buff5[0] = '\0';
    sprintf(buff5, "%d", ((int)sensorValue)*10);
    Pressure[0] = '\0';
    strcat(Pressure, buff5); //CoulD use strncpy()
    //!!! HUMIDITY & TEMPERATURE CODE:
    // Reading temperature or humidity takes about 250 milliseconds!
    float h = dht.readHumidity();
    float t = dht.readTemperature();
  
    if (isnan(t) || isnan(h)) {
      Serial.println("Failed to read from DHT");
    } else {
      Humidity[0] = '\0';
      char buff1[50];
      buff1[0] = '\0';
 
      if ((int)h - h < -0.5) {
        h += 1;
      }
 
      sprintf(buff1, "%d ", (int)(h * 10));
      strcat(Humidity, buff1);
      Temperature[0] = '\0';
      char buff2[50];
      buff2[0] = '\0';
 
      if ((int)t - t < -0.5) {
        t += 1;
      }
      sprintf(buff2, "%d ", (int)(t * 100));
      strcat(Humidity, buff2);
    }
  }
 
  //TIME SENSITIVE CODE:
  if (AllowRPMcount == true) {
    //!!! MY TIME CALC FOR WIND:
    int wiresConnector = digitalRead(3); //Changed from2 to 3 (ver transmitter.
    time = millis();
 
    if (wiresConnector == LOW) {
      float deltaTime = time - TimeOfConnection;
      TimeOfConnection = time;
 
      if (deltaTime > 50) {
        Serial.println(deltaTime);
        float rotSpeed = (0.054 * 2 * 3.14) / (deltaTime * 0.001);
        Serial.println(rotSpeed, 7);
 
        if ((int)rotSpeed - rotSpeed < -0.5) {
          rotSpeed += 1;
        }
 
        char buff0[50];
        buff0[0] = '\0';
        sprintf(buff0, "%d ", (int)(rotSpeed * 100));
        WindSpeed[0] = '\0';
        strcat(WindSpeed, buff0); //Could use strncpy()
        Serial.print(" \n Windspeed:");
        Serial.print(WindSpeed);
        Serial.print("\n");
      }
    }
  }
}
