#include "plugins/GroupPlugin.h"

const char *GroupPlugin::getName() const
{
  return name;
}

void GroupPlugin::setup()
{
    currentMode = 0;
    lastStart = millis();
    for (Plugin* member : members)
    {
        member->setup();
    }
}

void GroupPlugin::loop()
{
    if (millis() > (lastStart + interval[currentMode % interval.size()]))
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
