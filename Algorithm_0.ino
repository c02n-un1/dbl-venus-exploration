#include <Arduino.h>
#define grid_size 12

int mode = 0;
int loop_count = 0;

int double_turn = 0;
int turncount = 0;
int rocks_collect;
int tot_rocks=5;
int directionnow=-1; // N = 1; S = -1; E = 2; W = 2
int x = 0;
int y = 0;
int x_max = grid_size;
int y_max = grid_size;
String str;
String venusmap[grid_size] = {  "oooooooooooo",
                                "o    s     o",
                                "o          o",
                                "o    o     o",
                                "o          o",
                                "o         oo",
                                "o          o",
                                "o          o",
                                "oo     or  o",
                                "o          o",
                                "o          o",
                                "oooooooooooo"}; 

// SIMULATION OF SENSOR DATA & MOVEMENT FUNCTION
int sensor[3] = {0,0,0};
boolean init_algo2 = true;

boolean receive_data(int t_stop) {
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

void sensor_data(){
  sensor[0] = -1;
  sensor[1] = -1;
  sensor[2] = -1;
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
}

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

void movement(char a){    // PRINT OUTPUT ALGORITHM

  Serial.print("\n\n\n---------------------------------------\n");
  Serial.print("POSITION:   ");
  Serial.print("x = ");
  Serial.print(x);
  Serial.print("   Y = ");
  Serial.print(y);
  Serial.print("   DIREC: ");
  Serial.print(dirchar(directionnow));
  Serial.print("\n");

  Serial.print("OUTPUT ALGORITHM:         ");
  Serial.print(a);
  Serial.print("\n");

  Serial.print("operation nr: ");
  Serial.print(loop_count);
  Serial.print("\t\tturncount: ");
  Serial.print(turncount);
  Serial.print("\n");
  Serial.print("---------------------------------------\n");
}

void print_map(){         // PRINT MAP
 
  Serial.print(" _");
  for(int a = 0; a<grid_size-2 ; a++){
    Serial.print("____");
  }
  Serial.print("\n");
  
  for(int i = 1; i<grid_size-1 ; i++){
    Serial.print(" | ");
    for(int j = 1; j<grid_size-1 ; j++){
      Serial.print(venusmap[i][j]);
      Serial.print(" | ");
    }
    Serial.print("\n");
    
    Serial.print(" _");
    for(int a = 0; a<grid_size-2 ; a++){
      Serial.print("____");
    }
    Serial.print("\n");
  }
  Serial.print("\n");
  Serial.print("\n");
  Serial.print("\n");
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
  movement('0');
  print_map();
}

// Map Functions

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
    // check whether 180 turn should be done or not
    double_turn++; //incrementing double turn
    switch(dirchar(directionnow)){
      default:
        return '\0';
        break;
      case 'N':
        //turncheck() checks if number of turns taken is even or odd
        if(turncheck(turncount)){
          directionnow = 2; //robot is facing east
          return 'r'; //right turn
        }else{
          directionnow = -2; //robot is facing west
          return 'l'; //left turn
        }
        break;
      case 'S':
        if(turncheck(turncount)){
          directionnow = -2;
          return 'r';
        }else{
          directionnow = 2; //robot is facing east
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
  
  int US_check = 0;
  int IR_obst_check = 0;
  int IR_rock_check = 0;

  char object = check_map('0',1);

  
  if(object == 'o' || object == 'p' || US_check == 1 || IR_obst_check == 1){ // change direction
    if(object != 'p'){
      update_map('0','o',1);
    }
    
    char turn_direct = algodirection_0();
    // if(turn_direct != 'b'){
      turncount++;
    // }

    return turn_direct;
  }else if(object == 'r' || IR_rock_check == 1){
    loop_count = 0;
    Serial.print("\n\n\n\n-----------------------------\nRock Found!!!\nReturn to base sequence started!!!\n-----------------------------\n");
    mode = 2;
    movement('\0');
    print_map();
    return '\0';
  }else{ // go forward
    double_turn=0;
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
      Serial.print("algo 0 engaged\n");
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
      Serial.print("algo 0 engaged\n");
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
    
    Serial.print("\n\n\n\n-----------------------------\nStart Found!!!\nReturn to base sequence SUCESSFUL!!!\n-----------------------------\n");
    mode = 1;
    movement('\0');
    print_map();
    
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

  set_start();
}

void loop() {
  // MODES:
  // 0 = FIND ROCK
  // 1 = GET ROCK
  // 2 = BACKTRACK
  
//  receive_data(100)
  // delay(5000);
  
  if(mode == 0){
    char algo_out = algo_0();
    // movement(algo_out);

    if(loop_count>75){
      movement(algo_out);
      print_map();
      
      mode = 3;
    }

    loop_count++;
  }
}
