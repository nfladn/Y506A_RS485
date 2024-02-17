#include "Y506A.h"
#include <math.h>
#include <stdio.h>
#include "HardwareSerial.h"

typedef union {
  float f;
  uint8_t b[sizeof(float)];
} cracked_float_t;

cracked_float_t sendf;
cracked_float_t receivef;

Y506A::Y506A(byte modbusPinA, byte modbusPinB) {
  init();
}

void Y506A::init() {
  Serial2.begin(9600, SERIAL_8N1);

  int len_started = sizeof(started) / sizeof(started[0]);
  for (int i = 0; i < len_started ; i++) {
    Serial2.write(started[i]);
  }
  Serial2.setTimeout(32);
  len_started  = 0;

  return;
}

void Y506A::initCall() {

  int len_infocall = sizeof(infocall) / sizeof(infocall[0]);
  for (int i = 0; i < len_infocall; i++) {
    Serial2.write(infocall[i]);
  }

  int cnt_infocall = 0;
  while (Serial2.available()) {
    ByteArray[cnt_infocall] = Serial2.read();
    cnt_infocall++;
  }

  int crc_infocall = sizeof(ByteArray) / sizeof(ByteArray[0]);

  // 0-9 byte checker HEKval
  HEKval = "";
  for (int i = 6; i >= 3; i--) {
    if (ByteArray[i] < 16) {
      HEKval += ("0" + String(ByteArray[i], HEX));
    }
    else {
      HEKval += String(ByteArray[i], HEX);
    }
  }

  int HEKval_len = HEKval.length() + 1;
  char chkval[HEKval_len];
  HEKval.toCharArray(chkval, HEKval_len);
  long lkval = strtol(chkval, NULL, 16);
  fkval = *((float*)&lkval);

  // 0-9 byte checker HEBval
  HEBval = "";
  for (int i = 10; i >= 7; i--) {
    if (ByteArray[i] < 16) {
      HEBval += ("0" + String(ByteArray[i], HEX));
    }
    else {
      HEBval += String(ByteArray[i], HEX);
    }
  }

  int HEBval_len = HEBval.length() + 1;
  char chbval[HEBval_len];
  HEBval.toCharArray(chbval, HEBval_len);
  long lbval = strtol(chbval, NULL, 16);
  fbval = *((float*)&lbval);

  len_infocall = 0;
  cnt_infocall = 0;
  crc_infocall = 0;

}

float Y506A::infoKval() {
  float Y506getKval;
  Y506getKval = fkval;
  return Y506getKval;
}

float Y506A::infoBval() {
  float Y506getBval;
  Y506getBval = fbval;
  return Y506getBval;
}

float Y506A::getKval() {
  int len_infocall = sizeof(infocall) / sizeof(infocall[0]);
  for (int i = 0; i < len_infocall; i++) {
    Serial2.write(infocall[i]);
  }

  int cnt_infocall = 0;
  while (Serial2.available()) {
    ByteArray[cnt_infocall] = Serial2.read();
    cnt_infocall++;
  }

  int crc_infocall = sizeof(ByteArray) / sizeof(ByteArray[0]);

  // 0-9 byte checker HEKval
  HEKval = "";
  for (int i = 6; i >= 3; i--) {
    if (ByteArray[i] < 16) {
      HEKval += ("0" + String(ByteArray[i], HEX));
    }
    else {
      HEKval += String(ByteArray[i], HEX);
    }
  }

  int HEKval_len = HEKval.length() + 1;
  char chkval[HEKval_len];
  HEKval.toCharArray(chkval, HEKval_len);
  long lkval = strtol(chkval, NULL, 16);
  fkval = *((float*)&lkval);


  len_infocall = 0;
  cnt_infocall = 0;
  crc_infocall = 0;

  return fkval;

}

