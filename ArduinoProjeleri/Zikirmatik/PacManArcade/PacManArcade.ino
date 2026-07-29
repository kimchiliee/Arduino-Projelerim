#include <LiquidCrystal.h>

// LCD Keypad pinler
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// Özel Karakterler (canavar)
byte kapali_agiz[8] = { B01110, B11111, B11111, B11111, B11111, B11111, B01110, B00000 };
byte acik_agiz[8]   = { B01110, B11101, B11000, B11100, B11000, B11111, B01110, B00000 };
byte canavar[8]     = { B01110, B11111, B10101, B11111, B11111, B11111, B10101, B00000 }; 

#define SAG   0
#define UST   1
#define ALT   2
#define SOL   3
#define YOK   4

int px = 0;               
int skor = 0;             
bool agiz_durumu = false; 
char alt_satir[16];       


int seviye = 1;
int gx = 15;      
int g_yon = -1;     

unsigned long sonHareket = 0;
unsigned long sonCanavarHareket = 0;

int tus_oku() {
  int deger = analogRead(0);
  if (deger > 1000) return YOK;
  if (deger < 50)   return SAG;
  if (deger < 250)  return UST; 
  if (deger < 450)  return ALT;
  if (deger < 650)  return SOL;
  return YOK;
}

void sahneyi_hazirla() {
  for(int x = 0; x < 16; x++) {
    alt_satir[x] = '*'; 
  }
  alt_satir[px] = ' ';
  gx = 15;            

void setup() {
  lcd.begin(16, 2);
  
  lcd.createChar(0, kapali_agiz);
  lcd.createChar(1, acik_agiz);
  lcd.createChar(2, canavar); 
  
  sahneyi_hazirla();
}

void loop() {
  int tus = tus_oku();
  
  if(millis() - sonHareket > 200) { 
    if(tus == SOL) { 
      px--; 
      if(px < 0) px = 0; 
    }
    else if(tus == SAG) { 
      px++; 
      if(px > 15) px = 15; 
    }
    
    if(alt_satir[px] == '*') {
      skor++;               
      alt_satir[px] = ' ';  
    }
    
    agiz_durumu = !agiz_durumu; 
    sonHareket = millis();
  }

  int canavarHizi = 400 - (seviye * 40); 
  if (canavarHizi < 100) canavarHizi = 100; 
  if(millis() - sonCanavarHareket > canavarHizi) {
    gx += g_yon; 
    
     if(gx <= 0) { gx = 0; g_yon = 1; }
    else if(gx >= 15) { gx = 15; g_yon = -1; }
    
    sonCanavarHareket = millis();
  }

  if(px == gx) {
    lcd.clear();
    lcd.setCursor(3, 0); lcd.print("OYUN BITTI");
    lcd.setCursor(2, 1); lcd.print("Skor: "); lcd.print(skor);
    while(1); // Cihaz resetlenene kadar bekle
  }

  bool yildizKaldiMi = false;
  for(int x = 0; x < 16; x++) {
    if(alt_satir[x] == '*') yildizKaldiMi = true;
  }

  if(!yildizKaldiMi) {
    seviye++;
    px = 0;
    sahneyi_hazirla();
    
    lcd.clear();
    lcd.setCursor(4, 0); lcd.print("SEVIYE "); lcd.print(seviye);
    delay(1500); 
    sonHareket = millis(); 
  }

  lcd.setCursor(0, 0);
  lcd.print("Skor:");
  lcd.print(skor);
  lcd.print(" Lvl:"); 
  lcd.print(seviye);
  lcd.print("   "); 

  lcd.setCursor(0, 1);
  for(int x = 0; x < 16; x++) {
    if(x == px) {
      lcd.write(agiz_durumu ? byte(1) : byte(0));
    } else if (x == gx) {
      lcd.write(byte(2)); 
    } else {
      lcd.print(alt_satir[x]); 
    }
  }
}
