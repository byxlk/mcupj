#include "api_config.h"

void relayCtrl_XJ1(bool flag)
{
    if(flag) SLV_RELAY_CTRL1_XJ = 0; // relay 吸合
    else     SLV_RELAY_CTRL1_XJ = 1;
}

void relayCtrl_SS1(bool flag)
{
    if(flag) SLV_RELAY_CTRL1_SS = 0; // relay 吸合
    else     SLV_RELAY_CTRL1_SS = 1;
}

void relayCtrl_XJ2(bool flag)
{
    if(flag) SLV_RELAY_CTRL2_XJ = 0; // relay 吸合
    else     SLV_RELAY_CTRL2_XJ = 1;
}

void relayCtrl_SS2(bool flag)
{
    if(flag) SLV_RELAY_CTRL2_SS = 0; // relay 吸合
    else     SLV_RELAY_CTRL2_SS = 1;
}

void MSR_relayCtrl_PWR(bool flag)
{
    if(flag) MSR_RELY_CTRL = 0; // relay 吸合
    else     MSR_RELY_CTRL = 1;
}

void MSR_relayCtrl_WAR(bool flag)
{
    if(flag) MSR_WARN_OUTPUT = 0; // relay 吸合
    else     MSR_WARN_OUTPUT = 1;
}