float Y506A::getBval() {
  int len_infocall = sizeof(infocall) / sizeof(infocall[0]);
  for (int i = 0; i < len_infocall; i++) {
    Serial2.write(infocall[i]);
  }

  int cnt_infocall = 0;
  while (Serial2.available()) {
    ByteArray[cnt_infocall] = Serial2.read();
    cnt_infocall++;
  }

  int crc_infocall = sizeof(ByteArray) / sizeof(ByteArray[0]);

  // 0-9 byte checker HEBval
  HEBval = "";
  for (int i = 10; i >= 7; i--) {
    if (ByteArray[i] < 16) {
      HEBval += ("0" + String(ByteArray[i], HEX));
    }
    else {
      HEBval += String(ByteArray[i], HEX);
    }
  }

  int HEBval_len = HEBval.length() + 1;
  char chbval[HEBval_len];
  HEBval.toCharArray(chbval, HEBval_len);
  long lbval = strtol(chbval, NULL, 16);
  fbval = *((float*)&lbval);

  len_infocall = 0;
  cnt_infocall = 0;
  crc_infocall = 0;

  return fbval;

}

String Y506A::fToLEs(float ffToLEs) {
  String sfToLEs;

  sendf.f = ffToLEs;

  //for (byte i = 0; i < sizeof(float); i++) {
  for (byte i = 0; i < 4; i++) {
    receivef.b[i] = sendf.b[i];
    if (sendf.b[i] == 0) {
      sfToLEs +=  "00";
    } else {
      sfToLEs +=  String(sendf.b[i], HEX);
    }

    //    Serial.print(F("Byte "));
    //    Serial.print(i);
    //    Serial.print(F(": "));
    //    Serial.println(sendf.b[i], HEX);
  }

  return sfToLEs;

}

byte Y506A::nibble(char c)
{
  if (c >= '0' && c <= '9')
    return c - '0';

  if (c >= 'a' && c <= 'f')
    return c - 'a' + 10;

  if (c >= 'A' && c <= 'F')
    return c - 'A' + 10;

  return 0;  // Not a valid hexadecimal character
}

void Y506A::hexCharacterStringToBytes(byte *byteArray, String datString)
{
  char hexString[10];

  datString.toCharArray(hexString, datString.length() + 1);

  bool oddLength = strlen(hexString) & 1;

  byte currentByte = 0;
  byte byteIndex = 0;

  for (byte charIndex = 0; charIndex < strlen(hexString); charIndex++)
  {
    bool oddCharIndex = charIndex & 1;

    if (oddLength)
    {
      // If the length is odd
      if (oddCharIndex)
      {
        // odd characters go in high nibble
        currentByte = nibble(hexString[charIndex]) << 4;
      }
      else
      {
        // Even characters go into low nibble
        currentByte |= nibble(hexString[charIndex]);
        byteArray[byteIndex++] = currentByte;
        currentByte = 0;
      }
    }
    else
    {
      // If the length is even
      if (!oddCharIndex)
      {
        // Odd characters go into the high nibble
        currentByte = nibble(hexString[charIndex]) << 4;
      }
      else
      {
        // Odd characters go into low nibble
        currentByte |= nibble(hexString[charIndex]);
        byteArray[byteIndex++] = currentByte;
        currentByte = 0;
      }
    }
  }
}

void Y506A::dumpByteArray(const byte * byteArray, const byte arraySize)
{

  for (int i = 0; i < arraySize; i++)
  {
    Serial.print("0x");
    if (byteArray[i] < 0x10)
      Serial.print("0");
    Serial.print(byteArray[i], HEX);
    Serial.print(", ");
  }
  Serial.println();
}



void Y506A::resetCall() {

  int len_defcall = sizeof(defcall) / sizeof(defcall[0]);

  for (int i = 0; i < len_defcall; i++) {
    Serial2.write(defcall[i]);
  }

  int cnt_defcall = 0;
  while (Serial2.available()) {
    ByteArray[cnt_defcall] = Serial2.read();
    cnt_defcall++;
  }

  int crc_defcall = sizeof(ByteArray) / sizeof(ByteArray[0]);


  len_defcall  = 0;
  cnt_defcall = 0;
  crc_defcall = 0;

}

