
  // If water level is high, close the solenoid valve
  if (waterLevel == HIGH) {
    digitalWrite(SOLENOID_VALVE_PIN, LOW);  // Close solenoid valve
    Serial.println("Water level is high. Solenoid valve closed.");
  } else {
    digitalWrite(SOLENOID_VALVE_PIN, HIGH);   // Open solenoid valve
    Serial.println("Water level is low. Solenoid valve open.");
  }

  delay(1000);  // Delay between readings
}