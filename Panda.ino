#include <Servo.h>
#include <Stepper.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *myMotor = AFMS.getMotor(1);       // MOTOR for Player1
Adafruit_DCMotor *myMotor2 = AFMS.getMotor(4);       // MOTOR for Player2
/*
 *  GAME ON/OFF CONTROL Variables
 */
int onOffSwitch = 49;     // Game ON/OFF Switch
bool gameIsOn = false;      // Players can play the game
bool wasTurnedOn = false;
bool gameEnded1 = false;
bool gameEnded2 = false;
/*
 *   PLAYER_1 Variables
 */
int jumpSensor1 = 24;         // Sensor for Jumping
int runSensor1 = 26;          // Sensor for Running
int startPin1 = 28;           // Startline for P1
int endPin1 = 30;             // Finishline for P1
Servo jumpServo1;             // Servo for jumping
int pos = 0;

int obstaclePinA = 2;        // Obstacles for P1
int obstaclePinB = 3;
int obstaclePinC = 4;

bool atStartline1 = false;         // Player1 stands at the start line
bool canRun1 = false;             // Player1 can run
bool canJump1 = false;            // Player1 can jump
bool atEndline1 = false;          // Player1 stands at the end line

/*
 *   PLAYER_2 Variables
 */
int jumpSensor2 = 25;         // Sensor for Jumping
int runSensor2 = 27;          // Sensor for Running
int startPin2 = 29;           // Startline for P1
int endPin2 = 31;             // Finishline for P1
Servo jumpServo2;             // Servo for jumping
int pos2 = 0;

int obstaclePinD = 5;        // Obstacles for P1
int obstaclePinE = 6;
int obstaclePinF = 7;

bool atStartline2 = false;         // Player1 stands at the start line
bool canRun2 = false;             // Player1 can run
bool canJump2 = false;            // Player1 can jump
bool atEndline2 = false;          // Player1 stands at the end line

bool onOffVal;
bool jumpVal1;
bool runVal1;
bool startPinVal1;
bool endPinVal1;
bool obstaclePinValA;
bool obstaclePinValB;
bool obstaclePinValC;
bool jumpVal2;
bool runVal2;
bool startPinVal2;
bool endPinVal2;
bool obstaclePinValD;
bool obstaclePinValE;
bool obstaclePinValF;

/**********************************************
 * 
 * SETUP
 * 
 **********************************************/
void setup() {
  Serial.begin(9600);
  AFMS.begin();
  pinMode(onOffSwitch, INPUT);
  // PLAYER1
  pinMode(jumpSensor1, INPUT) ;  
  jumpServo1.attach(10);    
  pinMode(runSensor1, INPUT);
  pinMode(startPin1, INPUT);
  pinMode(endPin1, INPUT);
  pinMode(obstaclePinA, INPUT);
  pinMode(obstaclePinB, INPUT);
  pinMode(obstaclePinC, INPUT);
  // PLAYER2
  pinMode(jumpSensor2, INPUT) ;  
  jumpServo2.attach(11);    
  pinMode(runSensor2, INPUT);
  pinMode(startPin2, INPUT);
  pinMode(endPin2, INPUT);
  pinMode(obstaclePinD, INPUT);
  pinMode(obstaclePinE, INPUT);
  pinMode(obstaclePinF, INPUT);

  myMotor->run(RELEASE);
  myMotor2->run(RELEASE);
}


/**********************************************
 * 
 * LOOP
 * 
 **********************************************/
