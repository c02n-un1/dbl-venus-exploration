// Libraries
#include <Servo.h>

//Communcation Variables
#define PAN_ID "4321"
#define CHANNEL_ID "F"
#define led 12
String str;
boolean xbee_connected;

//Mapping Variables
#define grid_size 10
char venusmap[grid_size][grid_size] = {"s000000000",
                                       "0000000000",
                                       "0000000000",
                                       "0000000000",
                                       "0000000000",
                                       "0000000000",
                                       "0000000000",
                                       "0000000000",
                                       "0000000000",
                                       "0000000000"}; 
int directionnow=1;
int x = 0;
int y = 0;
                                       

//Servo Variables
Servo servo_grap;    // 10
Servo servo_sensor;  // 11
Servo servo_right;   // 12
Servo servo_left;    // 13

int degree180_right = 1657;
int degree180_left = 1481;
int rotation = 1695;  // don't know yet
int lenght = 400;

//Algorithm Variables

int mode = 0;
int turncount = 0;
int right = 0;
int left = 0;
int rocks_collect;
int tot_rocks=5;
int x_max = grid_size;
int y_max = grid_size;

// Sensor Variables



/////////////*************** PRINT FUNCTIONS  ***************/////////////
//void reset_map(){
//  for(int i = 0; i<W ; i++){
//    for(int j = 0; j<H ; j++){
//      map_venus[i][j] = 0;
//    }
//  }
//}

void print_map(){         // PRINT MAP
  Serial.print("\nMAP VISUALIZATION:\n\n");
  for(int a = 0; a<grid_size ; a++){
    Serial.print("----");
  }
  Serial.print("\n");
  for(int i = 0; i<grid_size ; i++){
    for(int j = 0; j<grid_size ; j++){
      Serial.print(venusmap[i][j]);
      Serial.print(" | ");
    }
    Serial.print("\n");
    for(int a = 0; a<grid_size ; a++){
      Serial.print("----");
    }
    Serial.print("\n");
  }
}

/////////////*************** COMMUNICATION FUNCTIONS  ***************/////////////

boolean receive_data(int t_stop) {     /// Recieve data          
  String message;
  float t_start = millis();
  
  while ((millis()-t_start) < t_stop){
    
    while (Serial.available() > 0) {
      message = Serial.readString();
      str = message;
      return true;
    }
  }
  return false;
}

void xbee_init_master() { /// initiate communication with slave
  Serial.begin(9600);
  
  Serial.print("Starting connection:\n");
  for(int i = 0; i<10 && xbee_connected == false ; i++){
    Serial.print("x");
    xbee_connected = receive_data(1000);
  }
  Serial.print("\n");
  
  if(xbee_connected == false){
    digitalWrite(led,HIGH);
    delay(5000);
    digitalWrite(led,LOW);
  }else{
    for(int i = 0 ; i<10 ; i++){
      digitalWrite(led,HIGH);
      delay(100);
      digitalWrite(led,LOW);
      delay(100);
    }
  }
}
/////////////*************** SENSORS FUNCTIONS  ***************/////////////
int *sensor_data(){   ///// Fake sensor data
  int sensor[3] = {-1,-1,-1};
  for(int i = 0; i<3 ; i++){
    if(str[i] == '1'){
      sensor[i] = 1;
    }else{
      sensor[i] = 0;
    }
  }
  
  Serial.print("SENSOR DATA: \t");
  for(int i = 0; i<3 ; i++){
    Serial.print(sensor[i]);
    if(i == 2){
      Serial.print("\n");
    }
  }
  
  return sensor;
}

/////////////*************** MOVEMENT FUNCTIONS  ***************/////////////

void servo_attach(char a) {
  if(a == 'g'){
    servo_grap.attach(10);
  }
  if(a == 'r'){
    servo_right.attach(12);
  }
  if(a == 'l'){
    servo_left.attach(13);
  }
  if(a == 's'){
    servo_sensor.attach(11);
  }
  if(a == 'a'){
    servo_grap.attach(10);
    servo_sensor.attach(11);
    servo_right.attach(12);
    servo_left.attach(13);
  }
}
void servo_deatach() {
  servo_grap.detach();
  servo_right.detach();
  servo_left.detach();
  servo_sensor.detach();
}
void goForward() {
  servo_attach('r');
  servo_attach('l');
  servo_right.write(0);
  servo_left.write(180);
  delay(lenght);
  servo_deatach();
}
void goBackward() {
  servo_attach('r');
  servo_attach('l');
  servo_right.write(180);
  servo_left.write(0);
  delay(lenght);
  servo_deatach();
}
void turnLeft() {
  servo_attach('r');
  servo_attach('l');
  servo_right.write(0);
  servo_left.write(0);
  delay(degree180_left/2);
  servo_deatach();
}
void turnRight() {
  servo_attach('r');
  servo_attach('l');
  servo_right.write(180);
  servo_left.write(180);
  delay(degree180_right/2);
  servo_deatach();
}
void grapClose() {
  servo_attach('g');
  servo_grap.write(0);
  delay(1000);
  servo_deatach();
}
void grapOpen() {
  servo_attach('g');
  servo_grap.write(90);
  delay(1000);
  servo_deatach();
}

void movement(char a){    // Makes movement based on command
  Serial.print("OUTPUT ALGORITHM:\t");
  Serial.print(a);
  Serial.print("\n");

//  switch(a){
//    default:
//      break;
//    case '':
//      break;
//    
//  }
}


/////////////*************** ALGOTHM FUNCTIONS  ***************/////////////
bool check_map_forward(char letter){ // This checks the map
  if (directionnow == 1) {
    y++;
    if(venusmap[x][y] == letter) {
      y--;
      return true;
    }
  } else if (directionnow == -1) {
    y--;
    if(venusmap[x][y] == letter) {
      y++;
      return true;
    }
  } else if (directionnow == 2) {
    x++;
    if(venusmap[x][y] == letter) {
      x--;
      return true;
    }
  } else if (directionnow == -2) {
    x--;
    if(venusmap[x][y] == letter) {
      x++; 
      return true;
    }
  }
  
  return false;
}

char check_forward (int US, int cliff, int rock) { /// Check for what is in front SENSORS + MAP
  //obstacle check
  if(check_map_forward('p') || US == 1 || cliff == 1 ){

    return 'o'; //Does this mean that this redefines the 'p' values in the array to 'o' values?
  }
  //rock check
  if(check_map_forward('r')){
    return 'r';
  }else{
    return '1';
  }
  
}

void update_map(char letter){   /// Updates the map
//  venusmap[][] = check_forward();
}



char algo_0(int data[3]){               ///// ALGORITHM FIND ROCK (0)
  // INITIALIZE THE MAP BY FILLING IT WITH 0S
  
  int US_check = data[0];
  int IR_obst_check = data[1];
  int IR_rock_check = data[2];
  
  if(check_forward(US_check,IR_obst_check,IR_rock_check) == 'o'){ // change direction
    //right/left
  }
  if(check_forward(US_check,IR_obst_check,IR_rock_check) == 'r'){
    //change mode=1
  }
  if(check_forward(US_check,IR_obst_check,IR_rock_check) == '1'){ // go forward
    //turncount++
  }
}

void setup() {
  Serial.begin(9600);
  print_map();
  digitalWrite(led,LOW);
  
  xbee_init_master();
}

void loop() {
  // MODES:
  // 0 = FIND ROCK
  // 1 = GET ROCK
  // 2 = BACKTRACK
  
  if(receive_data(1000)){
    movement(algo_0(sensor_data()));
  }

}
