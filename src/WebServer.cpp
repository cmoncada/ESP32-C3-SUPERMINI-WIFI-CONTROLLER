#include "WebServer.h"

RobotWebServer::RobotWebServer(MotorController* motorController, int port)
  : _server(port), _motorController(motorController) {
}

void RobotWebServer::initAccessPoint(const char* ssid, const char* password) {
  // Configurar IP fija para el punto de acceso
  IPAddress local_IP(192, 168, 0, 1);
  IPAddress gateway(192, 168, 0, 1);
  IPAddress subnet(255, 255, 255, 0);

  // Configurar el AP con IP fija
  WiFi.softAPConfig(local_IP, gateway, subnet);
  
  // Iniciar el modo AP
  bool result = WiFi.softAP(ssid, password);
  
  if (result) {
    Serial.println("Punto de acceso WiFi iniciado correctamente");
    Serial.print("SSID: ");
    Serial.println(ssid);
    Serial.print("Contraseña: ");
    Serial.println(password);
    Serial.print("IP del punto de acceso: ");
    Serial.println(WiFi.softAPIP());
  } else {
    Serial.println("Error al iniciar el punto de acceso WiFi");
  }
  
  // Configurar rutas
  setupRoutes();
  
  // Iniciar servidor
  _server.begin();
  Serial.println("Servidor web iniciado");
}

void RobotWebServer::setupRoutes() {
  // Página principal con controles
  _server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request) {
    request->send(200, "text/html", getHtmlPage());
  });
  
  // Ruta para avanzar
  _server.on("/forward", HTTP_GET, [this](AsyncWebServerRequest *request) {
    _motorController->forward();
    request->send(200, "text/plain", "Avanzando");
  });
  
  // Ruta para retroceder
  _server.on("/backward", HTTP_GET, [this](AsyncWebServerRequest *request) {
    _motorController->backward();
    request->send(200, "text/plain", "Retrocediendo");
  });
  
  // Ruta para girar a la izquierda
  _server.on("/left", HTTP_GET, [this](AsyncWebServerRequest *request) {
    _motorController->turnLeft();
    request->send(200, "text/plain", "Girando a la izquierda");
  });
  
  // Ruta para girar a la derecha
  _server.on("/right", HTTP_GET, [this](AsyncWebServerRequest *request) {
    _motorController->turnRight();
    request->send(200, "text/plain", "Girando a la derecha");
  });
  
  // Ruta para detener
  _server.on("/stop", HTTP_GET, [this](AsyncWebServerRequest *request) {
    _motorController->stop();
    request->send(200, "text/plain", "Detenido");
  });
  
  // Ruta para ajustar velocidad
  _server.on("/speed", HTTP_GET, [this](AsyncWebServerRequest *request) {
    if (request->hasParam("value")) {
      String value = request->getParam("value")->value();
      _motorController->setSpeed(value.toInt());
      request->send(200, "text/plain", "Velocidad ajustada a " + value);
    } else {
      request->send(400, "text/plain", "Parámetro 'value' requerido");
    }
  });
}

