#include <MCP4725.h>
#include <WiFi.h>
#include "Wire.h"
#include <FastPID.h>


#define POT1  10
#define POT2  11
#define POT3  12
#define POT4  13
#define HALL  14
#define SPIN  15
#define steps 50

MCP4725 MCP(0x63);

float Kp=0.1,Ki=0.5,Kd=0;

void setup() {
  // put your setup code here, to run once:
  pinMode(POT1,INPUT);
  pinMode(POT2,INPUT);
  pinMode(POT3,INPUT);
  pinMode(POT4,INPUT);
  pinMode(HALL,INPUT);
  // only needed for faster live updating
  Wire.setClock(800000);
  FastPID myPID(Kp,Ki,Kd,1000/steps,1,false);
  while(1){
    float t1 = analogRead(POT1);
    float s1 = analogRead(POT2);
    float t2 = analogRead(POT3);
    float s2 = analogRead(POT4);
    //ideally, another core keeps time. C3 ESP32 does not have another core so it is omitted here.
    if(digitalRead(SPIN) > 0){
      for(int i=0;i<t1;i+=steps){
        float real_speed = analogRead(HALL);
        if(real_speed < s1 * 0.9){ MCP.setValue(s1); }
        else{MCP.setValue(s1+myPID.step(s1,real_speed)); }
        delay(steps);
      }
      for(int i=0;i<t2;i+=steps){
        float real_speed = analogRead(HALL);
        if(real_speed < s2 * 0.9){ MCP.setValue(s2); }
        else{MCP.setValue(s2+myPID.step(s2,real_speed)); }
        delay(steps);
      }
    }  
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  
}