void loop() {
  // digitalRead
  onOffVal = digitalRead(onOffSwitch);
  
  jumpVal1 = digitalRead (jumpSensor1) ;
  runVal1 = digitalRead(runSensor1);
  startPinVal1 = digitalRead (startPin1);
  endPinVal1 = digitalRead(endPin1);
  obstaclePinValA = digitalRead (obstaclePinA);
  obstaclePinValB = digitalRead (obstaclePinB);
  obstaclePinValC = digitalRead (obstaclePinC);

  jumpVal2 = digitalRead (jumpSensor2) ;
  runVal2 = digitalRead(runSensor2);
  startPinVal2 = digitalRead (startPin2);
  endPinVal2 = digitalRead(endPin2);
  obstaclePinValD = digitalRead (obstaclePinD);
  obstaclePinValE = digitalRead (obstaclePinE);
  obstaclePinValF = digitalRead (obstaclePinF);
 
  myMotor->setSpeed(255);
  myMotor2->setSpeed(255);
  //delay(5); 

// Is the game ON or OFF?
  if(onOffVal == HIGH) {                    
    Serial.println("====================GAME-ON===================");
    gameIsOn = true;
    wasTurnedOn = true;
    if(atStartline1){
       readyForGame1(); 
    } else{
        if(!canRun1 && !wasTurnedOn){
           endGame1();
        }
     }


    if(atStartline2){
       readyForGame2(); 
    } else{
        if(!canRun2 && !wasTurnedOn){
           endGame2();
        }   
    }             
  } else {
    Serial.println("====================GAME-OFF==================");
    if(wasTurnedOn){
       endGame1();
       endGame2();
    }
  }


  if(gameIsOn){
    // Wenn the player is standing at the start line
    if(atStartline1){
           runCheck1();
            
    // Wenn the player is NOT standing at the start line      
    }else{
          if(atEndline1){
            endGame1();
          }else{
              
              canJump1 = true;
              runCheck1();
          }
     }   


     if(atStartline2){
           runCheck();
            
    // Wenn the player is NOT standing at the start line      
    }else{
          if(atEndline2){
            endGame2();
          }else{
              
              canJump2 = true;
              runCheck2();
          }
     }   
  }

jumpCheck1();
jumpCheck2();

/*
 * Interaction with Obstacles
 */

 if(obstaclePinValA == HIGH || obstaclePinValB == HIGH || obstaclePinValC == HIGH) {
   Serial.println("P1: Contact with Obstacles");
   canRun1 = false;
   canJump1 = false;
   stopRun1();
   delay(2);
   runBackward1();
   delay(500);
   canRun1 = true;
   canJump1 = true;
   //stopRun1();
 } else {
    Serial.println("P1: No Contact with Obstacles");
 }

  if(obstaclePinValD == HIGH || obstaclePinValE == HIGH || obstaclePinValF == HIGH) {
   Serial.println("P2: Contact with Obstacles");
   canRun2 = false;
   canJump2 = false;
   stopRun2();
   delay(2);
   runBackward2();
   delay(500);
   canRun2 = true;
   canJump2 = true;
   //stopRun2();
 } else {
    Serial.println("P2: No Contact with Obstacles");
 }

  /*
   * START LINE
   */
  if(startPinVal1 == HIGH){
    Serial.println("=== P1 : AT THE START LINE ===");
    atStartline1 = true;
    //canRun1 = true;
    //canJump1 = true;
      readyForGame1();
    if(atEndline1){
      atEndline1 = false;
    }
  } else {
    Serial.println("=== P1 : NOT at the START LINE ===");
    atStartline1 = false;
  }


   if(startPinVal2 == HIGH){
    Serial.println("=== P2 : AT THE START LINE ===");
    atStartline2 = true;
    //canRun2 = true;
    //canJump2 = true;
      readyForGame2();
    if(atEndline2){
      atEndline2 = false;
    }
  } else {
    Serial.println("=== P2 : NOT at the START LINE ===");
    atStartline2 = false;
  }




 /*
  * END LINE
  */

  if(endPinVal1 == HIGH) {      // at the end line
      Serial.println("=== P1 : AT THE END LINE ===");
      atEndline1 = true;
      //canJump1 = false;
      //canRun1 = false;
      //stopRun1();
      //myMotor -> setSpeed(0);
      endGame1();
  } else {
      Serial.println("=== P1 : NOT at the END LINE ===");
      if(atStartline1 && atEndline1){
        atEndline1 = false;
      }
  }


  if(endPinVal2 == HIGH) {      // at the end line
      Serial.println("=== P2 : AT THE END LINE ===");
      atEndline2 = true;
      //canJump2 = false;
      //canRun2 = false;
      //stopRun2();
      //myMotor2 -> setSpeed(0);
      endGame2();
  } else {
      Serial.println("=== P2 : NOT at the END LINE ===");
      if(atStartline2 && atEndline2){
        atEndline2 = false;
      }
  }
  
}

/*
 * LOOP END
 */



void runCheck1(){
  if(runVal1 == HIGH){
                Serial.println("P1: No signal for running");
                if(!canRun1){
                  canRun1 = true;
                }
                stopRun1();
                //myMotor -> setSpeed(0);
          }else{
                Serial.println("P1: Signal for running");
                if(canRun1){
                 // myMotor -> setSpeed(255);
                  runForward(); 
                  delay(500);
                  canRun1 = false;
                  stopRun1(); 
                }else{
                  stopRun1();
                  //delay(5);
                }
          }
}


void runCheck2(){
  if(runVal2 == HIGH){
                Serial.println("P2: No signal for running");
                if(!canRun2){
                  canRun2 = true;
                }
                stopRun2();
                //myMotor -> setSpeed(0);
          }else{
                Serial.println("P2: Signal for running");
                if(canRun2){
                 // myMotor -> setSpeed(255);
                  runForward(); 
                  delay(500);
                  canRun2 = false;
                  stopRun2(); 
                }else{
                  stopRun2();
                  //delay(5);
                }
          }
}


