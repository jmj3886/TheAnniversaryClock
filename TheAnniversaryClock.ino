// Display definitions
#define sclk 13
#define mosi 11
#define cs 10
#define rst 9
#define dc 8

// Color definitions
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

// CMD Button definitions
#define READ_CMD_BUTTON_PIN 2
#define SECONDS_IN_18_HOURS 64800

// WIFI definitions
#define WIFI_NETWORK_NAME "NETWORK_NAME"
#define WIFI_NETWORK_PASSCODE "NETWORK_PASSCODE"

// Anniversary definitions
#define ANNIVERSARY_DATE_STRING "Sept. 19, 2020"
#define ANNIVERSARY_DATE_DAY 19
#define ANNIVERSARY_DATE_MONTH 9
#define ANNIVERSARY_DATE_YEAR 2020


#include <Adafruit_GFX.h>
#include <Adafruit_SSD1331.h>
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>


Adafruit_SSD1331 display = Adafruit_SSD1331(cs, dc, rst);

const char * ssid = WIFI_NETWORK_NAME; // your network SSID (name)
const char * pass = WIFI_NETWORK_PASSCODE;  // your network password
unsigned int localPort = 2390;      // local port to listen for UDP packets
IPAddress timeServerIP; // time.nist.gov NTP server address
const char* ntpServerName = "time.nist.gov";
const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets
WiFiUDP udp; // A UDP instance to let us send and receive packets over UDP
bool continued_read_status = false;
unsigned int cmd_timer = 0;
unsigned int ntp_timer = 0;
unsigned int cmd_wait = 0;

void setup()
{
    //Setup Read Cmd Button
    pinmode(READ_CMD_BUTTON_PIN, INPUT);
    
    //Begin Display
    display.begin();
    display.fillScreen(BLACK);
    delay(1000);
    
    //Initialize Wifi
    Serial.begin(115200);
    Serial.println();
    Serial.println();

    // We start by connecting to a WiFi network
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pass);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");

    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    Serial.println("Starting UDP");
    udp.begin(localPort);
    Serial.print("Local port: ");
    Serial.println(udp.localPort());
}

void check_read_cmd()
{
    int status = digitalRead(READ_CMD_BUTTON_PIN);
    if((status == 0) || (continued_read_status))
    {
        read_cmd();
    }
}


void read_cmd()
{

}


void display_cmd_icon()
{
    display.setCursor(20,5);
    display.setTextColor(RED);
    display.setTextSize(2);
    display.print(“Heart”);
}


void display_clock()
{
    if(ntp_timer == 0)
    {
        display.fillScreen(BLACK);

        get_time();

        unsigned int days = 365;
        unsigned int years = 0;
        bool is_from = false;

        //Display Years/Days
        String time_string = ""
        if(years > 0)
        {
            time_string += String(years) + " Years";
        }
        if((years > 0) && (days > 0))
        {
            time_string += " & ";
        }
        if(days > 0)
        {
            time_string += String(days) + " Days";
        }
        display.setCursor(30,5);
        display.setTextColor(BLUE);
        display.setTextSize(2);
        display.print(time_string);

        //Display To/From
        String direction_string = "To"
        if(is_from)
        {
            direction_string = "From"
        }
        display.setCursor(40,5);
        display.setTextColor(BLUE);
        display.setTextSize(2);
        display.print(direction_string);

        //Display Anniversary Date
        display.setCursor(50,5);
        display.setTextColor(BLUE);
        display.setTextSize(2);
        display.print(ANNIVERSARY_DATE_STRING);
    }
}


void get_time()
{
  //get a random server from the pool
  WiFi.hostByName(ntpServerName, timeServerIP);

  sendNTPpacket(timeServerIP); // send an NTP packet to a time server
  // wait to see if a reply is available
  delay(1000);

  int cb = udp.parsePacket();
  if (!cb) {
    Serial.println("no packet yet");
  } else {
    Serial.print("packet received, length=");
    Serial.println(cb);
    // We've received a packet, read the data from it
    udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

    //the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, esxtract the two words:

    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    Serial.print("Seconds since Jan 1 1900 = ");
    Serial.println(secsSince1900);

    // now convert NTP time into everyday time:
    Serial.print("Unix time = ");
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;
    // subtract seventy years:
    unsigned long epoch = secsSince1900 - seventyYears;
    // print Unix time:
    Serial.println(epoch);

    // print the hour, minute and second:
    Serial.print("The UTC time is ");       // UTC is the time at Greenwich Meridian (GMT)
    Serial.print((epoch  % 86400L) / 3600); // print the hour (86400 equals secs per day)
    Serial.print(':');
    if (((epoch % 3600) / 60) < 10) {
      // In the first 10 minutes of each hour, we'll want a leading '0'
      Serial.print('0');
    }
    Serial.print((epoch  % 3600) / 60); // print the minute (3600 equals secs per minute)
    Serial.print(':');
    if ((epoch % 60) < 10) {
      // In the first 10 seconds of each minute, we'll want a leading '0'
      Serial.print('0');
    }
    Serial.println(epoch % 60); // print the second
  }
}


// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress& address) {
  Serial.println("sending NTP packet...");
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


void update_timers()
{
    delay(1000);
    ntp_timer += 1;
    cmd_timer += 1;
    if(ntp_timer >= SECONDS_IN_18_HOURS)
    {
        ntp_timer = 0;
    }
    if(cmd_timer >= cmd_wait)
    {
        cmd_timer = 0;
    }
}


// Main Operating Loop
void loop()
{
    check_read_cmd();
    display_cmd_icon();
    display_clock();
    update_timers();
}
