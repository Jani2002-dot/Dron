#include <IBusBM.h>
IBusBM ibus;

//ibus.readChannel(0-9) 10 csatorna
#define RX_PIN 16
#define TX_PIN -1

//static const uint8_t Csatorna_Szam = 8;
//static azt jelenti, hogy ez a változó “fájl-szintű / globális” élettartamú, vagyis a program teljes futása alatt létezik, és ugyanaz az egy példánya van.
//csak ebben a .cpp fájlban látható (internal linkage). Tehát ha több fájlod van, másik .cpp-ből nem tudod elérni név szerint.
uint16_t Vevo_Ertekek[]={0,0,0,0,0,0,0,0};

void Csatorna_olvasas() {
    for (uint8_t i = 0; i < 8; i++) {
    Vevo_Ertekek[i] = ibus.readChannel(i);
  }
}

void setup() {

  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN);
  ibus.begin(Serial2);
}

void loop() {

  static uint8_t elozo_cnt = 0;
  if (ibus.cnt_rec != elozo_cnt) { //Csak akkor olvass/írj, ha érkezett új iBUS frame
    elozo_cnt = ibus.cnt_rec;
    
    // olvasás + print
    Csatorna_olvasas();

    Serial.print("Roll [µs]: ");
    Serial.print(Vevo_Ertekek[0]);
    Serial.print(" Pitch [µs]: ");
    Serial.print(Vevo_Ertekek[1]);
    Serial.print(" Throttle [µs]: ");
    Serial.print(Vevo_Ertekek[2]);
    Serial.print(" Yaw [µs]: ");
    Serial.println(Vevo_Ertekek[3]);
  }

  delay(15);  
}
