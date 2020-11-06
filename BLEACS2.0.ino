//THE SMARTBLINDS AKA ALARM CLOCK SYSTEM
//


#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLEClient.h>
#include <RTClib.h>
RTC_DS3231  rtc;
#include <Wire.h>
#include <ESP32Servo.h>
Servo myservo;  
int pos = 0;    
int servoPin = 13; 
#include <Adafruit_NeoPixel.h>
//pin 15 is LED strip
Adafruit_NeoPixel strip(28, 15, NEO_GRB + NEO_KHZ800);

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define BLINDS_UUID "97763f12-db42-11ea-87d0-0242ac130003"

#define CURRENTHOUR_UUID "97764282-db42-11ea-87d0-0242ac130003"

#define CURRENTMIN_UUID "97764386-db42-11ea-87d0-0242ac130003"

#define ALARMSTATUS_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

#define LEDMODE_UUID "a8f5b418-cf10-11ea-87d0-0242ac130003"

#define REDLED_UUID "a8f5b648-cf10-11ea-87d0-0242ac130003"

#define GREENLED_UUID "a8f5b756-cf10-11ea-87d0-0242ac130003"

#define BLUELED_UUID "a8f5b81e-cf10-11ea-87d0-0242ac130003"

#define ALARMHOUR_UUID "a8f5b8dc-cf10-11ea-87d0-0242ac130003"

#define ALARMMIN_UUID "a8f5bb8e-cf10-11ea-87d0-0242ac130003"

#define CLOCK_INTERRUPT_PIN 34


BLEServer* pServer = NULL;
bool deviceConnected = false;

int LEDMODE1 = 0;
int STATUS1 = 1;
int RED1 = 0;
int GREEN1 = 0;
int BLUE1 = 0;
uint8_t ALARMHOUR1 ;
uint8_t ALARMMIN1 ;
DateTime ALARMTIME = DateTime (0, 0, 0, ALARMHOUR1, ALARMMIN1, 0);
int HOUR1 = 0;
int MIN1 = 0;
int BLINDS1 = 1;

uint32_t red = strip.Color(255, 0, 0);
DateTime now = rtc.now();

  int Yearnow = (now.year());
  int Monthnow = (now.month());
  int Daynow = (now.day());
  
bool motor = false;
int last;




void colorWipe(uint32_t color, int wait) {
  for (int i = 0; i < strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
};

void fade() {

  for (long firstPixelHue = 0; firstPixelHue < 3 * 65536; firstPixelHue += 256) {
    for (int i = 0; i < strip.numPixels(); i++) {
      int pixelHue = firstPixelHue + (i * 65536L);

      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
      delay(50);
    }

    strip.show(); // Update strip with new contents
  }
  delay(50);
};

void onAlarm() {
  Serial.println("Alarm occured!");
  BLINDS1=BLINDS1 + 1;
  LEDMODE1=5;
  
};

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      Serial.println("YESSIRSKI");


    }


    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;

    }

};

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic*ALARMSTATUS ) {
      String STATUS = ALARMSTATUS->getValue().c_str();
      STATUS1 = STATUS.toInt();
      Serial.println("*********");
      Serial.println("ALARM STATUS: ");


      Serial.println(STATUS);
      if (STATUS1 == 1) {
        rtc.clearAlarm(1);
        rtc.setAlarm1(ALARMTIME, DS3231_A1_Hour);
      }
      else {
        rtc.disableAlarm(1);
      }

    }
}
;


class MyCallbacks1: public BLECharacteristicCallbacks {

    void onWrite(BLECharacteristic *LEDCHAR ) {
      String LEDMODE = LEDCHAR->getValue().c_str();
      int LEDMODE1 = LEDMODE.toInt();
      Serial.println("*********");
      Serial.print("New LED: ");
      Serial.println(LEDMODE1);
      LEDSWITCH(LEDMODE1);
    }

};

class MyCallbacks2: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic*ALARMHOUR ) {
      String ALARMHOURret = ALARMHOUR->getValue().c_str();
      ALARMHOUR1 = ALARMHOURret.toInt();
      Serial.println("*********");
      Serial.print("ALARM HOUR: ");
      Serial.println(ALARMHOUR1);
      char buf1[] = "hh:mm";
      DateTime ALARMTIME = DateTime (0, 0, 0, ALARMHOUR1, ALARMMIN1, 0);
      Serial.println(ALARMTIME.toString(buf1));
      char date[10] = "hh:mm:ss";
      rtc.now().toString(date);
      Serial.print(date);
      if (STATUS1 == 1) {
        //rtc.clearAlarm(1);
        rtc.setAlarm1(ALARMTIME, DS3231_A1_Hour);
      }
      else {
        rtc.disableAlarm(1);
      }

    }
};

