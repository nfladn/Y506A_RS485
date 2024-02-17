#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <printf.h>
#include <stdbool.h>
#include "Y506A.h"
#include "HardwareSerial.h"

Y506A sensorDO(16, 17); //(RX,TX)


float fcelcius, fdopersen, fdomg;

void setup() {
  Serial.begin(115200, SERIAL_8N1); // initialize serial communication at 115200 bits per second:
  delay(1000); //Take some time to open up the Serial Monitor
  sensorDO.init();

}

unsigned long rtimer00 = 0, timer00 = 500;
unsigned long rtimer01 = 0, timer01 = 1000;
unsigned long rtimer02 = 0, timer02 = 4000;
unsigned long rtimer03 = 0, timer03 = 1000;

long cnt0 = 0, cnt1 = 0, cnt2 = 0;

void loop() {
  unsigned long ct = millis();

  //BEGIN-TIMER01
  if (ct - rtimer01 >= timer01) {

    //sensorDO.setCall(1.10, 0.20);
    //sensorDO.resetCall();

    switch (cnt1) {
      case 1: {
          sensorDO.initCall();
          break;
        }
      case 2: {
          sensorDO.initCall();
          float Kval = sensorDO.infoKval();
          Serial.printf("\n awal nilai K : %f", Kval);
          float Bval = sensorDO.infoBval();
          Serial.printf("\n awal nilai B : %f", Bval);

          break;
        }
      case 3: {
          sensorDO.getTemp();
          break;
        }
      case 4 ... 10: {
          sensorDO.getData();

          fcelcius = sensorDO.valTemp();
          //fcelcius = sensorDO.getTemp();
          //Serial.printf("\n Suhu : %f Celcius \n", fcelcius);

          fdopersen = sensorDO.valDoA();
          //fdopersen = sensorDO.getDoA();
          //Serial.printf("\n Persentase DO : %f % \n", fdopersen);

          fdomg = sensorDO.valDoB();
          //fdomg = sensorDO.getDoB();
          //Serial.printf("\n mg/L DO : %f % \n", fdomg);

          break;
        }
      case 11: {
          sensorDO.stopedCall();
          break;
        }
    }
    cnt1 ++;
    if (cnt1 > 11) {
      cnt1 = 1;
    }
    Serial.printf("\n Suhu : %f Celcius \n", fcelcius);
    Serial.printf("\n Persentase DO : %f % \n", fdopersen);
    Serial.printf("\n dum mg/L DO : %f % \n", fdomg);

    rtimer01 = ct;
  }//END-TIMER01

}
