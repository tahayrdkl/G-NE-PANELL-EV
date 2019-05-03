  /*
**************************************************************************************************************************************
**                                                                                                                                  **
**                                                                                                                                  **
**      ____________   ________  ___    ___  ________             ___    ___  ________  _______     ___  __    ___                  **
**     |\_____   ____\|\   __  \|\  \  |\  \|\   __  \           |\  \  /  /||\   __  \|\   ___  \ |\  \|\  \ |\  \                 **
**     \|____|\  \___|\ \  \|\  \ \  \_\_\  \ \  \|\  \          \ \  \/  / |\ \  \|\  \ \  \__|\  \ \  \/  /|  \  \                **
**           \ \  \    \ \   __  \ \   ___   \ \   __  \          \ \    / /  \ \   _  _\ \  \  \\  | \   ___  \ \  \               **
**            \ \  \    \ \  \ \  \ \  \__|\  \ \  \ \  \   __     \ /  / /    \ \  \\  \\ \  \__/  /\ \  \\ \  \ \  \____          **
**             \ \__\    \ \__\ \__\ \__\ \ \__\ \__\ \__\ |\__\ __ /  / /      \ \__\\ _\\ \______/ |\ \__\\ \__\ \_______\        **
**              \|__|     \|__|\|__|\|__|  \|__|\|__|\|__| \|__||\____/ /        \|__|\|__|\|______|/  \|__| \|__|\|_______|        **
**                                                               \|___|/                                                            **
**                                                                                                                                  **
**                                                                                                                                  **
**                                                            TAHA YURDAKUL                                                         **
**                                                       tahayurdakul@icloud.com                                                    **
**                                                                                                                                  **
**                                                 GUNES PANELLI EV PROTOTIP YAZILIMI                                               **
**                                                                                                                                  **
**      Not : Bu kodlama gunes panelli ev sistemi icin yazilmistir. sistem icerisinde arduino nano, 5 adet 6V 1W gunes paneli,      **
**     i2c/iic 128x64 pixel 0,96 inch oled ekran, RGB led (sarj göstergesi icin), L7805 voltaj regulatoru, 750mAh Lİ-PO pil ve      **
**                                            3,7 volt Lİ-PO sarj devresi kullanilmistir.                                           **
**                         Bu projenin kodlamasını, kütphanelerini ve 3B ciktilari icin gereken stl dosyalarini                     **
**                               https://github.com/tahayrdkl/GUNES-PANELLI-EV adresinden bulabilirsiniz.                           **
**                                                                                                                                  **
**************************************************************************************************************************************
*/
//Ekran icin gerekli kutuphaneleri ekliyoruz
  #include <SPI.h>
  #include <Wire.h>
  #include <Adafruit_GFX.h>
  #include <Adafruit_SSD1306.h>
  Adafruit_SSD1306 display; // ekran rengi kutuphanesi

// RGB Led baglantilari
  #define besvolt         2 // 5volt destegi
  #define yesil           3 // RGB Led yesil
  #define mavi            4 // RGB Led mavi
  #define kirmizi         5 // RGB Led kirmizi
  
  

// deger giris baglantilari
  #define pil            A0 // pil volt okuma girisi
  #define gunes_paneli   A1 // gunes paneli volt okuma girisi

//*****************************************************************************************

int   gerilim  = 0; // pilden olculen voltaj degeri
int   gunes    = 0; // gunes panelinden olculen voltaj degeri
float yuzde    = 0; 
int   deger    = 0;
float fark     = 0;

//*****************************************************************************************

//Arduinonun voltlar karsiliginda aldigi degerler;

// pil;
   int maximum_gerilim = 823;//823
   int minimum_gerilim = 615;//615
/* 
 -pil;
  
    maximum volt - 4,02V = 823 
    minimum volt - 3,00V = 615
*/

//*****************************************************************************************

// gunes_paneli
   int gunes_paneli_esik_degeri = 614 ;//819 yazılabilir .614 =3 volt değeri0 için
/*
 -Gunes paneli;

    gunes paneli maximumdegeri - 5,00V = 1024 
    günes paneli esik degeri   - 3,00V =  614
*/

void setup() 
{
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  Wire.begin();


// RGB Led Cikislari;
    pinMode(kirmizi      ,OUTPUT); // RGB Led Kirmizi
    pinMode(yesil        ,OUTPUT); // RGB Led Yesil
    pinMode(mavi         ,OUTPUT); // RGB Led Mavi
    pinMode(besvolt      ,OUTPUT); // RGB Led icin +5V cikisi


digitalWrite(besvolt,HIGH ); // RGB Led icin +5V cikisi

//*****************************************************************************************

// Olcum Girisleri;
    pinMode(pil          , INPUT); // Pil Olcum Girisi
    pinMode(gunes_paneli , INPUT); // Günes Paneli Olcum Girisi

Serial.begin(9600);

}

