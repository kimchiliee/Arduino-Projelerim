#define BUTON_ILERI 10
#define BUTON_GERI 11
#define BUTON_ENTER 12

int sayac = 0;
String yazi = "";

void convert(int sayi) {
  for (int pin = 2; pin <= 9; pin++) {
    bool bitDurumu = bitRead(sayi, pin - 2); 
    
    digitalWrite(pin, !bitDurumu);
  }

  Serial.print("Tekrar Sayi = ");
  Serial.println(sayi);
  delay(300);
}

void setup() {
  for(byte led_pin = 2; led_pin <= 9; led_pin++) {
    pinMode(led_pin, OUTPUT);
    digitalWrite(led_pin, HIGH); 
  }

  
  pinMode(BUTON_ILERI, INPUT_PULLUP);
  pinMode(BUTON_GERI, INPUT_PULLUP);
  pinMode(BUTON_ENTER, INPUT_PULLUP);

  Serial.begin(9600); 
  Serial.println("--- Binary Sayici ve Metin Tekrarlayici ---");
  Serial.println("Lutfen yazmak istediginiz kelimeyi girin ve Enter'a basin.");
}

void loop() {
  
 
  if(Serial.available()) {
    yazi = ""; 
    while (Serial.available()) {
      char okunan = Serial.read();
      yazi += okunan; 
      delay(5);
    }
    
    yazi.trim(); 
    Serial.print("Kaydedilen Metin: ");
    Serial.println(yazi);
  }

 
  if(digitalRead(BUTON_ILERI) == LOW && sayac < 255) {
    sayac++;
    convert(sayac);
  }


  if(digitalRead(BUTON_GERI) == LOW && sayac > 0) {
    sayac--;
    convert(sayac);
  }


  if(digitalRead(BUTON_ENTER) == LOW) {
    if(yazi == "") {
      Serial.println("HATA: Once bir metin girmelisiniz!");
    } else {
      Serial.println("--- Yazdirma Basliyor ---");
      for(int i = 0; i < sayac; i++) {
        Serial.print(i + 1);
        Serial.print(". ");
        Serial.println(yazi);
      }
    }

    sayac = 0;
    convert(sayac);
    delay(300);
  }
}
