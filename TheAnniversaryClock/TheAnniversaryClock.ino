#include <Adafruit_GFX.h>
#include <Adafruit_SSD1331.h>
#include <SPI.h>
#include <SoftwareSerial.h>

// Display definitions
#define sclk 13
#define mosi 11
#define cs   10
#define rst  9
#define dc   8

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
#define ANNIVERSARY_EPOCH 1600488000

Adafruit_SSD1331 display = Adafruit_SSD1331(cs, dc, rst);
const unsigned char HMail_map [] PROGMEM = {
 0x00, 0x00, 0x38, 0x38, 0x7e, 0xfc, 0x3f, 0xf8, 0x1f, 0xf0, 0x8f, 0xe2, 0xc7, 0xc6, 0xf1, 0x1e, 
 0xf8, 0x3e, 0xfe, 0xfe
};
const unsigned char H_map [] PROGMEM = {
  0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xc0, 0x00, 0x00, 0x00, 0x1e, 0x00, 0xf0, 0x00, 
  0x00, 0x03, 0xc0, 0x1c, 0x00, 0x00, 0x00, 0x70, 0x00, 0x0e, 0x00, 0x00, 0x1c, 0x00, 0x03, 0x80, 
  0x00, 0x01, 0x80, 0x00, 0x01, 0x00, 0x00, 0x60, 0x00, 0x00, 0x60, 0x00, 0x06, 0x00, 0x00, 0x00, 
  0xc0, 0x01, 0x80, 0x00, 0x00, 0x30, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x30, 0x03, 0x00, 0x00, 0x00, 
  0x08, 0x00, 0x18, 0x00, 0x00, 0x00, 0x18, 0x04, 0x00, 0x00, 0x00, 0x04, 0x00, 0x10, 0x00, 0x00, 
  0x00, 0x0c, 0x08, 0x00, 0x00, 0x00, 0x02, 0x00, 0x20, 0x00, 0x00, 0x00, 0x06, 0x10, 0x00, 0x00, 
  0x00, 0x03, 0x00, 0x60, 0x00, 0x00, 0x00, 0x03, 0x20, 0x00, 0x00, 0x00, 0x01, 0x00, 0x40, 0x00, 
  0x00, 0x00, 0x01, 0x60, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 
  0x00, 0x00, 0x00, 0x80, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 
  0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 
  0x80, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x20, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x02, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x18, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x0c, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x02, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x20, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 
  0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 
  0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 
  0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 
  0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 
  0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0xe0, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x38, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x18, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xc0, 0x01, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x06, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x03, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x80, 0x00, 0x00, 0x00, 0x00, 0x00
};

bool continued_read_status = false;
bool cmd_read_status = false;
bool waiting_cmd = true;
unsigned int cmd_timer = 0;
unsigned int ntp_timer = 0;
unsigned int cmd_wait = 0;
unsigned int msg_timer = 0;
SoftwareSerial wifi_controller(3,4);

void setup()
{
    //Setup Read Cmd Button
    pinMode(READ_CMD_BUTTON_PIN, INPUT);
    attachInterrupt(digitalPinToInterrupt(READ_CMD_BUTTON_PIN), set_read_cmd, HIGH);
    
    //Begin Display
    display.begin();
    display.fillScreen(BLACK);
    delay(1000);
    
    //Initialize Wifi
    Serial.begin(9600);
    Serial.println("Initialize Wifi");
    wifi_controller.begin(115200);
    wait_for_wifi_response();
    wifi_controller.readString();
    wifi_controller.print(WIFI_NETWORK_NAME);
    Serial.println("Sent SSID");
    wait_for_wifi_response();
    wifi_controller.readString();
    wifi_controller.print(WIFI_NETWORK_PASSCODE);
    Serial.println("Sent Passcode");
    wait_for_wifi_response();
    wifi_controller.readString();
}

void wait_for_wifi_response()
{
    while(!wifi_controller.available())
    {
    delay(250);
    }
}

void check_read_cmd()
{
    if((cmd_read_status) || (continued_read_status))
    {
        cmd_read_status = false;
        read_cmd();
    }
}

void set_read_cmd()
{
    cmd_read_status = true;
}

