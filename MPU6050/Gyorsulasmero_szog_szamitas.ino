#include <Wire.h> //I2C könyvtár

//nyers Gyro értékeknek változó
int16_t Gyro_X;
int16_t Gyro_Y;
int16_t Gyro_Z;
//számított szögsebességeknek változó
float Rate_Roll;
float Rate_Pitch;
float Rate_Yaw;

//nyers Acc értékeknek változó
int16_t Acc_XLSB;
int16_t Acc_YLSB;
int16_t Acc_ZLSB;
//kiszámmolt Acc értékek
float Acc_X;
float Acc_Y;
float Acc_Z;
//számított szögs változó
float Szog_Roll;
float Szog_Pitch;


int16_t Temp;

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

  //Gyorsulásmérő +-8g beállítása 4096 LSB/g
  Wire.beginTransmission(0x68); //MPU-val való komunikáció 
  Wire.write(0x1C); //kérés küldése a regiszer indítására 
  Wire.write(0x10); //MPU felébesztésének beleírása HEX-be 
  Wire.endTransmission(); //küldés 
}

void gyro_acc_adat_olvasas(void){

  //Girszkóp adatainak kiolvasása
  Wire.beginTransmission(0x68); //MPU-val való komunikáció 
  Wire.write(0x3B); //Acc adatok itt keződnek
  Wire.endTransmission(false);
  Wire.requestFrom(0x68,14); // 14 bytot kérünk

  uint32_t t0 = micros();
  while (Wire.available() < 14) {  // vár a 14 byte-ra 
    if (micros() - t0 > 3000) {  // 3000 us = 3 ms timeout
    // I2C hiba / nincs adat
    Serial.println("MPU6050 timeout!");
    return;  // kilép a függvényből
  }
}


  //MPU 16 bites értékeit bele bitshiftelni egy 16 bites változóba mivel I2C rigsztereken csak 8 bites byte-okban kommunikál 
  // << bitshift 8 bittel feltoljuk a 16 bites változó 15-8 bitjébe
  // | (bitwise OR) összekapcsolja a byte.okat
  Acc_XLSB = Wire.read()<<8 | Wire.read();
  Acc_YLSB = Wire.read()<<8 | Wire.read();
  Acc_ZLSB = Wire.read()<<8 | Wire.read();
  Temp = Wire.read()<<8 | Wire.read();
  Gyro_X = Wire.read()<<8 | Wire.read();
  Gyro_Y = Wire.read()<<8 | Wire.read();
  Gyro_Z = Wire.read()<<8 | Wire.read();


  // °/s szögsebességek kiszámolás mivel +-500°/s LSB 65.5/°/s 
  // MPU 16 bites adat olvasás 2^16, +-500 = 1000 ------> 2^16/1000=65.536 ~ 65.5
  Rate_Roll=(float)Gyro_X/65.5;
  Rate_Pitch=(float)Gyro_Y/65.5;
  Rate_Yaw=(float)Gyro_Z/65.5;

  //Alakítsd át a mért értékeket fizikai (valós) egységekbe.
  //a gyorsulásmérő nyers LSB értékeiből csinálsz g-t vagy m/s²-t (ha m/s^2 akkor * 9.81)
  Acc_X = (float)Acc_XLSB / 4096 - 0.0495;
  Acc_Y = (float)Acc_YLSB / 4096 + 0.023; 
  Acc_Z = (float)Acc_ZLSB / 4096 - 0.093;

  //Szögek kiszámítása árkusz tangens fgv-el de  az Arduinoba Radiánt ad ezért meg kell szorzoni 180/pi vel pi=3.14159265359
  Szog_Roll = atan(Acc_Y / sqrt(Acc_X * Acc_X + Acc_Z * Acc_Z)) * (180.0/PI);
  Szog_Pitch = atan(-Acc_X / sqrt(Acc_Y * Acc_Y + Acc_Z * Acc_Z)) * (180.0/PI);

  /*Szog_Roll  = atan2(Acc_Y,  sqrt(Acc_X*Acc_X + Acc_Z*Acc_Z)) * (180.0/PI);
  Szog_Pitch = atan2(-Acc_X, sqrt(Acc_Y*Acc_Y + Acc_Z*Acc_Z)) * (180.0/PI);*/

}

void setup() {
  Serial.begin(57600);
  Wire.setClock(400000); //400kHz ClockSpeed MPU6050 adatlap írja I2C kommunikáció esetén
  Wire.begin(21, 22); //SDA, SCL
  delay(250);
  mpu_6050_regiszterek_beallitasa();
}

void loop() {
  gyro_acc_adat_olvasas();
  /*Serial.print("Acceleration X [g]= ");
  Serial.print(Acc_X);
  Serial.print(" Acceleration Y [g]= ");
  Serial.print(Acc_Y);
  Serial.print(" Acceleration Z [g]= ");
  Serial.println(Acc_Z);*/

  Serial.print("Roll szög [°]= ");
  Serial.print(Szog_Roll);
  Serial.print("\tPitch szög [°]= ");
  Serial.println(Szog_Pitch);

  delay(50);

}
