
#define va_macro(...) va_sub(-9,##__VA_ARGS__,9)

// Unimportant, just to have something the va_macro can compile to
void va_sub(int first, int last) {
  Serial.print("va_sub(");
  Serial.print(first);
  Serial.print(",");
  Serial.print(last);
  Serial.println(")");    
}

void va_sub(int first, int va1, int last) {
  Serial.print("va_sub(");
  Serial.print(first);
  Serial.print(",");
  Serial.print(va1);
  Serial.print(",");  
  Serial.print(last);
  Serial.println(")");    
}

void va_sub(int first, int va1, int va2, int last) {
  Serial.print("va_sub(");
  Serial.print(first);
  Serial.print(",");
  Serial.print(va1);
  Serial.print(",");
  Serial.print(va2);  
  Serial.print(",");
  Serial.print(last);
  Serial.println(")");    
}

void setup() {
   va_macro(); // compile time error just on ESP 8266
   va_macro(1);
   va_macro(1,2);   
}

void loop() {

}