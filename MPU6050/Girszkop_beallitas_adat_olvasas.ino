#include <Wire.h> //I2C könyvtár

int16_t Gyro_X;
int16_t Gyro_Y;
int16_t Gyro_Z;
float Rate_Roll;
float Rate_Pitch;
float Rate_Yaw;
long Gyro_X_Kalibracios;
long Gyro_Y_Kalibracios;
long Gyro_Z_Kalibracios;
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

void gyro_read_data(void){

  //Girszkóp adatainak kiolvasása
  Wire.beginTransmission(0x68); //MPU-val való komunikáció 
  Wire.write(0x43);
  Wire.endTransmission(false);
  Wire.requestFrom(0x68,6);

  while (Wire.available()<6) {}; // vár a 6 byte-ra 

  //MPU 16 bites értékeit bele bitshiftelni egy 16 bites változóba mivel I2C rigsztereken csak 8 bites byte-okban kommunikál 
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
Wire.setClock(400000);
Wire.begin(21, 22); //SDA, SCL
delay(250);
mpu_6050_regiszterek_beallitasa();
}

void loop() {
  // put your main code here, to run repeatedly:
gyro_read_data();
Serial.print("Roll: ");
Serial.print(Rate_Roll);
Serial.print(" °/s ");
Serial.print("\t");

Serial.print("Pitch: ");
Serial.print(Rate_Pitch);
Serial.print(" °/s ");
Serial.print("\t");

Serial.print("Yaw: ");
Serial.print(Rate_Yaw);
Serial.println(" °/s ");
delay(50);
}
