#include "api_config.h"
#include "EEPROM.h"

static bool sysinfoIsInit = FALSE;
static SYS_INFO_S sysinfo;

SYS_INFO_S *getSystemInfo(void)
{
    return &sysinfo;
}

void readInitSystemInfo(void)
{
    //EEPROM_read_n(0x00, &sysinfo, sizeof(SYS_INFO_S));
}

bool checkAddrSyncStatus(void)
{
    if(sysinfo.slaveDeviceIsAlreadyConfigAddr != 0x0)
        return TRUE;
    else
        return FALSE;
}

void updateRebootCounterInfo(void)
{
    sysinfo.reboot_counter++;
    //EEPROM_write_n(0x00, &sysinfo, sizeof(SYS_INFO_S));

    //LOGI(“The %bd times boot up system ...\n”,)sysinfo.reboot_counter;
}