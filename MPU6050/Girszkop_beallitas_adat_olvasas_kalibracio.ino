#include <Wire.h> //I2C könyvtár

//nyers Gyro értékeknek változó
int16_t Gyro_X;
int16_t Gyro_Y;
int16_t Gyro_Z;
//számított szögsebességeknek változó
float Rate_Roll;
float Rate_Pitch;
float Rate_Yaw;
//2000 mintavételt ezekhez adjuk hozzá
float Rate_Roll_Kalibracios=0;
float Rate_Pitch_Kalibracios=0;
float Rate_Yaw_Kalibracios=0;
//for ciklus változói
int Kalibracios_int;
int Kalibracio_mintavetel_szam=2000;


void mpu_6050_regiszterek_beallitasa(void){

  // 10 Hz aluláteresztő beállítása
  Wire.beginTransmission(0x68); //MPU-val való komunikáció 
  Wire.write(0x1A); //kérés küldése a regiszer indítására 
  Wire.write(0x05); //10 Hz beleírása HEX-be 
  Wire.endTransmission(); //küldés 

  // Girszkóp +-500 °/s beállítása 65.5 LSB/°/s
  Wire.beginTransmission(0x68); //MPU-val való komunikáció 
  Wire.write(0x1B); //kérés küldése a regiszer indítására 
  Wire.write(0x08); //+-500 °/s beleírása HEX-be 
  Wire.endTransmission(); //küldés 

  //MPU aktiválása (felébesztése)
  Wire.beginTransmission(0x68); //MPU-val való komunikáció 
  Wire.write(0x6B); //kérés küldése a regiszer indítására 
  Wire.write(0x00); //MPU felébesztésének beleírása HEX-be 
  Wire.endTransmission(); //küldés 
}

void gyro_adat_olvasas(void){

  //Girszkóp adatainak kiolvasása
  Wire.beginTransmission(0x68); //MPU-val való komunikáció 
  Wire.write(0x43); //Gyro adatok itt keződnek
  Wire.endTransmission(false);
  Wire.requestFrom(0x68,6); // 6 bytot kérünk

  while (Wire.available()<6) {}; // vár a 6 byte-ra 

  //MPU 16 bites értékeit bele bitshiftelni egy 16 bites változóba mivel I2C rigsztereken csak 8 bites byte-okban kommunikál 
  // << bitshift 8 bittel feltoljuk a 16 bites változó 15-8 bitjébe
  // | (bitwise OR) összekapcsolja a byte.okat
  Gyro_X = Wire.read()<<8 | Wire.read();
  Gyro_Y = Wire.read()<<8 | Wire.read();
  Gyro_Z = Wire.read()<<8 | Wire.read();
 
  // °/s szögsebességek kiszámolás mivel +-500°/s LSB 65.5/°/s 
  // MPU 16 bites adat olvasás 2^16, +-500 = 1000 ------> 2^16/1000=65.536 ~ 65.5
  Rate_Roll=(float)Gyro_X/65.5;
  Rate_Pitch=(float)Gyro_Y/65.5;
  Rate_Yaw=(float)Gyro_Z/65.5;
}

void setup() {
  // put your setup code here, to run once:
Serial.begin(57600);
Wire.setClock(400000); //400kHz ClockSpeed MPU6050 adatlap írja I2C kommunikáció esetén
Wire.begin(21, 22); //SDA, SCL
delay(250);
mpu_6050_regiszterek_beallitasa();


//2000 mintavételes kalibráció 
for (Kalibracios_int=0 ; Kalibracios_int < Kalibracio_mintavetel_szam ; Kalibracios_int++){
  gyro_adat_olvasas();
  Rate_Roll_Kalibracios += Rate_Roll;
  Rate_Pitch_Kalibracios += Rate_Pitch;
  Rate_Yaw_Kalibracios += Rate_Yaw;
  delay(1);
}
//Mintavételi számmal való vissza osztás
Rate_Roll_Kalibracios /= (float)Kalibracio_mintavetel_szam ;
Rate_Pitch_Kalibracios /= (float)Kalibracio_mintavetel_szam ;
Rate_Yaw_Kalibracios /= (float)Kalibracio_mintavetel_szam ;
}

void loop() {
  // put your main code here, to run repeatedly:
gyro_adat_olvasas(); //Gyro adat olvasó fgv meghívása
//Levonjuk az offset hibát az újra olvasott értékekből így közel 0°/s lesz minden
Rate_Roll -= Rate_Roll_Kalibracios;
Rate_Pitch -= Rate_Pitch_Kalibracios;
Rate_Yaw -= Rate_Yaw_Kalibracios;
//Sorosporton kiiratás
Serial.print("Roll rate [°/s]= ");
Serial.print(Rate_Roll);
Serial.print(" Pitch Rate [°/s]= ");
Serial.print(Rate_Pitch);
Serial.print(" Yaw Rate [°/s]= ");
Serial.println(Rate_Yaw);
delay(50);
}
