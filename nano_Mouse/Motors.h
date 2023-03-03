#include <Arduino.h>
#include <Servo.h>
#define RIGHT 1
#define LEFT -1
class Motors {
  private:
    Servo leftServo;
    Servo rightServo;

    static const byte boxlength = 30 , rbm = 144, raidus = 5, power = 100 , length = 15;
    float wheel = 2 * (22 / 7) * raidus;
    float speed = 2 * rbm * wheel / 60;
    float cir = 2 * (22 / 7) * length / 2;
  public:
    void  attach(byte l, byte r) {
      leftServo.attach(l);
      rightServo.attach(r);
    }
    void  forward() {
      leftServo.writeMicroseconds(1500 + power);
      rightServo.writeMicroseconds(1500 - power);
    }
    void stop (int time = 200) {
      leftServo.writeMicroseconds(1500);
      rightServo.writeMicroseconds(1500);
      delay(time);
    }
    void forwardfor(unsigned int time) {
      forward();
      delay(time);
      stop();
    }
    void Runboxes(byte numofbox){
      forwardfor((numofbox*boxlength/speed)*1000);
    }
    void turn( byte direction,int angle) {


      leftServo.writeMicroseconds(1500 + (power * direction));
      rightServo.writeMicroseconds(1500 + (power * direction));
      delay(angle/360*((cir/speed)*1000));
      stop();
    }
       void  forwardprop(int error) {
      leftServo.writeMicroseconds(1500 + (power-(error*2)));
      rightServo.writeMicroseconds(1500 - (power-(error*2)));
    }
};

