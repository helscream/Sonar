/********************************
Author: Omer Ikram ul Haq
Date: 2014-09-30
version: beta 0.1
*********************************/

/* Example for sonar.h
         This header file used interrupts for saving valuable calculation time 
         s1.sonnarPing(); function is used for triggering the sonar pulse
         s1.distance(); returns the distance based on previous sonar pulse
         
         for the header file to work attachInterrupt(s1.echo_INT, s1_ISR, HIGH); function is used for attaching "s1.echo_INT" to a locally defined ISR which is "void s1_ISR()". This local
         function than calls the actual important function which is "s1.pingTime();". 
         
         s1.initlize_sonar(#); initialize and calibrate sonar. # can be from 1-255 and it is used for storing the calculated offset so that we don't have to calibrate it every time we start the program.
         
*/

#include <EEPROM.h>
#include "sonar.h"



sonnar_hc_sr04 s1;  
void s1_ISR(){
  s1.pingTime();
}



void setup() {
  Serial.begin(9600);
  s1.trigger_PIN = 3;
  s1.echo_INT = 0;
  s1.maximumDistance = 2;
  s1.minimumDistance = 0.08;
  attachInterrupt(s1.echo_INT, s1_ISR, HIGH);
  s1.initlize_sonar(1);
  
  //s1.calibratePing();
  
}

void loop() {
 
  delay(500);
  s1.sonnarPing();
  /*
  You don't have to wait for echo and it will be taken care of while you can
  perform other tasks, that's the beauty of an Interrupt
  ......
  ...
  your code
  ...
  ......
  */
  Serial.print("Distance = ");
  Serial.print(s1.distance()*100);
  Serial.println(" cm");

  
}










