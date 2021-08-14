
#include <Keypad.h>
#include <SPI.h>
#include <SD.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11);

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {22, 23, 24, 25};
byte colPins[COLS] = {26, 27, 28, 29};

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

unsigned char pw[] = "1128";
unsigned char key; String date = ""; int rs = 0;
void csvfiles() {
  if (!SD.exists("btech.csv"))
  {
    File f = SD.open("btech.csv", FILE_WRITE);
    f.println("ROLL-NO,TOTAL-DAYs,PRESENT DAYs,%-AGE");
    f.close();
  }
  if (!SD.exists("mtech.csv"))
  {
    File f = SD.open("mtech.csv", FILE_WRITE);
    f.println("ROLL-NO,TOTAL-DAYs,PRESENT DAYs,%-AGE");
    f.close();
  }
  if (!SD.exists("btotclas.txt"))
  {
    File f = SD.open("btotclas.txt", FILE_WRITE);
    f.println("0");
    f.close();
  }
  if (!SD.exists("mtotclas.txt"))
  {
    File f = SD.open("mtotclas.txt", FILE_WRITE);
    f.println("0");
    f.close();
  }

}
void lock_files() {
  File myFile;

  if (!SD.exists("lockfile/"))
  { SD.mkdir("lockfile");

    myFile = SD.open("lockfile/attend.txt", FILE_WRITE);
    myFile.close();
    myFile = SD.open("lockfile/regd.txt", FILE_WRITE);
    myFile.close();
  }

  if (!SD.exists("id_count.txt")) {
    myFile = SD.open("id_count.txt", FILE_WRITE);
    myFile.print("0");
    myFile.close();
  }

  if (!SD.exists("reg_stu.txt")) {
    myFile = SD.open("reg_stu.txt", FILE_WRITE);
    myFile.close();
  }

}
short pass() {
  lcd.clear();
  lcd.print(F("PIN:"));
  lcd.setCursor(0, 1);
  short i = 0, f = 0;

  while (1)
  {
    key = keypad.getKey();
    if (key) {
      lcd.print(F("*"));
      if (pw[i] == key)
        f++;
      if (i == 3)
        break;
      i++;
    }

  }    if (f == 4)
  {
    lcd.clear();
    return 1;
  }
  else
  {
    lcd.clear();
    return 0;
  }
}

byte clearall() {
  lcd.clear();
  lcd.print(F("FULL ERASE?"));
  lcd.setCursor(0, 1);
  lcd.print(F(" 1-OK | 2-CANCEL"));
  int a = 0;
  while (1) {

    key = keypad.getKey();
    if (key == '2')
    {
      a = 2;
      break;
    }
    else if (key == '1')
    { lcd.clear();
      lcd.print(F("WAIT.."));
      finger.emptyDatabase();

      if (SD.exists("id_count.txt"))
        SD.remove("id_count.txt");

      if (SD.exists("reg_stu.txt"))
        SD.remove("reg_stu.txt");

      if (SD.exists("btech.csv"))
        SD.remove("btech.csv");

      delay(50);

      if (SD.exists("mtech.csv"))
        SD.remove("mtech.csv");

      if (SD.exists("temp1.csv"))
        SD.remove("temp1.csv");

      if (SD.exists("temp2.csv"))
        SD.remove("temp2.csv");

      delay(50);

      if (SD.exists("btotclas.txt"))
        SD.remove("btotclas.txt");

      if (SD.exists("mtotclas.txt"))
        SD.remove("mtotclas.txt");

      if (SD.exists("mdtc.txt"))
        SD.remove("mdtc.txt");

      delay(50);

      if (SD.exists("bdtc.txt"))
        SD.remove("bdtc.txt");

      if (SD.exists("tmproll.txt"))
        SD.remove("tmproll.txt");

      if (SD.exists("temp11.csv"))
        SD.remove("temp11.csv");

      delay(50);

      if (SD.exists("temp22.csv"))
        SD.remove("temp22.csv");

      File f;
      if (!SD.exists("btech.csv")) {
        f = SD.open("btech.csv", FILE_WRITE);
        f.println("ROLL-NO,TOTAL-DAYs,PRESENT DAYs,%-AGE");
        f.close();
      }
      delay(50);
      if (!SD.exists("mtech.csv")) {
        f = SD.open("mtech.csv", FILE_WRITE);
        f.println("ROLL-NO,TOTAL-DAYs,PRESENT DAYs,%-AGE");
        f.close();
      }

      delay(50);

      if (!SD.exists("btotclas.txt")) {
        f = SD.open("btotclas.txt", FILE_WRITE);
        f.println("0");
        f.close();
      }
      delay(50);
      if (!SD.exists("mtotclas.txt")) {
        f = SD.open("mtotclas.txt", FILE_WRITE);
        f.println("0");
        f.close();
      }
      delay(50);
      if (!SD.exists("reg_stu.txt")) {
        f = SD.open("reg_stu.txt", FILE_WRITE);
        f.close();
      }
      a = 1; break;
    }
  }
  if (a == 1) {
    lcd.clear();
    lcd.print(F("Erased.."));
    delay(800);
    return;
  }
  else if (a == 2)
    return;

}

