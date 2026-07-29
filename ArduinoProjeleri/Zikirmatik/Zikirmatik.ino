#define BUTON_ARTIR 10
#define BUTON_AZALT 11
#define BUTON_SIFIRLA 12

int sayac = 0;
bool flag = LOW;

void setup() 
{
  Serial.begin(9600);

pinMode(BUTON_ARTIR, INPUT_PULLUP);
  pinMode(BUTON_AZALT, INPUT_PULLUP);
  pinMode(BUTON_SIFIRLA, INPUT_PULLUP);

  Serial.println("--- Dijital Zikirmatik Baslatildi ---");
}

void loop() 
{
 if (digitalRead(BUTON_SIFIRLA) == LOW || digitalRead(BUTON_AZALT) == LOW || digitalRead(BUTON_ARTIR) == LOW) 
  {
   if (digitalRead(BUTON_ARTIR) == LOW) { 
        sayac++; 
    }
    
    if (digitalRead(BUTON_AZALT) == LOW && sayac > 0) { 
        sayac--; 
    }
    
    if (digitalRead(BUTON_SIFIRLA) == LOW) { 
        sayac = 0; 
        flag = HIGH; 
    }

    if (sayac != 0 || flag == HIGH) 
    {
      Serial.print("Kazandigin Sevap Sayisi >> ");
      Serial.println(sayac);
      flag = LOW;
    }

     delay(400);

    if (sayac == 33) 
    {
      Serial.println("Tebrikler Cennetliksiniz.");
    }
  }
}
