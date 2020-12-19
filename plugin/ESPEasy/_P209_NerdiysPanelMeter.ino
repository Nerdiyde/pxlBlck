/*                                 _   _                 _  _               _
                                 | \ | |               | |(_)             | |
  __      ____      ____      __ |  \| |  ___  _ __  __| | _  _   _     __| |  ___
  \ \ /\ / /\ \ /\ / /\ \ /\ / / | . ` | / _ \| '__|/ _` || || | | |   / _` | / _ \
   \ V  V /  \ V  V /  \ V  V /_ | |\  ||  __/| |  | (_| || || |_| | _| (_| ||  __/
    \_/\_/    \_/\_/    \_/\_/(_)|_| \_| \___||_|   \__,_||_| \__, |(_)\__,_| \___|
                                                               __/ |
                                                              |___/
     Plugin 201: Nerdiys-PanelMeter by Fabian Steppat
     Infos on www.nerdiy.de/nerdyispanelmeter

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
     https://www.Nerdiy.de/pxlblck

     Credits:

     Available commands:
     - pmset,{selected PanelMeter 0-16(zero=all)},{rangeMin -10000-10000},{rangeMax -10000-10000},{actualValue -10000-10000},{led color red 0-255},{led color green 0-255},{led color blue 0-255},
     - pmled,{<selected Led 0-16(zero=all)},{led color red 0-255},{led color green 0-255},{led color blue 0-255},

     Changelog:
     v0.1:
          - file created
*/
/*


   Available commands:

   TODO:
 * Support für RGBW LEDs: Checkbox zum aktivieren der WarmWhite Option. Dadurch wird noch ein Eingabefeld mehr für die warmweißHelligkeit angezeigt. Vllt. auch einen Modus der von RGB auf Warmweiß led modus umschaltet?
   kontrollierbar über befehle machen

   hilfebutton mit link zur nerdiyseite(wo alles erklärt wird) hinzufügen

   erledigt:

*/


// == Variable, Constants & Object Defintions ===========================================================================================================================

//#define USES_P209
#ifdef USES_P209_VVV

#ifdef PLUGIN_BUILD_NORMAL //if this plugin is disabled, change it to PLUGIN_BUILD_NORMAL to re-enable it

#include <Adafruit_PWMServoDriver.h>

//== Defines for plugin-values == Start ============================

#define PLUGIN_209
#define PLUGIN_ID_209        209
#define PLUGIN_NAME_209       "Display - Nerdiys-PanelMeter"
#define PLUGIN_209_DEBUG_OUTPUT
#define PLUGIN_209_VERSION 0.1

//== Defines for plugin-values == End ============================

//== Defines for permanent storage == Start ============================

#ifndef PLUGIN_209_CONFIG
#define PLUGIN_209_CONFIG(n) (Settings.TaskDevicePluginConfig[event->TaskIndex][n])
#endif

#ifndef PLUGIN_209_CONFIG_LONG
#define PLUGIN_209_CONFIG_LONG(n) (Settings.TaskDevicePluginConfigLong[event->TaskIndex][n])
#endif

//== Defines for permanent storage == End ============================

/*
  #define PLUGIN_209_COUNTER_REGISTER_VAL 33
  #define PLUGIN_209_COUNTER_TOTAL_REGISTER_VAL 34
  #define PLUGIN_209_COUNTER_TOTAL_OVERFLOW_REGISTER_VAL 35
  #define PLUGIN_209_COUNTER_TIME_DIFFERENCE_VAL 36

  #define PLUGIN_209_MAX_ADR 80
  #define PLUGIN_209_MIN_ADR 3

  #define PLUGIN_209_MAX_CC 120
  #define PLUGIN_209_MIN_CC 10

  #define PLUGIN_209_I2C_PULLUP_ENABLE_PIN D5
*/

//#define PLUGIN_209_LED_PIN 5 //2=D4
#define PLUGIN_209_PWM_DRIVER_STANDARD_I2C_ADRESS 0x40
#define PLUGIN_209_I2C_ADDRESSES_NUM 5

