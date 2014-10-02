/*
Author: Omer Ikram ul haq
Date: 2014-09-30
Location: Multan, Pakistan
Ver: 0.1 beta
*/

#ifndef sonar.h
  #define sonar.h
  
  #include <Arduino.h>
  
  
  #define soundSpeed 0.00034029 // m / micro second
  #define MINIMUM_DISTANCE 0.08 // Practically found for HC-SR04
  #define MAXIMUM_DISTANCE 2 // Practically found for HC-SR04
  
  
 
  
  class sonnar_hc_sr04{
  
    public:
      volatile unsigned long t_2;
      unsigned long t_1;
      unsigned int offset, trigger_PIN, echo_INT;
      float minimumDistance, maximumDistance;
      
      /**************************************
      Echo interrupt routine
      ***************************************/
      void pingTime() { // ISR for detecting the echo
        t_2 = micros();
      }    
      
      
      void initlize_sonar(int sonar_ID){
        pinMode(trigger_PIN, OUTPUT);
        digitalWrite(trigger_PIN, LOW);
        delay(1000); // the status of trigger output might change due to previous instruction
        // Reading stored offset if available
        offset = EEPROM.read(sonar_ID)*10 ;
        if (offset == 0){
          Serial.print("Sonar with ID # ");
          Serial.print(sonar_ID);
          Serial.print(" is not calibrated...");
        }else{
          Serial.print("Calibration offset of ");
          Serial.print(offset);
          Serial.print(" micro seconds is found for Sonar with ID # ");
          Serial.print(sonar_ID);
          Serial.println(". Do you want to load it...");
          Serial.print("Press any key if you don't want to load the offset with-in 5 sec.");
          for (int i=1;i<5;i++){
            delay(1000);
            Serial.print('.');
            if (Serial.available() != 0){
              Serial.println("");
              calibratePing();
              Serial.print("The offset is saved in the slot of Sonar ID # ");
              Serial.print(sonar_ID);
              EEPROM.write(sonar_ID, offset/10); 
              break;
            }
          }
          Serial.println('.');
        
      }
      }
      
      /**************************************
      Calibrate Function
      ***************************************/
      float calibratePing() {
        
        while(Serial.available() != 0){      // Clear out the serial buffer
        Serial.read();
        }
        
        Serial.println("Place HC-SR04 a certain distance from a flat vertical surface (e.g. a wall) and measure its distance between them...");
        Serial.print("Please enter the measured distance in meters: ");
        
        
      
    
        while(Serial.available() == 0) { }
        float Std_d = Serial.parseFloat();
        Serial.println(Std_d);
        
        
        unsigned long Std_d_t = 2*Std_d/soundSpeed;
       
        sonnarPing();
        offset =  sonarDeltaPing()-Std_d_t;
        
        for(int n=1;n < 100;n++){
          sonnarPing();
          offset = (offset + sonarDeltaPing()-Std_d_t)/2;
       }
        Serial.print("Offset calculated ... ");
        Serial.print(offset);
        Serial.println(" micro seconds");
        
      }
      
      
      /**************************************
      Trigger Sonar
      ***************************************/
      void sonnarPing(){ // Send a pulse
        t_1 = 0;
        digitalWrite(trigger_PIN, HIGH);
        delayMicroseconds(10);  // Making sure HC-SR04 reads the trigger
        digitalWrite(trigger_PIN, LOW);
        t_1 = micros();
      }
      
      
      /**************************************
      Reading Time difference
      ***************************************/
      unsigned long sonarDeltaPing(){
        unsigned long d_ping;
        while (micros() < t_1+2*maximumDistance/soundSpeed+offset){
            if (t_2 > t_1+2*minimumDistance/soundSpeed) {
              d_ping = t_2-t_1;
              break;  
            }else {
              d_ping = micros()-t_1;
            }
         }
        return(d_ping);
      }
      
      /**************************************
      Calculating distance
      ***************************************/
      float distance(){
        
        float distance = (sonarDeltaPing()-offset)*soundSpeed/2;
        return(distance);
      }
      
     
      

 
  };
  
#endif
