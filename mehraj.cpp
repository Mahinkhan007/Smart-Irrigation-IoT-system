// Blynk function to control the water pump manually
BLYNK_WRITE(V3) {
  int pumpState = param.asInt();
  digitalWrite(PUMP_PIN, pumpState);
  if (pumpState == HIGH) {
    Serial.println("Manual control: Water pump ON");
  } else {
    Serial.println("Manual control: Water pump OFF");
  }
}