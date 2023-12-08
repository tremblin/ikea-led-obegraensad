#pragma once

#include "PluginManager.h"

class TestPlugin : public Plugin
{
private:
    boolean once = false;
public:
    void setup() override;
    void loop() override;
    const char *getName() const override;
};
