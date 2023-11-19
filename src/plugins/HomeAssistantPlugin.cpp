#include "plugins/HomeAssistantPlugin.h"

#if defined(ENABLE_SERVER) && defined(HA_URL) && defined(HA_TOKEN)

template <typename T> void HomeAssistantPlugin<T>::setup()
{
  lastUpdate = millis();
  isStateSet = false;
  update();
}

template <typename T> void HomeAssistantPlugin<T>::loop()
{
  if (millis() >= lastUpdate + (1000 * 60))
  {
    update();
    lastUpdate = millis();
#ifdef DEBUG
    Serial.println("HomeAssistantPlugin::loop(): update");
#endif
  };
}

template <typename T> T HomeAssistantPlugin<T>::getState()
{
#ifdef DEBUG
  Serial.print("HomeAssistantPlugin::update(): fetching ");
  Serial.println(url);
#endif
#ifdef ESP32
  http.begin(url);
#endif
#ifdef ESP8266
  http.begin(wifiClient, url);
#endif

  http.addHeader("Content-Type", "application/json");
  http.addHeader("Authorization", "Bearer " + String(HA_TOKEN));
  int code = http.GET();

  if (code == HTTP_CODE_OK)
  {
#ifdef DEBUG
    Serial.println("HomeAssistantPlugin::update(): 200");
#endif
    DynamicJsonDocument doc(2048);
    deserializeJson(doc, http.getString());
    T state = doc["state"].as<T>();
    return state;
  }
  return 0;
}

template <typename T> void HomeAssistantPlugin<T>::update()
{
  T state = getState();
  if (!isStateSet || (state != currentState))
  {
    display(state);
  }
  currentState = state;
  isStateSet = true;
}

void TemperaturePlugin::display(float state)
{
#ifdef DEBUG
  Serial.print("TemperaturePlugin::display(): ");
  Serial.println(state);
#endif
  int rounded = round(state);

  Screen.clear();
  Screen.drawNumbers(3, 2, {(rounded - rounded % 10) / 10, rounded % 10});
  Screen.drawCharacter(6, 8, Screen.readBytes(celsiusSymbol), 8);
}

const char *TemperaturePlugin::getName() const
{
  return "Temperature";
}

void HumidityPlugin::display(float state)
{
#ifdef DEBUG
  Serial.print("HumidityPlugin::display(): ");
  Serial.println(state);
#endif
  int rounded = round(state);

  Screen.clear();
  Screen.drawNumbers(3, 2, {(rounded - rounded % 10) / 10, rounded % 10});
  Screen.drawCharacter(6, 8, Screen.readBytes(percentSymbol), 8);
}

const char *HumidityPlugin::getName() const
{
  return "Humidity";
}

#endif
