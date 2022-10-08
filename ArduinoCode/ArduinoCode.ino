#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
#include <SoftwareSerial.h> //Create software serial object to communicate with SIM800L

SoftwareSerial GSM(8, 9);   //SIM800L Tx & Rx is connected to Arduino #8 & #9
LiquidCrystal_I2C lcd(0x27, 20,4);
String phone_no1 = "+639558195216"; //change with phone number to sms
String phone_no2 = "+639070366799"; 

String        RxString  = ""; // Will hold the incoming String  from the GSM shield
char          RxChar    = ' ';
int           Counter   = 0;
String        GSM_Nr    = "";
String        GSM_Msg   = "";
       

#define Relay1 2 // Load1 Pin Out
#define Relay2 3 // Load2 Pin Out
#define Relay3 4 // Load3 Pin Out
#define Relay4 5 // Load4 Pin Out

int load1, load2, load3, load4;

void setup(){ // put your setup code here, to run once

 lcd.init();                      // initialize the lcd 
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
   lcd.setCursor(6,1);
  lcd.print("WELCOME!");
  delay(2000);
 

pinMode(Relay1, OUTPUT); digitalWrite(Relay1, 1); 
pinMode(Relay2, OUTPUT); digitalWrite(Relay2, 1); 
pinMode(Relay3, OUTPUT); digitalWrite(Relay3, 1); 
pinMode(Relay4, OUTPUT); digitalWrite(Relay4, 1); 

Serial.begin(9600);//Begin serial communication with Arduino and Arduino IDE (Serial Monitor)  
GSM.begin(9600);   //Begin serial communication with Arduino and SIM800L

Serial.println("Initializing....");
initModule("AT","OK",1000);                //Scan for GSM Module
initModule("AT+CPIN?","READY",1000);       //this command is used to check whether SIM card is inserted in GSM Module or not
initModule("AT+CMGF=1","OK",1000);         //Set SMS mode to ASCII
initModule("AT+CNMI=2,2,0,0,0","OK",1000); //Set device to read SMS if available and print to serial
Serial.println("Initialized Successfully"); 
  
load1 = EEPROM.read(1);
load2 = EEPROM.read(2);
load3 = EEPROM.read(3);
load4 = EEPROM.read(4);

relays();

 if(load1==0){
    lcd.setCursor(0,0);
    lcd.print("Device 1 :  ON");}
  if(load1==1){
    lcd.setCursor(0,0);
    lcd.print("Device 1 : OFF");}
  if(load2==0){
    lcd.setCursor(0,1);
    lcd.print("Device 2 :  ON");}
  if(load2==1){
    lcd.setCursor(0,1);
    lcd.print("Device 2 : OFF");}
  if(load3==0){
    lcd.setCursor(0,2);
    lcd.print("Device 3 :  ON");}
  if(load3==1){
    lcd.setCursor(0,2);
    lcd.print("Device 3 : OFF");}
  if(load4==0){
    lcd.setCursor(0,3);
    lcd.print("Device 4 :  ON");}
   if(load4==1){
    lcd.setCursor(0,3);
    lcd.print("Device 4 : OFF");}

delay(100);
}

