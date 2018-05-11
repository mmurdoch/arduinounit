void setup() {
  Serial.begin(115200L);
  while (!Serial) {}
  Serial.println("Test summary: 0 passed, 0 failed, and 0 skipped, out of 0 test(s).");
}

void loop() {
  static uint32_t next = 0;
  if (int32_t(millis()-next) >= 0) {
    Serial.println("run.");
    next += 1000;
  }
}
