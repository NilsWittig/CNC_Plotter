

//###############################  Servo   ###############################################
void servoMove(int pos){//send servo a position to take
  pwm = (pos * 11) + 500;
  digitalWrite(servo, HIGH);
  delayMicroseconds(pwm);
  digitalWrite(servo, LOW);
  delay(20);
}

void servoSetPos(int deg){//send servo the position a couple of times 
  for(int i = 0; i < 10; i++){
    servoMove(deg);
  }
  
}
//###############################  Stepper X   ###############################################

void stepX(int dirx){
  if(dirx == 1){ steppXB(); }
  else { steppXF(); }
}

void calibrateX(){
  
  while( ! digitalRead(borderPin) ){ //go to border
    steppXF();
    delay(cal_delay);
  }
  while( digitalRead(borderPin) ){ //keep distance to border
    steppXB();
    delay(cal_delay);
  }
  
  rangeX = 0;
  while( ! digitalRead(borderPin) ){// count steps
    steppXB();
    rangeX++;
    delay(cal_delay);
  }
  while( digitalRead(borderPin) ){ //keep distance to border
    steppXF();
    rangeX--;
    delay(cal_delay);
  }
  rangeX = rangeX - ((int)(rangeX * 0.1));// set range (-10%) for safety
  
  currentPosX = rangeX - 10; // set currentPos

  Serial.print("RangeX: ");
  Serial.println(rangeX);

  for(int i = 0; i < ((int)(rangeX * 0.5)); i++){// travel to mid
    steppXF();
    //currentPosX--;
    delay(2);
  }
  delay(1000);
}

void steppXF(){ // -> currentPos--
  //full stepping
  digitalWrite(in1X, full[ steppX     ] );
  digitalWrite(in2X, full[ steppX + 4 ] );
  digitalWrite(in3X, full[ steppX + 8 ] );
  digitalWrite(in4X, full[ steppX + 12 ] );
  if(steppX == 3){
    steppX = 0;
  }else{steppX++;}
  currentPosX--;
  //delay(2);
}


void steppXB(){ // -> currentPos++
  //full stepping
  digitalWrite(in1X, full[ steppX     ] );
  digitalWrite(in2X, full[ steppX + 4 ] );
  digitalWrite(in3X, full[ steppX + 8 ] );
  digitalWrite(in4X, full[ steppX + 12 ] );
  if(steppX == 0){
    steppX = 3;
  }else{steppX--;}
  currentPosX++;
  //delay(2);
}

//###############################  Stepper Y   ###############################################
void stepY(int diry){
  if(diry == 1){ steppYB(); }
  else { steppYF(); }
}

void calibrateY(){
  
  while( ! digitalRead(borderPin) ){ //go to border
    steppYF();
    delay(cal_delay);
  }
  while( digitalRead(borderPin) ){ //keep distance to border
    steppYB();
    delay(cal_delay);
  }
  
  rangeY = 0;
  while( ! digitalRead(borderPin) ){// count steps
    steppYB();
    rangeY++;
    delay(cal_delay);
  }
  while( digitalRead(borderPin) ){ //keep distance to border
    steppYF();
    rangeY--;
    delay(cal_delay);
  }
  rangeY = rangeY - ((int)(rangeY * 0.1));// set range (-10%) for safety
  
  currentPosY = rangeY - 10; // set currentPos

  Serial.print("RangeY: ");
  Serial.println(rangeY);

  for(int i = 0; i < ((int)(rangeY * 0.5)); i++){// travel to mid
    steppYF();
    //currentPosY--;
    delay(2);
  }
  delay(1000);
}

void steppYF(){ // -> currentPos--
  //full stepping
  digitalWrite(in1Y, full[ steppY     ] );
  digitalWrite(in2Y, full[ steppY + 4 ] );
  digitalWrite(in3Y, full[ steppY + 8 ] );
  digitalWrite(in4Y, full[ steppY + 12 ] );
  if(steppY == 3){
    steppY = 0;
  }else{steppY++;}
  currentPosY--;
  //delay(2);
}


void steppYB(){ // -> currentPos++
  //full stepping
  digitalWrite(in1Y, full[ steppY     ] );
  digitalWrite(in2Y, full[ steppY + 4 ] );
  digitalWrite(in3Y, full[ steppY + 8 ] );
  digitalWrite(in4Y, full[ steppY + 12 ] );
  if(steppY == 0){
    steppY = 3;
  }else{steppY--;}
  currentPosY++;
  //delay(2);
}