byte regd_student() {

  int id = 0; byte c = 0, g = 0;     File f, f1, f2; String roll = "", rid = "", rollw = "";
  // g is fingerprint stored success flag, c helps in retreiving id from file & id to store id from file.

  f = SD.open("id_count.txt");// To Retrieve ID from id_count.txt
  while (f.available())
  {
    id +=  (int(f.read()) - 48) * pow(10, c);
    c++;
  }
  f.close();

  id++;

  byte i = 0;
  lcd.clear();
  lcd.print(F("Univ. Roll No:"));
  lcd.setCursor(0, 6);

  while (1)
  {
    key = keypad.getKey();
    if (key)
    {
      if (key == '*')
      { if (i != 10)
          continue;
        else
        { int k = 0; String temp_roll, s;

          f1 = SD.open("lockfile/regd.txt");  // open file & read which program to work on i.e btech/mtech
          s = f1.readStringUntil('\n');
          s.remove(2); // remove \n
          f1.close();

          rollw = roll + s; //roll+branch
          f1 = SD.open("reg_stu.txt");
          while (f1.available())
          {
            temp_roll = f1.readStringUntil('\n'); // Read roll+id from file line by line
            temp_roll.remove(12);               // Trim and keep only the roll number part
            if (temp_roll.equalsIgnoreCase(rollw)) // check whether roll numbers are equal
            {
              f1.close();
              k = 10;
              break;
            }

          }
          if (k == 10)
          { lcd.clear();
            lcd.print(F("Roll Exists!"));

            digitalWrite(12, HIGH); delay(1000); digitalWrite(12, LOW);

            i = 0; roll = "";
            lcd.clear();
            lcd.print(F("Univ. Roll No:"));
            lcd.setCursor(0, 6);
          }
          else
          {
            break; // Exit from this keypad entry loop after everything is ok
          }
        }
      }

      if (i < 10 && key != '*')
      { i++;
        lcd.print(char(key));
        roll += char(key);
      }
      if ( key == 'C')
      { i = 0; roll = "";
        lcd.clear();
        lcd.print(F("Univ. Roll No:"));
        lcd.setCursor(0, 6);
      }

    }
  }
  f1.close();

  lcd.clear();
  lcd.print(F("WAIT.."));

  lcd.clear();
  lcd.print(F("Place Finger:"));
  int p = -1;
  while (p != FINGERPRINT_OK) {
    digitalWrite(12, LOW);
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        lcd.clear();
        digitalWrite(13, HIGH);
        break;
      case FINGERPRINT_NOFINGER:
        digitalWrite(12, HIGH);
        break;
      default: lcd.clear();
        lcd.print(F("FP ERROR 1.1")); delay(800);
        break;
    }
  }
  delay(300);
  digitalWrite(13, LOW);
  digitalWrite(12, LOW);


  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      digitalWrite(13, HIGH);
      break;

    default:
      lcd.clear();
      lcd.print(F("FP ERROR 1.2")); delay(800);

  }
  delay(300);
  digitalWrite(13, LOW);
  lcd.clear();
  lcd.print(F("REMOVE FINGER!"));
  delay(800);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  p = -1;
  lcd.clear();
  lcd.print(F("PLACE AGAIN.."));
  while (p != FINGERPRINT_OK) {
    digitalWrite(12, LOW);
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        digitalWrite(13, HIGH);
        break;
      case FINGERPRINT_NOFINGER:
        digitalWrite(12, HIGH);
        break;
      default:
        lcd.clear();
        lcd.print(F("FP ERROR 2.1"));
        break;
    }
  }
  digitalWrite(13, LOW); digitalWrite(12, LOW);
  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      digitalWrite(13, HIGH);
      break;
    default: digitalWrite(12, HIGH);
      lcd.clear();
      lcd.print(F("FP ERROR 2.2")); delay(800);
      break;
  } delay(300);
  digitalWrite(13, LOW); digitalWrite(12, LOW);
  // OK converted!


  p = finger.createModel();
  if (p == FINGERPRINT_OK) {

    p = finger.storeModel(id);
    if (p == FINGERPRINT_OK) {
      g = 1; // Set Flag Success for furher use in program, as fingerprint has been stored succesfully
    } else {
      lcd.clear(); lcd.println(F("FP Store Error!")); delay(1000);
    }

  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    lcd.clear();
    lcd.print(F("MISMATCHED"));
    delay(1000);
  } else {
    lcd.clear();
    lcd.print(F("FP Match Error")); delay(1000);

  }

  if (g == 1) // After fingerprint store Success
  {
    f1 = SD.open("reg_stu.txt", FILE_WRITE); //this part is to write everything in reg_stu.txt from ref.txt
    rid = rollw + id; // update rollno + id = RID
    f1.println(rid);
    f1.close();




    File j, k; String s, kl, fname, chkname; int jk = 0;

    j = SD.open("lockfile/regd.txt"); // open file & read which program to work on i.e btech/mtech
    s = j.readStringUntil('\n');
    s.remove(2);
    j.close();

    if (s.equals("lb"))       // here there are two String objects i.e. fname -> temp1 & for chkname->temp2
    { fname = "btech.csv";
      chkname = "temp1.csv"; jk = 1;
    }
    else
    { fname = "mtech.csv";
      chkname = "temp2.csv"; jk = 2;
    }

    if (rs == 0) {                        // rs = (0/1) is the flag variable which checks whether its first registration
      k = SD.open(chkname, FILE_WRITE);
      kl = roll + ",1,1,100%,P";
      k.println(kl);
      k.close();

      SD.remove(fname);
      j = SD.open(fname, FILE_WRITE);
      k = SD.open(chkname);

      while (k.available())
      {
        kl = k.readStringUntil('\n');
        kl.remove(kl.length() - 1);
        j.println(kl);
      }
      j.close();
      k.close();

      SD.remove("id_count.txt");
      f = SD.open("id_count.txt", FILE_WRITE);// To save updated ID in id_count.txt
      f.print(id);
      f.close();

    }
    else
    {
      k = SD.open(fname);
      kl = k.readStringUntil('\n');
      k.close();

      int comma = 0;
      for (int i = 0; i < kl.length(); i++)
      { if (kl[i] == ',')
          comma++;
      }
      comma -= 4;

      String fstr, tc; int rt; float er;

      if (jk == 1) {
        k = SD.open("btotclas.txt");
        tc = k.readStringUntil('\n');
        tc.remove(tc.length() - 1);
        k.close();
        rt = tc.toInt();
      }
      else if (jk == 2) {
        k = SD.open("mtotclas.txt");
        tc = k.readStringUntil('\n');
        tc.remove(tc.length() - 1);
        k.close();
        rt = tc.toInt();
      }

      er =  rt;

      float percent = (0 / er) * 100;

      fstr = roll + ',' + tc + ',' + "0" + ',' + int(percent) + '%';

      for (int i = 0; i <= comma; i++)
      {
        fstr += ",A";
      }

      k = SD.open(chkname, FILE_WRITE);
      k.println(fstr);
      k.close();

      SD.remove(fname);
      j = SD.open(fname, FILE_WRITE);
      k = SD.open(chkname);

      while (k.available())
      {
        kl = k.readStringUntil('\n');
        kl.remove(kl.length() - 1);
        j.println(kl);
      }
      j.close();
      k.close();

      SD.remove("id_count.txt");
      f = SD.open("id_count.txt", FILE_WRITE);// To save updated ID in id_count.txt
      f.print(id);
      f.close();

    }

    lcd.clear();
    lcd.print(F("STORED!")); // storing success
    digitalWrite(13, HIGH);  // show green light to user
    delay(700);
  }
  else
  { lcd.clear();
    lcd.print(F("TRY AGAIN :("));
    digitalWrite(12, HIGH); delay(300); digitalWrite(12, LOW); delay(300);
    digitalWrite(12, HIGH); delay(300); digitalWrite(12, LOW); delay(300);
    digitalWrite(12, HIGH); delay(300); digitalWrite(12, LOW); delay(300);
    digitalWrite(12, HIGH); delay(300); digitalWrite(12, LOW); delay(300);
  }

  digitalWrite(13, LOW);
  digitalWrite(12, LOW);
  f2.close();
  f1.close();


}          //end of regd_student() function

