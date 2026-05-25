
#include <Wire.h>
#include <RTClib.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <avr/pgmspace.h> 


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 telaAD(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
RTC_DS3231 relogioAD;
char diasDaSemana[7][4] = {"Dom", "Seg", "Ter", "Qua", "Qui", "Sex", "Sab"};


#define BTN_MAIS 2
#define BTN_MENOS 3

#define DHTPIN 2
#define DHTTYPE DHT22 
DHT sensorTemp(DHTPIN, DHTTYPE);

const unsigned char logoEspada[] PROGMEM = {
  0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 
  0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 
  0x01, 0x80, 0x03, 0xc0, 0x07, 0xe0, 0x1f, 0xf8, 
  0x1f, 0xf8, 0x03, 0xc0, 0x03, 0xc0, 0x03, 0xc0
};

void setup() {
  Serial.begin(9600);

  pinMode(DHTPIN, INPUT);
  pinMode(BTN_MAIS, INPUT_PULLUP);
  pinMode(BTN_MENOS, INPUT_PULLUP);

  sensorTemp.begin();
  telaAD.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  relogioAD.begin();

  relogioAD.adjust(DateTime(2026, 5, 23, 10, 0, 0));
  
}

void loop() {
  
  DateTime agora = relogioAD.now();
  float temperatura = sensorTemp.readTemperature();
  int umidade = sensorTemp.readHumidity();


  telaAD.clearDisplay();
  telaAD.setTextColor(WHITE);
  

  telaAD.drawBitmap(0, 0, logoEspada, 16, 16, 1);

  
  telaAD.setTextSize(1);
  telaAD.setCursor(25, 3);
  telaAD.print("IOT CLOCK"); 

  
  telaAD.setTextSize(2);
  telaAD.setCursor(15, 18);
  char bufferHora[10];
  sprintf(bufferHora, "%02d:%02d:%02d", agora.hour(), agora.minute(), agora.second());
  telaAD.print(bufferHora);

  
  telaAD.setTextSize(1);
  telaAD.setCursor(15, 39);
  char bufferData[15];
  sprintf(bufferData, "%s %02d/%02d/%04d", diasDaSemana[agora.dayOfTheWeek()], agora.day(), agora.month(), agora.year());
  telaAD.print(bufferData);


  telaAD.setCursor(16, 53);
  telaAD.print("T:");
  if (isnan(temperatura)) {
    telaAD.print("--"); 
  } else {
    telaAD.print(temperatura, 1);
  }
  telaAD.print("C  U:");
  
  if (isnan(umidade)) {
    telaAD.print("--");
  } else {
    telaAD.print(umidade, 1);
  }
  telaAD.print("%");

  // Envia todas as informações desenhadas para a tela de fato
  telaAD.display();
  delay(100);
}
