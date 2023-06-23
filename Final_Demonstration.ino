#include <Servo.h>
// **VARIABLES**
#define led_red 4
#define led_green 5

//Communication
#define PAN_ID "4321"
#define CHANNEL_ID "F"
#define pin_master 6

String str_message;
boolean xbee_connected = false;
boolean xbee_master = false;

//Algorithm
#define grid_size 12

int mode = 2;
int loop_count = 0;
boolean init_algo2 = true;
char algo_out = '\0';

int rocks_collect;
int tot_rocks=5;

int double_turn = 0;
int turncount = 0;
int turn_delay = 10000;
long t_turn = 0;

int directionnow=-1; // N = 1; S = -1; E = 2; W = 2 START DIRECTION
int x = 0; // START X POSITION
int y = 0; // START Y POSITION
String venusmap[grid_size] = {  "oooooosooooo",
                                "o         oo",
                                "o         oo",
                                "o     o   oo",
                                "o         oo",
                                "o         oo",
                                "o   o     oo",
                                "o        ooo",
                                "o        roo",
                                "o    o    oo",
                                "o         oo",
                                "oooooooooooo"}; 

// Movement
Servo servo_grap;
#define pin_servo_grap 10 
Servo servo_sensor;
#define pin_servo_sensor 11 
Servo servo_right; 
#define pin_servo_right 12 
Servo servo_left;  
#define pin_servo_left 13 

int degree180_right = 975;
int degree180_left = 905;
int degree90_right = degree180_right/2;
int degree90_left = degree180_left/2;
int dist_forward = 700;

// Sensors

#define ultra_pin 9
#define pin_ir_rock 3
#define pin_ir_cliff 2

boolean Ultra_obs = false;
boolean IR_cliff = false;
boolean IR_rock = false;

int trig_dist = 18;

// **FUNCTIONS**

// Communication
boolean receive_data(int t_stop) {  // function input is t_stop denotes the time in milliseconds that the microcontroller will be looking waiting for the message to arrive
  String message;                   // function outputs a boolean to notify if a message has been recieved or not
  float t_start = millis();
  
  while ((millis()-t_start) < t_stop){
    
    while (Serial.available() > 0) {
      message = Serial.readString();
      str_message = message;
      return true;
    }
  }
  return false;
}

// Function of initializing communication between master and slave
boolean xbee_init(int nr_sec) {
  Serial.begin(9600);
  boolean first_check = false;
  boolean second_check = false;
  
  for(int i = 0; i<nr_sec && second_check == false && first_check == false ; i++){
    if(xbee_master){
      Serial.print("x");
    }
    
    if(receive_data(1000)){
      for(int i = 0 ; i<str_message.length() ; i++){
        if(str_message[i] == '&' && xbee_master){
          first_check = true;
        }else{
          if(str_message[i] == 'x'){
            first_check = true;
          }
        }
      }
      
      if(first_check == true && xbee_master){
        Serial.print("&&&\n");
        second_check = true;
      }
    }
  }
  
  for(int i = 0; i<nr_sec && second_check == false && first_check == true && xbee_master == false; i++){
    Serial.print("&");
    if(receive_data(1000)){
      for(int i = 0 ; i<str_message.length() ; i++){
        if(str_message[i] == '&'){
          second_check = true;
        }
      }
      if(second_check == true){
        Serial.print("\n");
      }
    }
  }
  
  
  if(first_check == false || second_check == false){
    Serial.print("Communication FAILED");
    digitalWrite(led_red,HIGH);
    delay(5000);
    digitalWrite(led_red,LOW);
    return false;
  }else{
    Serial.print("Communication SUCCESSFUL");
    digitalWrite(led_green,HIGH);
    delay(5000);
    digitalWrite(led_green,LOW);
    return true;
  }
}

void send_loc(){
  
}

void recieve(){
  
}

