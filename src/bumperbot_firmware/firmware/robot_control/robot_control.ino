
#include <PID_v1.h>

#define L298N_enA 4
#define L298N_in1 2
#define L298N_in2 15
#define L298N_in3 5
#define L298N_in4 18
#define L298N_enB 21
#define right_encoder_phaseA 13
#define right_encoder_phaseB 12
#define left_encoder_phaseA 14
#define left_encoder_phaseB 27

unsigned int right_encoder_counter = 0;
unsigned int left_encoder_counter = 0;
String right_encoder_sign = "p" ;
String left_encoder_sign = "p" ;
double right_wheel_meas_vel = 0.0;
double left_wheel_meas_vel = 0.0;
bool is_right_wheel_cmd = false;
bool is_left_wheel_cmd = false;
char value[]= "00.00";
uint8_t value_idx = 0;
bool is_cmd_complete = false;
bool is_right_wheel_forward = false;
bool is_left_wheel_forward = false;
double right_wheel_cmd_vel = 0.0;
double left_wheel_cmd_vel = 0.0;
unsigned long last_millis = 0.0;
const unsigned long interval = 100;

double right_wheel_cmd = 0.0;
double left_wheel_cmd = 0.0;

double kp_r = 11.5;
double ki_r = 7.5;
double kd_r = 0.1;
double kp_l = 12.8;
double ki_l = 8.3;
double kd_l = 0.1;

PID rightmotor(&right_wheel_meas_vel, &right_wheel_cmd, &right_wheel_cmd_vel,kp_r,ki_r,kd_r,DIRECT);
PID leftmotor(&left_wheel_meas_vel, &left_wheel_cmd, &left_wheel_cmd_vel,kp_l,ki_l,kd_l,DIRECT);



void setup() {
  // put your setup code here, to run once:
  pinMode(L298N_enA,OUTPUT);
  pinMode(L298N_in1,OUTPUT);
  pinMode(L298N_in2,OUTPUT);
  pinMode(L298N_enB,OUTPUT);
  pinMode(L298N_in3,OUTPUT);
  pinMode(L298N_in4,OUTPUT);
  
  pinMode(right_encoder_phaseB,INPUT);
  pinMode(left_encoder_phaseB,INPUT);
  
  attachInterrupt(digitalPinToInterrupt(right_encoder_phaseA),rightencoderCallback,RISING);
  attachInterrupt(digitalPinToInterrupt(left_encoder_phaseA),leftencoderCallback,RISING);

  digitalWrite(L298N_in1,HIGH);
  digitalWrite(L298N_in2,LOW);
  digitalWrite(L298N_in3,HIGH);
  digitalWrite(L298N_in4,LOW);

  rightmotor.SetMode(AUTOMATIC);
  leftmotor.SetMode(AUTOMATIC);
  
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:

  if(Serial.available())
  {
  char chr = Serial.read();
  if(chr == 'r')
  {
    is_right_wheel_cmd = true;
    is_left_wheel_cmd = false;
    value_idx = 0;
    is_cmd_complete = false;
  }
  else if(chr == 'l')
  {
    is_right_wheel_cmd = false;
    is_left_wheel_cmd = true;
    value_idx = 0;
  }
  else if(chr == 'p'){
    if(is_right_wheel_cmd && !is_right_wheel_forward){
      digitalWrite(L298N_in1, HIGH - digitalRead(L298N_in1));
      digitalWrite(L298N_in2, HIGH - digitalRead(L298N_in2));
      is_right_wheel_forward = true;
      }
     else if(is_left_wheel_cmd && !is_left_wheel_forward){
      digitalWrite(L298N_in3, HIGH - digitalRead(L298N_in3));
      digitalWrite(L298N_in4, HIGH - digitalRead(L298N_in4));
      is_left_wheel_forward = true;
      }
    }
   else if (chr == 'n'){
    if(is_right_wheel_cmd && is_right_wheel_forward){
      digitalWrite(L298N_in1, HIGH - digitalRead(L298N_in1));
      digitalWrite(L298N_in2, HIGH - digitalRead(L298N_in2));
      is_right_wheel_forward = false;
      }
     else if(is_left_wheel_cmd && is_left_wheel_forward){
      digitalWrite(L298N_in3, HIGH - digitalRead(L298N_in3));
      digitalWrite(L298N_in4, HIGH - digitalRead(L298N_in4));
      is_left_wheel_forward = false;
      }
    }
    else if(chr == ',')
    {
      if(is_right_wheel_cmd)
      {
        right_wheel_cmd_vel = atof(value);
      }
       else if(is_left_wheel_cmd)
       {
        left_wheel_cmd_vel = atof(value);
        is_cmd_complete = true;
       }
        value_idx = 0;
        value[0] = '0';
        value[1] = '0';
        value[2] = '.';
        value[3] = '0';
        value[4] = '0';
        value[5] = '\0';
    }
    else
    {
      if(value_idx < 5)
      {
        value[value_idx] = chr;
        value_idx++;
      }
    }
  }

  unsigned long current_millis = millis();
  if(current_millis - last_millis >= interval){
    
    right_wheel_meas_vel = 10.0*right_encoder_counter*(60.0/26720.0)*0.05*0.10472;
    left_wheel_meas_vel = 10.0*left_encoder_counter*(60.0/26720.0)*0.05*0.10472;

    rightmotor.Compute();
    leftmotor.Compute();

    if(right_wheel_cmd_vel == 0.0)
    {
      right_wheel_cmd = 0.0;
    }
    if(left_wheel_cmd_vel == 0.0)
    {
      left_wheel_cmd = 0.0;
    }

    String encoder_read = "r"+right_encoder_sign + String(right_wheel_meas_vel) + "l" + left_encoder_sign + String(left_wheel_meas_vel) + ",";
    Serial.println(encoder_read);
    last_millis = current_millis;
    right_encoder_counter = 0;
    left_encoder_counter = 0;

    analogWrite(L298N_enA, right_wheel_cmd);
    analogWrite(L298N_enB, left_wheel_cmd);
    }
}

void rightencoderCallback(){

  right_encoder_counter ++;

  if(digitalRead(right_encoder_phaseB)== HIGH){
    right_encoder_sign = "p";
    }

    else{
      right_encoder_sign = "n";
      }
  }

 void leftencoderCallback(){

  left_encoder_counter ++;

  if(digitalRead(left_encoder_phaseB)== HIGH)
    {
    left_encoder_sign = "p";
    }

  else
    {
      left_encoder_sign = "n";
    }
  }
