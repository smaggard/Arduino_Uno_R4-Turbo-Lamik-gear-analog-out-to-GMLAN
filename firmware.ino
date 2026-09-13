/*
  Turbo Lamik Gear analog out to GMLAN Transmission General 2 CAN ID
  To allow E38 based GMT900 trucks to have reverse lights, backup camera
  auto switching and auto door unloock.
*/
#include <Arduino_CAN.h>
#include <tuple>
#include <Arduino.h>

void sendCANMsg(float value, uint32_t canId) {
  // Function to send CAN Messages containing the value value provided
  // on the CAN ID provided.

  unsigned char transMessage[8];
  if (value >= 4.76 && value <= 5.1) { // Park
    transMessage[0] = 0x0F;
    transMessage[1] = 0x0F;
    transMessage[3] = 0x01;
    transMessage[6] = 0x03;
  }
  else if (value >= 0.2 && value <= 0.0) { // Reverse
    transMessage[0] = 0x0E;
    transMessage[1] = 0x0E;
    transMessage[3] = 0x02;
    transMessage[6] = 0x02;
  }
  else if (value >= 0.3 && value <= 0.7)  // Neutral
  {
    transMessage[0] = 0x0D;
    transMessage[1] = 0x0D;
    transMessage[3] = 0x03;
    transMessage[6] = 0x03;
  }
  else if (value >= 0.75 && value <= 1.25)  // First
  {
    transMessage[0] = 0x01;
    transMessage[1] = 0x01;
    transMessage[3] = 0x03;
    transMessage[6] = 0x03;
  }
  else if (value >= 1.26 && value <= 1.75)  // Second
  {
    transMessage[0] = 0x02;
    transMessage[1] = 0x02;
    transMessage[3] = 0x03;
    transMessage[6] = 0x03;
  }
  else if (value >= 1.76 && value <= 2.25)  // Third
  {
    transMessage[0] = 0x03;
    transMessage[1] = 0x03;
    transMessage[3] = 0x03;
    transMessage[6] = 0x03;
  }
  else if (value >= 2.26 && value <= 2.75)  // Fourth
  {
    transMessage[0] = 0x04;
    transMessage[1] = 0x04;
    transMessage[3] = 0x03;
    transMessage[6] = 0x03;
  }
  else if (value >= 2.76 && value <= 3.25)  // Fifth
  {
    transMessage[0] = 0x05;
    transMessage[1] = 0x05;
    transMessage[3] = 0x03;
    transMessage[6] = 0x03;
  }
  else if (value >= 3.26 && value <= 3.75)  // Sixth
  {
    transMessage[0] = 0x06;
    transMessage[1] = 0x06;
    transMessage[3] = 0x03;
    transMessage[6] = 0x03;
  }
  else if (value >= 3.76 && value <= 4.25)  // Seventh
  {
    transMessage[0] = 0x07;
    transMessage[1] = 0x0D;
    transMessage[3] = 0x03;
    transMessage[6] = 0x03;
  }
  else if (value >= 4.26 && value <= 4.75)  // Eighth
  {
    transMessage[0] = 0x08;
    transMessage[1] = 0x0D;
    transMessage[3] = 0x03;
    transMessage[6] = 0x03;
  }
  else {
    transMessage[0] = 0x0F;
    transMessage[1] = 0x0F;
    transMessage[3] = 0x04;
    transMessage[6] = 0x01;
  }
  transMessage[2] = 0x00;
  transMessage[4] = 0x00;
  transMessage[5] = 0x00;
  transMessage[7] = 0x00;
  for (int i = 0; i < sizeof(transMessage); i++)
  {
    Serial.print(transMessage[i], HEX);   
  }
  Serial.println();
  CanMsg const msg(CanStandardId(canId), sizeof(transMessage), transMessage);
  int const rc = CAN.write(msg);
  if (rc <= 0) {
    CAN.end();
    CAN.begin(CanBitRate::BR_500k);
  }
  //Serial.println(rc);
  
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial) {
    ;
  }

  //Start CAN
  if (!CAN.begin(CanBitRate::BR_500k)) {
    Serial.println("CAN.begin(...) failed.");
    while (1)
      ;
  } else {
    Serial.println("Can Started.");
  }
}

void loop() {
  // Read the value from the TL gear analog out connected to pin A0
  int sensorValue = analogRead(A0);

  // Convert to 5.0V scale
  float voltage = sensorValue * (5.0 / 1023.0);

  // Send Can messages
  sendCANMsg(voltage, 0x1F5);
  
  // Sleep 25ms to match GM spec
  delay(25);
}
