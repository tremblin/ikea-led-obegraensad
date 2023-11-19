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
  std::vector<String> uris;
  std::vector<T> currentStates;
  bool isStateSet = false;
protected:
  std::vector<T> getStates();
  void update();
  virtual void display(std::vector<T> states) = 0;
public:
  HomeAssistantPlugin(std::vector<String> uris)
  {
    this->uris = uris;
  }
  void setup() override;
  void loop() override;
};

class TemperaturePlugin : public HomeAssistantPlugin<float>
{
protected:
  void display(std::vector<float> states);
public:
  TemperaturePlugin() : HomeAssistantPlugin({"/api/states/sensor.apartment_temperature"}) {}
  const char *getName() const override;
};

class HumidityPlugin : public HomeAssistantPlugin<float>
{
protected:
  void display(std::vector<float> states);
public:
  HumidityPlugin() : HomeAssistantPlugin({"/api/states/sensor.apartment_humidity"}) {}
  const char *getName() const override;
};

class TemperatureHumidityPlugin : public HomeAssistantPlugin<float>
{
protected:
  void display(std::vector<float> states);
public:
  TemperatureHumidityPlugin() : HomeAssistantPlugin({"/api/states/sensor.apartment_temperature", "/api/states/sensor.apartment_humidity"}) {}
  const char *getName() const override;
};

#endif