class MyCallbacks3: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic*ALARMMIN ) {
      String ALARMMINret = ALARMMIN->getValue().c_str();
      ALARMMIN1 = ALARMMINret.toInt();
      Serial.println("*********");
      Serial.print("ALARM MIN: ");
      Serial.println(ALARMMIN1);
      char buf1[] = "hh:mm";
      DateTime ALARMTIME = DateTime (0, 0, 0, ALARMHOUR1, ALARMMIN1, 0);
      Serial.println(ALARMTIME.toString(buf1));
      char date[10] = "hh:mm:ss";
      rtc.now().toString(date);
      Serial.print(date);
      if (STATUS1 == 1) {
        rtc.clearAlarm(1);
        
        rtc.setAlarm1(ALARMTIME, DS3231_A1_Hour);
      }
      else {
        //rtc.disableAlarm(1);
      }

    }
};


class MyCallbacks4: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic*REDLED ) {
      String RED = REDLED->getValue().c_str();
      RED1 = RED.toInt();
      Serial.println("*********");
      Serial.print("RED: ");
      Serial.println(RED1);
    LEDSWITCH(LEDMODE1);

    }
};


class MyCallbacks5: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic*GREENLED ) {
      String GREEN = GREENLED->getValue().c_str();
      GREEN1 = GREEN.toInt();
      Serial.println("*********");
      Serial.print("GREEN: ");
      Serial.println(GREEN1);
      LEDSWITCH(LEDMODE1);
    }
};

class MyCallbacks6: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic*BLUELED ) {
      String BLUE = BLUELED->getValue().c_str();
      BLUE1 = BLUE.toInt();
      Serial.println("*********");
      Serial.print("BLUE: ");
      Serial.println(BLUE1);
    LEDSWITCH(LEDMODE1);

    }

};



class MyCallbacks7: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic* CURRENTHOUR) {
      String HOUR = CURRENTHOUR->getValue().c_str();
      HOUR1 = HOUR.toInt();

      Serial.println("*********");
      Serial.println("CURRENTHOUR: ");


      Serial.println(HOUR);
rtc.adjust(DateTime(Yearnow,Monthnow,Daynow,HOUR1,MIN1,0));

    }
};

class MyCallbacks8: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic* CURRENTMIN) {
      String MIN = CURRENTMIN->getValue().c_str();
      MIN1 = MIN.toInt();

      Serial.println("*********");
      Serial.println("CURRENTMIN: ");


      Serial.println(MIN);
    rtc.adjust(DateTime(Yearnow,Monthnow,Daynow,HOUR1,MIN1,0));
    char date[10] = "hh:mm:ss";
      rtc.now().toString(date);
      Serial.print(date);

    }
};
class MyCallbacks9: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic* BLINDS) {
      motor = true;
      BLINDS1 = BLINDS1 + 1;


      Serial.println("*********");
      Serial.println("BLINDS: ");
      Serial.print(BLINDS1);


    }
};

