#include "DMC40457_clock.h"
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Ticker.h>
#include <Adafruit_AHT10.h>

// sens
Adafruit_AHT10 aht;

// display
DMC40457_clock display(0x20,40,4);

// wifi for time
const char *ssid     = "";
const char *password = "";
const long  utcOffsetInSeconds = 3600;
bool update_display_flag = false;
Ticker timer;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "ntp.telenet.be", utcOffsetInSeconds*2);

// dimmable light
const int zero_cross = 13;
const int triac_pin  = 14;
const int rot_1 = 1;
const int rot_2 = 3;
const int rot_B = 2;
int dim_value = 0;
bool light_on = false;
int last_btn_debounce = 0;
const int btn_debounce_time = 500;

// interrupt handlers
void ICACHE_RAM_ATTR trigger_triac();
void ICACHE_RAM_ATTR dim_light();
void ICACHE_RAM_ATTR toggle_light();
void update_display_ISR();

void setup() {
  Serial.begin(115200);
  
  // temp sens
  aht.begin();
  
  // display
  display.begin();

  // time
  WiFi.begin(ssid,password);
  connect_wifi();
  timeClient.begin();
  timer.attach_ms(10000, update_display_ISR);

  // dimmable light
  pinMode(triac_pin, OUTPUT);
  pinMode(rot_1, INPUT);
  pinMode(rot_2, INPUT);
  pinMode(rot_B, INPUT);
  attachInterrupt(digitalPinToInterrupt(rot_1), dim_light, CHANGE);
  attachInterrupt(digitalPinToInterrupt(rot_B), toggle_light, FALLING);
  update_display();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(update_display_flag){update_display();}
}

void update_display(){

  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);

  if(!timeClient.update()){return;}
  int hours   = timeClient.getHours();
  int minutes = timeClient.getMinutes();
  display.update(hours, minutes, (int)temp.temperature);
  update_display_flag = false;
}
// time
void connect_wifi(){
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print("."); 
  }
}

void update_display_ISR(){
  update_display_flag = true;
}


// dimmable light interrupts
void trigger_triac(){
  delayMicroseconds(map(dim_value,0,200,10,6500)); // read AD1 
  digitalWrite(triac_pin, HIGH);
  delayMicroseconds(10);  
  digitalWrite(triac_pin, LOW);
} 

void dim_light(){
  if((digitalRead(rot_1) != digitalRead(rot_2)) && dim_value > 0){
    dim_value--;
  }else if(dim_value < 200){
    dim_value++;
  }
}

void toggle_light(){
  if(millis() - last_btn_debounce < btn_debounce_time){
    return;
  }
  last_btn_debounce = millis();
  if(light_on){
    detachInterrupt(digitalPinToInterrupt(zero_cross));
  }else{
    attachInterrupt(digitalPinToInterrupt(zero_cross), trigger_triac, FALLING);
  }
  light_on = !light_on;
}
