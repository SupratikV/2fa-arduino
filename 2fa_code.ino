#include <Keypad.h>                 // This sketch uses the Keypad.h library
#include <SPI.h>                    // This sketch uses the SPI.h library
#include <MFRC522.h>                // This sketch uses the MFRC522.h library

#define SS_PIN 10                   // Define SS_PIN of the RC522 RFID Reader to digital pin 10 of the Arduino
#define RST_PIN 9                   // Define RST_PIN of the RC522 RFID Reader to digital pin 9 of the Arduino
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance. 

int RedLed    = A3;                 // Red LED output is analog pin A3
int GreenLed  = A4;                 // Green LED output is analog pin A4
int Buzzer    = A5;                 // Buzzer output is analog pin A5
unsigned long Timer;                // Variable for the Timer



const byte ROWS = 4;                // Four rows
const byte COLS = 3;                // Three columns

                              
char Keys[ROWS][COLS] = {           // Define the symbols on the buttons of the keypads
  {'1','2','3',},
  {'4','5','6',},
  {'7','8','9',},
  {'*','0','#',}
};

byte rowPins[ROWS] = {5, 4, 3, 2};  // Connect to the row pinouts of the keypad
byte colPins[COLS] = {8, 7, 6};     // Connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(Keys), rowPins, colPins, ROWS, COLS); // Initialize an instance of class NewKeypad

int RightCard;                      // Variable for a known Mifare Card
int RightPinCode;                   // Variable for valid key input in combination with a known Mifare Card
int WrongPinCode;                   // Variable for a invalid key input in combination with a known Mifare Card
int PinCodeCounter;                 // Variable Counter for counting the number of pincode inputs on the keypad

int Code1Correct;                   // Variable for the first correct digit (Code) input on the keypad
int Code2Correct;                   // Variable for the second correct digit (Code) input on the keypad
int Code3Correct;                   // Variable for the third correct digit (Code) input on the keypad
int Code4Correct;                   // Variable for the fourth correct digit (Code) input on the keypad
int Code5Correct;                   // Variable for the fifth correct digit (Code) input on the keypad
int Code6Correct;                   // Variable for the sixth correct digit (Code) input on the keypad
int Reset;                          // Reset for the loop

int MifareCard1;                               // We need this specific variable to determine the valid PinCode in combination with the UID nummer of the Mifare Card
const int Code1MifareCard1 = '6';              // Determine here the first digit of your 6 digit PIN
const int Code2MifareCard1 = '6';              // Determine here the second digit of your 6 digit PIN
const int Code3MifareCard1 = '6';              // Determine here the third digit of your 6 digit PIN
const int Code4MifareCard1 = '6';              // Determine here the fourth digit of your 6 digit PIN
const int Code5MifareCard1 = '6';              // Determine here the fifth digit of your 6 digit PIN
const int Code6MifareCard1 = '6';              // Determine here the sixth digit of your 6 digit PIN

int MifareCard2;                               // We need this specific variable to determine the valid PinCode in combination with the UID nummer of the Mifare Card
const int Code1MifareCard2 = '5';              // Determine here the first digit of your 6 digit PIN
const int Code2MifareCard2 = '5';              // Determine here the second digit of your 6 digit PIN
const int Code3MifareCard2 = '5';              // Determine here the third digit of your 6 digit PIN
const int Code4MifareCard2 = '5';              // Determine here the fourth digit of your 6 digit PIN
const int Code5MifareCard2 = '5';              // Determine here the fifth digit of your 6 digit PIN
const int Code6MifareCard2 = '5';              // Determine here the sixth digit of your 6 digit PIN





void setup() 

{  
  Serial.begin(9600);                                     // Initialize serial communications with the PC
  SPI.begin();                                            // Initialize the SPI bus
  mfrc522.PCD_Init();                                     // Initialize the MFRC522 
  pinMode (RedLed, OUTPUT);                               // Define RedLed as OUTPUT
  pinMode (GreenLed, OUTPUT);                             // Define GreenLed as OUTPUT
  pinMode (Buzzer, OUTPUT);                               // Define Buzzer as OUTPUT
}





