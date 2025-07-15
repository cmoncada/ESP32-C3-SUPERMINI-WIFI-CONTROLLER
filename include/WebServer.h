#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "MotorController.h"

class RobotWebServer {
  public:
    RobotWebServer(MotorController* motorController, int port = 80);
    void initAccessPoint(const char* ssid, const char* password, int channel = 1, int maxConnections = 1);

  private:
    AsyncWebServer _server;
    MotorController* _motorController;
    
    void setupRoutes();
    String getHtmlPage();
    String getWifiInfo();
};

#endif
