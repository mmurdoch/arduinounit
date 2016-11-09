#line 2 "filter.ino"
#include <ArduinoUnit.h>

// empty tests just to play with
test(net_tcp) { pass(); }
testing(net_udp) { pass(); }
test(net_ftp) { pass(); }
test(crypto_aes) { pass(); }
testing(crypto_rng) { pass(); }
test(crypto_sha256) { pass(); }

void setup()
{
  Serial.begin(9600);
  while(!Serial); // for the Arduino Leonardo/Micro only
  
  // all tests named net_ - something, except net_ftp
  Test::exclude("*");
  Test::include("net_*");
  Test::exclude("net_ftp");
}

void loop()
{
  Test::run();
}