void readyForGame1() {
    Serial.println("====P1: Ready For Game====");
   
    stopRun1();
    pos = 90;
    jumpServo1.write(pos); 
    delay(5);
    canRun1 = true;
    canJump1 = true;
}

void readyForGame2() {
    Serial.println("====P2: Ready For Game====");
   
      stopRun2();
    pos2 = 90;
    jumpServo2.write(pos2); 
    delay(5);
    canRun2 = true;
    canJump2 = true;
}


void endGame1() {
  Serial.println("End Game is called");
  gameEnded1 = true;
  if(!atStartline1){   // while????
    Serial.println("P1: End Game: not standing at the start line");
    canRun1 = false;
    canJump1 = false;
    //stopRun1();
    //myMotor -> setSpeed(0);
    //delay(5);
    pos = 0;
    jumpServo1.write(pos); 
    delay(5);
    runBackward1(); 
  } 
    //readyForGame1();
   // canRun1 = true;
   // canJump1 = true;
}

void endGame2() {
  Serial.println("P2: End Game is called");
  gameEnded2 = true;
  if(!atStartline2){   // while????
    Serial.println("End Game: not standing at the start line");
    canRun2 = false;
    canJump2 = false;
    //stopRun1();
    //myMotor -> setSpeed(0);
    //delay(5);
    pos2 = 0;
    jumpServo2.write(pos2); 
    delay(5);
    runBackward2(); 
  } 
    //readyForGame1();
   // canRun1 = true;
   // canJump1 = true;
}




void runForward(){
    Serial.println("P1: Run Forward");
    //myMotor->setSpeed(255);
    myMotor->run(FORWARD);
}



void runForward2(){
    Serial.println("P2: Run Forward");
    //myMotor->setSpeed(255);
    myMotor2->run(FORWARD);
}


void runBackward1() {
   Serial.println("P2: Run Backward");
   myMotor->setSpeed(255);
   myMotor->run(BACKWARD);
}

void runBackward2() {
   Serial.println("P2: Run Backward");
   myMotor2->setSpeed(255);
   myMotor2->run(BACKWARD);
}

void stopRun1() {
    Serial.println("P1: Stop Running");
    //myMotor -> setSpeed(0);
    myMotor->run(RELEASE);
}

void stopRun2() {
    Serial.println("P2: Stop Running");
    //myMotor -> setSpeed(0);
    myMotor2->run(RELEASE);
}


void jumpCheck1() {
  // No siganl for jumping
  if (jumpVal1 == HIGH) {   
         Serial.println("P1: No Signal for jumping"); 
         delay(5);
         
  } 
  // Signal for jumping
  else {      
         Serial.println("P1: Signal for jumping");               
         if(canJump1){
          jump1();
          delay(5);
        }
   }
}

void jumpCheck2() {
  // No siganl for jumping
  if (jumpVal2 == HIGH) {   
         Serial.println("P2: No Signal for jumping"); 
         delay(5);
         
  } 
  // Signal for jumping
  else {      
         Serial.println("P2: Signal for jumping");               
         if(canJump2){
          jump2();
          delay(5);
        }
   }
}

void jump1() { 
       for (pos = 90; pos >= 0; pos -= 1) {     // goForwardes from 90 degrees to 0 degrees
            jumpServo1.write(pos);              // tell servo to goForward to position in variable 'pos'
            delay(5);                           // waits 5ms for the servo to reach the position
       }    
       runForward();
       delay(2300);

       for (pos = 0; pos <= 90; pos += 1) {     // goForwardes from 0 degrees to 90 degreese
           jumpServo1.write(pos);              // tell servo to goForward to position in variable 'pos'
           delay(5);                          // waits 5ms for the servo to reach the position
        }   
}


void jump2() { 
       for (pos2 = 90; pos2 >= 0; pos2 -= 1) {     // goForwardes from 90 degrees to 0 degrees
            jumpServo2.write(pos2);              // tell servo to goForward to position in variable 'pos'
            delay(5);                           // waits 5ms for the servo to reach the position
       }    
       runForward2();
       delay(2300);

       for (pos2 = 0; pos2 <= 90; pos2 += 1) {     // goForwardes from 0 degrees to 90 degreese
           jumpServo2.write(pos2);              // tell servo to goForward to position in variable 'pos'
           delay(5);                          // waits 5ms for the servo to reach the position
        }   
}