#define PLUGIN_209_MAX_PANEL_METER 16
#define PLUGIN_209_MAX_PWM_VALUE 4095

#define PLUGIN_209_STANDARD_LED_COLOR_R 255
#define PLUGIN_209_STANDARD_LED_COLOR_G 255
#define PLUGIN_209_STANDARD_LED_COLOR_B 255



//== Variables for runtime-values == Start ============================

Adafruit_PWMServoDriver *Plugin_209_pwm;
Adafruit_NeoPixel *Plugin_209_pixels;

uint8_t Plugin_209_setI2cAdress = PLUGIN_209_PWM_DRIVER_STANDARD_I2C_ADRESS;
uint8_t Plugin_209_pmNum = 1;


//== Variables for runtime-values == End ============================

boolean Plugin_209(byte function, struct EventStruct *event, String& string)
{
  boolean success = false;

  switch (function)
  {
    case PLUGIN_DEVICE_ADD:
      {
        Device[++deviceCount].Number = PLUGIN_ID_209;
        Device[deviceCount].Type = DEVICE_TYPE_SINGLE;
        Device[deviceCount].VType = SENSOR_TYPE_SINGLE;
        Device[deviceCount].Ports = 0;
        Device[deviceCount].PullUpOption = false;
        Device[deviceCount].InverseLogicOption = false;
        Device[deviceCount].FormulaOption = false;
        Device[deviceCount].ValueCount = 0;
        Device[deviceCount].SendDataOption = false;
        break;
      }

    case PLUGIN_GET_DEVICENAME:
      {
        string = F(PLUGIN_NAME_209);
        break;
      }

    case PLUGIN_GET_DEVICEVALUENAMES:
      {
        break;
      }
    case PLUGIN_WEBFORM_LOAD:
      {
        addFormSubHeader(F("PanelMeter-Display configuration"));

        Plugin_209_addHelpButton(F("Help"), F("nerdiyspanelmeter"));
        addFormNote(F("To get more information click on the ?"));

        addFormNumericBox(F("Number of connected PanelMeter"), F("num"), Plugin_209_pmNum, 1, PLUGIN_209_MAX_PANEL_METER);
        addFormNote(F("Number of the connected panel-meter"));

        String possibleAddresses[PLUGIN_209_I2C_ADDRESSES_NUM] = {F("1: 0x40"), F("2: 0x41"), F("3: 0x42"), F("4: 0x43"), F("5: 0x44")};
        int possibleAddressesValues[PLUGIN_209_I2C_ADDRESSES_NUM] = {0x40, 0x41, 0x42, 0x43, 0x44};
        addFormSelector(F("PCA9685 I2C Address"), F("i2cadr"), PLUGIN_209_I2C_ADDRESSES_NUM, possibleAddresses, possibleAddressesValues, Plugin_209_setI2cAdress);
        addFormNote(F("Enter I2C address of the connected PC9685 pwm module"));

        success = true;
        break;
      }

    case PLUGIN_WEBFORM_SAVE:
      {

        PLUGIN_209_CONFIG(0) = getFormItemInt(F("num"));
        Plugin_209_pmNum = PLUGIN_209_CONFIG(0);

        PLUGIN_209_CONFIG(1) = getFormItemInt(F("i2cadr"));
        Plugin_209_setI2cAdress = PLUGIN_209_CONFIG(1);

        success = true;
        break;
      }

    case PLUGIN_INIT:
      {
        //load permanent data to the runtime variables
        Plugin_209_pmNum = PLUGIN_209_CONFIG(0);
        Plugin_209_setI2cAdress = PLUGIN_209_CONFIG(1);

        //initialize led-matrix
        if (Plugin_209_pwm == NULL)
        {
          Plugin_209_pwm = new Adafruit_PWMServoDriver(Plugin_209_setI2cAdress);
          Plugin_209_pwm->begin();
          Plugin_209_pwm->setPWMFreq(100);
          Plugin_209_set_all_PanelMeter(2048);
        }

        if (Plugin_209_pixels == NULL)
        {
          Plugin_209_pixels = new Adafruit_NeoPixel(Plugin_209_pmNum, CONFIG_PIN1, NEO_GRB + NEO_KHZ800);
          Plugin_209_pixels->begin(); // This initializes the NeoPixel library.
          Plugin_209_set_all_leds(PLUGIN_209_STANDARD_LED_COLOR_R, PLUGIN_209_STANDARD_LED_COLOR_G, PLUGIN_209_STANDARD_LED_COLOR_B);
          Plugin_209_update_leds();
        }

        success = true;
        break;
      }


    case PLUGIN_WRITE:
      {
        if (Plugin_209_pwm == NULL || Plugin_209_pixels == NULL) //to avoid access of variables that are not initalized
          break;


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

        if (command == F("pmset")) // pmset,<selected PanelMeter 0-16(zero=all)>,<rangeMin -10000-10000>,<rangeMax -10000-10000>,<actualValue -10000-10000>,<led color red 0-255>,<led color green 0-255>,<led color blue 0-255>,
        {
          uint8_t selectedPanelMeter = 0;
          int16_t rangeMin = 0;
          int16_t rangeMax = 100;
          int16_t actualValue = 0;
          int16_t r = -1;
          int16_t g = -1;
          int16_t b = -1;

          if (param1 != "")
          {
            uint8_t selectedPM = param1.toInt();
            if (selectedPM > 0 && selectedPM <= Plugin_209_pmNum)
            {
              selectedPanelMeter = selectedPM;
            }
          }

          if (param2 != "")
          {
            int16_t rMin = param2.toInt();
            if (rMin > -10000 && rMin <= 10000)
            {
              rangeMin = rMin;
            }
          }

          if (param3 != "")
          {
            int16_t rMax = param3.toInt();
            if (rMax > -10000 && rMax <= 10000)
            {
              rangeMax = rMax;
            }
          }

          if (param4 != "")
          {
            int16_t actParam = param4.toInt();
            if (actParam > -10000 && actParam <= 10000)
            {
              actualValue = actParam;
            }
          }

          if (param5 != "")
          {
            uint8_t rLoc = param5.toInt();
            if (rLoc > -1 && rLoc <= 255)
            {
              r = rLoc;
            }
          }

          if (param6 != "")
          {
            uint8_t gLoc = param6.toInt();
            if (gLoc > -1 && gLoc <= 255)
            {
              g = gLoc;
            }
          }

          if (param7 != "")
          {
            uint8_t bLoc = param7.toInt();
            if (bLoc > -1 && bLoc <= 255)
            {
              b = bLoc;
            }
          }

          if (r != -1 || g != -1 || b != -1)
          {
            if (r == -1)
              r = 0;
            if (g == -1)
              g = 0;
            if (b == -1)
              b = 0;
          }

#ifdef PLUGIN_209_DEBUG_OUTPUT
          String log = F("NerdiysPanelMeter: ");
          addLog(LOG_LEVEL_INFO, log);
          log = F("   selectedPanelMeter=");
          log += selectedPanelMeter;
          addLog(LOG_LEVEL_INFO, log);
          log = F("   rangeMin=");
          log += rangeMin;
          addLog(LOG_LEVEL_INFO, log);
          log = F("   rangeMax=");
          log += rangeMax;
          addLog(LOG_LEVEL_INFO, log);
          log = F("   actualValue=");
          log += actualValue;
          addLog(LOG_LEVEL_INFO, log);
          log = F("   r=");
          log += r;
          addLog(LOG_LEVEL_INFO, log);
          log = F("   g=");
          log += g;
          addLog(LOG_LEVEL_INFO, log);
          log = F("   b=");
          log += b;
          addLog(LOG_LEVEL_INFO, log);
#endif

          uint16_t val = map(actualValue, rangeMin, rangeMax, 0, PLUGIN_209_MAX_PWM_VALUE);


#ifdef PLUGIN_209_DEBUG_OUTPUT
          log = F("   val=");
          log += val;
          addLog(LOG_LEVEL_INFO, log);
#endif

          if (selectedPanelMeter == 0)
          {
            Plugin_209_set_all_PanelMeter(val);

            if (r != -1 || g != -1 || b != -1) //change the leds only if there was at least one value modified
            {
              Plugin_209_set_all_leds(r, g, b);
              Plugin_209_update_leds();
            }
          } else
          {
            Plugin_209_set_PanelMeter((selectedPanelMeter - 1), val);

            if (r != -1 || g != -1 || b != -1 ) //change the leds only if there was at least one value modified
            {
              Plugin_209_set_led((selectedPanelMeter - 1), r, g, b);
              Plugin_209_update_leds();
            }
          }

          success = true;
        } else if (command == F("pmled")) // pmled,<selected Led 0-16(zero=all)>,<led color red 0-255>,<led color green 0-255>,<led color blue 0-255>,
        {
          uint8_t selectedLed = 0;
          uint8_t r = 0;
          uint8_t g = 0;
          uint8_t b = 0;

          if (param1 != "")
          {
            uint8_t selectedPM = param1.toInt();
            if (selectedPM > 0 && selectedPM <= Plugin_209_pmNum)
            {
              selectedLed = selectedPM;
            }
          }

          if (param2 != "")
          {
            uint8_t rLoc = param2.toInt();
            if (rLoc > -1 && rLoc <= 255)
            {
              r = rLoc;
            }
          }

          if (param3 != "")
          {
            uint8_t gLoc = param3.toInt();
            if (gLoc > -1 && gLoc <= 255)
            {
              g = gLoc;
            }
          }

          if (param4 != "")
          {
            uint8_t bLoc = param4.toInt();
            if (bLoc > -1 && bLoc <= 255)
            {
              b = bLoc;
            }
          }


          if (selectedLed == 0)
          {
            Plugin_209_set_all_leds(r, g, b);
            Plugin_209_update_leds();
          } else
          {
            Plugin_209_set_led((selectedLed - 1), r, g, b);
            Plugin_209_update_leds();
          }


          success = true;
        }

        break;
      }

    case PLUGIN_EXIT:
      {
        if (Plugin_209_pwm != NULL)
        {
          delete Plugin_209_pwm;
          Plugin_209_pwm = NULL;
        }

        if (Plugin_209_pixels != NULL)
        {
          delete Plugin_209_pixels;
          Plugin_209_pixels = NULL;
        }
        break;
      }
  }
  return success;
}


