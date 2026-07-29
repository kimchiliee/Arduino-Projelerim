#define BUTON_ILERI 10
#define BUTON_GERI 11
#define BUTON_RESET 12

int sayac = 0;


void convert(int sayi) {
  for (int pin = 2; pin <= 9; pin++) {
    digitalWrite(pin, !bitRead(sayi, pin - 2)); 
  }

  Serial.print("Sayi = ");
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
  pinMode(BUTON_RESET, INPUT_PULLUP);

  Serial.begin(9600); 
  Serial.println("--- 8-Bit Binary Sayici Baslatildi ---");
}

void loop() {
  
  if(digitalRead(BUTON_ILERI) == LOW && sayac < 255) {
    sayac++;
    convert(sayac);
  }

  if(digitalRead(BUTON_GERI) == LOW && sayac > 0) {
    sayac--;
    convert(sayac);
  }

  if(digitalRead(BUTON_RESET) == LOW) {
    sayac = 0;
    convert(sayac);
  }
}
