#ifndef Y506A_H
#define Y506A_H

#include <Arduino.h>
#include <math.h>

class Y506A {

  private:
    byte modbusPinA;
    byte modbusPinB;
    long rtima, tima, rtimb, timb, rtimc, timc;
    byte ByteArray[250];
    int ByteData[20], a = 0, b = 0, c = 0;
    String pbalas;
    char* pbalasanDO;
    float total = 0.00;
    float readings[10];
    int readIndex = 0;
    byte started[8] = {0x01, 0x03, 0x25, 0x00, 0x00, 0x01, 0x8F, 0x06};
    byte infocall[8] = {0x01, 0x03, 0x11, 0x00, 0x00, 0x04, 0x41, 0x35};
    //------------------0xda, 0xfc, 0xsa, 0xsa, 0xnr, 0xnr, 0xnb, 0xrk, 0xrk, 0xrk, 0xrk, 0xrb, 0xrb, 0xrb, 0xrb, crc, crc
    byte defcall[17] = {0x01, 0x10, 0x11, 0x00, 0x00, 0x04, 0x08, 0x00, 0x00, 0x80, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x81, 0xAE};
    byte setcall[17] = {0x01, 0x10, 0x11, 0x00, 0x00, 0x04, 0x08, 0x00, 0x00, 0x80, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x81, 0xAE};
    byte measure[8] = {0x01, 0x03, 0x26, 0x00, 0x00, 0x06, 0xCE, 0x80};
    byte stoped[8] = {0x01, 0x03, 0x2E, 0x00, 0x00, 0x01, 0x8D, 0x22};
    int lena, lenb , lenc, lend, lene;
    String HEKval, HEBval, HETempt, HEDOA, HEDOB;
    float fkval, fbval, ft, fdoa, fdob;
    const byte MaxByteArraySize = 4;

  public:
    // Setup pin and call init()
    Y506A(byte modbusPinA, byte modbusPinB);

    void init();

    void initCall();

    float infoKval();

    float infoBval();

    float getKval();

    float getBval();

    String fToLEs(float ffToLEs);

    byte nibble(char c);

    void hexCharacterStringToBytes(byte *byteArray, String datString);

    void dumpByteArray(const byte * byteArray, const byte arraySize);

    void setKval(float fsetKval);

    void setBval(float fsetBval);

    void setCall(float fsetKval, float fsetBval);

    void resetCall();

    void getData();

    float valTemp();

    float valDoA();

    float valDoB();

    float getTemp();

    float getDoA();

    float getDoB();

    void stopedCall();

};

#endif
