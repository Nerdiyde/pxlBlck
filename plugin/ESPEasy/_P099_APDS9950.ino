//#######################################################################################################
//#################################### Plugin 099: APDS9950 PROXIMITY      ##############################
//#######################################################################################################

// ESPEasy Plugin to scan a proximity and light chip APDS9950
// written by Hans Verbeek
// Derived from
//  ESPEasy Plugin to scan a gesture, proximity and light chip APDS9960
//  written by Jochen Krapf (jk@nerd2nerd.org)

// Proximity and Light are scanned frequently by given 'Delay' setting.
// RGB is not scanned because there are only 4 vars per task.

// Note: The chip has a wide view-of-angle. If housing is in this angle the chip blocks!


#ifdef PLUGIN_BUILD_DEV_VVV

#define PLUGIN_099
#define PLUGIN_ID_099         99
#define PLUGIN_NAME_099       "Gesture - APDS9950 [HJV - DEVELOPMENT]"
#define PLUGIN_VALUENAME1_099 "Proximity"
#define PLUGIN_VALUENAME2_099 "Light"

#include <SparkFun_APDS9960.h>   //Lib is modified to work with ESP

#ifndef CONFIG
#define CONFIG(n) (Settings.TaskDevicePluginConfig[event->TaskIndex][n])
#endif

SparkFun_APDS9950* PLUGIN_099_pds = NULL;

boolean Plugin_099(byte function, struct EventStruct *event, String& string)
{
  boolean success = false;

  switch (function)
  {
    case PLUGIN_DEVICE_ADD:
      {
        Device[++deviceCount].Number = PLUGIN_ID_099;
        Device[deviceCount].Type = DEVICE_TYPE_I2C;
        Device[deviceCount].Ports = 0;
        Device[deviceCount].VType = SENSOR_TYPE_SWITCH;
        Device[deviceCount].PullUpOption = false;
        Device[deviceCount].InverseLogicOption = false;
        Device[deviceCount].FormulaOption = false;
        Device[deviceCount].ValueCount = 2;
        Device[deviceCount].SendDataOption = true;
        Device[deviceCount].TimerOption = true;
        Device[deviceCount].TimerOptional = false;
        Device[deviceCount].GlobalSyncOption = true;
        break;
      }

    case PLUGIN_GET_DEVICENAME:
      {
        string = F(PLUGIN_NAME_099);
        break;
      }

    case PLUGIN_GET_DEVICEVALUENAMES:
      {
        strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[0], PSTR(PLUGIN_VALUENAME1_099));
        strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[1], PSTR(PLUGIN_VALUENAME2_099));
        /*
        strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[2], PSTR(PLUGIN_VALUENAME3_099));
        strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[3], PSTR(PLUGIN_VALUENAME4_099));
        strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[4], PSTR(PLUGIN_VALUENAME5_099));
        strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[5], PSTR(PLUGIN_VALUENAME6_099));
        */
        break;
      }

    case PLUGIN_WEBFORM_LOAD:
      {
        byte addr = 0x39;   // CONFIG(0); chip has only 1 address

        int optionValues[1] = { 0x39 };
        addFormSelectorI2C(string, F("i2c_addr"), 1, optionValues, addr);  //Only for display I2C address

        success = true;
        break;
      }

    case PLUGIN_WEBFORM_SAVE:
      {
        //CONFIG(0) = getFormItemInt(F("i2c_addr"));

        success = true;
        break;
      }

    case PLUGIN_INIT:
      {
        if (PLUGIN_099_pds)
          delete PLUGIN_099_pds;
        PLUGIN_099_pds = new SparkFun_APDS9950();

        String log = F("APDS : ");
        if ( PLUGIN_099_pds->init() )
        {
          log += F("Init");

          if (! PLUGIN_099_pds->enablePower())
            log += F(" - Error during powering!");

          if (! PLUGIN_099_pds->enableLightSensor(false))
            log += F(" - Error during light sensor init!");
          if (! PLUGIN_099_pds->enableProximitySensor(false))
            log += F(" - Error during proximity sensor init!");

            
          log += F("finished");
        }
        else
        {
          log += F("Error during APDS-9950 init!");
        }

        addLog(LOG_LEVEL_INFO, log);
        success = true;
        break;
      }

	  case PLUGIN_READ:
      {
        if (!PLUGIN_099_pds)
          break;

        if (1)
        {
          uint8_t proximity_data = 0;
          PLUGIN_099_pds->readProximity(proximity_data);
          UserVar[event->BaseVarIndex + 0] = (float)proximity_data;

          uint16_t ambient_light = 0;
          PLUGIN_099_pds->readAmbientLight(ambient_light);
          UserVar[event->BaseVarIndex + 1] = (float)ambient_light;

          /*
          uint16_t red_light   = 0;
          uint16_t green_light = 0;
          uint16_t blue_light  = 0;
          PLUGIN_099_pds->readRedLight(red_light);
          PLUGIN_099_pds->readGreenLight(green_light);
          PLUGIN_099_pds->readBlueLight(blue_light);
          UserVar[event->BaseVarIndex + 2] = (float)red_light;
          UserVar[event->BaseVarIndex + 3] = (float)green_light;
          UserVar[event->BaseVarIndex + 4] = (float)blue_light;
          */
        }

        success = true;
        break;
      }

  }
  return success;
}

#endif
