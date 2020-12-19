/*                                 _   _                 _  _               _
                                 | \ | |               | |(_)             | |
  __      ____      ____      __ |  \| |  ___  _ __  __| | _  _   _     __| |  ___
  \ \ /\ / /\ \ /\ / /\ \ /\ / / | . ` | / _ \| '__|/ _` || || | | |   / _` | / _ \
   \ V  V /  \ V  V /  \ V  V /_ | |\  ||  __/| |  | (_| || || |_| | _| (_| ||  __/
    \_/\_/    \_/\_/    \_/\_/(_)|_| \_| \___||_|   \__,_||_| \__, |(_)\__,_| \___|
                                                               __/ |
                                                              |___/
     Plugin 2216: DS3231 Syncer Plugin by Fabian Steppat
     Infos on www.nerdiy.de/ds3231syncerplugin

     This program is free software: you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by
     the Free Software Foundation, either version 3 of the License, or
     (at your option) any later version.

     This program is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
     GNU General Public License for more details.

     You should have received a copy of the GNU General Public License
     along with this program.  If not, see <http://www.gnu.org/licenses/>.

     You will find additional information to this project at the following address:
     https://www.Nerdiy.de/ds3231syncerplugin

     Credits:

     Available commands:
     ======================================================================================================================================================
      - pxlblckring,<Enabled 1/0>,<Digits brightness 0-255>,<Mark brightness 0-255>,
            <Hour Color Red 0-255>,<Hour Color Green 0-255>,<Hour Color Blue 0-255>,
            <Minute Color Red 0-255>,<Minute Color Green 0-255>,<Minute Color Blue 0-255>,
            <Second Color Red 0-255>,<Second Color Green 0-255>,<Second Color Blue 0-255>,
      - ringanimation,<Animation mode (1-6)>,
            <On Color Red 0-255>,<On Color Green 0-255>,<On Color Blue 0-255>,
            <Off Color Red 0-255>,<Off Color Green 0-255>,<Off Color Blue 0-255>,



  TODO:
  EInstellcommands um die RTC Uhrzeit manuell einstellen zu können
  RTC zu systemzeit sync manuell anstoßen
  UHrzeit auch im menu des plugins einstellbar machen, dazu sollte die zeit in den eingabefeldern aber mitlaufen



*/

//#define USES_P216
#ifdef USES_P216

#define PLUGIN_BUILD_TEST
#ifdef PLUGIN_BUILD_TEST

#include "RTClib.h"

#define PLUGIN_216
#define PLUGIN_ID_216     216
#define PLUGIN_NAME_216   "TIME - DS3231"
#define PLUGIN_VALUENAME1_216 "Temperature"
#define PLUGIN_VALUENAME2_216 "Tick"

#define PLUGIN_216_DS3231_ADDRESS_TEMPERATURE 0x11
#define PLUGIN_216_DS3231_ADDRESS 0x68

#define PLUGIN_216_NO_WIFI_MAX_TRIES 20


#ifndef PLUGIN_216_CONFIG
#define PLUGIN_216_CONFIG(n) (Settings.TaskDevicePluginConfig[event->TaskIndex][n])
#endif

#ifndef PLUGIN_216_CONFIG_LONG
#define PLUGIN_216_CONFIG_LONG(n) (Settings.TaskDevicePluginConfigLong[event->TaskIndex][n])
#endif

RTC_DS3231 *Plugin_216_rtcInstance;
bool Plugin_216_initialNtpSyncActivated = true;
boolean Plugin_216_syncNtpToRtcDone = false;
uint8_t Plugin_216_noWifiCounter = 0;

