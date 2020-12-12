//#define USES_P133
#ifdef USES_P133
//#######################################################################################################
//########################### Plugin 133 VL53L0X I2C Ranging LIDAR      #################################
//#######################################################################################################
//###################################### stefan@clumsy.ch      ##########################################
//#######################################################################################################


// needs VL53L0X library from pololu https://github.com/pololu/vl53l0x-arduino



#define PLUGIN_133
#define PLUGIN_ID_133        133
#define PLUGIN_NAME_133       "Distance - VL53L0X [TESTING]"
#define PLUGIN_VALUENAME1_133 "Distance"


#include <Wire.h>
#include <VL53L0X.h>

VL53L0X sensor;

////////////////////////////
// VL53L0X Command Codes //
////////////////////////////

boolean Plugin_133_init[2] = {false, false};

boolean Plugin_133(byte function, struct EventStruct *event, String& string)
{
  boolean success = false;

  switch (function)
  {
    case PLUGIN_DEVICE_ADD:
      {
        Device[++deviceCount].Number = PLUGIN_ID_133;
        Device[deviceCount].Type = DEVICE_TYPE_SINGLE;
        Device[deviceCount].VType = SENSOR_TYPE_SINGLE;
        Device[deviceCount].Ports = 0;
        Device[deviceCount].PullUpOption = false;
        Device[deviceCount].InverseLogicOption = false;
        Device[deviceCount].FormulaOption = true;
        Device[deviceCount].ValueCount = 1;
        Device[deviceCount].SendDataOption = true;
        Device[deviceCount].TimerOption = true;
        Device[deviceCount].GlobalSyncOption = true;
        break;
      }

    case PLUGIN_GET_DEVICENAME:
      {
        string = F(PLUGIN_NAME_133);
        break;
      }

    case PLUGIN_GET_DEVICEVALUENAMES:
      {
        strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[0], PSTR(PLUGIN_VALUENAME1_133));
        break;
      }

    case PLUGIN_GET_DEVICEGPIONAMES:
      {
        event->String1 = formatGpioName_input(F("Reset-Pin"));
        break;
      }

    case PLUGIN_WEBFORM_LOAD:
      {
        
        addFormNote(F("This is the GPIO that is connected to XSHUT of the VL53L0X."));
        byte choice = Settings.TaskDevicePluginConfig[event->TaskIndex][0];
        int optionValues[2] = { 0x29, 0x30 };
        addFormSelectorI2C(F("plugin_133_vl53l0x_i2c"), 2, optionValues, choice);
        addFormNote(F("SDO Low=0x29, High=0x30"));

        boolean choiceMode3 = Settings.TaskDevicePluginConfig[event->TaskIndex][1];
        String optionsMode3[2];
        optionsMode3[0] = F("Normal(<120cm)");
        optionsMode3[1] = F("Long(<200cm)");
        int optionValuesMode3[2];
        optionValuesMode3[0] = 0;
        optionValuesMode3[1] = 1;
        addFormSelector(F("Range"), F("plugin_133_vl53l0x_range"), 2, optionsMode3, optionValuesMode3, choiceMode3);

        success = true;
        break;
      }

    case PLUGIN_WEBFORM_SAVE:
      {
        Settings.TaskDevicePluginConfig[event->TaskIndex][0] = getFormItemInt(F("plugin_133_vl53l0x_i2c"));
        //Settings.TaskDevicePluginConfig[event->TaskIndex][1] = getFormItemInt(F("plugin_133_vl53l0x_timing"));
        Settings.TaskDevicePluginConfig[event->TaskIndex][1] = getFormItemInt(F("plugin_133_vl53l0x_range"));

        Plugin_133_init[Settings.TaskDevicePluginConfig[event->TaskIndex][0]] = false;
        success = true;
        break;
      }

    case PLUGIN_READ:
      {

        int idx = Settings.TaskDevicePluginConfig[event->TaskIndex][0] ;
        //        Plugin_133_init[idx] &= Plugin_133_check(Settings.TaskDevicePluginConfig[event->TaskIndex][0]); // Check id device is present

        //        IT = Settings.TaskDevicePluginConfig[event->TaskIndex][1]; // set Integration Time

        if (!Plugin_133_init[idx])
        {
          Plugin_133_init[idx] = Plugin_133_begin(Settings.TaskDevicePluginConfig[event->TaskIndex][0], Settings.TaskDevicePin1[event->TaskIndex], Settings.TaskDevicePluginConfig[event->TaskIndex][1]);
        }

        String log = F("VL53L0X: Address: 0x");
        log += String(Settings.TaskDevicePluginConfig[event->TaskIndex][0], HEX);
        log += F(" / initialized: ");
        log += Plugin_133_init[idx] ? "yes" : "no";
        log += F(" / Range: ");
        log += Settings.TaskDevicePluginConfig[event->TaskIndex][1] == 1 ? "long" : "normal";

        if (Plugin_133_init[idx])
        {
          success = true;
          //long dist = sensor.readRangeSingleMillimeters();
          uint16_t dist = sensor.readRangeContinuousMillimeters();

          if (sensor.timeoutOccurred())
          {
            log += F(" / state: TIMEOUT");
            dist = 9991;
            success = false;

          } else if ( dist >= 2500 || dist == 8190 )
          {
            //addLog(LOG_LEVEL_DEBUG, "VL53L0X: OUT OF RANGE");

            log += F(" / state: OUT OF RANGE");
            log += F(" / measuredDistance: ");
            log += String(dist);

            dist = 9999;
            success = false;
          } else
          {

            UserVar[event->BaseVarIndex] = dist;

            log += F(" / state: GOOD");
            log += F(" / Distance: ");
            log += UserVar[event->BaseVarIndex];
          }
          addLog(LOG_LEVEL_INFO, log);

        }
        break;
      }

  }
  return success;
}

