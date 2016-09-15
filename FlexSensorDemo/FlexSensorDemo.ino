#if defined(ARDUINO) 
SYSTEM_MODE(MANUAL); 
#endif

// PIN EXAMPLE
// EXAMPLE USAGE
int flexSensor = A0;

void setup()
{
  pinMode(flexSensor, INPUT);    // sets pin as input
  Serial.begin(9600);
}

void loop()
{
  int flexPosition;    // Input value from the analog pin.

  flexPosition = analogRead(flexSensor);

  Serial.print("sensor: ");
  Serial.println(flexPosition);
}
