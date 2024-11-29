#include <Arduino.h>

void controlPump(bool state) {
  if (state) {
    digitalWrite(15, HIGH); // Bật bơm (ví dụ: chân 15)
  } else {
    digitalWrite(15, LOW);  // Tắt bơm
  }
}