void loop() 
{

// PIL DURUMU ***************************************************************************
  
  gerilim = analogRead(pil);

  if(gerilim >= maximum_gerilim)
   gerilim  = maximum_gerilim ;


  if(gerilim <= minimum_gerilim)
   gerilim  = minimum_gerilim ;  

  
  yuzde = gerilim - minimum_gerilim ;   
  fark = maximum_gerilim - minimum_gerilim ;
  
  deger = (200.00/fark) * yuzde ;

//serial monitörde hata ayiklama
   Serial.print  ("pil% ="); //serial ekranda bilgi kontrolu
   Serial.println(deger/2 ); //serial ekranda bilgi kontrolu

 
 if(deger >= 20)
 
 {
//yesil ledi yakiyoruz
  digitalWrite(yesil  , LOW  );
  digitalWrite(kirmizi, HIGH );
  digitalWrite(mavi   , HIGH );
  
  
//serial monitörde hata ayiklama
   Serial.println("yeterli pil seviyesi"); //serial monitörde bilgi kontrolu
   Serial.println("yesil isik yaniyor"  ); //serial monitörde bilgi kontrolu
   
// degerleri oled ekrana yazdirma  

  display.setTextColor(WHITE)        ; // rengi belirliyoruz (BEYAZ)
  display.setTextSize (2    )        ; // yazı boyutunu ayarlama     
  display.setCursor   (0,0)          ; // yazının baslayacagi pixel noktalari
  display.print       ("Pil= % ")    ;
  display.setTextSize (2)            ;
  display.print       (deger/2)      ;
  display.setCursor   (32,15)        ;
  display.setTextSize (1)            ;
  display.println     ("Yeterli Pil");
  display.setCursor   (40,24)        ;
  display.setTextSize (1)            ;
  display.println     ("Seviyesi")   ;
  display.display     ()             ;
  display.clearDisplay()             ;
   
 }
 
 else  

 {
//kirmizi ledi yakiyoruz
  digitalWrite(kirmizi, LOW  );
  digitalWrite(yesil  , HIGH );
  digitalWrite(mavi   , HIGH );
  
  
//serial monitörde hata ayiklama
   Serial.println("yetersiz pil seviyesi"); //serial monitörde bilgi kontrolu
   Serial.println("kirmizi isik yaniyor" ); //serial monitörde bilgi kontrolu

// Oled ekrana bilgi yazdirma   
   
   display.setTextColor(WHITE)         ;
   display.setTextSize (2)             ;
   display.setCursor   (0,0)           ;
   display.print       ("Pil= % ")     ;
   display.setTextSize (2)             ;
   display.print       (deger/2)       ;
   display.setCursor   (27,15)         ;
   display.setTextSize (1)             ;
   display.println     ("Yetersiz Pil");
   display.setCursor   (40,24)         ;
   display.setTextSize (1)             ;
   display.println     ("Seviyesi")    ;
   display.display     ()              ;
   display.clearDisplay()              ;
   
 }
   
   delay(2000);

// GUNES PANELI DURUMU ******************************************************************
  
  gunes = analogRead(gunes_paneli);
  
  if(gunes >= gunes_paneli_esik_degeri)
{
   
   Serial.println("GUNES VAR");         //serial monitörde bilgi kontrolu
   Serial.println("Pil sarj ediliyor"); //serial monitörde bilgi kontrolu
   
// Oled ekrana bilgi yazdirma

   display.setTextColor(WHITE);
   display.setTextSize (2)                   ;
   display.setCursor   (0,0)                 ;
   display.print       (" GUNES VAR")        ;
   display.setCursor   (8,22)                ;
   display.setTextSize (1)                   ;
   display.println     (" Pil Sarj Ediliyor");
   display.display     ()                    ;
   display.clearDisplay()                    ;

}
  
  
  else
{
   
   Serial.println("GUNES YOK           "); //serial monitörde bilgi kontrolu
   Serial.println("Pil sarj edilemiyor "); //serial monitörde bilgi kontrolu


// Oled ekrana bilgi yazdirma 
   
   display.setTextColor(WHITE)                 ;
   display.setTextSize (2)                     ;
   display.setCursor   (0,0)                   ;
   display.print       (" GUNES YOK")          ;
   display.setCursor   (8,22)                  ;
   display.setTextSize (1)                     ;
   display.println     ("Pil Sarj edilemiyor ");
   display.display     ()                      ;
   display.clearDisplay()                      ;

}
Serial.println("*****************************************************************************************"); //serial monitörde bilgi kontrolu
delay(2000);

}
