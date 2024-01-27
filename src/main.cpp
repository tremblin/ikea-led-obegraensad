#include <Arduino.h>
#include <SPI.h>

#ifdef ESP32
#include <ESPmDNS.h>
#endif
#ifdef ESP8266
#include <ESP8266WiFi.h>
#endif

#include "PluginManager.h"

#include "plugins/BreakoutPlugin.h"
#include "plugins/CirclePlugin.h"
#include "plugins/DrawPlugin.h"
#include "plugins/FireworkPlugin.h"
#include "plugins/GameOfLifePlugin.h"
#include "plugins/LinesPlugin.h"
#include "plugins/RainPlugin.h"
#include "plugins/SnakePlugin.h"
#include "plugins/StarsPlugin.h"
#include "plugins/PongClockPlugin.h"
#include "plugins/GroupPlugin.h"
#include "plugins/TestPlugin.h"

#include "fades/NoneFade.h"
#include "fades/SnakeFade.h"
#include "fades/RainFade.h"
#include "fades/HorizontalWashFade.h"
#include "fades/VerticalWashFade.h"
#include "fades/CrashFade.h"

#ifdef ENABLE_SERVER
#include "plugins/AnimationPlugin.h"
#include "plugins/BigClockPlugin.h"
#include "plugins/MediumClockPlugin.h"
#include "plugins/ClockPlugin.h"
#include "plugins/WeatherPlugin.h"
#include "plugins/AnimationPlugin.h"
#include "plugins/TickingClockPlugin.h"
#include "plugins/HomeAssistantPlugin.h"
#endif

#include "asyncwebserver.h"
#include "ota.h"
#include "screen.h"
#include "secrets.h"
#include "websocket.h"
#include "messages.h"

unsigned long previousMillis = 0;
unsigned long interval = 30000;

PluginManager pluginManager;
SYSTEM_STATUS currentStatus = NONE;

unsigned long lastConnectionAttempt = 0;
const unsigned long connectionInterval = 10000;

#ifdef ENABLE_DIMMING
static std::vector<u_int8_t> dimming = DIMMING;
#endif

#ifdef ESP32
void connectToWiFi()
{
  Serial.println("Connecting to Wi-Fi...");

  // Delete old config
  WiFi.disconnect(true);

  WiFi.setHostname(WIFI_HOSTNAME);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  // Wait for connection
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20)
  {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  // Check connection result
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());
  }
  else
  {
    Serial.println("\nFailed to connect to Wi-Fi. Please check credentials.");
  }

  lastConnectionAttempt = millis();
}
#endif

#ifdef ESP8266
void connectToWiFi()
{
  Serial.println("Connecting to Wi-Fi...");

  // Delete old config
  WiFi.disconnect(true);

#if defined(IP_ADDRESS) && defined(GWY) && defined(SUBNET) && defined(DNS1) && \
    defined(DNS2)
  auto ip = IPAddress();
  ip.fromString(IP_ADDRESS);

  auto gwy = IPAddress();
  gwy.fromString(GWY);

  auto subnet = IPAddress();
  subnet.fromString(SUBNET);

  auto dns1 = IPAddress();
  dns1.fromString(DNS1);

  auto dns2 = IPAddress();
  dns2.fromString(DNS2);

  WiFi.config(ip, gwy, subnet, dns1, dns2);
#endif

  WiFi.setHostname(WIFI_HOSTNAME);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  // Wait for connection
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20)
  {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  // Check connection result
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());
  }
  else
  {
    Serial.println("\nFailed to connect to Wi-Fi. Please check credentials.");
  }

  lastConnectionAttempt = millis();
}
#endif

void setup()
{
  Serial.begin(115200);

  pinMode(PIN_LATCH, OUTPUT);
  pinMode(PIN_CLOCK, OUTPUT);
  pinMode(PIN_DATA, OUTPUT);
  pinMode(PIN_ENABLE, OUTPUT);
  pinMode(PIN_BUTTON, INPUT_PULLUP);

  Screen.setup();

// server
#ifdef ENABLE_SERVER
  Serial.println("Starting Wi-Fi...");
  connectToWiFi();
  Serial.println("Starting time sync...");
  configTzTime(TZ_INFO, NTP_SERVER);
  Serial.println("Starting OTA...");
  initOTA(server);
  Serial.println("Starting websocket server...");
  initWebsocketServer(server);
  Serial.println("Starting web server...");
  initWebServer();
#endif

  // drains too much power
//  Screen.test();

  pluginManager.addPlugin(new DrawPlugin());
  pluginManager.addPlugin(new BreakoutPlugin());
  pluginManager.addPlugin(new SnakePlugin());
  pluginManager.addPlugin(new GameOfLifePlugin());
  pluginManager.addPlugin(new StarsPlugin());
  pluginManager.addPlugin(new LinesPlugin());
  pluginManager.addPlugin(new CirclePlugin());
  pluginManager.addPlugin(new RainPlugin());
  pluginManager.addPlugin(new FireworkPlugin());
  pluginManager.addPlugin(new PongClockPlugin());
  pluginManager.addPlugin(new TestPlugin());

#ifdef ENABLE_SERVER
  pluginManager.addPlugin(new MediumClockPlugin());
  pluginManager.addPlugin(new BigClockPlugin());
  pluginManager.addPlugin(new ClockPlugin());
  pluginManager.addPlugin(new WeatherPlugin());
  pluginManager.addPlugin(new AnimationPlugin());
  pluginManager.addPlugin(new TickingClockPlugin());
  pluginManager.addPlugin(new TemperaturePlugin());
  pluginManager.addPlugin(new HumidityPlugin());
  pluginManager.addPlugin(new TemperatureHumidityPlugin());

  std::vector<Fade*> fades = { new RainFade(100), new SnakeFade(100), new HorizontalWashFade(100), new CrashFade(100), new VerticalWashFade(100), new NoneFade(300) };
  pluginManager.addPlugin(new GroupPlugin("Clock/Temperature/Humidity", { new ClockPlugin(), new TemperaturePlugin(), new HumidityPlugin() }, { 10000, 5000, 5000 }, fades));
  pluginManager.addPlugin(new GroupPlugin("Clock/Temperature+Humidity", { new ClockPlugin(), new TemperatureHumidityPlugin() }, { 10000, 5000 }, fades));
  pluginManager.addPlugin(new GroupPlugin("MediumClock/Temperature/Humidity", { new MediumClockPlugin(), new TemperaturePlugin(), new HumidityPlugin() }, { 10000, 5000, 5000 }, { new CrashFade(300) }));
#endif

  pluginManager.init();
}

void loop()
{

  Messages.scrollMessageEveryMinute();

  pluginManager.runActivePlugin();

  if (WiFi.status() != WL_CONNECTED && millis() - lastConnectionAttempt > connectionInterval)
  {
    Serial.println("Lost connection to Wi-Fi. Reconnecting...");
    connectToWiFi();
  }

#ifdef ENABLE_SERVER
  cleanUpClients();

#ifdef ENABLE_DIMMING
  struct tm timeinfo;

  if (getLocalTime(&timeinfo))
    Screen.setBrightness(dimming.at(timeinfo.tm_hour));
#endif
#endif
  delay(1);
}