String RobotWebServer::getHtmlPage() {
  // Página HTML con CSS y JavaScript para controlar el robot
  String html = R"rawliteral(<!doctypehtml><meta content="width=device-width,initial-scale=1"name=viewport><meta charset=UTF-8><title>Arkytas Controlador</title><style>:root{--font-primary-color:#131316;--font-secondary-color:#5e5f6e}body{font-family:Verdana,sans-serif;text-align:center;margin:0;padding:0;background-color:#f7f7f8;color:var(--font-primary-color);-webkit-font-smoothing:antialiased;-moz-osx-font-smoothing:grayscale}header{width:100vw;padding:20px 0}header .logo{width:auto;height:30px;margin:0 auto;display:block;fill:var(--font-primary-color)}header .logo svg{width:100%;height:100%}header h1{margin:15px 0 0 0;font-size:1em;color:var(--font-primary-color)}.container{margin-left:auto;margin-right:auto;background-color:#f6f7fa;border-radius:10px;box-shadow:rgba(34,42,53,.12) 0 10px 32px,rgba(0,0,0,.05) 0 1px 1px,rgba(34,42,53,.05) 0 0 0 1px,rgba(34,42,53,.08) 0 4px 6px,rgba(47,48,55,.1) 0 24px 108px;width:100%;max-width:300px;padding:40px;display:grid;grid-template-columns:repeat(3,1fr);grid-template-rows:repeat(3,1fr);grid-column-gap:10px;grid-row-gap:10px}.container .box button{aspect-ratio:1/1;width:100%;height:100%;border:none;box-shadow:rgba(34,42,53,.12) 0 10px 32px,rgba(0,0,0,.05) 0 1px 1px,rgba(34,42,53,.05) 0 0 0 1px,rgba(34,42,53,.08) 0 4px 6px,rgba(47,48,55,.1) 0 24px 108px;border-radius:15px;background-color:#4caf50;color:#fff;font-size:30px}.container .box button:active{background-color:#3e8e41}.div1{grid-area:1/1/2/2}.div2{grid-area:1/2/2/3}.div3{grid-area:1/3/2/4}.div4{grid-area:2/1/3/2}.div5{grid-area:2/2/3/3}.div6{grid-area:2/3/3/4}.div7{grid-area:3/1/4/2}.div8{grid-area:3/2/4/3}.div9{grid-area:3/3/4/4}.div10{grid-area:4/1/5/4}.button-row{display:flex;justify-content:center;margin:10px 0}#stop-btn{background-color:#f44336}#stop-btn:active{background-color:#d32f2f}.slider-container{margin-top:20px}#speed-slider{width:80%}#status{margin-top:20px;font-weight:700;color:#555}</style><header><div class=logo><?xml version="1.0" encoding="UTF-8"?><svg viewBox="0 0 805 100"xmlns=http://www.w3.org/2000/svg><g><path d="M254.15 60.81c14.97-2.77 22.55-12.39 22.55-28.62 0-19.84-10.49-30.32-30.31-30.32h-69.6l-.26 90.29-37.02-90.32H39.49L-.66 99.74l200.38.02.26-38.2h28.32l23.32 38.35h26.41l-22.78-37.46c-.32-.52-.68-1.07-1.09-1.64h-.01Zm-150 17.06 19.18-51.28 20.29 51.32-39.47-.03ZM253.5 33.13c0 4.98-2.13 7.1-7.1 7.1H200V23.22h46.4c4.98 0 7.1 2.13 7.1 7.1v2.8ZM759.44 37.15c-17.2 0-23-1.4-23-5.52 0-6.83 7.42-10.28 22.07-10.28 22.07 0 22.07 8.17 22.07 10.85v2.25h23.21V32.2c0-9.72-4.29-17.64-12.75-23.55-8.21-5.73-19.15-8.64-32.53-8.64-30.05 0-45.28 10.64-45.28 31.63 0 23.81 23.8 26.86 44.54 26.86 11.26 0 24.69 1.46 24.69 8.41 0 7.57-8.05 11.41-23.94 11.41-17.23 0-38.17-.59-38.39-.59l-4.4-.07-31.02-75.73H456.44l-18.83 29.13c-2.28 3.66-4.52 7.76-6.65 12.15l-.23.46-.2-.41c-1.42-3.33-3.66-7.45-6.71-12.27l-19-29.08h-38.57l-33.76 37.82h-16.4V1.91h-29.15V100h29.15V61.58h16.4L367.23 100h34.48l-45.98-50.02 30.45-35.29 32.92 50.95v34.33h23.21V65.64l27.63-42.76h45.03V99.8h23.21V22.88h37.86l-31.59 76.91 101.95.21h49.34l29.03-.23v-.03h2.53s17.2-.09 26.88-.09h4.33c31.29 0 47.15-11.02 47.15-32.74 0-19.75-15.56-29.76-46.22-29.76Zm-179 40.12 20.35-51.31 19.19 51.34-39.55-.03Z"/></g></svg></div><h1>Arkytas Controlador</h1></header><main><div class=container><div class="box div1"></div><div class="box div2"><button id=forward-btn>⇡</button></div><div class="box div3"></div><div class="box div4"><button id=left-btn>⇠</button></div><div class="box div5"><button id=stop-btn>■</button></div><div class="box div6"><button id=right-btn>⇢</button></div><div class="box div7"></div><div class="box div8"><button id=backward-btn>⇣</button></div><div class="box div9"></div><div class="box div10"><div class=slider-container><label for=speed-slider>Velocidad:</label> <input id=speed-slider max=255 min=0 type=range value=200> <span id=speed-value>200</span><div id=status>Detenido</div></div></div></div></main><script>document
      .getElementById("forward-btn")
      .addEventListener("click", function () {
        sendCommand("/forward");
      });

    document
      .getElementById("backward-btn")
      .addEventListener("click", function () {
        sendCommand("/backward");
      });

    document
      .getElementById("left-btn")
      .addEventListener("click", function () {
        sendCommand("/left");
      });

    document
      .getElementById("right-btn")
      .addEventListener("click", function () {
        sendCommand("/right");
      });

    document
      .getElementById("stop-btn")
      .addEventListener("click", function () {
        sendCommand("/stop");
      });

    const speedSlider = document.getElementById("speed-slider");
    const speedValue = document.getElementById("speed-value");

    speedSlider.addEventListener("input", function () {
      speedValue.textContent = this.value;
    });

    speedSlider.addEventListener("change", function () {
      fetch("/speed?value=" + this.value)
        .then((response) => response.text())
        .then((data) => {
          document.getElementById("status").textContent = data;
        });
    });

    function sendCommand(command) {
      fetch(command)
        .then((response) => response.text())
        .then((data) => {
          document.getElementById("status").textContent = data;
        });
    }</script>)rawliteral";

  return html;
}