//**************************************************************************/
// Check VL53L0X presence
//**************************************************************************/
/*bool Plugin_133_check(int a) {
  vl53l0x_i2caddr = a ? a : 0x29;
  bool wire_status = false;
  uint16_t deviceID = Plugin_133_getVL53L0XID(a);

  String log = F("VL53L0X  : ID: 0x");
  log += String(deviceID, HEX);
  addLog(LOG_LEVEL_DEBUG, log);

  if (deviceID != 0x29) {
    return false;
  } else {
    return true;
  }
  }
*/

//**************************************************************************/
// Initialize VL53L0X
//**************************************************************************/
bool Plugin_133_begin(int addr, uint8_t resetPin, boolean longRangeEnabled) {

  pinMode(resetPin, OUTPUT);
  digitalWrite(resetPin, LOW);
  delay(10);
  digitalWrite(resetPin, HIGH);
  delay(10);

  Wire.beginTransmission((uint8_t)addr);
  boolean sensorPresent (0 == Wire.endTransmission());

  boolean initSuccess = sensor.init();

  String log = F("VL53L0X: initialisation: ");
  log += initSuccess ? "successful" : "failed";
  addLog(LOG_LEVEL_DEBUG, log);

  if (initSuccess)
  {
    sensor.setTimeout(500);

    if ( longRangeEnabled ) 
    {
      // lower the return signal rate limit (default is 0.25 MCPS)
      sensor.setSignalRateLimit(0.1);
      // increase laser pulse periods (defaults are 14 and 10 PCLKs)
      sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
      sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
    }

    sensor.startContinuous();

    log = F("VL53L0X: sensor found at adress 0x");
    log += String(sensor.getAddress(), HEX);
    log += F("?: ");
    log += sensorPresent ? "yes" : "no";
    addLog(LOG_LEVEL_DEBUG, log);

  }

  return initSuccess;
}


#endif
