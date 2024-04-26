#include <stdio.h>
#include <string.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_now.h"
#include "BluetoothSerial.h"
#include "LiquidCrystal_I2C.h"

#define LCD_COLUMNS 16
#define LCD_ROWS 2
#define PIN_I2C_SDA GPIO_NUM_32
#define PIN_I2C_SCL GPIO_NUM_33
#define THRESHOLD 1.0
#define VAR 0.5
#define DELAY_MS 100

typedef struct {
    float ax;
    float ay;
    float az;
    float gx;
    float gy;
    float gz;
} SensorData;

BluetoothSerial SerialBT;
LiquidCrystal_I2C lcd(0x27, LCD_COLUMNS, LCD_ROWS);

float int_ax, int_ay, int_az;
bool flags = true, flags2 = true, flags3 = true;
int countx = 0, county = 0, countz = 0;
float tempx, tempy, tempz;

void concatenate_string(char* s, char* s1) {
    int i;
    int j = strlen(s);
    for (i = 0; s1[i] != '\0'; i++) {
        s[i + j] = s1[i];
    }
    s[i + j] = '\0';
}

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
    SensorData incomingReadings;
    memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));

    float axx = incomingReadings.ax;
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
      tempx = (countx*125)/1000;
      Serial.println("Jump Finished , Time taken = ");
      Serial.print(tempx);
      // Serial.println("10^6 ns");
      tempx = tempx/2;
      Serial.print("Jump Hight = ");
      float ansx = 0.5*int_ax*tempx*tempx;
      Serial.println(ansx);
      // SerialBT.write(ansx);
      // delay(1000);
      countx = 0;
      if(county!=0){
        tempy = (county*125)/1000;
        tempy = tempy/2;
        tempy = (0.5*int_ax*tempy*tempy);
        // delay(1000);
        county = 0;
      }
      if(countz!=0){
        tempz = (countz*125)/1000;
        tempz = tempz/2;
        tempz = (0.5*int_ax*tempz*tempz);
        // delay(1000);
        countz = 0;
      }
      Serial.println("Horizontal Distance:-");
      float ansy = sqrt(tempy*tempy + tempz*tempz);
      // Serial.println(ansy);

      SerialBT.write(ansx);
      SerialBT.write(ansy);
      char xx[10];
      char yy[10];
      sprintf(xx, "%f", ansx);
      sprintf(yy, "%f", ansy); 
      char xxa[100] = "X Dis ";
      char yya[100] = "Y DIs ";
      concatenate_string(xxa, xx);
      concatenate_string(yya, yy);
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

void setup() {
    lcd.init();
    lcd.backlight();

    Serial.begin(115200);
    Serial.println("Code start");

    SerialBT.begin("ESP32-BT-Slave-1");

    WiFi.mode(WIFI_STA);

    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
	while(1){
    if (Serial.available()) {
        SerialBT.write(Serial.read());
    }
    if (SerialBT.available()) {
        Serial.write(SerialBT.read());
    }}
}

extern "C" void app_main() {
    setup();
	loop();

    while (1) {
        loop();
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
