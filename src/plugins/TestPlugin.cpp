#include "plugins/TestPlugin.h"

void TestPlugin::setup()
{
  once = false;
}

void TestPlugin::loop()
{
  if (!once)
  {
    Screen.test();
    once = true;
  }
}

const char *TestPlugin::getName() const
{
  return "Test";
}
