#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <SD.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11,8, 7, 6, 5);
const int chipSelect = 4;
File myFile;
int x=0;
int y=0;
int getFingerprintIDez();
uint8_t getFingerprintEnroll(uint8_t id);



SoftwareSerial mySerial(2, 3);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup()  
{
  lcd.begin(16,2); lcd.setCursor(0,0); lcd.print("Scanear Huella"); 
  pinMode(13,OUTPUT); 

pinMode(12,OUTPUT); 
pinMode(9,OUTPUT);
pinMode(11, OUTPUT); 
pinMode(0, OUTPUT);
pinMode(0, OUTPUT);// sd
pinMode(A0, INPUT);
 
  Serial.begin(9600);
  Serial.print("initializing SD card...");   
 pinMode(4, OUTPUT);                    
   
 if (!SD.begin(4)) {                    
   Serial.println("initializing error!");   
   return; 
 } 
 Serial.println("initializing ok.");
  
   // set the data rate for the sensor serial port
  finger.begin(57600);
  
  if (finger.verifyPassword()) {
     lcd.clear();
     lcd.print(" Inicializando"); 
     delay(1000); 
       Serial.println("Found fingerprint sensor!!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1);
  }

 
   
}
void loop()                     // run over and over again
{
  lcd.clear();
     lcd.print(" id="); 
     delay(1000);
  Serial.println("Type in the ID # you want to save this finger as......");
  uint8_t id = 0;
  while (true) {
    while (! Serial.available());
   char c = Serial.read();
   if (! isdigit(c)) break;
   id *= 10;
 id += c - '0';
  }
 lcd.clear();
     lcd.print(id); 
     delay(1000);
  Serial.print("Enrolling ID #");
  Serial.println(id);
  
  while (!  getFingerprintEnroll(id) );
}

uint8_t getFingerprintEnroll(uint8_t id) {
  uint8_t p = -1;
  Serial.println("Waiting for valid finger to enroll");
   lcd.clear();
     lcd.print(" waiting......¡"); 
     delay(1000);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
     lcd.clear();
     lcd.print(" Image taken¡"); 
     delay(1000);
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      lcd.clear();
     lcd.print("Image converted"); 
     delay(1000);
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
   lcd.clear();
     lcd.print(" Remove finger¡"); 
     delay(1000);
  Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }

  p = -1;
   lcd.clear();
     lcd.print(" Place same finger again...¡"); 
     delay(1000);
  Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      lcd.clear();
     lcd.print(" Image converted¡"); 
     delay(1000);
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  
  // OK converted!
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    lcd.clear();
     lcd.print(" Prints matched¡"); 
     delay(1000);
   
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
 
//  Stored! en sd
  myFile = SD.open("sensor.txt", FILE_WRITE);
  
  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to sensor.txt...");
    myFile.println("sensor 1, 2, 3.");
  // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error openingsensor.txt");
  }
  
  // re-open the file for reading:
  myFile = SD.open("sensor.txt");
  if (myFile) {
    Serial.println("sensor.txt:");
    
    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
    /// end sd
     lcd.clear();
       lcd.print("Stored!"); 
     delay(1000);
    
    Serial.println("¡¡ informacion Guardada");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }  
  
  
 
}