// == Misc functions == start ===============================================================================================================

void Plugin_209_set_all_PanelMeter(uint16_t value)
{
  for (int i = 0; i < Plugin_209_pmNum; i++)
  {
    Plugin_209_set_PanelMeter(i, value);
  }
}

void Plugin_209_set_PanelMeter(uint8_t panelNr, uint16_t value)
{
  Plugin_209_pwm->setPWM(panelNr, 0, value);
}

void Plugin_209_clear_all_leds()
{
  Plugin_209_set_all_leds(0);
}

void Plugin_209_set_all_leds(uint8_t r, uint8_t g, uint8_t b)
{
  Plugin_209_set_all_leds(Plugin_209_pixels->Color(r, g, b));
}

void Plugin_209_set_all_leds(uint32_t color)
{

  Plugin_209_pixels->fill(color,0);
  /*
    for (int i = 0; i < Plugin_209_pmNum; i++)
    {
    Plugin_209_set_led(i, color);
    }
  */
}

void Plugin_209_set_led(uint8_t led, uint8_t r, uint8_t g, uint8_t b)
{
  Plugin_209_set_led(led, Plugin_209_pixels->Color(r, g, b));
}

void Plugin_209_set_led(uint8_t led, uint32_t color)
{
  Plugin_209_pixels->setPixelColor(led, color);
}
void Plugin_209_update_leds()
{
  Plugin_209_pixels->show();
}

void Plugin_209_addHelpButton(const String & label, const String & url)
{
  addRowLabel(label);
  str += F(" <a class=\"button help\" href=\"http://www.nerdiy.de/");
  str += url;
  str += F("\" target=\"_blank\">&#10068;</a>");
  TXBuffer.checkFull();
}

// == Misc functions == end ===============================================================================================================

#endif
#endif
