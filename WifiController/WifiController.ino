#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

unsigned int localPort = 2390;      // local port to listen for UDP packets
IPAddress timeServerIP; // time.nist.gov NTP server address
const char* ntpServerName = "time.nist.gov";
const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets
bool inside_mol = false;

// A UDP instance to let us send and receive packets over UDP
WiFiUDP udp;

void setup() {
  Serial.begin(115200);

  delay(5000);
  
  Serial.println("Input SSID: ");
  wait_for_response();
  String ssid = Serial.readString();
  Serial.println("Input Passcode: ");
  wait_for_response();
  String pass = Serial.readString();  
    
  // We start by connecting to a WiFi network
  // Debugging - Serial.print("Connecting to ");
  // Debugging - Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    // Debugging - Serial.print(".");
  }
  // Debugging - Serial.println("");

  //Debugging - Serial.println("WiFi connected");
  // Debugging - Serial.println("IP address: ");
  // Debugging - Serial.println(WiFi.localIP());

  // Debugging - Serial.println("Starting UDP");
  udp.begin(localPort);
  // Debugging - Serial.print("Local port: ");
  // Debugging - Serial.println(udp.localPort());
}

void wait_for_response()
{
    while(!Serial.available())
    {
    delay(100);
    }
}

void loop() {  
    if(!inside_mol)
    {
        Serial.println("WiFi connected");
        inside_mol = true;
    }
    if(Serial.available())
    {
      delay(250);
        String cmd = Serial.readString();
        if(cmd == "T")
        {
          //get a random server from the pool
          WiFi.hostByName(ntpServerName, timeServerIP);

          sendNTPpacket(timeServerIP); // send an NTP packet to a time server
          // wait to see if a reply is available
          delay(1000);

          int cb = udp.parsePacket();
          if (!cb) {
            // Debugging - Serial.println("no packet yet");
          } else {
            // Debugging - Serial.print("packet received, length=");
            // Debugging - Serial.println(cb);
            // We've received a packet, read the data from it
            udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

            //the timestamp starts at byte 40 of the received packet and is four bytes,
            // or two words, long. First, esxtract the two words:

            unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
            unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
            // combine the four bytes (two words) into a long integer
            // this is NTP time (seconds since Jan 1 1900):
            unsigned long secsSince1900 = highWord << 16 | lowWord;
            // Debugging - Serial.println(secsSince1900);

            // now convert NTP time into everyday time:
            // Debugging - Serial.print("Unix time = ");
            // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
            const unsigned long seventyYears = 2208988800UL;
            // subtract seventy years:
            unsigned long epoch = secsSince1900 - seventyYears;
            // print Unix time:
            Serial.println(epoch);
          }
        }
        else if(cmd == "M")
        {
        }
    }
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress& address) {
  // Debugging - Serial.println("sending NTP packet...");
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  udp.beginPacket(address, 123); //NTP requests are to port 123
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}