void Y506A::setCall(float fsetKval, float fsetBval) {

  //int len_setcall = sizeof(setcall) / sizeof(setcall[0]);

  String ssetKval = fToLEs(fsetKval);
  String ssetBval = fToLEs(fsetBval);

  //Serial.println("Kval set : " + ssetKval);
  //Serial.println("Bval set : " + ssetBval);

  int ssetKval_len = ssetKval.length();

  int ssetBval_len = ssetBval.length();

  byte byteArrKval[MaxByteArraySize] = {0};

  byte byteArrBval[MaxByteArraySize] = {0};

  //hexCharacterStringToBytes(byteArrKval, "0000803F");
  //hexCharacterStringToBytes(byteArrBval, "00000000");

  hexCharacterStringToBytes(byteArrKval, ssetKval);
  hexCharacterStringToBytes(byteArrBval, ssetBval);

  //dumpByteArray(byteArrKval, MaxByteArraySize);
  //dumpByteArray(byteArrBval, MaxByteArraySize);

  int len_defcall = sizeof(defcall) / sizeof(defcall[0]);

  String defregs;
  //  Serial.println("\nDEF-REG");
  //  for (int i = 0; i < defcall_len; i++) {
  //    Serial.print("[");
  //    Serial.print(i);
  //    Serial.print("]");
  //    Serial.print("=");
  //
  //    defregs = String(defcall[i], HEX);
  //    Serial.print(defregs);
  //    Serial.print(" ");
  //  }

  int len_setcall = sizeof(setcall) / sizeof(setcall[0]);

  for (int i = 7; i <= 10; i++) {
    setcall[i] = byteArrKval[(i - 7)];
  }

  for (int i = 11; i <= 14; i++) {
    setcall[i] = byteArrBval[(i - 11)];
  }

  String setregs;
  //  Serial.println("\nSET-REG");
  //  for (int i = 0; i < setcall_len; i++) {
  //    Serial.print("[");
  //    Serial.print(i);
  //    Serial.print("]");
  //    Serial.print("=");
  //
  //    setregs = String(setcall[i], HEX);
  //    Serial.print(setregs);
  //    Serial.print(" ");
  //  }

  //for (int i = 0; i < setcall_len; i++) {
  //  Serial2.write(setcall[i]);
  //}

  int cnt_setcall = 0;
  while (Serial2.available()) {
    ByteArray[cnt_setcall] = Serial2.read();
    cnt_setcall++;
  }

  int crc_setcall = sizeof(ByteArray) / sizeof(ByteArray[0]);


  len_setcall = 0;
  cnt_setcall = 0;
  crc_setcall = 0;

}

void Y506A::getData() {
  int len_measure = sizeof(measure) / sizeof(measure[0]);
  for (int i = 0; i < len_measure; i++) {
    Serial2.write(measure[i]);
  }

  int cnt_measure = 0;
  while (Serial2.available()) {
    ByteArray[cnt_measure] = Serial2.read();
    cnt_measure++;
  }

  int crc_measure = sizeof(ByteArray) / sizeof(ByteArray[0]);

  // 0-9 byte checker HETempt
  HETempt = "";
  for (int i = 6; i >= 3; i--) {
    if (ByteArray[i] < 16) {
      HETempt += ("0" + String(ByteArray[i], HEX));
    }
    else {
      HETempt += String(ByteArray[i], HEX);
    }
  }
  int HETempt_len = HETempt.length() + 1;
  char cht[HETempt_len];
  HETempt.toCharArray(cht, HETempt_len);
  long lt = strtol(cht, NULL, 16);
  ft = *((float*)&lt);

  // 0-9 byte checker HEDOA
  HEDOA = "";
  for (int i = 10; i >= 7; i--) {
    if (ByteArray[i] < 16) {
      HEDOA += ("0" + String(ByteArray[i], HEX));
    }
    else {
      HEDOA += String(ByteArray[i], HEX);
    }
  }
  int HEDOA_len = HEDOA.length() + 1;
  char chdoa[HEDOA_len];
  HEDOA.toCharArray(chdoa, HEDOA_len);
  long ldoa = strtol(chdoa, NULL, 16);
  fdoa = *((float*)&ldoa);

  // 0-9 byte checker HEDOB
  HEDOB = "";
  for (int i = 14; i >= 11; i--) {
    if (ByteArray[i] <= 15) {
      HEDOB += ("0" + String(ByteArray[i], HEX));
    }
    else {
      HEDOB += String(ByteArray[i], HEX);
    }
  }
  int HEDOB_len = HEDOB.length() + 1;
  char chdob[HEDOB_len];
  HEDOB.toCharArray(chdob, HEDOB_len);
  long ldob = strtol(chdob, NULL, 16);
  fdob = *((float*)&ldob);

  len_measure = 0;
  cnt_measure = 0;
  crc_measure = 0;
}

float Y506A::valTemp() {
  float Y506valTempt;

  Y506valTempt = ft;

  return Y506valTempt;
}

