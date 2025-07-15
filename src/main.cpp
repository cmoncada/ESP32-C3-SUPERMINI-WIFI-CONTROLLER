#include <Arduino.h>
#include "MotorController.h"
#include "WebServer.h"

// Si no se definen en platformio.ini, usar valores predeterminados
#ifndef WIFI_SSID
  #define WIFI_SSID "RobotESP32"
#endif

#ifndef WIFI_PASSWORD
  #define WIFI_PASSWORD "robot123"
#endif

// Configuración de pines L298N (sin ENA/ENB)
// Motor A
const int IN1 = 0;  // GPIO5
const int IN2 = 1;  // GPIO6

// Motor B
const int IN3 = 2;  // GPIO8
const int IN4 = 3;  // GPIO9

// Inicializar objetos
MotorController motorController(IN1, IN2, IN3, IN4);
RobotWebServer webServer(&motorController);

void setup() {
  // Iniciar comunicación serial
  Serial.begin(115200);
  Serial.println("\n\n--- Iniciando Robot ESP32 ---");
  Serial.println("Modo: Control sin pines ENA/ENB");
  
  // Inicializar controlador de motores
  motorController.init();
  Serial.println("Controlador de motores inicializado");
  
  // Inicializar servidor web como punto de acceso
  webServer.initAccessPoint(WIFI_SSID, WIFI_PASSWORD, 6, 1);

  delay(5000);
  Serial.println("Sistema iniciado correctamente");
  Serial.println("Para controlar el robot, conectate a la red WiFi:");
  Serial.print("SSID: ");
  Serial.println(WIFI_SSID);
  Serial.print("Contraseña: ");
  Serial.println(WIFI_PASSWORD);
  Serial.println("Y accede a http://192.168.1.1 en tu navegador");
}

void loop() {
  // El servidor web asíncrono maneja las solicitudes automáticamente
  delay(100); // Pequeña pausa para estabilidad
}