void setup() {

  Serial.begin(115200);
  Serial.println("WELCOME TO THE ACS");

  rtc.begin();
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  rtc.clearAlarm(1);
  rtc.clearAlarm(2);
  rtc.disableAlarm(2);
  ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
	myservo.setPeriodHertz(50);    // standard 50 hz servo
	myservo.attach(servoPin, 500, 2400);
  
  pinMode(CLOCK_INTERRUPT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(CLOCK_INTERRUPT_PIN), onAlarm, FALLING);
  rtc.writeSqwPinMode(DS3231_OFF);
  


  strip.begin();
  strip.clear();

  BLEDevice::init("ACS");
  BLEServer *pServer = BLEDevice::createServer();

  BLEService *pService = pServer->createService(BLEUUID(SERVICE_UUID), 80);
  pServer->setCallbacks(new MyServerCallbacks());

  BLECharacteristic *ALARMSTATUS = pService->createCharacteristic(
                                     ALARMSTATUS_UUID,
                                     BLECharacteristic::PROPERTY_READ |
                                     BLECharacteristic::PROPERTY_WRITE
                                   );
  BLECharacteristic *LEDCHAR = pService->createCharacteristic(
                                 LEDMODE_UUID,
                                 BLECharacteristic::PROPERTY_READ |
                                 BLECharacteristic::PROPERTY_WRITE
                               );
  BLECharacteristic *REDLED = pService->createCharacteristic(
                                REDLED_UUID,
                                BLECharacteristic::PROPERTY_READ |
                                BLECharacteristic::PROPERTY_WRITE


                              );
  BLECharacteristic *BLUELED = pService->createCharacteristic(
                                 BLUELED_UUID,
                                 BLECharacteristic::PROPERTY_READ |
                                 BLECharacteristic::PROPERTY_WRITE
                               );
  BLECharacteristic *GREENLED = pService->createCharacteristic(
                                  GREENLED_UUID,
                                  BLECharacteristic::PROPERTY_READ |
                                  BLECharacteristic::PROPERTY_WRITE
                                );

  BLECharacteristic *ALARMHOUR = pService->createCharacteristic(
                                   ALARMHOUR_UUID,
                                   BLECharacteristic::PROPERTY_READ |
                                   BLECharacteristic::PROPERTY_WRITE
                                 );
  BLECharacteristic *ALARMMIN = pService->createCharacteristic(
                                  ALARMMIN_UUID,
                                  BLECharacteristic::PROPERTY_READ |
                                  BLECharacteristic::PROPERTY_WRITE
                                );
  BLECharacteristic *BLINDS = pService->createCharacteristic(
                                BLINDS_UUID,
                                BLECharacteristic::PROPERTY_READ |
                                BLECharacteristic::PROPERTY_WRITE);

  BLECharacteristic *CURRENTHOUR = pService->createCharacteristic(
                                     CURRENTHOUR_UUID,
                                     BLECharacteristic::PROPERTY_READ |
                                     BLECharacteristic::PROPERTY_WRITE
                                   );
  BLECharacteristic *CURRENTMIN = pService->createCharacteristic(
                                    CURRENTMIN_UUID,
                                    BLECharacteristic::PROPERTY_READ |
                                    BLECharacteristic::PROPERTY_WRITE
                                  );
  ALARMSTATUS->setCallbacks(new MyCallbacks());
  LEDCHAR->setCallbacks(new MyCallbacks1());
  ALARMHOUR->setCallbacks(new MyCallbacks2());
  ALARMMIN->setCallbacks(new MyCallbacks3());
  REDLED->setCallbacks(new MyCallbacks4());
  GREENLED->setCallbacks(new MyCallbacks5());
  BLUELED->setCallbacks(new MyCallbacks6());
  CURRENTHOUR->setCallbacks(new MyCallbacks7());
  CURRENTMIN->setCallbacks(new MyCallbacks8());
  BLINDS->setCallbacks(new MyCallbacks9());

  /*pServer->setCallbacks(new MyServerCallbacks());
  ALARMSTATUS->setValue("0");
  LEDCHAR->setValue("0");
  ALARMHOUR->setValue("0");
  ALARMMIN->setValue("0");
  REDLED->setValue("0");
  GREENLED->setValue("0");
  BLUELED->setValue("0");
  CURRENTHOUR->setValue("0");
  CURRENTMIN->setValue("0");
  
 BLINDS->setValue("0");
*/
  pService->start();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pServer->getAdvertising()->addServiceUUID(SERVICE_UUID);


  pAdvertising->start();
}

void loop() {

  if (deviceConnected == false) {
    Serial.println("Searching..");
    colorWipe(strip.Color(  0,   0, 255), 20);
    strip.clear();
  }


 


  if ((BLINDS1%2 == 0) & (motor == true)) {
    Serial.println("Opening");
    myservo.write(180);    
	  delay(1500);
    myservo.write(90);
    delay(1000);
    Serial.print("STOP");
    motor = false;
  }
  else if ((BLINDS1%2 == 1) & (BLINDS1 != 0) & (motor == true)) {
    Serial.println("Closing");
    myservo.write(0);    
	  delay(1500);
    myservo.write(90);
    delay(1000);
    Serial.print("STOP");
    motor = false;
  }
  
  else {
    myservo.write(90);
    delay(1000);
    Serial.println("Nothing so far");
  }
  

}

void LEDSWITCH (int LEDMODE1)
{


switch (LEDMODE1) {
    case 0:
      strip.clear();
      strip.show();
      break;
    case 1:
      strip.fill(RED1, GREEN1, BLUE1);
      strip.show();
      last = 1;
      break;
    case 2:
      fade();
      last = 2;
      break;
    case 3:
      strip.clear();
      strip.show();
      break;
      last = 3;
    case 4:
      strip.clear();
      strip.show();
      last = 4;
      break;
    case 5:
    for (int i=0; i<30; i++) {
   strip.fill(red);
  strip.show();
  delay(250);
  strip.clear();
  strip.show();
  delay(250);
}
LEDMODE1 = last;
    break;
    default:
      strip.clear();
      strip.show();
  }
}