void regd() {
  if (!pass())
  { lcd.clear(); lcd.print("ACCESS DENIED");
    digitalWrite(12, HIGH); delay(300); digitalWrite(12, LOW); delay(300);
    digitalWrite(12, HIGH); delay(300); digitalWrite(12, LOW); delay(300);
    digitalWrite(12, HIGH); delay(300); digitalWrite(12, LOW); delay(300);
    return 1;
  }

  lcd.clear();   // select btech/mtech
  lcd.print(F("1-B.TECH |#-BACK "));
  lcd.setCursor(0, 1);
  lcd.print(F("2-M.TECH |"));
  while (1)
  { key = keypad.getKey();

    if ( key == '1')
    {
      SD.remove("lockfile/regd.txt");
      File f;
      f = SD.open("lockfile/regd.txt", FILE_WRITE);
      f.print("lb");
      f.close(); break;
    }
    else if (key == '2')
    { SD.remove("lockfile/regd.txt");
      File f;
      f = SD.open("lockfile/regd.txt", FILE_WRITE);
      f.print("lm");
      f.close(); break;
    }
    if (key == '#')
      return;
  }
  lcd.clear();
  lcd.print(F("WAIT.."));
  File j, k, f; String s, h, fname, chkname; int op = 0;

  j = SD.open("lockfile/regd.txt");  // open file & read which program to work on i.e btech/mtech
  s = j.readStringUntil('\n');
  j.close();
  if (s.equals("lb"))
  { fname = "btech.csv";
    chkname = "temp1.csv";
  }
  else
  { fname = "mtech.csv";
    chkname = "temp2.csv";
  }

  if (rs == 0) {
    s = "";

    k = SD.open(fname);
    while (k.available())          // if the temp file of the respective btech/mtech file contains more than one row then file gets locked
    { h = k.readStringUntil('\n');
      op++;
    }
    if (op > 1)
    { lcd.clear(); lcd.print(F("LOCKED!"));
      digitalWrite(12, HIGH); delay(300); digitalWrite(12, LOW); delay(300);
      digitalWrite(12, HIGH); delay(300); digitalWrite(12, LOW); delay(300);
      digitalWrite(12, HIGH); delay(300); digitalWrite(12, LOW); delay(300);
      return;
    }
    j.close();

    // change first line of excel sheet

    j = SD.open(fname);
    k = SD.open(chkname, FILE_WRITE);

    s = j.readStringUntil('\n');
    s.remove(s.length() - 1);
    s = s + ',' + date;
    k.println(s);
    j.close();
    k.close();

    //below code to update totclass of btech/mtech respectively for first registration


    j = SD.open("lockfile/regd.txt"); // open file & read which program to work on i.e btech/mtech
    s = j.readStringUntil('\n');
    s.remove(2);
    j.close();

    String tc, dt; int rt;     // this is to update the respective total class in the file


    if (s.equals("lb"))
    { k = SD.open("btotclas.txt");
      tc = k.readStringUntil('\n');
      tc.remove(tc.length() - 1);
      k.close();
      rt = tc.toInt();
      rt++;
      SD.remove("btotclas.txt");
      k = SD.open("btotclas.txt", FILE_WRITE);
      k.println(rt);
      k.close();
      delay(50);
      f = SD.open("bdtc.txt", FILE_WRITE);
      f.println(date);
      f.close();

    }
    else
    {
      k = SD.open("mtotclas.txt");
      tc = k.readStringUntil('\n');
      tc.remove(tc.length() - 1);
      k.close();
      rt = tc.toInt();
      rt++;
      SD.remove("mtotclas.txt");
      k = SD.open("mtotclas.txt", FILE_WRITE);
      k.println(rt);
      k.close();
      delay(50);
      f = SD.open("mdtc.txt", FILE_WRITE);
      f.println(date);
      f.close();
    }

  }
  else {  // if rs flag is turned on then check the criteria below before handing control to regd_studnt functio above.

    k = SD.open(fname);
    while (k.available())          // if the temp file of the respective btech/mtech file contains more than one row then file gets locked
    { k.readStringUntil('\n');
      op++;
    }
    if (op <= 1)
    {
      lcd.clear(); lcd.print("REGD NOT DONE!");
      digitalWrite(12, HIGH); delay(300); digitalWrite(12, LOW); delay(300);
      digitalWrite(12, HIGH); delay(300); digitalWrite(12, LOW); delay(300);
      digitalWrite(12, HIGH); delay(300); digitalWrite(12, LOW); delay(300);
      rs = 0; return;

    }
  }



  lcd.clear();
  lcd.print(F("* - CONTINUE"));
  lcd.setCursor(0, 1);
  lcd.print(F("# - BACK  (ReGD)88"));

  while (1)
  { key = keypad.getKey();

    if ( key == '*')
    { regd_student(); lcd.clear();
      lcd.print(F("* - CONTINUE"));
      lcd.setCursor(0, 1);
      lcd.print(F("# - BACK  (ReGD)"));
    }
    else if (key == '#')
    { if (pass())
      { rs = 0;  // SET LATE REG FLAG DOWN
        break;
      }
      else {
        lcd.clear();
        lcd.print(F("* - CONTINUE"));
        lcd.setCursor(0, 1);
        lcd.print(F("# - BACK  (ReGD)"));
        continue;
      }
    }
  }
}
void admin() {

  if (!pass())
  { lcd.clear(); lcd.print("ACCESS DENIED");
    digitalWrite(12, HIGH); delay(300); digitalWrite(12, LOW); delay(300);
    digitalWrite(12, HIGH); delay(300); digitalWrite(12, LOW); delay(300);
    digitalWrite(12, HIGH); delay(300); digitalWrite(12, LOW); delay(300);
    return ;
  }
  lcd.clear();
  lcd.print(F("1-REG 2-ERASE"));
  lcd.setCursor(0, 1);
  lcd.print(F("3-LREG #-BACK"));
  while (1) {
    key = keypad.getKey();
    if (key == '1')
    { regd(); lcd.clear();
      lcd.print(F("1-REG 2-ERASE"));
      lcd.setCursor(0, 1);
      lcd.print(F("3-LREG #-BACK"));
    }
    else if (key == '2')
    { clearall(); lcd.clear();
      lcd.print(F("1-REG 2-ERASE"));
      lcd.setCursor(0, 1);
      lcd.print(F("3-LREG #-BACK"));
    }
    else if (key == '3')
    { rs = 1; // set late registration flag ON
      regd();
      lcd.clear();
      lcd.print(F("1-REG 2-ERASE"));
      lcd.setCursor(0, 1);
      lcd.print(F("3-LREG #-BACK"));
    }
    else if (key == '#')
    {
      break;
    }

  } return;
}

