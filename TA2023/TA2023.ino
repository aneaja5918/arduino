#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);

#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL6cKYAQtUr"
#define BLYNK_TEMPLATE_NAME "Semontric"
#define BLYNK_AUTH_TOKEN "2nsNLteprkAKT0zyNToMoB6dQjjhxt7k"

unsigned long previousMillis = 0;
unsigned long interval = 1000; 
int count = 0;
int limit = 120;
int tokenCount =120;
unsigned long previousMillis2 = 0;// narutooo
unsigned long interval2 = 1000; 
int count2 = 0;
int limit2 = 120;
int batas_kwh=100;
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

int taskIndex  = 0; 
int taskIndex1 = 0; 
int taskIndex2 = 0; 
int taskIndex3 = 0; 

char auth[] = BLYNK_AUTH_TOKEN ;
char ssid[] = "INFINITY MESS";
char pass[] = "c1tr4tropika";

#define buzzer D8
#define relay D3
#define DHTPIN D4
#define DHTTYPE DHT11 //Tipe DHT11
DHT dht(DHTPIN, DHTTYPE);

#include <PZEM004Tv30.h>
PZEM004Tv30 pzem(14, 12);//Rx ,Tx
float harga_KWh = 1467;
BlynkTimer timer;

BLYNK_WRITE(V1){
batas_kwh = param.asInt();
}
BLYNK_WRITE(V2){
int pinValue = param.asInt();
digitalWrite(relay,pinValue);
}
BLYNK_WRITE(V3){
int resetValue = param.asInt();
if (resetValue == 1) {
pzem.resetEnergy();
Serial.println("Energy reset successfully");
Blynk.virtualWrite(V3, 0);
}
}
void sendSensor(){
  
float Power=pzem.power();
if(isnan(Power)){
  Serial.println("Gagal membaca power");
}else{
  Serial.print("Power:");
  Serial.print(Power);
  Serial.println("watt");
}
float Energy=pzem.energy();
if(isnan(Energy)){
  Serial.println("Gagal membaca Energy");
}else{
  Serial.print("Energy:");
  Serial.print(Energy);
  Serial.println("Kwh");
}
float Voltage=pzem.voltage();
if(isnan(Voltage)){
  Serial.println("Gagal membaca Voltage");
}else{
  Serial.print("Voltage:");
  Serial.print(Voltage);
  Serial.println("Volt");
}
float Current=pzem.current();
if(isnan(Current)){
  Serial.println("Gagal membaca Current");
}else{
  Serial.print("Current:");
  Serial.print(Current);
  Serial.println("A");
}
float Frequency = pzem.frequency();
if(isnan(Frequency)){
  Serial.println("Gagal membaca Frequency");
}else{
  Serial.print("Frequency:");
  Serial.print(Frequency);
  Serial.println("Hz");
}
float totalHarga = Energy * harga_KWh;
  Serial.print("Rp:");
  Serial.print(totalHarga);
  Serial.println();

float t = dht.readTemperature(); //pembacaan sensor
float h = dht.readHumidity();
Serial.print("% Temperature: ");
Serial.print(t);
Serial.println("C ");
Serial.print("% Kelembaban: ");
Serial.print(h);
Serial.println("% ");


String tasks[] = {"Tegangan", "Arus", "Frekuensi","Daya","Energi","Biaya","Suhu","Kelembaban"}; 
String tasks1[] = {"T =", "A =", "F =","D =","E =","Rp=","S =","K ="}; 
float tasks2[] = {Voltage,Current,Frequency,Power,Energy,totalHarga,t,h}; 
String tasks3[] = {" Volt", "  A", " Hz"," Watt"," Kwh",""," C"," %"};

 unsigned long currentMillis2 = millis(); 
  if (currentMillis2 - previousMillis2 >= interval2) {
    previousMillis2 = currentMillis2; 
    count2++;
   if (count2 <= limit2) {
    taskIndex++; 
    if (taskIndex >= sizeof(tasks) / sizeof(tasks[0])) {
      taskIndex = 0;
    }
    taskIndex1++; 
    if (taskIndex1 >= sizeof(tasks1) / sizeof(tasks1[0])) {
      taskIndex1 = 0; 
    }
  taskIndex2++;  
    if (taskIndex2 >= sizeof(tasks2) / sizeof(tasks2[0])) {
      taskIndex2 = 0;  
    }
  taskIndex3++; 
    if (taskIndex3 >= sizeof(tasks3) / sizeof(tasks3[0])) {
      taskIndex3 = 0;  
      }
    lcd.clear(); 
    lcd.setCursor(0, 0);
    lcd.print(tasks[taskIndex]);
    lcd.setCursor(0, 1);
    lcd.print(tasks1[taskIndex1]);
    lcd.setCursor(4, 1);
    lcd.print(tasks2[taskIndex2]);
    lcd.setCursor(8, 1);
    lcd.print(tasks3[taskIndex3]);
    lcd.setCursor(13, 0);
    lcd.print(count);
    
   }else{
  count2 = 0;
 }
  }
 
if(Energy>=batas_kwh){
 unsigned long currentMillis = millis();
 Serial.println(count);
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
           count++;
      if (count <= limit) {
         for (int i = 0; i < tokenCount; i++) {
          tone(buzzer, 1000, 200); 
        }
        } else {
        digitalWrite(buzzer,LOW);
        digitalWrite(relay,HIGH);
       }
       }
    } else {
    digitalWrite(buzzer, LOW);
    count = 0;
   }
    Blynk.virtualWrite(V1, batas_kwh);
    Blynk.virtualWrite(V4, t);
    Blynk.virtualWrite(V5, Power);
    Blynk.virtualWrite(V6, Energy);
    Blynk.virtualWrite(V7,Voltage);
    Blynk.virtualWrite(V8,Current);
    Blynk.virtualWrite(V9,Frequency);
    Blynk.virtualWrite(V10,totalHarga);
    }
void setup(){
lcd.init();
lcd.backlight();
lcd.setCursor(3, 0);
lcd.print("Tugas Akhir");
lcd.setCursor(0, 1);
lcd.print("Eric Nurmansyah");
delay(2000);
lcd.clear();
pinMode(relay,OUTPUT);
pinMode(buzzer,OUTPUT);
Serial.begin(115200);
Blynk.begin(auth, ssid, pass);
dht.begin();
timer.setInterval(1000L, sendSensor);
}
void loop(){
Blynk.run();
timer.run();
}
