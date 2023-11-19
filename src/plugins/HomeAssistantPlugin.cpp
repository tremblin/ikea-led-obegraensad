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

template <typename T> std::vector<T> HomeAssistantPlugin<T>::getStates()
{
  std::vector<T> ret;
#ifdef DEBUG
  Serial.println("HomeAssistantPlugin::update(): fetching ");
#endif
  for (String uri : uris)
  {
#ifdef DEBUG
  Serial.println(String(HA_URL) + uri);
#endif
#ifdef ESP32
    http.begin(String(HA_URL) + uri);
#endif
#ifdef ESP8266
    http.begin(wifiClient, uri);
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
      ret.push_back(state);
    }
    else
    {
      ret.push_back(0);
    }
  }
  return ret;
}

template <typename T> void HomeAssistantPlugin<T>::update()
{
  std::vector<T> states = getStates();
  if (!isStateSet || (states != currentStates))
  {
    display(states);
  }
  currentStates = states;
  isStateSet = true;
}

void TemperaturePlugin::display(std::vector<float> states)
{
#ifdef DEBUG
  Serial.print("TemperaturePlugin::display(): ");
  Serial.println(states[0]);
#endif
  int rounded = round(states[0]);

  Screen.clear();
  Screen.drawNumbers(3, 2, {(rounded - rounded % 10) / 10, rounded % 10});
  Screen.drawCharacter(6, 8, Screen.readBytes(celsiusSymbol), 8);
}

const char *TemperaturePlugin::getName() const
{
  return "Temperature";
}

void HumidityPlugin::display(std::vector<float> states)
{
#ifdef DEBUG
  Serial.print("HumidityPlugin::display(): ");
  Serial.println(states[0]);
#endif
  int rounded = round(states[0]);

  Screen.clear();
  Screen.drawNumbers(3, 2, {(rounded - rounded % 10) / 10, rounded % 10});
  Screen.drawCharacter(6, 8, Screen.readBytes(percentSymbol), 8);
}

const char *HumidityPlugin::getName() const
{
  return "Humidity";
}

void TemperatureHumidityPlugin::display(std::vector<float> states)
{
#ifdef DEBUG
  Serial.print("TemperatureHumidityPlugin::display(): ");
  Serial.print(states[0]);
  Serial.print(",");
  Serial.println(states[1]);
#endif
  int rounded = round(states[0]);

  Screen.clear();
  Screen.drawNumbers(0, 2, {(rounded - rounded % 10) / 10, rounded % 10});
  Screen.drawCharacter(10, 2, Screen.readBytes(celsiusSymbol), 8);

  rounded = round(states[1]);
  Screen.drawNumbers(0, 8, {(rounded - rounded % 10) / 10, rounded % 10});
  Screen.drawCharacter(10, 8, Screen.readBytes(percentSymbol), 8);
}

const char *TemperatureHumidityPlugin::getName() const
{
  return "Temperature+Humidity";
}

#endif