void loop() {   


//----------------------------------------------Code for the reset function of the loop--------------------------------------------------------------------------------------------------------


if (Reset == 1)                                           // If Reset has value 1 (HIGH) all variables will be reset to 0 and the RC522 RFID reader will be waiting for a new Mifare Card.
{                                                         
  RightCard = 0;
  MifareCard1 = 0; 
  MifareCard2 = 0;                                      
  RightPinCode = 0;
  WrongPinCode = 0;
  Code1Correct = 0;
  Code2Correct = 0;
  Code3Correct = 0;
  Code4Correct = 0;
  Code5Correct = 0;
  Code6Correct = 0;
  PinCodeCounter = 0;  
  delay (50);
  Reset = 0;
}


//----------------------------------------------Code for the control for a valid pincode within the timelimit of 7 seconds after presenting/offering a valid Mifare Card ---------------------------


if (millis()- Timer > 7000 && RightCard == 1)            // If the Timer will be larger as 7 seconds and a valid Mifare Card is offered, reset value will be high.
   {                                                     // This means that you have after presenting a valid card, 7 seconds to enter a valid code or the loop resets.
    Reset = 1;
    Serial.println("CardAccesOff");
   }



   
//----------------------------------------------Reading the presented/offered Mifare Card at the MFRC522 ----------------------------------------------------------------------------------------- 

 
if   (mfrc522.PICC_IsNewCardPresent() &&                
      mfrc522.PICC_ReadCardSerial())
    { 

      
//----------------------------------------------Code for detect the first Mifare Card in the Database. This is a UID number with 4 pairs of digits. Example: 69 7C 01 9C------------------------------------
     if                                                  
     (mfrc522.uid.uidByte[0] == 0xF3   &&                // Fill in the first digits of the UID number of your Mifare card
      mfrc522.uid.uidByte[1] == 0x54   &&                // Fill in the seconds digits of the UID number of your Mifare card              
      mfrc522.uid.uidByte[2] == 0xE3   &&                // Fill in the third digits of the UID number of your Mifare card
      mfrc522.uid.uidByte[3] == 0x0D)                    // Fill in the fourth digits of the UID number of your Mifare card
   
     {
      RightCard = 1;                                     // The offered Mifare Card is known in the database, set variable "RightCard" to 1
      MifareCard1 = 1;                                   // This Mifare Card is known as Mifare Card 2 in the Database, so set variable MifareCard2 to value 1. 
      digitalWrite (Buzzer, HIGH);                       // Make a beep for the accepted Mifare Card             
      delay (150);                                       //
      digitalWrite (Buzzer, LOW);                        //
     
      PinCodeCounter = 0;                                // Reset the PinCodeCounter to 0
      Timer =  millis();                                 // Reset the Timer. The 7 seconds limit is running now for a valid PinCode
      Serial.println("CardAccesOn");                     // Print the text "CardAccesOn" to the serial monitor 
      delay (200);                                       // Wait 200 milliseconds
      }

      
//-----------------------------------------------Code for detect the second Mifare Card in the Database. This is a UID number with 4 pairs of digits. Example: EB 70 C0 BC----------------------------------------------
     if                                                    
     (mfrc522.uid.uidByte[0] == 0x33   &&                 // Fill in the first digits of the UID number of your Mifare card
      mfrc522.uid.uidByte[1] == 0x43   &&                 // Fill in the seconds digits of the UID number of your Mifare card              
      mfrc522.uid.uidByte[2] == 0x4A   &&                 // Fill in the third digits of the UID number of your Mifare card
      mfrc522.uid.uidByte[3] == 0x10)                     // Fill in the fourth digits of the UID number of your Mifare card
                                                       
    {     
      RightCard = 1;                                       // If the offered Mifare Card is known in the database, set variable "RightCard" to 1
      MifareCard2 = 1;
      digitalWrite (Buzzer, HIGH);                         // Make a beep for the accepted Mifare Card             
      delay (150);                                         //
      digitalWrite (Buzzer, LOW);                          //
     
      PinCodeCounter = 0;                                  // Reset the PinCodeCounter to 0
      Timer =  millis();                                   // Reset the Timer. The 7 seconds limit is running now for a valid PinCode
      Serial.println("CardAccesOn");                       // Print the text "CardAccesOn" to the serial monitor 
      delay (200);                                         // Wait 200 milliseconds
    } 

    }   

//-----------------------------------------------Code for accepting a valid MifareCard + valid PinCode-------------------------------------------------------------------------------------------------



if (Code6Correct == 1 && RightCard == 1)                    // If the PinCode is the correct and you did enter this within 7 seconds
  {
    RightPinCode = 1;                                       // Variable RightPinCode will be set to 1 

    digitalWrite (GreenLed, HIGH);                          // Make a nice beep for the correct Pincode and set the Green LED high for 1 second
    delay (150);                                            // 
    digitalWrite (Buzzer, HIGH);                            //
    delay (150);                                            //
    digitalWrite (Buzzer, LOW);                             //
    delay (50);                                             //
    digitalWrite (Buzzer, HIGH);                            //
    delay (150);                                            //
    digitalWrite (Buzzer, LOW);                             //
    delay (500);                                            //          
    digitalWrite (GreenLed, LOW);                           //                          
   
    Serial.println("Correct PinCode");                      // Print the text "Correct PinCode" to the serial monitor              
    Reset = 1;                                              // Reset the loop
  }

//-----------------------------------------------Code for refusing a valid MifareCard + invalid PinCode------------------------------------------------------------------------------------------------



if ((Code6Correct == 0) && (PinCodeCounter >= 6) && (RightCard == 1))       // If you offered a valid Mifare Card and the Pincode is not correct and you already entered 6 digitkeys 
  {  
    WrongPinCode = 1;                                                       // Variable WrongPinCode will be set to 1
    Serial.println("WrongCode");                                             // Print the text "WrongKey" to the serial monitor
    Reset = 1;                                                              // Reset the loop
  } 


//-----------------------------------------------Code for refusing a Wrong/Invalid PinCode or the Timer (7seconds) expired----------------------------------------------------------------------------



if ((WrongPinCode == 1) || (millis()- Timer > 7000 && RightCard == 1))      // If you offered a valid Mifare Card and  you entered a wrong PinCode or the timer (7 seconds) expires
  { 
    digitalWrite (Buzzer, HIGH);                                            // Make a long beep and set the Red LED HIGH for the wrong code or timer expiry
    digitalWrite (RedLed, HIGH);                                           //
    delay(1500);                                                            //
    digitalWrite (Buzzer, LOW);                                             //
    digitalWrite (RedLed, LOW);                                             //
                                                 
    Serial.println("WrongCode or Timer expired");                           // Print the text "WrongCode or Timer expired" to the serial monitor                      
    Reset = 1;                                                              // Reset the loop
  }


//-----------------------------------------------Code for counting the inputs on the keypad----------------------------------------------------------------------------------------------------------



char KeyDigit = keypad.getKey();                                           // Get the DigitKey from the keypad

if ((RightCard == 1) &&                                                    // If you offered a valid Mifare Card and any DigitKey on the keypad is pressed
    ((KeyDigit == '1') || 
    (KeyDigit == '2')  || 
    (KeyDigit == '3')  || 
    (KeyDigit == '4')  || 
    (KeyDigit == '5')  || 
    (KeyDigit == '6')  || 
    (KeyDigit == '7')  || 
    (KeyDigit == '8')  || 
    (KeyDigit == '9')  || 
    (KeyDigit == '0')  || 
    (KeyDigit == '*')  || 
    (KeyDigit == '#')))
 
    {                               
      PinCodeCounter++;                                                       // PinCodeCounter value +1 for every press on any Digitkey on the keypad
      digitalWrite (Buzzer, HIGH);                                            // Make a short beep for a DigitKey press on the keypad
      delay (50);                                                             //
      digitalWrite (Buzzer, LOW);                                             //
    }


//-----------------------------------------------Code for counting the inputs on the keypad----------------------------------------------------------------------------------------------------------

//-----------------------------------------------Code for counting the inputs on the keypad----------------------------------------------------------------------------------------------------------




//-----------------------------------------------Code for determine the correct PinCode for MifareCard1----------------------------------------------------------------------------------------------------------





if ((KeyDigit == Code1MifareCard1) && (RightCard == 1) && (Code1Correct == 0) && (MifareCard1 == 1))           // If you offered the valid MifareCard1, first PinCode and you did not entered it before (Otherwise we conflict with same KeyDigits)
    {
      Code1Correct = 1;                                                                                        // Variable Code1Correct is set to 1
      return;                                                                                                  // Return to begin loop
    } 
      
if ((KeyDigit == Code2MifareCard1) && (Code1Correct == 1) && (Code2Correct == 0) && (MifareCard1 == 1))        // If you offered the valid MifareCard1, second PinCode and you did not entered it before (Otherwise we conflict with same KeyDigits)
    {
      Code2Correct = 1;                                                                                        // Variable Code2Correct is set to 1
      return;                                                                                                  // Return to begin loop
    } 
    
if ((KeyDigit == Code3MifareCard1) && (Code2Correct == 1) && (Code3Correct == 0) && (MifareCard1 == 1))         // If you offered the valid MifareCard1, third PinCode and you did not entered it before (Otherwise we conflict with same KeyDigits)
    {
      Code3Correct = 1;                                                                                         // Variable Code3Correct is set to 1
      return;                                                                                                   // Return to begin loop
    } 
     
if ((KeyDigit == Code4MifareCard1) && (Code3Correct == 1) && (Code4Correct == 0) && (MifareCard1 == 1))         // If you offered the valid MifareCard1, fourth PinCode and you did not entered it before (Otherwise we conflict with same KeyDigits)
    {
      Code4Correct = 1;                                                                                         // Variable Code4Correct is set to 1
      return;                                                                                                   // Return to begin loop
    }  
if ((KeyDigit == Code5MifareCard1) && (Code4Correct == 1) && (Code5Correct == 0) && (MifareCard1 == 1))         // If you offered the valid MifareCard1, fifth PinCode and you did not entered it before (Otherwise we conflict with same KeyDigits)
    {
      Code5Correct = 1;                                                                                         // Variable Code5Correct is set to 1
      return;                                                                                                   // Return to begin loop
    } 

if ((KeyDigit == Code6MifareCard1) && (Code5Correct == 1) && (Code6Correct == 0) && (MifareCard1 == 1))         // If you offered the valid MifareCard1, sixth PinCode and you did not entered it before (Otherwise we conflict with same KeyDigits)
    {
      Code6Correct = 1;                                                                                         // Variable Code6Correct is set to 1
      return;                                                                                                   // Return to begin loop
    }


    

//-----------------------------------------------Code for determine the correct PinCode for MifareCard2----------------------------------------------------------------------------------------------------------



    

if ((KeyDigit == Code1MifareCard2) && (RightCard == 1) && (Code1Correct == 0) && (MifareCard2 == 1))              // If you offered a the valid MifareCard2, first PinCode and you did not entered it before (Otherwise we conflict with same KeyDigits)
    {
      Code1Correct = 1;                                                                                           // Variable Code1Correct is set to 1
      return;                                                                                                     // Return to begin loop
    } 
      
if ((KeyDigit == Code2MifareCard2) && (Code1Correct == 1) && (Code2Correct == 0) && (MifareCard2 == 1))           // If you offered a the valid MifareCard2, second PinCode and you did not entered it before (Otherwise we conflict with same KeyDigits)
    {
      Code2Correct = 1;                                                                                           // Variable Code2Correct is set to 1
      return;                                                                                                     // Return to begin loop
    } 
    
if ((KeyDigit == Code3MifareCard2) && (Code2Correct == 1) && (Code3Correct == 0) && (MifareCard2 == 1))           // If you offered a the valid MifareCard2, third PinCode and you did not entered it before (Otherwise we conflict with same KeyDigits)
    {
      Code3Correct = 1;                                                                                           // Variable Code3Correct is set to 1
      return;                                                                                                     // Return to begin loop
    } 
     
if ((KeyDigit == Code4MifareCard2) && (Code3Correct == 1) && (Code4Correct == 0) && (MifareCard2 == 1))          // If you offered a the valid MifareCard2, fourth PinCode and you did not entered it before (Otherwise we conflict with same KeyDigits)
    {
      Code4Correct = 1;                                                                                          // Variable Code4Correct is set to 1
      return;                                                                                                    // Return to begin loop
    }  
if ((KeyDigit == Code5MifareCard2) && (Code4Correct == 1) &&  (Code5Correct == 0) && (MifareCard2 == 1))         // If you offered a the valid MifareCard2, fifth PinCode and you did not entered it before (Otherwise we conflict with same KeyDigits)
    {
      Code5Correct = 1;                                                                                          // Variable Code5Correct is set to 1
      return;                                                                                                    // Return to begin loop
    } 

if ((KeyDigit == Code6MifareCard2) && (Code5Correct == 1) &&  (Code6Correct == 0) && (MifareCard2 == 1))         // If you offered a the valid MifareCard2, sixth PinCode and you did not entered it before (Otherwise we conflict with same KeyDigits)
    {
      Code6Correct = 1;                                                                                          // Variable Code6Correct is set to 1
      return;                                                                                                    // Return to begin loop
    } 
    
}