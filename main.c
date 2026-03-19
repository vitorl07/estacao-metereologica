/**
 * Estacao Metereologica 
 * Autor: Vitor Bruno
 */

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT11.h>
#include <Adafruit_BMP280.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_ADRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

DHT11 dht11(2); 


#define BMP_SCK  13
#define BMP_MISO 12
#define BMP_MOSI 11
#define BMP_CS   10
Adafruit_BMP280 bmp(BMP_CS, BMP_MOSI, BMP_MISO, BMP_SCK);


const int pinoBotao = 3;
int pagina = 1;
bool ultimoEstadoBotao = HIGH;

bool bmpOnline = false;

void setup() {
  
  Serial.begin(9600);
  pinMode(pinoBotao, INPUT_PULLUP);

  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADRESS);
  
  // Tenta inicializar o BMP280 até 5 vezes
  for(int i = 0; i < 5; i++) {
    if (bmp.begin()) {
      bmpOnline = true;
      break;
    }
    delay(200);
  }

  display.clearDisplay();
  display.setTextColor(WHITE);
  
  // Splash Screen
  display.setTextSize(2); 
  display.setCursor(22, 10);
  display.println("Estacao");
  display.setTextSize(1);
  display.setCursor(23, 35);
  display.println("Metereologica");
  display.display(); 
  delay(2000);

  display.clearDisplay();
  display.setCursor(20, 25);
  display.println("Por Vitor Bruno");
  display.display();
  delay(2000);
}

void loop() {
  bool estadoBotaoAtual = digitalRead(pinoBotao);
  if (ultimoEstadoBotao == HIGH && estadoBotaoAtual == LOW) {
    pagina++;
    if (pagina > 2) pagina = 1; 
    delay(150); 
  }
  ultimoEstadoBotao = estadoBotaoAtual;

  int temperatura, umidade;
  dht11.readTemperatureHumidity(temperatura, umidade);

  display.clearDisplay();
  display.setCursor(0, 55);
  display.print("Pagina: "); display.print(pagina);

  if (pagina == 1) {
    display.setCursor(0, 0);
    display.println("--- DADOS DO AR ---");
    display.setTextSize(2);
    display.setCursor(0, 20);
    display.print(temperatura); display.println(" C");
    display.print(umidade);  display.println(" %");
    display.setTextSize(1);
  } 
  else {
    display.setCursor(0, 0);
    display.println("--- ATMOSFERA ---");
    display.setCursor(0, 20);
    if(bmpOnline) {
      float pressao = bmp.readPressure() / 100.0F;
      float altitude = bmp.readAltitude(1013.25);
      display.print("Pressao: "); display.print(pressao, 1); display.println(" hPa");
      display.print("Altitude: "); display.print(altitude, 1); display.println(" m");
    } else {
      display.println("Sensor BMP OFFLINE");
      display.println("Verifique fios 10-13");
    }
  }

  display.display();
  delay(100);
}
