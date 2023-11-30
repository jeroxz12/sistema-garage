
#include <SPI.h>
#include <MFRC522.h>


#define SS_PIN 10
#define RST_PIN 9
#define LED_G 3  //define pin LED VERDE 
#define LED_R 2  //define pin LED ROJO

MFRC522 mfrc522(SS_PIN, RST_PIN);  // crea la instancia de MFRC522

#include <Servo.h>
Servo myServo;
int TRIG = 8;
int ECO = 7;
int tiempo;
int distancia;

void setup() {
  pinMode(TRIG, OUTPUT);
  pinMode(ECO, INPUT);
  myServo.attach(5);
  SPI.begin();         // Inicia bus SPI 
  mfrc522.PCD_Init();  // Inicia MFRC522

  pinMode(LED_G, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(A0, OUTPUT);
  Serial.begin(9600);
}
void loop() {
  // LBusca nuevas tarjetas
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  // Se fija si puede leer una tarjeta
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }

  content.toUpperCase();
  Serial.println(content.substring(1));
  if (content.substring(1).equalsIgnoreCase("43 E9 68 31"))  //Hace la comparacion del string mapeado de datos de la tarjeta
  {
    digitalWrite(LED_G, HIGH);
    delay(2000);
    digitalWrite(LED_G, LOW);
    leerDistancia();
    Serial.println("DISTANCIA: " + distancia);
    if (distancia < 20) {
      abrirPuerta();
      delay(2000);
      cerrarPuerta();
      delay(2000);
    }
  }

  else {
    Serial.println(content.substring(1));

    digitalWrite(LED_R, HIGH);
    digitalWrite(A0, HIGH);
    delay(3000);
    digitalWrite(LED_R, LOW);
    digitalWrite(A0, LOW);
  }
}
void leerDistancia() {
  digitalWrite(TRIG, HIGH);
  delay(1000);
  digitalWrite(TRIG, LOW);
  tiempo = pulseIn(ECO, HIGH);
  distancia = tiempo / 58.2;
  Serial.println(distancia);
}

void cerrarPuerta() {
  digitalWrite(LED_R, HIGH);
  digitalWrite(LED_G, LOW);
  int angulo = 0;
  do {
    angulo = angulo + 1;
    myServo.write(angulo);
    delay(50);
  } while (angulo < 95);
  digitalWrite(LED_G, LOW);
  digitalWrite(LED_R, LOW);
}
void abrirPuerta() {
  digitalWrite(LED_G, HIGH);
  digitalWrite(LED_R, LOW);
  Serial.println("ENTRO A ABRIR PUERTA: " + LED_G);
  int angulo = 90;
  do {
    angulo = angulo - 1;
    myServo.write(angulo);
    delay(50);
  } while (angulo > 0);
  digitalWrite(LED_G, LOW);
  digitalWrite(LED_R, LOW);
}