float Y506A::valDoA() {
  float Y506valDoA;

  Y506valDoA = fdoa;

  return Y506valDoA;
}

float Y506A::valDoB() {
  float Y506valDoB;

  Y506valDoB = fdob;

  return Y506valDoB;
}

float Y506A::getTemp() {
  float Y506getTempt;

  int len_measure = sizeof(measure) / sizeof(measure[0]);
  for (int i = 0; i < len_measure; i++) {
    Serial2.write(measure[i]);
  }

  int cnt_measure = 0;
  while (Serial2.available()) {
    ByteArray[cnt_measure] = Serial2.read();
    cnt_measure++;
  }

  int crc_measure = sizeof(ByteArray) / sizeof(ByteArray[0]);

  // 0-9 byte checker HETempt
  HETempt = "";
  for (int i = 6; i >= 3; i--) {
    if (ByteArray[i] < 16) {
      HETempt += ("0" + String(ByteArray[i], HEX));
    }
    else {
      HETempt += String(ByteArray[i], HEX);
    }
  }
  int HETempt_len = HETempt.length() + 1;
  char cht[HETempt_len];
  HETempt.toCharArray(cht, HETempt_len);
  long lt = strtol(cht, NULL, 16);
  ft = *((float*)&lt);
  Y506getTempt = ft;

  len_measure = 0;
  cnt_measure = 0;
  crc_measure = 0;

  return Y506getTempt;
}

float Y506A::getDoA() {
  float Y506getDo;

  int len_measure = sizeof(measure) / sizeof(measure[0]);
  for (int i = 0; i < len_measure; i++) {
    Serial2.write(measure[i]);
  }

  int cnt_measure = 0;
  while (Serial2.available()) {
    ByteArray[cnt_measure] = Serial2.read();
    cnt_measure++;
  }

  int crc_measure = sizeof(ByteArray) / sizeof(ByteArray[0]);


  // 0-9 byte checker HEDOA
  HEDOA = "";
  for (int i = 10; i >= 7; i--) {
    if (ByteArray[i] < 16) {
      HEDOA += ("0" + String(ByteArray[i], HEX));
    }
    else {
      HEDOA += String(ByteArray[i], HEX);
    }
  }
  int HEDOA_len = HEDOA.length() + 1;
  char chdoa[HEDOA_len];
  HEDOA.toCharArray(chdoa, HEDOA_len);
  long ldoa = strtol(chdoa, NULL, 16);
  fdoa = *((float*)&ldoa);
  Y506getDo = fdoa;

  len_measure = 0;
  cnt_measure = 0;
  crc_measure = 0;

  return Y506getDo;
}

float Y506A::getDoB() {
  float Y506getDo;

  int len_measure = sizeof(measure) / sizeof(measure[0]);
  for (int i = 0; i < len_measure; i++) {
    Serial2.write(measure[i]);
  }

  int cnt_measure = 0;
  while (Serial2.available()) {
    ByteArray[cnt_measure] = Serial2.read();
    cnt_measure++;
  }

  int crc_measure = sizeof(ByteArray) / sizeof(ByteArray[0]);


  // 0-9 byte checker HEDOB
  HEDOB = "";
  for (int i = 14; i >= 11; i--) {
    if (ByteArray[i] <= 15) {
      HEDOB += ("0" + String(ByteArray[i], HEX));
    }
    else {
      HEDOB += String(ByteArray[i], HEX);
    }
  }
  int HEDOB_len = HEDOB.length() + 1;
  char chdob[HEDOB_len];
  HEDOB.toCharArray(chdob, HEDOB_len);
  long ldob = strtol(chdob, NULL, 16);
  fdob = *((float*)&ldob);
  Y506getDo = fdob;

  len_measure = 0;
  cnt_measure = 0;
  crc_measure = 0;

  return Y506getDo;
}

void Y506A::stopedCall() {

  int len_stoped = sizeof(stoped) / sizeof(stoped[0]);
  for (int i = 0; i < len_stoped; i++) {
    Serial2.write(stoped[i]);
  }

  int cnt_stoped = 0;
  while (Serial2.available()) {
    ByteArray[cnt_stoped] = Serial2.read();
    cnt_stoped++;
  }

  int crc_stoped = sizeof(ByteArray) / sizeof(ByteArray[0]);

  len_stoped = 0;
  cnt_stoped = 0;
  crc_stoped = 0;

}
