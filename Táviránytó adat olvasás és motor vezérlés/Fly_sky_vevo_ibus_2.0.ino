#include <IBusBM.h>
IBusBM ibus;

//ibus.readChannel(0-9) 10 csatorna
#define RX_PIN 16
uint16_t Vevo_Ertekek[8]={0};

uint16_t Csatorna_olvasas(uint8_t Csatorna_bement) {
  return ibus.readChannel(Csatorna_bement);
}

void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
Serial2.begin(115200, SERIAL_8N1, RX_PIN, -1);
ibus.begin(Serial2);
}

void loop() {
  // put your main code here, to run repeatedly:

  static uint8_t last_cnt = 0;
  if (ibus.cnt_rec != last_cnt) { //Csak akkor olvass/írj, ha érkezett új iBUS frame
    last_cnt = ibus.cnt_rec;
  // olvasás + print


  for (uint8_t i = 0; i < 8; i++) {
    Vevo_Ertekek[i] = Csatorna_olvasas(i);
  }

  Serial.print(" Roll [µs]: ");
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
