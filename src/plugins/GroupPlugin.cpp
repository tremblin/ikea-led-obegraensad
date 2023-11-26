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
        if (totalFadeWeight > 0)
        {
            int rnd = random(0, totalFadeWeight);
            int cnt = 0;
            for (Fade* fade : fades)
            {
                cnt += fade->getWeight();
                if (rnd < cnt)
                {
                    fade->run();
                    delay(500);
                    break;
                }
            }
        }

        lastStart = millis();
        currentMode = (currentMode + 1) % members.size();
        members.at(currentMode)->setup();
    }
    if (currentMode < members.size())
    {
        members.at(currentMode)->loop();
    }
}
