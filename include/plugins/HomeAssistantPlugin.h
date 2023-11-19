#pragma once

#ifdef ESP32
#include <HTTPClient.h>
#endif
#ifdef ESP8266
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#endif

#include "PluginManager.h"
#include "constants.h"
#include "secrets.h"

#if defined(ENABLE_SERVER) && defined(HA_URL) && defined(HA_TOKEN)

template <typename T> class HomeAssistantPlugin : public Plugin
{
private:
  HTTPClient http;
  unsigned long lastUpdate;
  String url;
  T currentState;
  bool isStateSet = false;
protected:
  T getState();
  void update();
  virtual void display(T state) = 0;
public:
  HomeAssistantPlugin(String uri)
  {
    this->url = String(HA_URL) + uri;
  }
  void setup() override;
  void loop() override;
};

class TemperaturePlugin : public HomeAssistantPlugin<float>
{
protected:
  void display(float state);
public:
  TemperaturePlugin() : HomeAssistantPlugin("/api/states/sensor.apartment_temperature") {}
  const char *getName() const override;
};

class HumidityPlugin : public HomeAssistantPlugin<float>
{
protected:
  void display(float state);
public:
  HumidityPlugin() : HomeAssistantPlugin("/api/states/sensor.apartment_humidity") {}
  const char *getName() const override;
};

#endif
