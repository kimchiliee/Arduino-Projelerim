#include <LiquidCrystal.h>

// LCD Keypad Shield pinleri
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// Özel Karakterler (Pac-Man ve yeni eklenen Canavar)
byte kapali_agiz[8] = { B01110, B11111, B11111, B11111, B11111, B11111, B01110, B00000 };
byte acik_agiz[8]   = { B01110, B11101, B11000, B11100, B11000, B11111, B01110, B00000 };
byte canavar[8]     = { B01110, B11111, B10101, B11111, B11111, B11111, B10101, B00000 }; 

#define SAG   0
#define UST   1
#define ALT   2
#define SOL   3
#define YOK   4

// Pac-Man Değişkenleri
int px = 0;               
int skor = 0;             
bool agiz_durumu = false; 
char alt_satir[16];       

// Oyun ve Canavar Değişkenleri
int seviye = 1;
int gx = 15;        // Canavar en sağdan başlar
int g_yon = -1;     // Canavarın yönü (-1 sola, 1 sağa)

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

// Yeni seviyeye geçerken sahneyi yenileyen fonksiyon
void sahneyi_hazirla() {
  for(int x = 0; x < 16; x++) {
    alt_satir[x] = '*'; 
  }
  alt_satir[px] = ' '; // Pac-Man'in altını temizle
  gx = 15;             // Canavarı tekrar sağa at
}

void setup() {
  lcd.begin(16, 2);
  
  lcd.createChar(0, kapali_agiz);
  lcd.createChar(1, acik_agiz);
  lcd.createChar(2, canavar); // Canavar karakteri LCD'ye yüklendi
  
  sahneyi_hazirla();
}

void loop() {
  int tus = tus_oku();
  
  // 1. PAC-MAN HAREKETİ (Senin harika millis() zamanlaman korundu)
  if(millis() - sonHareket > 200) { 
    if(tus == SOL) { 
      px--; 
      if(px < 0) px = 0; 
    }
    else if(tus == SAG) { 
      px++; 
      if(px > 15) px = 15; 
    }
    
    // Yıldız yeme
    if(alt_satir[px] == '*') {
      skor++;               
      alt_satir[px] = ' ';  
    }
    
    agiz_durumu = !agiz_durumu; 
    sonHareket = millis();
  }

  // 2. CANAVAR HAREKETİ (Seviye arttıkça bekleme süresi düşer, hızlanır)
  int canavarHizi = 400 - (seviye * 40); 
  if (canavarHizi < 100) canavarHizi = 100; // Maksimum hız sınırı

  if(millis() - sonCanavarHareket > canavarHizi) {
    gx += g_yon; // Canavarı ilerlet
    
    // Sınır çarpma kontrolleri (Duvara çarpınca geri döner)
    if(gx <= 0) { gx = 0; g_yon = 1; }
    else if(gx >= 15) { gx = 15; g_yon = -1; }
    
    sonCanavarHareket = millis();
  }

  // 3. ÇARPIŞMA KONTROLÜ (Oyun Bitti)
  if(px == gx) {
    lcd.clear();
    lcd.setCursor(3, 0); lcd.print("OYUN BITTI");
    lcd.setCursor(2, 1); lcd.print("Skor: "); lcd.print(skor);
    while(1); // Cihaz resetlenene kadar bekle
  }

  // 4. SEVİYE ATLEMA KONTROLÜ
  bool yildizKaldiMi = false;
  for(int x = 0; x < 16; x++) {
    if(alt_satir[x] == '*') yildizKaldiMi = true;
  }

  // Hiç yıldız kalmadıysa seviye atlat
  if(!yildizKaldiMi) {
    seviye++;
    px = 0; // Pac-Man'i başa al
    sahneyi_hazirla();
    
    lcd.clear();
    lcd.setCursor(4, 0); lcd.print("SEVIYE "); lcd.print(seviye);
    delay(1500); // Sadece level arası geçiş ekranında ufak bir bekleme
    sonHareket = millis(); // Zamanlayıcıyı sıfırla
  }

  // 5. EKRANI ÇİZ
  lcd.setCursor(0, 0);
  lcd.print("Skor:");
  lcd.print(skor);
  lcd.print(" Lvl:"); 
  lcd.print(seviye);
  lcd.print("   "); // Eski yazıları silmek için boşluk

  lcd.setCursor(0, 1);
  for(int x = 0; x < 16; x++) {
    if(x == px) {
      lcd.write(agiz_durumu ? byte(1) : byte(0)); // Pac-Man
    } else if (x == gx) {
      lcd.write(byte(2)); // Canavarı çiz
    } else {
      lcd.print(alt_satir[x]); // Yıldız veya Boşluk
    }
  }
}
