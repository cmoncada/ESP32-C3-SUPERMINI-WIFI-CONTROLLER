#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H

#include <Arduino.h>

class MotorController {
  public:
    // Constructor sin pines ENA/ENB
    MotorController(int in1, int in2, int in3, int in4);
    void init();
    
    // Funciones de movimiento
    void forward();
    void backward();
    void turnLeft();
    void turnRight();
    void stop();
    
    // Control de velocidad
    void setSpeed(int speed); // 0-255
    int getSpeed();

  private:
    // Pines del motor A
    int _in1;
    int _in2;
    
    // Pines del motor B
    int _in3;
    int _in4;
    
    // Velocidad actual (0-255)
    int _speed;
    
    // Estado actual del movimiento
    enum Direction {
      STOP,
      FORWARD,
      BACKWARD,
      LEFT,
      RIGHT
    };
    
    Direction _currentDirection;
    
    // Aplica el movimiento actual con la velocidad actual
    void applyMovement();
};

#endif