int getFingerprintID() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)
  { if (p == FINGERPRINT_NOTFOUND)
    { Serial.println("No Match Found");
      return -2;
    }
    else
      return -1;
  }

  return finger.fingerID;
}
void atten() {


  // Read finger Print from figer print Sensor using a loop which terminates in 10seconds

  int i; String getid, temp_roll; File f1, f2, j, k;

  for (i = 0; i < 50; i++) {
    getid = getFingerprintID();
    if ( getid != "-1")
    { if (getid == "-2")
      {
        lcd.clear();
        lcd.print(F("No Match Found!"));
        digitalWrite(12, HIGH); delay(100); digitalWrite(12, LOW); delay(200);
        digitalWrite(12, HIGH); delay(100); digitalWrite(12, LOW); delay(400);
        digitalWrite(12, HIGH); delay(800); digitalWrite(12, LOW); delay(600);
      }
      break;
    }
    delay(50);
  }
  lcd.clear();
  lcd.print(F("WAIT.."));;
  if (i >= 50) {
    lcd.clear();
    digitalWrite(12, HIGH);
    lcd.print(F("TIMED OUT!")); delay(500);
    lcd.clear(); delay(300);
    lcd.print(F("TIMED OUT!")); delay(500);
    lcd.clear(); delay(300);
    lcd.print(F("TIMED OUT!")); delay(900);
    digitalWrite(12, LOW); delay(100);
  }
  else if (getid != "-1" && getid != "-2") {

    // Main attendance program goes here...

    File f; String s, dt, fname, chkname;
    f = SD.open("lockfile/attend.txt");
    s = f.readStringUntil('\n');
    f.close();

    if (s.equals("lb"))
    {
      fname = "btech.csv";
      chkname = "temp11.csv";
    }
    else
    {
      fname = "mtech.csv";
      chkname = "temp22.csv";
    }
    f.close();

    String roll1; int y = 0;

    f1 = SD.open("reg_stu.txt");
    while (f1.available())
    {
      temp_roll = f1.readStringUntil('\n'); // Read roll+id from reg-stu file line by line
      temp_roll.remove(temp_roll.length() - 1);

      if (temp_roll.substring(12) == getid && temp_roll.substring(10, 12) == s )
      { y = 1;
        break;
      }
    }
    if (y == 0)        // Check if the roll no. & its branch is matching
    { lcd.clear();
      lcd.print("Not Found!");
      digitalWrite(12, HIGH); delay(2000); digitalWrite(12, LOW); delay(300);
      return;
    }
    else {
      y = 0;

      if (!SD.exists("tmproll.txt"))
      {
        f1 = SD.open("tmproll.txt", FILE_WRITE);
        f1.println(temp_roll.substring(0, 12));
        f1.close();
      }
      else
      {
        f1 = SD.open("tmproll.txt");
        while (f1.available()) {
          roll1 = f1.readStringUntil('\n');
          roll1.remove(roll1.length() - 1);

          if (roll1.equals(temp_roll.substring(0, 12)))
          { y = 1; break;
          }
        }
        f1.close();
        if (y == 1) {
          lcd.clear();
          lcd.print("ALREADY DONE!");
          digitalWrite(12, HIGH); delay(2000); digitalWrite(12, LOW); delay(300);
          return;
        }
        else {
          f1 = SD.open("tmproll.txt", FILE_WRITE);
          f1.println(temp_roll.substring(0, 12));
          f1.close();
        }
      }
    }


    // retrieve data from main csv file
    int comma = 0, rt, index[5]; String rolls, disp_prcnt, presents, percents, rest, tc, kl; float tot, pr, er;

    if (s.equals("lb"))
    { k = SD.open("btotclas.txt");
      tc = k.readStringUntil('\n');
      tc.remove(tc.length() - 1);
      k.close();
      er = tc.toInt();
    }
    else
    {
      k = SD.open("mtotclas.txt");
      tc = k.readStringUntil('\n');
      tc.remove(tc.length() - 1);
      k.close();
      er = tc.toInt();
    }

    f1 = SD.open(fname);
    kl = f1.readStringUntil('\n');

    while (f1.available())
    {
      kl = f1.readStringUntil('\n');

      index[0] = kl.indexOf(',');
      index[1] = kl.indexOf(',', index[0] + 1);
      index[2] = kl.indexOf(',', index[1] + 1);
      index[3] = kl.indexOf(',', index[2] + 1);

      rolls = kl.substring(0, index[0]);
      presents = kl.substring(index[1] + 1, index[2]);


      if (rolls.equals(temp_roll.substring(0, 10)))
      { pr = presents.toInt() + 1;
        tot = (pr / er) * 100;
        rt = tot;
        disp_prcnt = rt;
        disp_prcnt += '%';
        break;
      }

    }
    f1.close();

    //Display Roll & Percentage to the User
    lcd.clear();
    lcd.print(temp_roll.substring(0, 10));
    lcd.print(F("  "));
    lcd.print(disp_prcnt);
    lcd.setCursor(0, 1);
    lcd.print(F("  THANK - U..  "));
    digitalWrite(13, HIGH); delay(400); digitalWrite(13, LOW); delay(300);
    digitalWrite(13, HIGH); delay(400); digitalWrite(13, LOW); delay(300);
    digitalWrite(13, HIGH); delay(400); digitalWrite(13, LOW);



    while (1)
    {
      key = keypad.getKey();
      if (key == '*')
        break;
    }
  }
}