void communication_setup() {
  pinMode(pin_master,INPUT);
  pinMode(7,OUTPUT);
  digitalWrite(7,HIGH);
  if(digitalRead(pin_master)==HIGH){ // if you have sorted the two master pins then its the master
    xbee_master = true;
  }
  digitalWrite(7,LOW);
  
  xbee_connected = xbee_init(20); // initializes communication between xbees
}



// Movement
void servo_attach(char a) {
  if(a == 'g'){
    servo_grap.attach(pin_servo_grap);
  }
  if(a == 'r'){
    servo_right.attach(12);
  }
  if(a == 'l'){
    servo_left.attach(13);
  }
  if(a == 's'){
    servo_sensor.attach(pin_servo_sensor);
  }
  if(a == 'a'){
    servo_grap.attach(pin_servo_grap);
    servo_sensor.attach(pin_servo_sensor);
    servo_right.attach(pin_servo_right);
    servo_left.attach(pin_servo_left);
  }
}
void servo_deatach() {
  servo_grap.detach();
  servo_right.detach();
  servo_left.detach();
  servo_sensor.detach();
}
void turnLeft(int t_move) {
  servo_attach('r');
  servo_attach('l');
  servo_right.write(0);
  servo_left.write(0);
  delay(t_move);
  servo_right.write(90);
  servo_left.write(90);
  servo_deatach();
}
void turnRight(int t_move) {
  servo_attach('r');
  servo_attach('l');
  servo_right.write(180);
  servo_left.write(180);
  delay(t_move);
  servo_right.write(90);
  servo_left.write(90);
  servo_deatach();
}
void goForward(int t_move) {
  servo_attach('r');
  servo_attach('l');
  servo_right.write(10);
  servo_left.write(170);
  if(t_move > 0){
    delay(t_move);
    servo_right.write(90);
    servo_left.write(90);
    servo_deatach();
  }
}
void goBackward(int t_move) {
  servo_attach('r');
  servo_attach('l');
  servo_right.write(180);
  servo_left.write(0);
  delay(t_move);
  servo_deatach();
}
void grapClose() {
  servo_attach('g');
  servo_grap.write(0);
}
void grapOpen() {
  servo_attach('g');
  servo_grap.write(90);
  delay(1000);
  servo_deatach();
}

void movement(char a){    // Makes movement based on command
  switch(a){
    default:
      break;
    case 'f':
      goForward(0);
      break;
    case 'r':
      turnRight(degree90_right);
      break;
    case 'l':
      turnLeft(degree90_left);
      break;
    case 'b':
      turnRight(degree180_right);
      break;
  }
}

// Sensors
void sensors_setup(){
  pinMode(pin_ir_rock,INPUT);
  pinMode(pin_ir_cliff,INPUT);
}