void loop(){
  
  // scan for data from software serial port
  //-----------------------------------------------
  RxString = "";
  Counter = 0;
  while(GSM.available()){
    delay(1);  // short delay to give time for new data to be placed in buffer
    // get new character
    RxChar = char(GSM.read());
    //add first 200 character to string
    if (Counter < 200) {
      RxString.concat(RxChar);
      Counter = Counter + 1;
    }
  }
 
  // Is there a new SMS?
  //-----------------------------------------------
  if (Received(F("CMT:")) ) GetSMS();

if(GSM_Nr==phone_no1 || GSM_Nr==phone_no2){
  
if(GSM_Msg=="DEVICE_1_ON") {load1=0;
   lcd.setCursor(0,0);
   lcd.print("Device 1 :  ON");
   sendSMS(GSM_Nr,"Load1 On");}
   
if(GSM_Msg=="DEVICE_1_OFF"){load1=1; 
   lcd.setCursor(0,0);
   lcd.print("Device 1 : OFF");
   sendSMS(GSM_Nr,"Load1 Off");}

if(GSM_Msg=="DEVICE_2_ON") {load2=0; 
   lcd.setCursor(0,1);
   lcd.print("Device 2 :  ON");
   sendSMS(GSM_Nr,"Load2 On");}

if(GSM_Msg=="DEVICE_2_OFF"){load2=1; 
   lcd.setCursor(0,1);
   lcd.print("Device 2 : OFF");
   sendSMS(GSM_Nr,"Load2 Off");}

if(GSM_Msg=="DEVICE_3_ON") {load3=0; 
   lcd.setCursor(0,2);
   lcd.print("Device 3 :  ON");
   sendSMS(GSM_Nr,"Load3 On");}
   
if(GSM_Msg=="DEVICE_3_OFF"){load3=1;
   lcd.setCursor(0,2);
   lcd.print("Device 3 : OFF");
   sendSMS(GSM_Nr,"Load3 Off");}

if(GSM_Msg=="DEVICE_4_ON") {load4=0;
   lcd.setCursor(0,3);
   lcd.print("Device 4 :  ON");
   sendSMS(GSM_Nr,"Load4 On");}

if(GSM_Msg=="DEVICE_4_OFF"){load4=1;
   lcd.setCursor(0,3);
   lcd.print("Device 4 : OFF");
   sendSMS(GSM_Nr,"Load4 Off");}

if(GSM_Msg=="ALL_ON") {load1=0, load2=0, load3=0, load4=0;
  lcd.setCursor(0,0);
  lcd.print("Device 1 :  ON");
  lcd.setCursor(0,1);
  lcd.print("Device 2 :  ON");
  lcd.setCursor(0,2);
  lcd.print("Device 3 :  ON");
  lcd.setCursor(0,3);
  lcd.print("Device 4 :  ON");
                        sendSMS(GSM_Nr,"All Load is On");}
if(GSM_Msg=="ALL_OFF"){load1=1, load2=1, load3=1, load4=1; 
  lcd.setCursor(0,0);
  lcd.print("Device 1 : OFF");
  lcd.setCursor(0,1);
  lcd.print("Device 2 : OFF");
  lcd.setCursor(0,2);
  lcd.print("Device 3 : OFF");
  lcd.setCursor(0,3);
  lcd.print("Device 4 : OFF");
  sendSMS(GSM_Nr,"All Load is Off");}

if(GSM_Msg=="loadstatus"){
String loadst = "";

if(load1==0){loadst="Load1 ON\r\n";}
        else{loadst="Load1 OFF\r\n";}
        
if(load2==0){loadst=loadst + "Load2 ON\r\n";}
        else{loadst=loadst + "Load2 OFF\r\n";}
        
if(load3==0){loadst=loadst + "Load3 ON\r\n";}
        else{loadst=loadst + "Load3 OFF\r\n";}
        
if(load4==0){loadst=loadst + "Load4 ON";}
        else{loadst=loadst + "Load4 OFF";}
        
sendSMS(GSM_Nr,loadst);
}

eeprom_write();
relays();
}


GSM_Nr="";
GSM_Msg="";
}

void eeprom_write(){
EEPROM.write(1,load1);
EEPROM.write(2,load2);
EEPROM.write(3,load3);
EEPROM.write(4,load4);  
}

void relays(){  
digitalWrite(Relay1, load1); 
digitalWrite(Relay2, load2); 
digitalWrite(Relay3, load3); 
digitalWrite(Relay4, load4); 
}

// Send SMS 
void sendSMS(String number, String msg){
GSM.print("AT+CMGS=\"");GSM.print(number);GSM.println("\"\r\n"); //AT+CMGS=”Mobile Number” <ENTER> - Assigning recipient’s mobile number
delay(500);
GSM.println(msg); // Message contents
delay(500);
GSM.write(byte(26)); //Ctrl+Z  send message command (26 in decimal).
delay(5000);  
}

// Get SMS Content
void GetSMS() {
  //Get SMS number
  //================================================
  GSM_Nr  = RxString;
  //get number
  int t1 = GSM_Nr.indexOf('"');
  GSM_Nr.remove(0,t1 + 1);
  t1 = GSM_Nr.indexOf('"');
  GSM_Nr.remove(t1);
   
  // Get SMS message
  //================================================
  GSM_Msg = RxString;
  t1 = GSM_Msg.indexOf('"');
  GSM_Msg.remove(0,t1 + 1);
  t1 = GSM_Msg.indexOf('"');
  GSM_Msg.remove(0,t1 + 1);
  t1 = GSM_Msg.indexOf('"');
  GSM_Msg.remove(0,t1 + 1);
  t1 = GSM_Msg.indexOf('"');
  GSM_Msg.remove(0,t1 + 1);
  t1 = GSM_Msg.indexOf('"');
  GSM_Msg.remove(0,t1 + 1);
  t1 = GSM_Msg.indexOf('"');
  GSM_Msg.remove(0,t1 + 1);
  GSM_Msg.remove(0,1);
  GSM_Msg.trim();

Serial.print("Number:"); Serial.println(GSM_Nr);
Serial.print("SMS:"); Serial.println(GSM_Msg);
}

// Search for specific characters inside RxString 
boolean Received(String S) {
  if (RxString.indexOf(S) >= 0) return true; else return false;
}


// Init GSM Module 
void initModule(String cmd, char *res, int t){
while(1){
    Serial.println(cmd);
    GSM.println(cmd);
    delay(100);
    while(GSM.available()>0){
       if(GSM.find(res)){
        Serial.println(res);
        delay(t);
        return;
       }else{Serial.println("Error");}}
    delay(t);
  }
}
