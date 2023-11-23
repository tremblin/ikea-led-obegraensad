#pragma once

#include "PluginManager.h"

class GroupPlugin : public Plugin
{
private:
  unsigned long lastStart;
  int currentMode;
  std::vector<int> interval;
  std::vector<Plugin*> members;
  const char *name;

public:
  GroupPlugin(const char* name, std::vector<Plugin*> members = {}, std::vector<int> interval = {5000}) : Plugin()
  {
    this->name = name;
    this->interval = interval;
    this->members = members;
  }
public:
  void setup() override;
  void loop() override;
  const char *getName() const override;
};
