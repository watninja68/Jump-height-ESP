#include <esp_now.h>
#include <WiFi.h>
#include<math.h>
#include "BluetoothSerial.h"
#include <LiquidCrystal_I2C.h>

int lcdColumns = 16;
int lcdRows = 2;

BluetoothSerial SerialBT;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  
float tthold = 1;
//#define USE_PIN // Uncomment this to use PIN during pairing. The pin is specified on the line below
void concatenate_string(char* s, char* s1)
{
    int i;
 
    int j = strlen(s);
 
    for (i = 0; s1[i] != '\0'; i++) {
        s[i + j] = s1[i];
    }
 
    s[i + j] = '\0';
 
    return;
}
String device_name = "ESP32-BT-Slave-1";
float var = 0.5;
int del = 100;
typedef struct values {
  float ax;
  float ay;
  float az;
  float gx;
  float gy;
  float gz;
} values;

bool flags = true;
bool flags2 = true;
bool flags3 = true;
int countx = 0 ;
int county = 0;
int countz = 0;
values incomingReadings;
float int_ax;
float int_ay;
float int_az;

// Variable to store if sending data was successful
String success;
double tempx;
double tempy;
double tempz;

// Callback when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len)
{
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  // Serial.print("Bytes received: ");
  // Serial.println(len);
  delay(del);
  float axx =  incomingReadings.ax;
  float ayy = incomingReadings.ay;
  float azz = incomingReadings.az;

  if(flags){
    int_ax = axx ;
    flags = false;
  }
  if(flags2){
    int_ay = ayy ;
    flags2 = false;
  }
  if(flags3){
    int_az = azz ;
    flags3 = false;
  }

  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  // Serial.print("Bytes received: ");
  // Serial.println(len);
  // Serial.print("Acceleration X: ");
  // Serial.print(axx);
  // Serial.print(", Y: ");
  // Serial.print(incomingReadings.ay);
  // Serial.print(", Z: ");
  // Serial.print(incomingReadings.az);
  // Serial.println(" m/s^2");

  // Serial.print("Rotation X: ");
  // Serial.print (incomingReadings.gx );
  // Serial.print(", Y: ");
  // Serial.print(incomingReadings.gy);
  // Serial.print(", Z: ");
  // Serial.print(incomingReadings.gz);
  // Serial.println(" rad/s");
  //Serial.print("axx");
  // Serial.println("The value is");
  // Serial.println(azz);
  //Serial.print("int_ax");
  // Serial.println(int_az);
  
  if(axx> int_ax +tthold|| axx<int_ax-tthold || azz> int_az +tthold|| azz<int_az-tthold ||ayy> int_ay +tthold|| ayy<int_ay-tthold ){
    if(axx> int_ax + var|| axx<int_ax - var ){
      countx++;
    }
    if(axx> int_ay + var|| axx<int_ay - var){
      county++;
    }
    if(axx> int_az + var|| axx<int_az - var){
      countz++;
    }
  }
  else{
    if(countx != 0){
    Serial.print(countx);

      tempx = (countx*125)/100;
      Serial.println("Jump Finished , Time taken = ");
      Serial.print(tempx);
      // Serial.println("10^6 ns");
      tempx = tempx/2;
      Serial.print("Jump Hight = ");
      float ansx = 0.5*int_ax*tempx*tempx/50;
      Serial.println(ansx);
      // SerialBT.write(ansx);
      // delay(1000);
      countx = 0;
      if(county!=0){
        tempy = (county*125)/100;
        tempy = tempy/2;
        tempy = (0.5*int_ax*tempy*tempy)/50;
        // delay(1000);
        county = 0;
      }
      if(countz!=0){
        tempz = (countz*125)/100;
        tempz = tempz/2;
        tempz = (0.5*int_ax*tempz*tempz)/500;
        // delay(1000);
        countz = 0;
      }
      Serial.println("Horizontal Distance:-");
      float ansy = sqrt(tempy*tempy + tempz*tempz);
      // Serial.println(ansy);


  
      char xx[10];
      char yy[10];
      sprintf(xx, "%f", ansx);
      sprintf(yy, "%f", ansy); 

      char xxa[100] = "X Dis ";
      char yya[100] = "Y DIs ";
      concatenate_string(xxa, xx);
      concatenate_string(yya, yy);
 
      SerialBT.print(ansx);
      SerialBT.print(ansy);

      // delay(1000);
      // SerialBT.write(tempy);
      Serial.println("bluettoth data sent");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(yya);
      lcd.setCursor(0,1);
      lcd.print( xxa);
      delay(1000);
    }
    else{
      lcd.clear();
      Serial.println("In ground");
      lcd.setCursor(0,1);
      lcd.print("In ground");

      countx = 0;
      countz = 0;
      county = 0;
    }
  }
  Serial.println("--");
  delay(del);
}

void setup()
{
  lcd.init();
  lcd.backlight();

  Serial.begin(115200);
  Serial.println("Code start");

  SerialBT.begin(device_name); //Bluetooth device name
  Serial.printf("The device with name \"%s\" is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str());
  //Serial.printf("The device with name \"%s\" and MAC address %s is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str(), SerialBT.getMacString()); // Use this after the MAC method is implemented
  #ifdef USE_PIN
    SerialBT.setPin(pin);
    Serial.println("Using PIN");
  #endif

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);
}

void loop()
{
  if (Serial.available()) {
    int aa = 123;
    SerialBT.write(Serial.read());
    
  }
  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }
}
