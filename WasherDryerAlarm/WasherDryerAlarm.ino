//Code Based on http://www.instructables.com/id/Washer-Dryer-Laundry-Alarm-using-Arudino-SMS-Text-/?ALLSTEPS
const int waitCount = 2;             // wait time in minutes
const float axisSensitivity = 0.1; 
const int asxisPins[] = {A2,A3, A4};
float asxisValue[] = {0, 0, 0};

int moveCount = 0;
int stillCount = 0;

void setup() {
  Serial.begin(9600);      

  for (int i = 0; i < 3; i = i + 1) {
    pinMode(i, INPUT);
  }
  pinMode(13, OUTPUT);
  delay(2000);
}

void loop()
{
  if(isMoving())
  {
    stillCount=0;
    moveCount++;  
  }
  else
  {
    stillCount++;
    moveCount=0;   
  }
  
  Notification();
  storeNewAxisValues();
  delay(1000);
}

void Notification()
{
   Serial.print("Move Count: "); 
   Serial.println(moveCount); 
   
   Serial.print("Still Count: "); 
   Serial.println(stillCount); 

   
   if (moveCount == waitCount)
   {
     Serial.println("Move Notification"); 
     digitalWrite(13, HIGH);
   }

   if (stillCount == waitCount)
   {
      Serial.println("Still Notification"); 
      digitalWrite(13, LOW);
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
  return axisPercentChange(axisNumber) > axisSensitivity;
}

float axisPercentChange(int axisNumber)
{
  return abs(axisChange(axisNumber) / asxisValue[axisNumber] );
}

float axisChange(int axisNumber)
{
  return asxisValue[axisNumber] - readAxis(axisNumber);
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





