#include "_Plugin_Helper.h"

//#define USES_P206

#ifdef USES_P206

#include "PCF8575.h"

#define PLUGIN_206
#define PLUGIN_ID_206         206
#define PLUGIN_NAME_206       "GPIO Ext. - PCF8575"
#define PLUGIN_VALUENAME0_206 "State"
#define PLUGIN_VALUENAME1_206 "Changed_pin"
#define PLUGIN_VALUENAME2_206 "Pin_state"
#define Plugin_206_I2C_ADDRESSES_NUM 8
#define PLUGIN_206_PWM_DRIVER_STANDARD_I2C_ADRESS 0x22



//== Variables for runtime-values == Start ============================

PCF8575 *Plugin_206_PCF8575_instance = nullptr;

uint8_t Plugin_206_setI2cAdress = PLUGIN_206_PWM_DRIVER_STANDARD_I2C_ADRESS;
uint16_t Plugin_206_old_state = 0;
uint16_t Plugin_206_current_state = 0;
uint16_t Plugin_206_last_changed_pin_id = 0;
uint16_t Plugin_206_last_changed_pin_state = 0;

//== Variables for runtime-values == End ============================

boolean Plugin_206(byte function, struct EventStruct *event, String& string)
{
  boolean success = false;

  switch (function)
  {
    case PLUGIN_DEVICE_ADD:
      {
        Device[++deviceCount].Number           = PLUGIN_ID_206;
        Device[deviceCount].Type               = DEVICE_TYPE_I2C;
        Device[deviceCount].VType              = Sensor_VType::SENSOR_TYPE_SINGLE;
        Device[deviceCount].Ports              = 0;
        Device[deviceCount].PullUpOption       = false;
        Device[deviceCount].InverseLogicOption = false;
        Device[deviceCount].FormulaOption      = false;
        Device[deviceCount].ValueCount         = 3;
        Device[deviceCount].SendDataOption     = true;
        Device[deviceCount].GlobalSyncOption   = true;
        Device[deviceCount].TimerOption = true;
        Device[deviceCount].TimerOptional = false;
        break;

      }

    case PLUGIN_GET_DEVICENAME:
      {
        string = F(PLUGIN_NAME_206);
        break;
      }

    case PLUGIN_GET_DEVICEVALUENAMES:
      {
        strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[0], PSTR(PLUGIN_VALUENAME0_206));
        strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[1], PSTR(PLUGIN_VALUENAME1_206));
        strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[2], PSTR(PLUGIN_VALUENAME2_206));
        break;
      }

    case PLUGIN_WEBFORM_LOAD:
      {
        addFormSubHeader(F("PCF8575 GPIO extender"));

        String possible_addresses[Plugin_206_I2C_ADDRESSES_NUM] = {F("0x20"), F("0x21"), F("0x22"), F("0x23"), F("0x24"), F("0x25"), F("0x26"), F("0x27")};
        int possible_address_values[Plugin_206_I2C_ADDRESSES_NUM] = {0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27};
        addFormSelector(F("PCF8575 I2C Address"), F("i2cadr"), Plugin_206_I2C_ADDRESSES_NUM, possible_addresses, possible_address_values, Plugin_206_setI2cAdress);
        addFormNote(F("Enter I2C address of the connected PCF8575."));

        success = true;
        break;
      }

    case PLUGIN_WEBFORM_SAVE:
      {

        Plugin_206_setI2cAdress = getFormItemInt(F("i2cadr"));
        Settings.TaskDevicePluginConfig[event->TaskIndex][1] = Plugin_206_setI2cAdress;

        Plugin_206_PCF8575_instance = nullptr;
        Plugin_206_initialize();

        success = true;
        break;
      }

    case PLUGIN_INIT:
      {

        //load permanent data to the runtime variables
        Plugin_206_setI2cAdress = Settings.TaskDevicePluginConfig[event->TaskIndex][1];

        //initialize
        if (Plugin_206_PCF8575_instance == nullptr)
        {
          Plugin_206_initialize();
        }

        success = true;
        break;
      }

    case PLUGIN_ONCE_A_SECOND:
      {

        Plugin_206_current_state = Plugin_206_PCF8575_instance->read16();

        if (Plugin_206_current_state != Plugin_206_old_state)
        {
          for (uint8_t i = 0; i < 16; i++)
          {
            uint8_t old_pin_state = (Plugin_206_old_state >> i) & 1;
            uint8_t new_pin_state = (Plugin_206_current_state >> i) & 1;
            
            if (old_pin_state != new_pin_state)
            {
              Plugin_206_last_changed_pin_id = i;
              Plugin_206_last_changed_pin_state = new_pin_state;
              break;
            }
          }


          String log = F(PLUGIN_NAME_206);
          addLog(LOG_LEVEL_DEBUG, log);
          log = F("   - Current state changed to: ");
          log += String(Plugin_206_current_state, BIN);
          addLog(LOG_LEVEL_DEBUG, log);
          log = F("   - Pin ");
          log += String(Plugin_206_last_changed_pin_id);
          log += F(" changed to state ");
          log += String(Plugin_206_last_changed_pin_state);
          addLog(LOG_LEVEL_DEBUG, log);
        }
        Plugin_206_old_state = Plugin_206_current_state;


        success = true;
        break;
      }

    case PLUGIN_EXIT:
      {
        if (Plugin_206_PCF8575_instance != nullptr)
        {
          Plugin_206_PCF8575_instance = nullptr;
        }
        break;
      }

    case PLUGIN_READ:
      {

        String log = F(PLUGIN_NAME_206);
        addLog(LOG_LEVEL_DEBUG, log);

        if (Plugin_206_PCF8575_instance != nullptr)
        {
          UserVar[event->BaseVarIndex] = Plugin_206_current_state;
          UserVar[event->BaseVarIndex + 1] = Plugin_206_last_changed_pin_id;
          UserVar[event->BaseVarIndex + 2] = Plugin_206_last_changed_pin_state;
          log = F("   - Current state: ");
          log += String(Plugin_206_current_state, BIN);

        } else
        {
          UserVar[event->BaseVarIndex] = 0;
          UserVar[event->BaseVarIndex + 1] = 0;
          UserVar[event->BaseVarIndex + 2] = 0;
          log = F("   - Current state: ");
          log += F("Not yet initialized.");
        }

        addLog(LOG_LEVEL_DEBUG, log);

        success = true;
        break;
      }

    case PLUGIN_WRITE:
      {

        if (Plugin_206_PCF8575_instance == nullptr) //to avoid access of matrix variables that are not initalized
          break;

        String command = Plugin_206_parseString(string, 1);
        command.toLowerCase();
        String param1 = Plugin_206_parseString(string, 2);

        //Command debuging routine
        String log = F(PLUGIN_NAME_206);
        addLog(LOG_LEVEL_DEBUG, log);
        log = F("   command = ");
        log += command;
        addLog(LOG_LEVEL_DEBUG, log);
        log = F("   param1 = ");
        log += param1;
        addLog(LOG_LEVEL_DEBUG, log);

        if (command == F("pcf8575_get_state")) //Command: pcf8575_get_state,<pin 0-15>,
        {
          //command to read the gpio states
          if (param1 != "")
          {
            if (param1.toInt() >= 0 && param1.toInt() < 15)
            {
              Plugin_206_current_state = Plugin_206_PCF8575_instance->read16();
              uint8_t bit_state = (Plugin_206_current_state >> param1.toInt()) & 1;

            }
          }
        }


        success = true;
        break;
      }
  }
  return success;
}

void Plugin_206_initialize()
{
  Plugin_206_PCF8575_instance = new PCF8575(Plugin_206_setI2cAdress);
  Plugin_206_PCF8575_instance->begin();
}

String Plugin_206_parseString(String & string, byte indexFind)
{
  //This seperate function is needed because the (in ESPEasy) integrated function does not allow capital letters
  String tmpString = string;
  tmpString += ",";
  //tmpString.replace(" ", ",");  //removed this becaus it blocks Message strings including space. Why was it integrated here? :D
  String locateString = "";
  byte count = 0;
  int index = tmpString.indexOf(',');
  while (index > 0)
  {
    count++;
    locateString = tmpString.substring(0, index);
    tmpString = tmpString.substring(index + 1);
    index = tmpString.indexOf(',');
    if (count == indexFind)
    {
      return locateString;
    }
  }
  return "";
}

#endif // USES_P206
