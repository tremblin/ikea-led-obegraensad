#include "plugins/ClearPlugin.h"

void ClearPlugin::setup()
{
  once = false;
}

void ClearPlugin::loop()
{
  if (!once)
  {
    Screen.clear();
    once = true;
  }
}

const char *ClearPlugin::getName() const
{
  return "Clear";
}
