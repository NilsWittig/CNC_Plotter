#define BAUD (9600)
#define MAX_BUF (86)
#define CM_PER_SEGMENT (0.1)

char buffer[MAX_BUF];
int sofar; //how much is in the buffer

int mode_abs = 0;

bool incomming = false;

//############### plotter variables ##################################
char full[] = {1,0,0,1,
               1,1,0,0,
               0,1,1,0,
               0,0,1,1};
int in1X = 6;
int in2X = 7;
int in3X = 4;
int in4X = 5;

int in1Y = 10;
int in2Y = 11;
int in3Y = 12;
int in4Y = 13;

int steppX = 0; 
int steppY = 0; 

//Calibration
int borderPin = 9;
int rangeX = 0;
int rangeY = 0;
int currentPosX = 0;
int currentPosY = 0;


//servo Variablen
int servo = 8;
int pwm;
int positi = 90;
int servoUP = 100;
int servoDOWN = 70;

//maxFeedRate = 1034;

int step_delay = 4000;
int cal_delay = 4;

//#########################################################################

void setup() {

  pinMode(in1X, OUTPUT);digitalWrite(in1X, LOW);
  pinMode(in2X, OUTPUT);digitalWrite(in2X, LOW);
  pinMode(in3X, OUTPUT);digitalWrite(in3X, LOW);
  pinMode(in4X, OUTPUT);digitalWrite(in4X, LOW);

  pinMode(in1Y, OUTPUT);digitalWrite(in1Y, LOW);
  pinMode(in2Y, OUTPUT);digitalWrite(in2Y, LOW);
  pinMode(in3Y, OUTPUT);digitalWrite(in3Y, LOW);
  pinMode(in4Y, OUTPUT);digitalWrite(in4Y, LOW);

  pinMode(servo, OUTPUT);
  digitalWrite(servo, LOW);

  pinMode(borderPin, INPUT);

  Serial.begin(BAUD);
  //help();
  Serial.println("Calibrating... ");

  servoSetPos(servoUP);
  calibrateX();
  calibrateY();
  

  //set_feedrate(200); // set default speed
  //Serial.println("Grbl 1.0d ['$' for help]");//trick UGS and grbl 
  
  ready();
}



/**
 * After setup() this machine will repeat loop() forever.
 */
void loop() { 
  // listen for commands
  
  char c = Serial.read(); // get it
  if( c == '@' ){ incomming = !incomming; }
  delay(10);
  if( incomming ){
    //Serial.print(c); // optional: repeat back what I got for debugging
  
    // store the byte as long as there's room in the buffer.
    // if the buffer is full some data might get lost
    if(sofar < MAX_BUF) buffer[sofar++]=c;
    // if we got a return character (\n) the message is done.
    if(c=='\n') {
      buffer[sofar] = 0;
      Serial.println(buffer); // optional: send back a return for debugging
  
      // strings must end with a \0.
      buffer[sofar]=0;
      processCommand(); // do something with the command
      ready();
      }
  }
  
}
