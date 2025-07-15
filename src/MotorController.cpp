#include "MotorController.h"

// Constructor sin pines ENA/ENB
MotorController::MotorController(int in1, int in2, int in3, int in4) {
  _in1 = in1;
  _in2 = in2;
  _in3 = in3;
  _in4 = in4;
  _speed = 200; // Velocidad predeterminada (0-255)
  _currentDirection = STOP;
}

void MotorController::init() {
  // Configurar pines como salidas
  pinMode(_in1, OUTPUT);
  pinMode(_in2, OUTPUT);
  pinMode(_in3, OUTPUT);
  pinMode(_in4, OUTPUT);
  
  // Iniciar con los motores detenidos
  stop();
}

// Aplica el movimiento actual con la velocidad actual
void MotorController::applyMovement() {
  int pwmValue = _speed;
  
  // Si la velocidad es 0 o estamos detenidos, apagar todos los motores
  if (pwmValue == 0 || _currentDirection == STOP) {
    analogWrite(_in1, 0);
    analogWrite(_in2, 0);
    analogWrite(_in3, 0);
    analogWrite(_in4, 0);
    return;
  }
  
  // Aplicar señal PWM según la dirección actual
  switch(_currentDirection) {
    case FORWARD:
      analogWrite(_in1, pwmValue);
      analogWrite(_in2, 0);
      analogWrite(_in3, pwmValue);
      analogWrite(_in4, 0);
      Serial.println("Avanzando");
      break;
      
    case BACKWARD:
      analogWrite(_in1, 0);
      analogWrite(_in2, pwmValue);
      analogWrite(_in3, 0);
      analogWrite(_in4, pwmValue);
      Serial.println("Reversa");
      break;
      
    case LEFT:
      analogWrite(_in1, 0);
      analogWrite(_in2, pwmValue);
      analogWrite(_in3, pwmValue);
      Serial.println("Izquierda");
      analogWrite(_in4, 0);
      break;
      
    case RIGHT:
      analogWrite(_in1, pwmValue);
      analogWrite(_in2, 0);
      analogWrite(_in3, 0);
      analogWrite(_in4, pwmValue);
      Serial.println("Derecha");
      break;
      
    default:
      // Si llegamos aquí, algo salió mal, así que detenemos por seguridad
      stop();
      break;
  }
}

void MotorController::forward() {
  _currentDirection = FORWARD;
  applyMovement();
}

void MotorController::backward() {
  _currentDirection = BACKWARD;
  applyMovement();
}

void MotorController::turnLeft() {
  _currentDirection = LEFT;
  applyMovement();
}

void MotorController::turnRight() {
  _currentDirection = RIGHT;
  applyMovement();
}

void MotorController::stop() {
  _currentDirection = STOP;
  analogWrite(_in1, 0);
  analogWrite(_in2, 0);
  analogWrite(_in3, 0);
  analogWrite(_in4, 0);
}

void MotorController::setSpeed(int speed) {
  // Limitar velocidad entre 0-255
  _speed = constrain(speed, 0, 255);
  
  // Aplicar la nueva velocidad al movimiento actual
  applyMovement();
}

int MotorController::getSpeed() {
  return _speed;
}
