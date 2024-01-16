#include <WiFi.h>
#include <UniversalTelegramBot.h>
#include "lsm6dsm.h"
#include "WiFiClientSecure.h"
#include "time.h"

#define WIFI_SSID "is that a supraaaa"
#define WIFI_PASS "numan001"
#define BOT_TOKEN "6903697379:AAE0UvntPAhOnq9GdXUnea-jEK1GE-fMPj0"
#define TELEGRAM_CHATID "5099173759"

const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 0;
const int daylightOffset_sec = 3600;
double lastGyroX = NULL;
double sensibility = 0.5;

WiFiClientSecure secured_client;
LSM6DSM IMU;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

void setup()
{
  Serial.begin(115200);
  IMU.begin();
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  configTime(0, 0, "pool.ntp.org");
  time_t now = time(nullptr);
  while (now < 24 * 3600)
  {
    Serial.print(".");
    delay(100);
    now = time(nullptr);
  }
  Serial.println(now);
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void loop()
{
  double nowGyroX = IMU.readFloatGyroX();
  if (lastGyroX == NULL)
  {
    lastGyroX = nowGyroX;
    return;
  }
  if ((nowGyroX - lastGyroX) >= sensibility)
  {
    bot.sendMessage(TELEGRAM_CHATID, "Sensör [2]  titreşim :[" + String((nowGyroX - lastGyroX)) + "]");
    bot.getUpdates(bot.last_message_received + 1);
    delay(500);
  }
  lastGyroX = nowGyroX;
}
