#include <SPI.h>
#include <Ethernet.h>
#include <LCD_I2C.h> // Library: LCD_I2C by BlackHack Version 2.2.1

byte mac[] = {
  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 // Change by any custom or random one if needed
};
// remote website domains setup
static byte LOOKUP_LEN = 3; // must be equal to len of websites[]
const char aws[] PROGMEM = "status.aws.amazon.com";
const char speedtest[] PROGMEM = "speedtest.net";
const char google[] PROGMEM = "www.google.com";
const char* const websites[] PROGMEM = { aws, speedtest, google };
// site_buffer must be equal to the longest url size (so copy from PROGMEM
// works and the entire site domain is retrieved correctly)
char site_buffer[38];

EthernetClient client;
LCD_I2C lcd(0x3F);

void setup() {
  // Initialize led controller
  lcd.begin();
  lcd.backlight();
  lcd.noCursor();

  lcd.print("Initializing...");

  // Initialize ethernet controller
  Ethernet.init(10);

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // start the Ethernet connection:
  Serial.println("Initialize Ethernet with DHCP:");
  if (Ethernet.begin(mac) == 0) {
    lcd.setCursor(0, 1);
    Serial.println("Failed to configure Ethernet using DHCP");
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      lcd.print("EthernetNoHardware");
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    } else if (Ethernet.linkStatus() == LinkOFF) {
      lcd.print("Link OFF");
      Serial.println("Ethernet cable is not connected.");
    } else {
      lcd.print("Unknown error");
    }
    // no point in carrying on, so do nothing forevermore:
    while (true) {
      delay(1000);
    }
  }

  lcd.clear();

  // print your local IP address:
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP());
}

unsigned long next;
unsigned long startTime = 0;
unsigned long elapsedTime = 0;
byte current_site = 0;

void loop() {
  while (true) {
    strcpy_P(site_buffer, (char*)pgm_read_word(&(websites[current_site])));

    Serial.print("connecting to site ");
    Serial.println(site_buffer);
    // if you get a connection, report back via serial:

    startTime = millis();
    int connResult = client.connect(site_buffer, 80);
    if (connResult == 1) {
      elapsedTime = millis() - startTime;
      Serial.print("Remote IP address: ");
      Serial.println(client.remoteIP());
      client.stop();

      lcd.print(site_buffer);
      lcd.setCursor(0, 1);
      lcd.print("ACK: ");
      lcd.print(elapsedTime);
      lcd.print("ms");
    }
    else {
      Serial.println("connection failed");

      lcd.print(site_buffer);
      lcd.setCursor(0, 1);
      lcd.print("FAIL: ");

      switch (connResult) {
        case -1: {
            lcd.print("TIMED_OUT");
            break;
          }
        case -2: {
            lcd.print("INVALID_SERVER");
            break;
          }
        case -3: {
            lcd.print("TRUNCATED");
            break;
          }
        case -4: {
            lcd.print("INVALID_RESPONSE");
            break;
          }
        default: {
            lcd.print("LINK_OFF");
            break;
          }
      }
    }

    current_site = (current_site + 1) % LOOKUP_LEN;
    delay(5000);
    lcd.clear();
  }
}
