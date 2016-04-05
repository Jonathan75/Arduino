//Code Based on http://www.instructables.com/id/Washer-Dryer-Laundry-Alarm-using-Arudino-SMS-Text-/?ALLSTEPS
const int ledPin = 7;
const long waitCount = 60*10;
const float axisSensitivity = 0.1; 
const int asxisPins[] = {A0,A1, A2};
float asxisValue[] = {0, 0, 0};

long moveCount = 0;
long stillCount = waitCount+1;

void setup() {
  Serial.begin(9600);      

  for (int i = 0; i < 3; i = i + 1) {
    pinMode(i, INPUT);
  }
  pinMode(ledPin, OUTPUT);
  delay(2000);
}

void loop()
{
  if(isMoving())
  {
    digitalWrite(ledPin, HIGH);  
    moveCount++;  
  }
  else
  {
    stillCount++;
  }
  
  Notification();
  storeNewAxisValues();
  delay(1000);
  digitalWrite(ledPin, LOW);
}

void Notification()
{
  Serial.print("Move Count: "); 
  Serial.println(moveCount); 
   
//   Serial.print("Still Count: "); 
//   Serial.println(stillCount); 

   if (moveCount == waitCount)
   {
     Serial.println("Move Notification"); 
    //  digitalWrite(ledPin, HIGH);
     stillCount=0;
     Particle.publish("Wash_Status","Moving");
   }

   if (stillCount == waitCount)
   {
      Serial.println("Still Notification"); 
    //   digitalWrite(ledPin, LOW);
      moveCount=0; 
      Particle.publish("Wash_Status","Still");
   }
}

bool isMoving()
{
  for (int i = 0; i < 3; i = i + 1) {    
    if(isAxisMoving(i))
      return true;
  } 
  return false;
}

bool isAxisMoving(int axisNumber)
{
  Serial.print("Axis % Change: "); 
  Serial.println(axisPercentChange(axisNumber));
  return axisPercentChange(axisNumber) > axisSensitivity;
}

float axisPercentChange(int axisNumber)
{
  return abs(axisChange(axisNumber) / asxisValue[axisNumber]);
}

float axisChange(int axisNumber)
{
  return (asxisValue[axisNumber] - readAxis(axisNumber)) * 100;
}

float readAxis(int axisNumber)
{
  return analogRead(asxisPins[axisNumber]);
}

void storeNewAxisValues()
{
  for (int i = 0; i < 3; i = i + 1) {    
    asxisValue[i] = readAxis(i);
  }
}
