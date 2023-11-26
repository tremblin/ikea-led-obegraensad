#pragma once

#include "PluginManager.h"
#include "fades/fade.h"

class GroupPlugin : public Plugin
{
private:
  unsigned long lastStart;
  int currentMode;
  std::vector<int> interval;
  std::vector<Plugin*> members;
  std::vector<Fade*> fades;
  int totalFadeWeight;
  const char *name;

public:
  GroupPlugin(const char* name, std::vector<Plugin*> members = {}, std::vector<int> interval = {5000}, std::vector<Fade*> fades = {}) : Plugin(), name(name), members(members), interval(interval), fades(fades)
  {
    this->totalFadeWeight = 0;
    for (Fade* fade : fades)
    {
      this->totalFadeWeight += fade->getWeight();
    }
  }
public:
  void setup() override;
  void loop() override;
  const char *getName() const override;
};
