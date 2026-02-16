float feszultseg;
#define feszultseg_meres_PIN 36;
#define CAL 1.115 // kalibráció eltéréshez 

void aksi_fesz(void) { // (void) Ez a paraméterlista. Nem vár bemenő paramétert
  feszultseg=(float)analogRead(feszultseg_meres_PIN)* 3.3 * (2000+510) / (4095 * 510) * CAL; // 4095 / (3.3 x 5) = 248.18182 (float) Itt történik egy típuskonverzió (cast)
}                                                         // Vin = adc × 3.3 × (R1+R2) / (4095 × R2)

void setup() {
  // put your setup code here, to run once:

analogSetAttenuation(ADC_11db);   
// ESP32 ADC bemeneti csillapítás beállítása dB = decibel Ez a belső csillapítás mértéke.
// 0–3.3V mérési tartomány engedélyezése (akku osztóról érkező feszültség miatt)
// Nélküle az ADC kb. 1.1V fölött torzítana
// Ez hardver szinten történik a chipen belül.

analogReadResolution(12);        
// ADC felbontás beállítása 12 bitre
// 0–4095 közötti mérési érték (nagyobb felbontás = pontosabb lépcsőzés)

Serial.begin(57600);
pinMode(feszultseg_meres_PIN,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  aksi_fesz();
  Serial.print(feszultseg);
  Serial.println("V");
  delay(50);
}

