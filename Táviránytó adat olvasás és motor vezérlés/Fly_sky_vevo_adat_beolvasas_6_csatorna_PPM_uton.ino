#include <PulsePosition.h> //PPM  könyvtár
PulsePositionInput VevoBemenet(RISING); //obejktum létrehozása RISING azt jelenti: az impulzusokat felmenő élre (0→1) időzíti / szinkronizálja

#define PPM_PIN 14
float Vevo_Ertek[]={0, 0, 0, 0, 0, 0, 0, 0}; //0-tól keződik a tömb tehát első helyen a [0] van
//float Vevo_Ertek[8]={0}; ugyan az mitn felette
int Csatorna_szam=0; 

void vevo_olvasas(void){                      //fgv a vevő olvasásához
//ReceiverInput.available() Megmondja, hány csatornaérték érhető el az utolsó teljes PPM frame-ből. 
//Ha 0, akkor épp nem érkezett új frame (vagy nincs jel).
  Csatorna_szam = VevoBemenet.available();  //aktív csatornák kiolvasása 
  if (Csatorna_szam > 0) {
    for (int i=1; i<=Csatorna_szam && i <= 8 ; i++){ //&& i <= 8 Tömb túlindexelés elleni védelem pl egyszer 9 csatornát olvasna i elmenne 9 ig és 
                                                     //Vevo_Ertek[9-1] lenne ami Vevo_Ertek[8] de ilyen nem létezik mert 0-7 ig indexeli a 8 elemes tömböt
      Vevo_Ertek[i-1]=VevoBemenet.read(i); //[i-1] ezért kell mert tömb (Arduino/C) 0-tól indexel: Vevo_Ertek[0] == 1. csatorna=Roll 
      }                                    //VevoBemenet.read(i); itt meg i kell mert pl i=1 1.csatronaát olvassa
    } 
  }

void setup() {
  // put your setup code here, to run once:
Serial.begin(57600);
VevoBemenet.begin(PPM_PIN); //elindítja a PPM dekódolást a megadott lábon.
} 

void loop() {
  // put your main code here, to run repeatedly:

//PPM jel beolvasása és kiíratása
vevo_olvasas();
Serial.print("Csatornák száma: ");
Serial.print(Csatorna_szam);
Serial.print(" Roll [µs]: ");
Serial.print(Vevo_Ertek[0]);
Serial.print(" Pitch [µs]: ");
Serial.print(Vevo_Ertek[1]);
Serial.print(" Throttle [µs]: ");
Serial.print(Vevo_Ertek[2]);
Serial.print(" Yaw [µs]: ");
Serial.println(Vevo_Ertek[3]);
delay(50);
}
