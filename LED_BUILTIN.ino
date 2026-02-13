/*Mielőtt repülni tudnál, a mikrokontrollernek el kell indítania a kiegészítő érzékelőket és kalibrálnia kell őket.
Ez körülbelül négy másodpercet vesz igénybe, amely alatt a quadcopter még nem tud elindulni.
Ez idő alatt bekapcsolja a piros LED-et, hogy jelezze, hogy a quadcopter még mindig a beállítási folyamatban van.
Amikor a beállítási folyamat sikeresen befejeződött, kikapcsolja a piros LED-et, és bekapcsolja a zöld LED-et.*/

int piros_LED=4;
int zold_LED=13;
void setup() {
  // put your setup code here, to run once:
pinMode(LED_BUILTIN,OUTPUT);
pinMode(piros_LED,OUTPUT);
pinMode(zold_LED,OUTPUT);

digitalWrite(LED_BUILTIN,HIGH);
digitalWrite(piros_LED,HIGH);
delay(4000);
digitalWrite(piros_LED,LOW);
digitalWrite(zold_LED,HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
}
