const int ledPin = 0;
const int czujnik = 2;
const int przyciskDol = 3;
const int przyciskGora = 4;

bool poprzedniStan = HIGH;
bool czyPracuje = false;
float faza = 0.5;

unsigned long czasWcisniecia = 0;
unsigned long ostatniaZmiana = 0;
unsigned long ostatniKrokAnimacji = 0;

bool oczekiwanie = false;

float maxJasnosc = 125;
const float krokJasnosci = 3;
const int odstepZmiany = 30;
const int odstepAnimacji = 1;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(czujnik, INPUT_PULLUP);
  pinMode(przyciskDol, INPUT_PULLUP);
  pinMode(przyciskGora, INPUT_PULLUP);
}

void loop() {
  bool stan = digitalRead(czujnik);
  bool dol = digitalRead(przyciskDol) == LOW;
  bool gora = digitalRead(przyciskGora) == LOW;


  if (millis() - ostatniaZmiana >= odstepZmiany) {
    if (dol) {
      maxJasnosc -= krokJasnosci;
      if (maxJasnosc < 10) maxJasnosc = 10;
      ostatniaZmiana = millis();
    }
    if (gora) {
      maxJasnosc += krokJasnosci;
      if (maxJasnosc > 255) maxJasnosc = 255;
      ostatniaZmiana = millis();
    }
  }


  if (stan == LOW && poprzedniStan == HIGH && !czyPracuje) {
    oczekiwanie = true;
    czasWcisniecia = millis();
  }


  if (stan == HIGH && poprzedniStan == LOW) {
    oczekiwanie = false;
  }


  if (oczekiwanie && millis() - czasWcisniecia >= 150) {
    oczekiwanie = false;
    czyPracuje = true;
    faza = 0.5;
  }


  if (czyPracuje && millis() - ostatniKrokAnimacji >= odstepAnimacji) {
    ostatniKrokAnimacji = millis();
    faza += 0.00055;

    float sine = sin(faza * PI) * 0.5 + 0.5;
    int pwm = (int)(pow(sine, 2.2) * (float)maxJasnosc);
    analogWrite(ledPin, pwm);

    if (faza >= 2.5) {
      faza = 0.5;
      if (stan == HIGH) {
        czyPracuje = false;
        analogWrite(ledPin, maxJasnosc);
      }
    }
  } else if (!czyPracuje) {
    analogWrite(ledPin, maxJasnosc);
  }

  poprzedniStan = stan;
}