boolean Plugin_216(byte function, struct EventStruct *event, String& string)
{
  boolean success = false;

  switch (function)
  {
    case PLUGIN_DEVICE_ADD:
      {
        //This case defines the device characteristics, edit appropriately

        Device[++deviceCount].Number = PLUGIN_ID_216;
        Device[deviceCount].Type = DEVICE_TYPE_I2C;
        Device[deviceCount].VType = Sensor_VType::SENSOR_TYPE_DUAL;
        Device[deviceCount].Ports = 0;
        Device[deviceCount].PullUpOption = false;
        Device[deviceCount].InverseLogicOption = false;
        Device[deviceCount].FormulaOption = true;
        Device[deviceCount].ValueCount = 2;
        Device[deviceCount].SendDataOption = true;
        Device[deviceCount].TimerOption = true;
        Device[deviceCount].TimerOptional = false;
        Device[deviceCount].GlobalSyncOption = true;
        Device[deviceCount].DecimalsOnly = true;
        break;
      }

    case PLUGIN_GET_DEVICENAME:
      {
        string = F(PLUGIN_NAME_216);
        break;
      }

    case PLUGIN_GET_DEVICEVALUENAMES:
      {
        strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[0], PSTR(PLUGIN_VALUENAME1_216));
        strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[1], PSTR(PLUGIN_VALUENAME2_216));
        break;
      }

    case PLUGIN_WEBFORM_LOAD:
      {
        addFormSubHeader(F("DS3231-Syncer configuration"));

        Plugin_216_addHelpButton(F("Help"), F("ds3231syncerplugin"));
        addFormNote(F("To get more information click on the ?"));

        addFormCheckBox(F("Enable NTP sync at startup"), F("ntpSyncAct"), Plugin_216_initialNtpSyncActivated);
        addFormNote(F("If the NTP-sync is activated the time will fetched at start once from NTP-server and synced with RTC. After that the RTC-time will be synced to the system-time every data-acquisition-read-interval."));

        //int choice = Settings.TaskDevicePluginConfig[event->TaskIndex][1];
        //String alarmOptions[4];
        //alarmOptions[0] = F("disabled");
        //alarmOptions[1] = F("seconds");
        //alarmOptions[2] = F("minutes");
        //alarmOptions[3] = F("hours");
        //int alarmoptionValues[4] = { 0, 1, 2, 3 };
        //addFormSelector(F("Alarm1 interval unit"), F("plugin_216_alarmmode"), 4, alarmOptions, alarmoptionValues, choice);
        //addFormNumericBox(F("Alarm1 interval value"), F("plugin_216_alarminterval"), Settings.TaskDevicePluginConfig[event->TaskIndex][2], 0, 5040);
        //addFormNote(F("When alarm active it pulls INT/SQW output active Low - requires external pullup"));


        success = true;
        break;
      }

    case PLUGIN_WEBFORM_SAVE:
      {
        PLUGIN_216_CONFIG(0) = isFormItemChecked(F("ntpSyncAct"));

        Plugin_216_initialNtpSyncActivated = PLUGIN_216_CONFIG(0);

        success = true;
        break;

      }
    case PLUGIN_INIT:
      {
        Plugin_216_initialNtpSyncActivated = PLUGIN_216_CONFIG(0);

        Plugin_216_rtcInstance = new RTC_DS3231();

        if (!Plugin_216_rtcInstance->begin())
        {
          String log = F(PLUGIN_NAME_216);
          addLog(LOG_LEVEL_ERROR, log);
          log = F("  -Couldn't initialize RTC. :'(");
          addLog(LOG_LEVEL_ERROR, log);
        } else
        {
          String log = F(PLUGIN_NAME_216);
          addLog(LOG_LEVEL_ERROR, log);
          log = F("  -RTC initialised! :)");
          addLog(LOG_LEVEL_ERROR, log);
          log = F("  -In RTC saved unixtime is: ");
          log += String(Plugin_216_actualRtcUnixtime());
          addLog(LOG_LEVEL_ERROR, log);

          if (Plugin_216_rtcInstance->lostPower())
          {
            log = F("  -RTC initialised for the first time.");
            addLog(LOG_LEVEL_ERROR, log);
          }

          Plugin_216_writeRtcTimeToSystemTime(Settings.TaskDeviceTimer[event->TaskIndex]);
          node_time.initTime();
        }
        break;
      }

    case PLUGIN_READ:
      {
        unsigned long unixTime = Plugin_216_writeRtcTimeToSystemTime(Settings.TaskDeviceTimer[event->TaskIndex]);
        float rtcTemperature = Plugin_216_returnDS3231Temperature();

        UserVar[event->BaseVarIndex] = rtcTemperature;
        UserVar[event->BaseVarIndex + 1] = unixTime;

        String log = F(PLUGIN_NAME_216);
        addLog(LOG_LEVEL_DEBUG, log);
        log = F("   - RTC-Time in unixtime: ");
        log += unixTime;
        addLog(LOG_LEVEL_DEBUG, log);
        log = F("   - DS3231-Temperature: ");
        log += rtcTemperature;
        addLog(LOG_LEVEL_DEBUG, log);

        success = true;
        break;
      }

    case PLUGIN_WRITE:
      {
        String command = parseString(string, 1);
        command.toLowerCase();
        String param1 = parseString(string, 2);
        String param2 = parseString(string, 3);
        String param3 = parseString(string, 4);
        String param4 = parseString(string, 5);
        String param5 = parseString(string, 6);
        String param6 = parseString(string, 7);
        String param7 = parseString(string, 8);
        String param8 = parseString(string, 9);
        String param9 = parseString(string, 10);
        String param10 = parseString(string, 11);
        String param11 = parseString(string, 12);
        String param12 = parseString(string, 13);

        if (command == F("incrhr"))
        {
          Plugin_216_modifyTimeBySeconds(3600);
          Plugin_216_writeRtcTimeToSystemTime(Settings.TaskDeviceTimer[event->TaskIndex]);
          success = true;
        } else if (command == F("incrmnt"))
        {
          Plugin_216_modifyTimeBySeconds(60);
          Plugin_216_writeRtcTimeToSystemTime(Settings.TaskDeviceTimer[event->TaskIndex]);
          success = true;
        } else if (command == F("decrhr"))
        {
          Plugin_216_modifyTimeBySeconds(-3600);
          Plugin_216_writeRtcTimeToSystemTime(Settings.TaskDeviceTimer[event->TaskIndex]);
          success = true;
        } else if (command == F("decrmnt"))
        {
          Plugin_216_modifyTimeBySeconds(-60);
          Plugin_216_writeRtcTimeToSystemTime(Settings.TaskDeviceTimer[event->TaskIndex]);
          success = true;
        }
        break;
      }

    case PLUGIN_EXIT:
      {
        if (Plugin_216_rtcInstance != NULL)
        {
          String log = F(PLUGIN_NAME_216);
          addLog(LOG_LEVEL_DEBUG, log);
          log = F("   - Status: garbage collection done: Variables cleared.");
          addLog(LOG_LEVEL_DEBUG, log);
          delete Plugin_216_rtcInstance;
          Plugin_216_rtcInstance = NULL;
        }
        success = true;
        break;
      }

    case PLUGIN_ONCE_A_SECOND:
      {
        if (!Plugin_216_syncNtpToRtcDone && Plugin_216_initialNtpSyncActivated) //just check if the rtc was already synced to ntp (if available and activated)
          Plugin_216_syncNtpToRtcTime();

        success = true;
      }

  }
  return success;
}