void takattend() {
  File f; String s, dt, fname;


  lcd.clear();
  lcd.print(F("* - CONTINUE"));
  lcd.setCursor(0, 1);
  lcd.print(F("# - SAVE & BACK"));

  while (1)
  { key = keypad.getKey();

    if ( key == '*')
    {
      atten();
      lcd.clear();
      lcd.print(F("* - CONTINUE"));
      lcd.setCursor(0, 1);
      lcd.print(F("# - SAVE & BACK"));

    }
    else if (key == '#')
    { if (pass())
      { lcd.clear();
        lcd.print(F("PLEASE WAIT!"));
        lcd.setCursor(0, 1);
        lcd.print(F("SAVING.."));

        // SAVE THE WHOLE DATA

        File f, f1, f2, j, k; String s, dt, fname, ffname, chkname;


        f = SD.open("lockfile/attend.txt");
        s = f.readStringUntil('\n');
        f.close();

        if (s.equals("lb"))
        {
          fname = "btech.csv";
          ffname = "temp1.csv";
          chkname = "temp11.csv";
        }
        else
        {
          fname = "mtech.csv";
          ffname = "temp2.csv";
          chkname = "temp22.csv";
        }
        f.close();



        int flag = 0, rt, index[5], count = 0;
        String rol[90], roll, rolls, totals, presents, percents, rest, tc, kl, finall;
        float tot, pr, er;

        if (s.equals("lb"))
        { k = SD.open("btotclas.txt");
          tc = k.readStringUntil('\n');
          tc.remove(tc.length() - 1);
          k.close();
          er = tc.toInt();
        }
        else
        {
          k = SD.open("mtotclas.txt");
          tc = k.readStringUntil('\n');
          tc.remove(tc.length() - 1);
          k.close();
          er = tc.toInt();
        }

        j = SD.open("tmproll.txt");   //open roll recoder
        while (j.available()) {
          roll = j.readStringUntil('\n');
          roll.remove(roll.length() - 3);
          rol[count] = roll;
          count++;

        }
        j.close();

        delay(50);

        f1 = SD.open(fname);
        f2 = SD.open(chkname, FILE_WRITE);
        kl = f1.readStringUntil('\n');
        while (f1.available())
        {
          kl = f1.readStringUntil('\n');
          index[0] = kl.indexOf(',');
          index[1] = kl.indexOf(',', index[0] + 1);
          index[2] = kl.indexOf(',', index[1] + 1);
          index[3] = kl.indexOf(',', index[2] + 1);

          rolls = kl.substring(0, index[0]);
          totals = kl.substring(index[0] + 1, index[1]);
          presents = kl.substring(index[1] + 1, index[2]);
          percents = kl.substring(index[2] + 1, index[3]);
          rest = kl.substring(index[3]);
          rest.remove(rest.length() - 1);
          flag = 0;

          for (int i = 0; i <= count ; i++)
          {
            Serial.println(rol[i]);
            if (rolls.equals(rol[i]))
            {
              flag = 1;
              break;
            }
          }
          if (flag == 1) {
            totals = (totals.toInt() + 1);
            presents = (presents.toInt() + 1);

            pr = presents.toInt();
            tot = (pr / er) * 100;
            rt = tot;
            percents = rt;
            percents += '%';
            finall = rolls + ',' + totals + ',' + presents + ',' + percents  + rest + ",P";

          }
          else {
            totals = (totals.toInt() + 1);
            pr = presents.toInt();

            tot = (pr / er) * 100;
            rt = tot;
            percents = rt;
            percents += '%';
            finall = rolls + ',' + totals + ',' + presents + ',' + percents  + rest + ",A";

          }
          f2.println(finall);
          delay(5);
        }
        f1.close();
        f2.close();
        j.close();
        //now remove and write everything from temp file to main file


        SD.remove(fname);
        SD.remove(ffname);

        j = SD.open(fname, FILE_WRITE);
        k = SD.open(chkname);

        while (k.available())
        {
          kl = k.readStringUntil('\n');
          kl.remove(kl.length() - 1);
          j.println(kl);
        }

        j.close();
        k.close();
        delay(100);

        j = SD.open(ffname, FILE_WRITE);
        k = SD.open(chkname);

        while (k.available())
        {
          kl = k.readStringUntil('\n');
          kl.remove(kl.length() - 1);
          j.println(kl);
        }

        j.close();
        k.close();


        delay(100);


        SD.remove("att_lo.txt");  // Turn the attendance lock off
        f = SD.open("att_lo.txt", FILE_WRITE);
        f.print("0");
        f.close();

        SD.remove("tmproll.txt"); // delete  temp roll recorder
        break;
      }
      lcd.clear();
      lcd.print(F("* - CONTINUE"));
      lcd.setCursor(0, 1);
      lcd.print(F("# - SAVE & BACK"));
      continue;
    }
  }
}
void clas() {
  if (!pass())
  { lcd.clear(); lcd.print("ACCESS DENIED");
    digitalWrite(12, HIGH); delay(300); digitalWrite(12, LOW); delay(300);
    digitalWrite(12, HIGH); delay(300); digitalWrite(12, LOW); delay(300);
    digitalWrite(12, HIGH); delay(300); digitalWrite(12, LOW); delay(300);
    return ;
  }
  File myFile;
  myFile = SD.open("reg_stu.txt", FILE_WRITE);
  if (myFile.size() < 14)
  { lcd.clear(); lcd.print("NO RECORD FOUND");
    digitalWrite(12, HIGH); delay(300); digitalWrite(12, LOW); delay(300);
    digitalWrite(12, HIGH); delay(300); digitalWrite(12, LOW); delay(300);
    digitalWrite(12, HIGH); delay(300); digitalWrite(12, LOW); delay(600);
    return ;
    myFile.close();
  }
  myFile.close();
  lcd.clear();
  lcd.print(F("WAIT.."));


  File f, j, k; String s, h, fname, chkname; int op = 0;


  // Retrieve date from date file & update the csv column of the respective branch
  String dt, dt2; int dtchk = 0;
  f = SD.open("date.txt");
  dt = f.readStringUntil('\n');
  f.close();

  lcd.clear();
  lcd.print(F("1-B.TECH |#-BACK "));
  lcd.setCursor(0, 1);
  lcd.print(F("2-M.TECH |"));


  String tc; int rt;      // this is to update the respective total class in the file

  while (1)
  { key = keypad.getKey();

    if ( key == '1')
    { lcd.clear();
      lcd.print(F("WAIT.."));

      File myFile; String  fg;int t=0;
      myFile = SD.open("reg_stu.txt");
      while(myFile.available()){

        fg = myFile.readStringUntil('\n');
        fg.remove(fg.length()-1);

        if(fg.substring(10,12) == "lb")
        { t = 1; break; }
      }
      myFile.close();
      if(t==0)
      { lcd.clear(); lcd.print("BTech Empty!");
    digitalWrite(12, HIGH); delay(300); digitalWrite(12, LOW); delay(300);
    digitalWrite(12, HIGH); delay(300); digitalWrite(12, LOW); delay(300);
    digitalWrite(12, HIGH); delay(300); digitalWrite(12, LOW); delay(600);
    return ;}


      if (!SD.exists("bdtc.txt"))      // create new btech attendance recorder file if not present and add new record to it
      {
        f = SD.open("bdtc.txt", FILE_WRITE);
        f.println(dt);
        f.close();
      }
      else {
        f = SD.open("bdtc.txt");          // check in btech attendance recorder if attendance is already taken on the entered date
        while (f.available()) {
          dt2 = f.readStringUntil('\n');
          dt2.remove(dt2.length() - 1);
          if (dt.equals(dt2))
          {
            dtchk = 1;
            break;
          }

        }
        if (dtchk == 1)
        {
          lcd.clear();
          lcd.print(F("ATTEND TAKEN.."));
          lcd.setCursor(0, 1);
          lcd.print(F("ON THIS DATE!."));
          digitalWrite(12, HIGH); delay(3000); digitalWrite(12, LOW);
          return;
        }
        else {
          f = SD.open("bdtc.txt", FILE_WRITE);
          f.println(dt);
          f.close();
        }

      }

      k = SD.open("btotclas.txt");                 // Update the btech total class
      tc = k.readStringUntil('\n');
      tc.remove(tc.length() - 1);
      k.close();
      rt = tc.toInt();
      rt++;
      SD.remove("btotclas.txt");
      k = SD.open("btotclas.txt", FILE_WRITE);
      k.println(rt);
      k.close();

      SD.remove("att_lo.txt");                       // Turn the attendance Lock ON
      f = SD.open("att_lo.txt", FILE_WRITE);
      f.print("1");
      f.close();
      SD.remove("lockfile/attend.txt");              // Set the select-lock file to btech mode
      f = SD.open("lockfile/attend.txt", FILE_WRITE);
      f.print("lb");
      f.close();

      break;
    }

    else if (key == '2')
    { lcd.clear();
      lcd.print(F("WAIT.."));

      File myFile; String  fg;int t=0;
      myFile = SD.open("reg_stu.txt");
      while(myFile.available()){

        fg = myFile.readStringUntil('\n');
        fg.remove(fg.length()-1);

        if(fg.substring(10,12) == "lm")
        { t = 1; break; }
      }
      myFile.close();
      if(t==0)
      { lcd.clear(); lcd.print("MTech Empty!");
    digitalWrite(12, HIGH); delay(300); digitalWrite(12, LOW); delay(300);
    digitalWrite(12, HIGH); delay(300); digitalWrite(12, LOW); delay(300);
    digitalWrite(12, HIGH); delay(300); digitalWrite(12, LOW); delay(600);
    return ;}
      

      if (!SD.exists("mdtc.txt"))    // create new mtech attendance recorder file if not present and add new record to it
      {
        f = SD.open("mdtc.txt", FILE_WRITE);
        f.println(dt);
        f.close();
      }
      else {
        f = SD.open("mdtc.txt");        // check in mtech attendance recorder if attendance is already taken on the entered date
        while (f.available()) {
          dt2 = f.readStringUntil('\n');
          dt2.remove(dt2.length() - 1);
          if (dt.equals(dt2))
          {
            dtchk = 1;
            break;
          }

        }
        if (dtchk == 1)
        {
          lcd.clear();
          lcd.print(F("ATTEND TAKEN.."));
          lcd.setCursor(0, 1);
          lcd.print(F("ON THIS DATE!."));
          digitalWrite(12, HIGH); delay(3000); digitalWrite(12, LOW);
          return;
        }
        else {
          f = SD.open("mdtc.txt", FILE_WRITE);
          f.println(dt);
          f.close();
        }

      }

      k = SD.open("mtotclas.txt");           // Update the mtech total class
      tc = k.readStringUntil('\n');
      tc.remove(tc.length() - 1);
      k.close();
      rt = tc.toInt();
      rt++;
      SD.remove("mtotclas.txt");
      k = SD.open("mtotclas.txt", FILE_WRITE);
      k.println(rt);
      k.close();

      SD.remove("att_lo.txt");               // Turn attendance lock ON
      f = SD.open("att_lo.txt", FILE_WRITE);
      f.print("1");
      f.close();
      SD.remove("lockfile/attend.txt");      // Turn select-Lock File to Mtech Mode
      f = SD.open("lockfile/attend.txt", FILE_WRITE);
      f.print("lm");
      f.close();
      break;
    }
    if (key == '#') {
      lcd.clear();
      lcd.print(F("WAIT.."));
      SD.remove("att_lo.txt");             // Turn Attendance Lock OFF
      f = SD.open("att_lo.txt", FILE_WRITE);
      f.print("0");
      f.close();
      return;
    }
  }

  j = SD.open("lockfile/attend.txt");  // open file & read which program to work on i.e btech/mtech
  s = j.readStringUntil('\n');
  j.close();
  if (s.equals("lb"))
  { fname = "btech.csv";
    chkname = "temp11.csv";
  }
  else
  { fname = "mtech.csv";
    chkname = "temp22.csv";
  }

  j = SD.open(fname);
  SD.remove(chkname);            // Remove the old temp11/temp22.csv file and open a new one and write freshly the new data.
  k = SD.open(chkname, FILE_WRITE);

  s = j.readStringUntil('\n');
  s.remove(s.length() - 1);
  s = s + ',' + date;            // To update the temp11/temp22 first column with new date
  k.println(s);
  j.close();
  k.close();

  // Call the middle function of attendance mode
  takattend();
}

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  if (!SD.begin(53)) {
    lcd.print(F("SD Card Error!"));
    while (1) {}
  }

  finger.begin(57600);

  if (!finger.verifyPassword()) {
    lcd.print(F("Fingerprint Error!"));
    while (1) { }
  }



  File f; String s, dt;

  if (!SD.exists("att_lo.txt"))
  {
    f = SD.open("att_lo.txt", FILE_WRITE);
    f.print("0");
    f.close();
  }

  f = SD.open("att_lo.txt");
  s = f.readStringUntil('\n');
  s.remove(1);
  f.close();
  if (s.equals("0"))
  { lock_files();
    csvfiles();
    lcd.print(F("    PORTABLE"));
    lcd.setCursor(1, 5);
    lcd.print(F("   REGISTER"));
    delay(1000);
    lcd.clear();
    lcd.print(F("TODAY'S DATE:"));
    lcd.setCursor(2, 1);
    lcd.print(F("-"));
    lcd.setCursor(5, 1);
    lcd.print(F("-"));
    lcd.setCursor(0, 1);

    String str;
    int i = 0, s = 0;
    File f1;
    while (1)
    {
      key = keypad.getKey();
      if (key)
      {
        if (key == '*')
        { if (i != 8)
            continue;
          else
          { SD.remove("date.txt");
            f1 = SD.open("date.txt", FILE_WRITE);
            f1.print(date);
            f1.close();
            break;
          }
        }

        if (i < 8 && key != '*')
        {
          if (s == 2 || s == 5)
          {
            lcd.setCursor(++s, 1);  //to skip in date intervals
            date += '-';
          }
          lcd.print(char(key));
          date += char(key);

          i++; s++;

        }
        if ( key == 'C')
        { i = 0; s = 0; date = "";
          lcd.clear();
          lcd.print(F("TODAY'S DATE:"));

          lcd.setCursor(2, 1);
          lcd.print(F("-"));
          lcd.setCursor(5, 1);
          lcd.print(F("-"));
          lcd.setCursor(0, 1);
        }

      }
    }
  }

  else
  { f.close();
    f = SD.open("lockfile/attend.txt");
    s = f.readStringUntil('\n');
    s.remove(2);
    f.close();

    f = SD.open("date.txt");
    dt = f.readStringUntil('\n');
    dt.remove(dt.length());
    f.close();

    if (s.equals("lb"))
    {
      lcd.print(dt);
      lcd.print(F(" BTech"));
      lcd.setCursor(5, 1);
      lcd.print(F("Cont..."));
    }
    else
    {
      lcd.print(dt);
      lcd.print(F(" MTech"));
      lcd.setCursor(5, 1);
      lcd.print(F("Cont..."));
    }
    delay(1000);
    takattend();
  }
}
void loop() {
  lcd.clear(); lcd.print(F("MODE:")); lcd.print(F(" 1 - ADMIN")); lcd.setCursor(5, 1); lcd.print(F(" 2 - CLASS"));

  while (1)
  { key = keypad.getKey();
    if (key == '1' || key == '2')
      break;
  }
  if (key == '1')
    admin();
  if (key == '2')
    clas();
}