boolean ultra_sense(){

  pinMode(ultra_pin, OUTPUT);
  digitalWrite(ultra_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(ultra_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(ultra_pin, LOW);

  pinMode(ultra_pin,INPUT);
  long duration = pulseIn(ultra_pin, HIGH);
  int distanceCm = duration * 0.034 / 2;

  if(distanceCm < trig_dist && distanceCm > 0){
    Ultra_obs = true;
//    Serial.print("Distance = ");
//    Serial.print(distanceCm);
//    Serial.print("\n");
    return true;
  }else{
    Ultra_obs = false;
    return false;
  }
}

boolean rock_sense(){
  if(digitalRead(pin_ir_rock) == 0){
    IR_rock = true;
    return true;
  }else{
    IR_rock = false;
    return false;
  }
}

boolean cliff_sense(){
  if(digitalRead(pin_ir_cliff) == 1){
    IR_cliff = true;
    return true;
  }else{
    IR_cliff = false;
    return false;
  }
}

// Algorithm
char dirchar(int dirrectionnow) {
  switch (dirrectionnow) {
    default:
      return '\0';
      break;
    case 1:
      return 'N';
      break;
    case -1:
      return 'S';
      break;
    case 2:
      return 'E';
      break;
    case -2:
      return 'W';
      break;
  }
}

void set_start(){
  for(int i = 0; i<grid_size ; i++){
    for(int j = 0; j<grid_size ; j++){
      if(venusmap[i][j] == 's'){
        y = i;
        x = j;
      }
    }
  }
}

void update_map(char direc, char a, int increment){
  if(direc=='0'){
    direc = dirchar(directionnow);
  }
  switch(direc){
    default:
      break;
    case 'N':
      venusmap[y-increment][x] = a;
      break;
    case 'S':
      venusmap[y+increment][x] = a;
      break;
    case 'E':
      venusmap[y][x+increment] = a;
      break;
    case 'W':
      venusmap[y][x-increment] = a;
      break;
  }
}

void update_pos(char direc, int increment){
  if(direc=='0'){
    direc = dirchar(directionnow);
  }
  switch(direc){
    default:
      break;
    case 'N':
      y = y-increment;
      break;
    case 'S':
      y = y+increment;
      break;
    case 'E':
      x = x+increment;
      break;
    case 'W':
      x = x-increment;
      break;
  }
}

char check_map(char direc, int increment){ // This checks if the robot already travelled to a certain square
  if(direc=='0'){
    direc = dirchar(directionnow);
  }  
  
  switch(direc){
    default:
      return false;
      break;
    case 'N':
      return venusmap[y-increment][x];
      break;
    case 'S':
      return venusmap[y+increment][x];
      break;
    case 'E':
      return venusmap[y][x+increment];
      break;
    case 'W':
      return venusmap[y][x-increment];
      break;
  }
}

// ALGORITHM Functions

//even or odd checking 
boolean turncheck(int turncount){
  if (turncount%2==0){
    return false;
  }else{
    //turn: right turn, right increment=1
    return true;
  }
}


//changing direction and outputting turn left,right,180
char algodirection_0(){

  if(double_turn == 0){
    double_turn++;
    switch(dirchar(directionnow)){
      default:
        return '\0';
        break;
      case 'N':
        if(turncheck(turncount)){
          directionnow = 2;
          return 'r';
        }else{
          directionnow = -2;
          return 'l';
        }
        break;
      case 'S':
        if(turncheck(turncount)){
          directionnow = -2;
          return 'r';
        }else{
          directionnow = 2;
          return 'l';
        }
        break;
      case 'E':
        if(turncheck(turncount)){
          directionnow = -1;
          return 'r';
        }else{
          directionnow = 1;
          return 'l';
        }
        break;
      case 'W':
        if(turncheck(turncount)){
          directionnow = 1;
          return 'r';
        }else{
          directionnow = -1;
          return 'l';
        }
        break;
    }
  }else if(double_turn == 1){
    
    double_turn++;
    directionnow=directionnow*-1;
    return 'b';
  }else if(double_turn == 2){

    double_turn=0;
    switch(dirchar(directionnow)){
      default:
        return '\0';
        break;
      case 'N':
        if(check_map('W',1) == '0'){
          directionnow = 2;
          return 'r';
        }else if(check_map('E',1) == '0'){
          directionnow = -2;
          return 'l';
        }
        break;
      case 'S':
        if(check_map('E',1) == '0'){
          directionnow = -2;
          return 'r';
        }else if(check_map('W',1) == '0'){
          directionnow = 2;
          return 'l';
        }
        break;
      case 'E':
        if(check_map('N',1) == '0'){
          directionnow = -1;
          return 'r';
        }else if(check_map('S',1) == '0'){
          directionnow = 1;
          return 'l';
        }
        break;
      case 'W':
        if(check_map('S',1) == '0'){
          directionnow = 1;
          return 'r';
        }else if(check_map('N',1) == '0'){
          directionnow = -1;
          return 'l';
        }
        break;
    }
  }
  return '\0';
}

char algo_0(){

  char object = '0';//check_map('0',1);

  
  if(object == 'o' || object == 'p' || IR_cliff || Ultra_obs ){ // change direction
    if(object != 'p'){
      update_map('0','o',1);
    }
    t_turn = millis();
    char turn_direct = algodirection_0();
    // if(turn_direct != 'b'){
      turncount++;
    // }

    return turn_direct;
  }else if(object == 'r' || IR_rock ){
    loop_count = 0;
    
    mode = 1;
    return '\0';
  }else{ // go forward
    if(turn_delay < millis()-t_turn){
      double_turn=0;
    }
    
    if(check_map('0',0) != 's'){
      update_map('0','p',0);
    }
    update_map('0','x',1);
    update_pos('0',1);
    return 'f';
  }
}

char algodirection_2(){
  int x_s;
  int y_s;
  for(int i = 0; i<grid_size; i++){
    for(int j = 0; j<grid_size; j++){
      if(venusmap[i][j] == 's'){
        x_s = j;
        y_s = i;
      }
    }
  }
  
  if(directionnow == 1 || directionnow == -1){
    if(x-x_s > 0){
      directionnow = -2;
      if(directionnow == 1){
        return 'r';
      }else{
        return 'l';
      }
      
    }else if(x-x_s < 0){
      directionnow = 2;
      if(directionnow == 1){
        return 'l';
      }else{
        return 'r';
      }
    }else{
      return algodirection_0();
    }
  }else if(directionnow == 2 || directionnow == -2){
    if(y-y_s > 0){
      directionnow = 1;
      if(directionnow == 2){
        return 'r';
      }else{
        return 'l';
      }
    }else if(y-y_s < 0){
      directionnow = -1;
      if(directionnow == 2){
        return 'l';
      }else{
        return 'r';
      }
    }else{
      return algodirection_0();
    }
  }else{
    return '\0';
  }
}

char algo_2(){
  if(init_algo2 == true){
    turncount = 0;
    double_turn = 0;
    init_algo2 = false;
    directionnow=directionnow*-1;
    return 'b';
  }

  char object = check_map('0',1);

  if(object == 's'){   
    update_map('0','p',0);
    update_pos('0',1);
    
    mode = 3;
    
    return 'f';
  }else if(object == 'p'){ // go forward
    double_turn = 0;
    update_map('0','p',0);
    update_map('0','x',1);
    update_pos('0',1);
    
    return 'f';
  }else{
    char turn_direct = algodirection_2();
    double_turn++;
    turncount++;
    return turn_direct;
  }
}

void setup() {
  Serial.begin(9600);
  communication_setup();
  
  
  // pinMode(led_red,OUTPUT);
  // pinMode(led_green,OUTPUT);
  // sensors_setup();

  // grapClose();
  delay(2000);
}

void loop() {
  // if(mode == 0){
  //   movement('f');
  //   if(turn_delay < millis()-t_turn){
  //     double_turn=0;
  //   }
    
  //   if(ultra_sense() || cliff_sense() || rock_sense()){
  //     servo_deatach();
  //     algo_out = algo_0();
  //     Serial.print("Algo out = ");
  //     Serial.print(algo_out);
  //     Serial.print("\n");
  //     if(algo_out != '\0'){
  //       movement(algo_out);
  //     }else{
  //       digitalWrite(led_green, HIGH);
  //       delay(2000);
  //       digitalWrite(led_green, LOW);
  //     }
  //   }
  // }else if(mode == 1){
    
  //   turnLeft(10);
  //   grapOpen();
  //   goForward(450);
  //   grapClose();
  //   mode = 2;
  // }else if(mode == 2){
  //   movement('f');
  //   if(cliff_sense()){
  //     goForward(300);
  //     grapOpen();
  //     digitalWrite(led_green, HIGH);
  //     delay(3000);
  //     digitalWrite(led_green, LOW);
  //     mode = 3;
  //   }    
  // }
    
}
