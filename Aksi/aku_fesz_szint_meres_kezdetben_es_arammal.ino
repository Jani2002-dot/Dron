#define feszultseg_meres_PIN 36
#define aram_meres_PIN 34
#define Piros_LED 4
float Feszultseg;
float Aram;
float Aksi_maradek;               // %-ban 
float Aksi_kezdetben;             // mAh-ban
float Elfogyasztott_kapacitas=0;  // mAh-ban 
float Alap_aksi_kapacitas=1300;   // 1300mAh

void aksi_feszultseg(void){
  Feszultseg=(float)analogRead(feszultseg_meres_PIN)/248.1818 //4095/(3.3*5)=248.1818
  Aram=(float)analogRead(aram_meres_PIN)*0,022385 // ADC / ((4095/3.3V)*0.036 V/A) ---> ADC*0,022385
}

void setup() {
  pinMode(feszultseg_meres_PIN, INPUT);
  pinMode(aram_meres_PIN, INPUT);
  pinMode(Piros_LED, OUTPUT);

  digitalWrite(Piros_LED, HIGH);

  aksi_feszultseg();
  if (Feszultseg > 8.3) {         // terhelés nélkűl mérjük az aksit így valós értéket kapunk mivel terhelés alatt beesik a feszülség szintje és nem lesz valós érték ezért terhlés alatt áramot mmérünk
    digitalWrite(Piros_LED, LOW); //piros led kikapcs
    Aksi_kezdetben = Alap_aksi_kapacitas;//felöltve tehát 1300mAh 
  }
  else if (Feszultseg < 7.5){
    Aksi_kezdetben=30/100*Alap_aksi_kapacitas; //30% át vesszük
  }
  else {
    digitalWrite(Piros_LED, LOW);
    Aksi_kezdetben = (82 * Feszultseg - 580)/100 * Alap_aksi_kapacitas; 
  }

}

void loop() {
  aksi_feszultseg();
  Elfogyasztott_kapacitas = Aram * 1000 * 0.004 / 3600 + Elfogyasztott_kapacitas;
  Aksi_maradek = (Aksi_kezdetben - Elfogyasztott_kapacitas) / Alap_aksi_kapacitas * 100;
  if (Aksi_maradek <= 30) digitalWrite(Piros_LED, HIGH);
  else digitalWrite(Piros_LED, LOW);
}