void read_cmd()
{
    if(cmd_timer == 0)
    {
        //               READ_NEXT_CMD      WAIT_TILL_NEXT_CMD  OPERATION                     OPERATION_MSG
        // cmd message = [S=Stop/C=Continue][00-60=SecondsToWait][F=Fill/M=Message/H=Heartbeat][...]
        // Fill message = [0x0=HexColorDef]
        // Message message = [TextToDisplay]
        // Heartbeat message = [0-60=SecondsToBeatFor]
        String cmd = "S10H60";
        waiting_cmd = false;
        continued_read_status = false;
        cmd_wait = 0;
        if(cmd.charAt(0) == 'C')
        {
            continued_read_status = true;
            cmd_wait = cmd.substring(1,3).toInt();
        }
        else
        {
            ntp_timer = 0;
        }
        switch(cmd.charAt(3))
        {
            case 'F':
            {
                display.fillScreen(strtoul(cmd.substring(4,cmd.length()).c_str(), NULL, 16));
                break;
            }
            case 'M':
            {
                unsigned int char_pos = 4;
                while(char_pos < cmd.length())
                {
                    display.fillScreen(BLACK);
                    int line_pos = 1;
                    while((line_pos < 6) && (char_pos < cmd.length()))
                    {
                        String message = cmd.substring(char_pos, cmd.length());
                        if(message.length() > 14)
                        {
                            int end_pos = 16;
                            do{
                                end_pos--;
                                message = message.substring(0, end_pos);
                            } while(message.charAt(end_pos - 1) != ' ');
                            char_pos += end_pos;
                        }
                        else
                        {
                            char_pos = cmd.length();
                        }
                        message.trim();
                        display.setTextColor(MAGENTA);
                        display.setTextSize(1);
                        display.setCursor((96 - (message.length() * 5.8)) / 2, (10 * line_pos));
                        display.println(message);
                        line_pos++;
                    }
                    delay(10000);
                }
                break;
            }
            case 'H':
            {
                display.fillScreen(BLACK);
                int heartbeat_timer = 0;
                while(heartbeat_timer < cmd.substring(4,cmd.length()).toInt())
                {
                    display.drawBitmap(7, 0, H_map, 82, 50, RED);
                    delay(500);
                    display.fillScreen(BLACK);
                    delay(500);
                    display.drawBitmap(7, 0, H_map, 82, 50, RED);
                    delay(250);
                    display.fillScreen(BLACK);
                    delay(250);
                    display.drawBitmap(7, 0, H_map, 82, 50, RED);
                    delay(250);
                    display.fillScreen(BLACK);
                    delay(250);
                    heartbeat_timer += 2;               
                }
                break;
            }
        }
    }
}

bool check_waiting_cmd()
{
  return waiting_cmd;
}

void display_cmd_icon()
{
    if(check_waiting_cmd() && (!continued_read_status))
    {
        if(msg_timer == 0){
            display.drawBitmap(0, 0, HMail_map, 15,10, RED);
        }
        else
        {
            display.drawBitmap(0, 0, HMail_map, 15,10, BLACK);
            display.drawBitmap(7, 0, H_map, 82, 50, RED);
        }
    }
}


void display_clock()
{
    if((ntp_timer == 0) && (!continued_read_status))
    {
        display.fillScreen(BLACK);
        display.drawBitmap(7, 0, H_map, 82, 50, RED);

        uint32_t current_epoch = get_time();
        uint32_t time_diff = get_time_diff(current_epoch);

        unsigned int days = get_days(time_diff);
        float years = get_years(time_diff);
        String days_string = "";
        String years_string = "";
        bool is_from = get_time_direction(current_epoch);
        unsigned int x_pos = 0;

        //Display Years/Days
        String time_string = "";  
        if(years >= 1)
        {
            years_string = String(years, 1);
            time_string += years_string + " Years";
        }
        else if(years < 1)
        {
            days_string = String(days);
            time_string += days_string + " Days";
        }   
        if(days_string.length() == 3)
        {
            x_pos = 25;
        }
        else if(days_string.length() == 2)
        {
            x_pos = 28;
        }
        else if(days_string.length() == 1)
        {
            x_pos = 30;
        }
        else if(years_string != "")
        {
            x_pos = 22;
        }
        display.setCursor(x_pos,17);
        display.setTextColor(MAGENTA);
        display.setTextSize(1);
        display.print(time_string);

        //Display To/From
        String direction_string = "To";
        x_pos = 40;
        if(is_from)
        {
            x_pos = 36;
            direction_string = "From";
        }
        display.setCursor(x_pos,31);
        display.setTextColor(MAGENTA);
        display.setTextSize(1);
        display.print(direction_string);

        //Display Anniversary Date
        display.setCursor(5, 53);
        display.setTextColor(MAGENTA);
        display.setTextSize(1);
        display.print(ANNIVERSARY_DATE_STRING);
    }
}

unsigned int get_days(uint32_t time_diff)
{
    return ceil(time_diff / 60.0 / 60.0 / 24.0);
}

float get_years(uint32_t time_diff)
{
    return time_diff / 60.0 / 60.0 / 24.0 / 365.25;
}

uint32_t get_time()
{
  wifi_controller.print("T");
  wait_for_wifi_response();
  String message = wifi_controller.readString();
  Serial.println("*");
  Serial.println(String("|")+message+String("|"));
  Serial.println("*");
  uint32_t current_epoch = strtoul(message.c_str(), NULL, 10);
  return current_epoch;
}

bool get_time_direction(uint32_t current_epoch)
{
    bool is_from = false;
    if(ANNIVERSARY_EPOCH < current_epoch)
    {
        is_from = true;
    }
    return is_from;
}

uint32_t get_time_diff(uint32_t current_epoch)
{
    uint32_t time_diff = 0;
    if(ANNIVERSARY_EPOCH > current_epoch)
    {
        time_diff = ANNIVERSARY_EPOCH - current_epoch;
    }
    else
    {
        time_diff = current_epoch - ANNIVERSARY_EPOCH;
    }
    return time_diff;
}

void update_timers()
{
    delay(1000);
    ntp_timer += 1;
    cmd_timer += 1;
    msg_timer += 1;
    if(ntp_timer >= SECONDS_IN_18_HOURS)
    {
        ntp_timer = 0;
    }
    if(cmd_timer >= cmd_wait)
    {
        cmd_timer = 0;
    }
    if(msg_timer == 2)
    {
      msg_timer = 0;
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
