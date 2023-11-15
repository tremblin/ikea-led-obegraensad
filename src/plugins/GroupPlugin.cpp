#include "plugins/GroupPlugin.h"

const char *GroupPlugin::getName() const
{
  return name;
}

void GroupPlugin::setup()
{
    currentMode = 0;
    lastStart = millis();
    if (members.size() > 0)
    {
        members.at(currentMode)->setup();
    }
}

void GroupPlugin::loop()
{
    if (millis() > (lastStart + interval))
    {
        lastStart = millis();
        currentMode = (currentMode + 1) % members.size();
        members.at(currentMode)->setup();
    }
    if (currentMode < members.size())
    {
        members.at(currentMode)->loop();
    }
}
