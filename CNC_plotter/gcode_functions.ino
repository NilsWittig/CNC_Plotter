
/** * delay for the appropriate number of microseconds * @input ms how many milliseconds to wait */
void pause(long ms) {
  delay(ms/1000); //delayMicroseconds(ms%1000); // delayMicroseconds doesn't work for values > ~16k. } 
}


int exp10(int exponent){
  if(exponent == 0){ return 1; }
  int ex = 1;
  for(int i = 1; i <= exponent; i++){
    ex = ex * 10;
  }
  return ex;
}

int parsenumber(char key, int d){
  for(int i = 0; i < sofar; i++){
    if(buffer[i] == key){
      i++;
      int parsed = 0;
      int pars[10];
      int digit = 0;
      bool neg = false;
      i++;
      bool end = true;
      while(end){
        switch(buffer[i]){
          case 48: { pars[digit++] = 0; i++; break; }
          case 49: { pars[digit++] = 1; i++; break; }
          case 50: { pars[digit++] = 2; i++; break; }
          case 51: { pars[digit++] = 3; i++; break; }
          case 52: { pars[digit++] = 4; i++; break; }
          case 53: { pars[digit++] = 5; i++; break; }
          case 54: { pars[digit++] = 6; i++; break; }
          case 55: { pars[digit++] = 7; i++; break; }
          case 56: { pars[digit++] = 8; i++; break; }
          case 57: { pars[digit++] = 9; i++; break; }
          case 45: { neg = true; i++; break; }
          default: { end = false; break; }
        }
      }
      for(int j = 0; j < digit; j++){
        parsed += pars[j] * exp10( (digit - 1) - j);
      }
      if(neg){ parsed = 0 - parsed;}
      Serial.print(F("found: "));
      Serial.println(parsed);
      Serial.print(F("after: "));
      Serial.println(key);
      return parsed;
    }
  }
  return d;
}

/**
 * Uses Bresenham's line algorithm to move both motors
 * @input newx the destination x position
 * @input newy the destination y position
 **/
void line(float newx,float newy) {
  long dx=newx-currentPosX; // distance to move (delta)
  long dy=newy-currentPosY;
  int dirx=dx > 0?1:-1; // direction to move
  int diry=dy > 0?1:-1;
  dx=abs(dx); // absolute delta
  dy=abs(dy);

  long i;
  long over=0;
  
  if(dx > dy) {
    for(i=0;i < dx;++i) {
      stepX(dirx);
      over+=dy;
      if(over>=dx) {
        over-=dx;
        stepY(diry);
      }
      pause(step_delay); // step_delay is a global connected to feed rate.
      // test limits and/or e-stop here
    }
  } else {
    for(i=0;i < dy;++i) {
      stepY(diry);
      over+=dx;
      if(over>=dy) {
        over-=dy; 
        stepX(dirx); 
      } 
      pause(step_delay); // step_delay is a global connected to feed rate. 
      // test limits and/or e-stop here
      // update the logical position. We don't just = newx because 
      // px + dx * dirx == newx could be false by a tiny margin and we don't want rounding errors. px+= dx*dirx; py+= dy*diry; } 
      
    }
  }
  
}


/**
 * Read the input buffer and find any recognized commands. One G or M command per line.
 */
void processCommand() {
  // look for commands that start with 'G'
  int cmd=parsenumber('G',-1);
  Serial.print("\n found G");
  Serial.print(cmd);
  Serial.print("\n");
  switch(cmd) {
  case 0: {// move in a line
    
    //set_feedrate(parsenumber('F',fr));
    if( parsenumber('Z',1) <= 0 ) { servoSetPos(servoDOWN); Serial.println( "writing..."); }
    else { servoSetPos(servoUP); Serial.println( "floating..."); }
    line( parsenumber('X',currentPosX), parsenumber('Y',currentPosY) );
    break;}
  case 1: {// move in a line
    //set_feedrate(parsenumber('F',fr));
    if( parsenumber('Z',1) <= 0 ) { servoSetPos(servoDOWN); Serial.println( "writing..."); }
    else { servoSetPos(servoUP); Serial.println( "floating..."); }
    line( parsenumber('X',currentPosX), parsenumber('Y',currentPosY) );
    break;}
  case 2: {if( parsenumber('Z',1) <= 0 ) { servoSetPos(servoDOWN); Serial.println( "writing..."); }
          else { servoSetPos(servoUP); Serial.println( "floating..."); }
          arc(parsenumber('X',currentPosX), 
              parsenumber('Y',currentPosY), 
              parsenumber('I',currentPosX), 
              parsenumber('J',currentPosY),
              1.0);// clockwise arc
          break;}
  case 3: {if( parsenumber('Z',1) <= 0 ) { servoSetPos(servoDOWN); Serial.println( "writing..."); }
          else { servoSetPos(servoUP); Serial.println( "floating..."); }
          arc(parsenumber('X',currentPosX), 
              parsenumber('Y',currentPosY), 
              parsenumber('I',currentPosX), 
              parsenumber('J',currentPosY),
              -1.0);// counter-clockwise arc
          break;}
  case 4: pause(parsenumber('P',0)*1000); break; // wait a while
  case 90: mode_abs=1; break; // absolute mode
  case 91: mode_abs=0; break; // relative mode
  case 92: // set logical position
    //position( parsenumber('X',0), parsenumber('Y',0) );
    break;
  default: break;
  }
  /*
  // look for commands that start with 'M'
  cmd=parsenumber('M',-1);
  switch(cmd) {
  case 18: // turns off power to steppers (releases the grip)
    m1.release();
    m2.release();
    break;
  case 100: help(); break;
  case 114: where(); break; // prints px, py, fr, and mode.
  default: break;
  }
  */
  // if the string has no G or M commands it will get here and the Arduino will silently ignore it
}


