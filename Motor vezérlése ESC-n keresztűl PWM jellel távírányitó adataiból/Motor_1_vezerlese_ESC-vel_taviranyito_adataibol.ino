#include <IBusBM.h>
IBusBM ibus;

//ibus.readChannel(0-9) 10 csatorna
#define RX_PIN 16
#define TX_PIN -1

//#define Motorok_ESC_PWM_felbontas  12  //12    //12 bit ---> 0-4095
#define Motorok_ESC_PWM_felbontas  16  //16    //16 bit ---> 0-65 535
//#define Motorok_ESC_PWM_frekvencia 250 //250  //250Hz=4000us
#define Motorok_ESC_PWM_frekvencia 50 //50   //50Hz=20 000us
#define Motor_1_ESC_Pin 17
#define Motor_1_ESC_Csatorna 0


//static azt jelenti, hogy ez a változó “fájl-szintű / globális” élettartamú, vagyis a program teljes futása alatt létezik, és ugyanaz az egy példánya van.
//csak ebben a .cpp fájlban látható (internal linkage). Tehát ha több fájlod van, másik .cpp-ből nem tudod elérni név szerint.
uint16_t Vevo_Ertekek[]={0,0,0,0,0,0,0,0};
float Gaz_bemenet; //0% gaz=1000 us 100%gaz=2000us

void Csatorna_olvasas() {
    for (uint8_t i = 0; i < 8; i++) {
    Vevo_Ertekek[i] = ibus.readChannel(i);
  }
}

void setup() {

  Serial.begin(115200); //sorosport
  Serial2.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN); //soros vevőnek
  ibus.begin(Serial2); //ibus inicializálása

  //ledcAttachChannel(Motor_1_ESC_Pin, Motorok_ESC_PWM_frekvencia, Motorok_ESC_PWM_felbontas, Motor_1_ESC_Csatorna); //ESP32 core 3.3.6
  ledcSetup(Motor_1_ESC_Csatorna, Motorok_ESC_PWM_frekvencia, Motorok_ESC_PWM_felbontas); //ESP32 core 2.0.17
  ledcAttachPin(Motor_1_ESC_Pin, Motor_1_ESC_Csatorna); //ESP32 core 2.0.17

// 250Hz=4ms=4000us
// 12 bites felbontás = 0-4095 
// 0 ---> 0us        4095--->4000us
// PWM jel küldése us-ban az ESC nek akkor 4095/4000=1.024-el kell majd szorozi 250Hz esetén
// PWM jel küldése us-ban az ESC nek akkor 65 535/20 000=3.27675-el kell majd szorozi 50 Hz esetén
  delay(250);

// kontrollálatlan motorindítás elkerülése 
  while (Vevo_Ertekek[2] < 1020 || Vevo_Ertekek[2] > 1050){  //Vevo_Ertekek[2] ---> Gáz kar értéke  kontrollálatlan motorindítás elkerülése 
    Csatorna_olvasas();
    delay(4);
  }
}

void loop() {

  static uint8_t elozo_cnt = 0;
  if (ibus.cnt_rec != elozo_cnt) { //Csak akkor olvass/írj, ha érkezett új iBUS frame
    elozo_cnt = ibus.cnt_rec;
    
    // olvasás + print
    Csatorna_olvasas();
    Gaz_bemenet = Vevo_Ertekek[2];
    //ledcWriteChannel(Motor_1_ESC_Csatorna, Gaz_bemenet * 1.024); Core 3.3 //ESP32 core 3.3.6
    ledcWrite(Motor_1_ESC_Csatorna, Gaz_bemenet * 3.27675); //ESP32 core 2.0.17
    Serial.print(" Throttle [µs]: ");
    Serial.println(Vevo_Ertekek[2]);
  
  }

  delay(15);  
}