unsigned long Plugin_216_actualRtcUnixtime()
{
  unsigned long timeT = Plugin_216_rtcInstance->now().unixtime();
  return timeT;
}

float Plugin_216_returnDS3231Temperature()
{
  //get temperature fo DS3231
  uint8_t tempMsb = 0;
  uint8_t tempLsb = 0;
  int8_t temp;

  Wire.beginTransmission(PLUGIN_216_DS3231_ADDRESS);
  Wire.write(PLUGIN_216_DS3231_ADDRESS_TEMPERATURE);
  Wire.endTransmission();

  Wire.requestFrom(PLUGIN_216_DS3231_ADDRESS, 2);
  tempMsb = Wire.read();
  tempLsb = Wire.read() >> 6;

  if ((tempMsb & 0x80) != 0)
  {
    temp = tempMsb | ~((1 << 8) - 1);      // if negative get two's complement
  } else
  {
    temp = tempMsb;
  }

  return (float)(0.25 * tempLsb + temp);
}

void Plugin_216_modifyTimeBySeconds(int16_t deltaSeconds)
{
  unsigned long rtcTime = Plugin_216_actualRtcUnixtime() + deltaSeconds;
  Plugin_216_rtcInstance->adjust(DateTime(rtcTime));
}

void Plugin_216_syncNtpToRtcTime()
{
  double ntpTime;

  String log = F(PLUGIN_NAME_216);
  addLog(LOG_LEVEL_ERROR, log);
  log = F("   Starting NTP-to-RTC-sync");
  addLog(LOG_LEVEL_ERROR, log);

  boolean wifiGood = WiFiConnected();
  boolean ntpActivated = Settings.UseNTP;
  boolean ntpFetchSuccessfull = getNtpTime(ntpTime);

  if (wifiGood && ntpActivated && ntpFetchSuccessfull)
  {
    Plugin_216_syncNtpToRtcDone = true;
    unsigned long rtcTime = Plugin_216_actualRtcUnixtime();
    struct tm rtcTimeElements;
    breakTime(rtcTime, rtcTimeElements);

    String log = F(PLUGIN_NAME_216);
    addLog(LOG_LEVEL_DEBUG, log);
    log = F("   - Data saved in RTC before update: ");
    addLog(LOG_LEVEL_DEBUG, log);
    log = F("   - Hour: ");
    log += rtcTimeElements.tm_hour;
    addLog(LOG_LEVEL_DEBUG, log);
    log = F("   - Minute: ");
    log += rtcTimeElements.tm_min;
    addLog(LOG_LEVEL_DEBUG, log);
    log = F("   - Second: ");
    log += rtcTimeElements.tm_sec;
    addLog(LOG_LEVEL_DEBUG, log);
    log = F("   - Day: ");
    log += rtcTimeElements.tm_mday;
    addLog(LOG_LEVEL_DEBUG, log);
    log = F("   - Month: ");
    log += rtcTimeElements.tm_mon;
    addLog(LOG_LEVEL_DEBUG, log);
    log = F("   - Year: ");
    log += 1970 + rtcTimeElements.tm_year;
    addLog(LOG_LEVEL_DEBUG, log);

    Plugin_216_rtcInstance->adjust(DateTime(ntpTime));
    struct tm ntpTimeElemente;
    breakTime(ntpTime, ntpTimeElemente); //Convert a time_t number to normal date & time. The tm input is a TimeElements variable which breakTime fills with the 7 numbers, computed from the "t" input.

    //collect (updated) data from rtc again
    rtcTime = Plugin_216_actualRtcUnixtime();
    breakTime(rtcTime, rtcTimeElements);

    log = F(PLUGIN_NAME_216);
    addLog(LOG_LEVEL_DEBUG, log);
    log = F("   - Data fetched from NTP and saved to RTC: ");
    addLog(LOG_LEVEL_DEBUG, log);
    log = F("   - Hour: ");
    log += rtcTimeElements.tm_hour;
    addLog(LOG_LEVEL_DEBUG, log);
    log = F("   - Minute: ");
    log += rtcTimeElements.tm_min;
    addLog(LOG_LEVEL_DEBUG, log);
    log = F("   - Second: ");
    log += rtcTimeElements.tm_sec;
    addLog(LOG_LEVEL_DEBUG, log);
    log = F("   - Day: ");
    log += rtcTimeElements.tm_mday;
    addLog(LOG_LEVEL_DEBUG, log);
    log = F("   - Month: ");
    log += rtcTimeElements.tm_mon + 1;
    addLog(LOG_LEVEL_DEBUG, log);
    log = F("   - Year: ");
    log += 1970 + rtcTimeElements.tm_year;
    addLog(LOG_LEVEL_DEBUG, log);

  } else
  {
    String log = F(PLUGIN_NAME_216);
    addLog(LOG_LEVEL_DEBUG, log);
    log = F("   - NTP-to-RTC-sync failed. RTC-time was not updated.");
    addLog(LOG_LEVEL_DEBUG, log);

    if (!wifiGood && (Plugin_216_noWifiCounter < PLUGIN_216_NO_WIFI_MAX_TRIES))
    {
      Plugin_216_noWifiCounter++;

      log = F("   -Reason: No wifi-connection. ");
      log += String(PLUGIN_216_NO_WIFI_MAX_TRIES - Plugin_216_noWifiCounter);
      log += F(" tries left.");
    }
    else if (!ntpActivated)
    {
      Plugin_216_syncNtpToRtcDone = true;
      log = F("   - Reason: NTP not activated.");
    }
    else if (!ntpFetchSuccessfull)
      log = F("   - Reason: NTP data could not be retrieved.");

    addLog(LOG_LEVEL_DEBUG, log);
  }
}

unsigned long Plugin_216_writeRtcTimeToSystemTime(uint16_t pluginIntervalTime)
{
  //set nextSyncTime so that no NTP-time-update will be started in ESPEasy.now()-function and only the external set time (from the RTC) will be used. This deactivates any future NTP-sync and only the RTC-time will be synced to the system-time.
  nextSyncTime = (sysTime - (pluginIntervalTime + 10));

  unsigned long unixTime = Plugin_216_actualRtcUnixtime();

  externalTimeSource = unixTime;

  return unixTime;
}

void Plugin_216_addHelpButton(const String & label, const String & url)
{
  String completeUrl = "http://www.nerdiy.de/" + url;
  addRowLabel(label);
  addHtmlLink(F("button help"), completeUrl, F("&#10068;"));
}
#endif
#endif