/**
 * display helpful information
 */
void help() {
  Serial.print(F("CNC Robot "));
  //Serial.println(VERSION);
  Serial.println(F("Commands:"));
  Serial.println(F("G00 [X(steps)] [Y(steps)] [F(feedrate)]; - linear move"));
  Serial.println(F("G01 [X(steps)] [Y(steps)] [F(feedrate)]; - linear move"));
  Serial.println(F("G04 P[seconds]; - delay"));
  //Serial.println(F("G90; - absolute mode"));
  //Serial.println(F("G91; - relative mode"));
  //Serial.println(F("G92 [X(steps)] [Y(steps)]; - change logical position"));
  //Serial.println(F("M18; - disable motors"));
  Serial.println(F("M100; - this help message"));
  //Serial.println(F("M114; - report position and feedrate"));
  Serial.print(F("CurrentPosX: "));
  Serial.println(currentPosX);
  Serial.print(F("CurrentPosY: "));
  Serial.println(currentPosY);
}

/**
 * prepares the input buffer to receive a new message and 
 * tells the serial connected device it is ready for more.
 */
void ready() {
  sofar=0; // clear input buffer
  Serial.print(F("CurrentPosX: "));
  Serial.println(currentPosX);
  Serial.print(F("CurrentPosY: "));
  Serial.println(currentPosY);
  Serial.println("\n");
  Serial.println(F(">")); // signal ready to receive input
}


/** * Set the feedrate (speed motors will move) * @input nfr the new speed in steps/second */
void set_feedrate(float nfr) {
  /*
  if(fr==nfr) return; // same as last time? quit now.
  if(nfr > MAX_FEEDRATE || nfr < MIN_FEEDRATE) { // don't allow crazy feed rates
    Serial.print(F("New feedrate must be greater than "));
    Serial.print(MIN_FEEDRATE);
    Serial.print(F("steps/s and less than "));
    Serial.print(MAX_FEEDRATE);
    Serial.println(F("steps/s."));
    return;
  }
  step_delay = 1000000.0/nfr;
  fr=nfr;
  */
}

// This method assumes the limits have already been checked.
// This method assumes the start and end radius match (both points are same distance from center)
// This method assumes arcs are not >180 degrees (PI radians)
// This method assumes all movement is at a constant Z height
// cx/cy - center of circle
// x/y - end position
// dir - ARC_CW or ARC_CCW to control direction of arc
// (posx,posy,posz) is the starting position
// line() is our existing command to draw a straight line using Bresenham's algorithm.
void arc(float cx,float cy,float x,float y,float dir) {
  // get radius
  float dx = currentPosX - cx;
  float dy = currentPosY - cy;
  float radius=sqrt(dx*dx+dy*dy);

  // find the sweep of the arc
  float angle1 = atan3(dy,dx);
  float angle2 = atan3(y-cy,x-cx);
  float sweep = angle2-angle1;

  if(dir>0 && sweep<0) angle2+=2*PI;
  else if(dir<0) angle1+=2*PI;

  sweep=angle2-angle1;

  // get length of arc
  // float circumference=PI*2.0*radius;
  // float len=sweep*circumference/(PI*2.0);
  // simplifies to
  float len = abs(sweep) * radius;

  int i, num_segments = floor( len / CM_PER_SEGMENT );

  // declare variables outside of loops because compilers can be really dumb and inefficient some times.
  float nx, ny, nz, angle3, fraction;

  for(i=0;i<num_segments;++i) {
    // interpolate around the arc
    fraction = ((float)i)/((float)num_segments);
    angle3 = ( sweep * fraction ) + angle1;

    // find the intermediate position
    nx = cx + cos(angle3) * radius;
    ny = cy + sin(angle3) * radius;
    // make a line to that intermediate position
    line(nx,ny);
  }

  // one last line hit the end
  line(x,y);
}

// returns angle of dy/dx as a value from 0...2PI
float atan3(float dy,float dx) {
  float a=atan2(dy,dx);
  if(a<0) a=(PI*2.0)+a;
  return a;
}
