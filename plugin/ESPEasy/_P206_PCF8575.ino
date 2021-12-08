#include "_Plugin_Helper.h"
#define USES_P206
#ifdef USES_P206

#include "src/DataStructs/PinMode.h"
#include "src/Commands/GPIO.h"

#define PLUGIN_206
#define PLUGIN_ID_206         206
#define PLUGIN_NAME_206       "GPIO Ext. - PCF8575"
#define PLUGIN_VALUENAME1_206 "State"
#define PLUGIN_206_DOUBLECLICK_MIN_INTERVAL 1000
#define PLUGIN_206_DOUBLECLICK_MAX_INTERVAL 3000
#define PLUGIN_206_LONGPRESS_MIN_INTERVAL 1000
#define PLUGIN_206_LONGPRESS_MAX_INTERVAL 5000
#define PLUGIN_206_DC_DISABLED 0
#define PLUGIN_206_DC_LOW 1
#define PLUGIN_206_DC_HIGH 2
#define PLUGIN_206_DC_BOTH 3
#define PLUGIN_206_LONGPRESS_DISABLED 0
#define PLUGIN_206_LONGPRESS_LOW 1
#define PLUGIN_206_LONGPRESS_HIGH 2
#define PLUGIN_206_LONGPRESS_BOTH 3

boolean Plugin_206(byte function, struct EventStruct *event, String& string)
{
  boolean success = false;

  // static int8_t switchstate[TASKS_MAX];

  switch (function)
  {
    case PLUGIN_DEVICE_ADD:
    {
      Device[++deviceCount].Number           = PLUGIN_ID_206;
      Device[deviceCount].Type               = DEVICE_TYPE_I2C;
      Device[deviceCount].VType              = Sensor_VType::SENSOR_TYPE_SWITCH;
      Device[deviceCount].Ports              = 16;
      Device[deviceCount].PullUpOption       = false;
      Device[deviceCount].InverseLogicOption = true;
      Device[deviceCount].FormulaOption      = false;
      Device[deviceCount].ValueCount         = 1;
      Device[deviceCount].SendDataOption     = true;
      Device[deviceCount].TimerOption        = true;
      Device[deviceCount].TimerOptional      = true;
      Device[deviceCount].GlobalSyncOption   = true;
      break;
    }

    case PLUGIN_GET_DEVICENAME:
    {
      string = F(PLUGIN_NAME_206);
      break;
    }

    case PLUGIN_GET_DEVICEVALUENAMES:
    {
      strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[0], PSTR(PLUGIN_VALUENAME1_206));
      break;
    }

    case PLUGIN_WEBFORM_LOAD:
    {
      // @giig1967g: set current task value for taking actions after changes
      const uint32_t key = createKey(PLUGIN_ID_206, CONFIG_PORT);

      if (existPortStatus(key)) {
        globalMapPortStatus[key].previousTask = event->TaskIndex;
      }

      addFormCheckBox(F("Send Boot state"), F("p206_boot"), PCONFIG(0));

      // @giig1967-20181022
      addFormSubHeader(F("Advanced event management"));

      addFormNumericBox(F("De-bounce (ms)"), F("p206_debounce"), round(PCONFIG_FLOAT(0)), 0, 250);

      // set minimum value for doubleclick MIN max speed
      if (PCONFIG_FLOAT(1) < PLUGIN_206_DOUBLECLICK_MIN_INTERVAL) {
        PCONFIG_FLOAT(1) = PLUGIN_206_DOUBLECLICK_MIN_INTERVAL;
      }

      byte   choiceDC = PCONFIG(4);
      String buttonDC[4];
      buttonDC[0] = F("Disabled");
      buttonDC[1] = F("Active only on LOW (EVENT=3)");
      buttonDC[2] = F("Active only on HIGH (EVENT=3)");
      buttonDC[3] = F("Active on LOW & HIGH (EVENT=3)");
      int buttonDCValues[4] = { PLUGIN_206_DC_DISABLED, PLUGIN_206_DC_LOW, PLUGIN_206_DC_HIGH, PLUGIN_206_DC_BOTH };
      addFormSelector(F("Doubleclick event"), F("p206_dc"), 4, buttonDC, buttonDCValues, choiceDC);

      addFormNumericBox(F("Doubleclick max. interval (ms)"),
                        F("p206_dcmaxinterval"),
                        round(PCONFIG_FLOAT(1)),
                        PLUGIN_206_DOUBLECLICK_MIN_INTERVAL,
                        PLUGIN_206_DOUBLECLICK_MAX_INTERVAL);

      // set minimum value for longpress MIN max speed
      if (PCONFIG_FLOAT(2) < PLUGIN_206_LONGPRESS_MIN_INTERVAL) {
        PCONFIG_FLOAT(2) = PLUGIN_206_LONGPRESS_MIN_INTERVAL;
      }

      byte   choiceLP = PCONFIG(5);
      String buttonLP[4];
      buttonLP[0] = F("Disabled");
      buttonLP[1] = F("Active only on LOW (EVENT= 10 [NORMAL] or 11 [INVERSED])");
      buttonLP[2] = F("Active only on HIGH (EVENT= 11 [NORMAL] or 10 [INVERSED])");
      buttonLP[3] = F("Active on LOW & HIGH (EVENT= 10 or 11)");
      int buttonLPValues[4] =
      { PLUGIN_206_LONGPRESS_DISABLED, PLUGIN_206_LONGPRESS_LOW, PLUGIN_206_LONGPRESS_HIGH, PLUGIN_206_LONGPRESS_BOTH };
      addFormSelector(F("Longpress event"), F("p206_lp"), 4, buttonLP, buttonLPValues, choiceLP);

      addFormNumericBox(F("Longpress min. interval (ms)"),
                        F("p206_lpmininterval"),
                        round(PCONFIG_FLOAT(2)),
                        PLUGIN_206_LONGPRESS_MIN_INTERVAL,
                        PLUGIN_206_LONGPRESS_MAX_INTERVAL);

      addFormCheckBox(F("Use Safe Button (slower)"), F("p206_sb"), round(PCONFIG_FLOAT(3)));

      success = true;
      break;
    }

    case PLUGIN_WEBFORM_SAVE:
    {
      PCONFIG(0) = isFormItemChecked(F("p206_boot"));

      // @giig1967-20181022
      PCONFIG_FLOAT(0) = getFormItemInt(F("p206_debounce"));

      PCONFIG(4)       = getFormItemInt(F("p206_dc"));
      PCONFIG_FLOAT(1) = getFormItemInt(F("p206_dcmaxinterval"));

      PCONFIG(5)       = getFormItemInt(F("p206_lp"));
      PCONFIG_FLOAT(2) = getFormItemInt(F("p206_lpmininterval"));

      PCONFIG_FLOAT(3) = isFormItemChecked(F("p206_sb"));

      // check if a task has been edited and remove task flag from the previous pin
      for (std::map<uint32_t, portStatusStruct>::iterator it = globalMapPortStatus.begin(); it != globalMapPortStatus.end(); ++it) {
        if ((it->second.previousTask == event->TaskIndex) && (getPluginFromKey(it->first) == PLUGIN_ID_206)) {
          globalMapPortStatus[it->first].previousTask = -1;
          removeTaskFromPort(it->first);
          break;
        }
      }
      success = true;
      break;
    }

    case PLUGIN_INIT:
    {
      // apply INIT only if PIN is in range. Do not start INIT if pin not set in the device page.
      if (CONFIG_PORT >= 0)
      {
        portStatusStruct newStatus;
        const uint32_t   key = createKey(PLUGIN_ID_206, CONFIG_PORT);

        // Read current status or create empty if it does not exist
        newStatus = globalMapPortStatus[key];

        // read and store current state to prevent switching at boot time
        // "state" could be -1, 0 or 1
        newStatus.state                          = Plugin_206_Read(CONFIG_PORT);
        newStatus.output                         = newStatus.state;
        (newStatus.state == -1) ? newStatus.mode = PIN_MODE_OFFLINE : newStatus.mode = PIN_MODE_INPUT; // @giig1967g: if it is in the device
                                                                                                       // list we assume it's an input pin
        newStatus.task++;                                                                              // add this GPIO/port as a task

        // @giig1967g-20181022: set initial UserVar of the switch
        if ((newStatus.state != -1) && Settings.TaskDevicePin1Inversed[event->TaskIndex]) {
          UserVar[event->BaseVarIndex] = !newStatus.state;
        } else {
          UserVar[event->BaseVarIndex] = newStatus.state;
        }

        // if boot state must be send, inverse default state
        // this is done to force the trigger in PLUGIN_TEN_PER_SECOND
        if (PCONFIG(0)) {
          newStatus.state = !newStatus.state;
        }

        // @giig1967g-20181022: counter = 0
        PCONFIG(7)      = 0; // doubleclick counter
        PCONFIG_LONG(3) = 0; // safebutton counter

        // @giig1967g-20181022: used to track if LP has fired
        PCONFIG(6) = false;

        // @giig1967g-20181022: store millis for debounce, doubleclick and long press
        PCONFIG_LONG(0) = millis(); // debounce timer
        PCONFIG_LONG(1) = millis(); // doubleclick timer
        PCONFIG_LONG(2) = millis(); // longpress timer

        // @giig1967g-20181022: set minimum value for doubleclick MIN max speed
        if (PCONFIG_FLOAT(1) < PLUGIN_206_DOUBLECLICK_MIN_INTERVAL) {
          PCONFIG_FLOAT(1) = PLUGIN_206_DOUBLECLICK_MIN_INTERVAL;
        }

        // @giig1967g-20181022: set minimum value for longpress MIN max speed
        if (PCONFIG_FLOAT(2) < PLUGIN_206_LONGPRESS_MIN_INTERVAL) {
          PCONFIG_FLOAT(2) = PLUGIN_206_LONGPRESS_MIN_INTERVAL;
        }

        // setPinState(PLUGIN_ID_206, CONFIG_PORT, PIN_MODE_INPUT, switchstate[event->TaskIndex]);
        savePortStatus(key, newStatus);
      }
      success = true;
      break;
    }

    /*
          case PLUGIN_UNCONDITIONAL_POLL:
            {
              // port monitoring, generates an event by rule command 'monitor,pcf,port#'
              for (std::map<uint32_t,portStatusStruct>::iterator it=globalMapPortStatus.begin(); it!=globalMapPortStatus.end(); ++it) {
                if (getPluginFromKey(it->first)==PLUGIN_ID_206 && (it->second.monitor || it->second.command || it->second.init)) {
                  const uint16_t port = getPortFromKey(it->first);
                  int8_t state = Plugin_206_Read(port);
                  if (it->second.state != state || it->second.forceMonitor) {
                    if (it->second.mode == PIN_MODE_OFFLINE) it->second.mode=PIN_MODE_UNDEFINED; //changed from offline to online
                    if (state == -1) it->second.mode=PIN_MODE_OFFLINE; //changed from online to offline
                    if (!it->second.task) it->second.state = state; //do not update state if task flag=1 otherwise it will not be picked up
                       by 10xSEC function
                    if (it->second.monitor) {
                      it->second.forceMonitor=0; //reset flag
                      String eventString = F("PCF#");
                      eventString += port;
                      eventString += '=';
                      eventString += state;
                      rulesProcessing(eventString);
                    }
                  }
                }
              }
              break;
            }
          }
          break;
        }
     */
    /*
        case PLUGIN_MONITOR:
        {
          // port monitoring, generates an event by rule command 'monitor,gpio,port#'
          const uint32_t key                   = createKey(PLUGIN_ID_206, event->Par1);
          const portStatusStruct currentStatus = globalMapPortStatus[key];

          //  if (currentStatus.monitor || currentStatus.command || currentStatus.init) {
          const int8_t state = Plugin_206_Read(event->Par1);

          if ((currentStatus.state != state) || currentStatus.forceMonitor) {
            if (!currentStatus.task) { globalMapPortStatus[key].state = state; // do not update state if task flag=1 otherwise it will not
               be
                                                                               // picked up by 10xSEC function
            }


            if (currentStatus.monitor) {
              globalMapPortStatus[key].forceMonitor = 0; // reset flag
              String eventString = F("PCF#");
              eventString += event->Par1;
              eventString += '=';
              eventString += state;
              rulesProcessing(eventString);
            }
          }

          // }

          break;
        }
     */
    case PLUGIN_TEN_PER_SECOND:
    {
      const int8_t state = Plugin_206_Read(CONFIG_PORT);
      const String monitorEventString = F("PCF");

      /**************************************************************************\
         20181022 - @giig1967g: new doubleclick logic is:
         if there is a 'state' change, check debounce period.
         Then if doubleclick interval exceeded, reset PCONFIG(7) to 0
         PCONFIG(7) contains the current status for doubleclick:
         0: start counting
         1: 1st click
         2: 2nd click
         3: 3rd click = doubleclick event if inside interval (calculated as: '3rd click time' minus '1st click time')
         Returned EVENT value is = 3 always for doubleclick
         In rules this can be checked:
         on Button#State=3 do //will fire if doubleclick
      \**************************************************************************/
      portStatusStruct currentStatus;
      const uint32_t   key = createKey(PLUGIN_ID_206, CONFIG_PORT);

      // WARNING operator [],creates an entry in map if key doesn't exist:
      currentStatus = globalMapPortStatus[key];

      // Bug fixed: avoid 10xSEC in case of a non-fully configured device (no port defined yet)
      if ((state != -1) && (CONFIG_PORT >= 0)) {
        // CASE 1: using SafeButton, so wait 1 more 100ms cycle to acknowledge the status change
        // QUESTION: MAYBE IT'S BETTER TO WAIT 2 CYCLES??
        if (round(PCONFIG_FLOAT(3)) && (state != currentStatus.state) && (PCONFIG_LONG(3) == 0))
        {
          addLog(LOG_LEVEL_DEBUG, F("PCF :SafeButton 1st click."))
          PCONFIG_LONG(3) = 1;
        }

        // CASE 2: not using SafeButton, or already waited 1 more 100ms cycle, so proceed.
        else if ((state != currentStatus.state) || currentStatus.forceEvent)
        {
          // Reset forceEvent
          currentStatus.forceEvent = 0;

          // Reset SafeButton counter
          PCONFIG_LONG(3) = 0;

          const unsigned long debounceTime = timePassedSince(PCONFIG_LONG(0));

          if (debounceTime >= (unsigned long)lround(PCONFIG_FLOAT(0))) // de-bounce check
          {
            const unsigned long deltaDC = timePassedSince(PCONFIG_LONG(1));

            if ((deltaDC >= (unsigned long)lround(PCONFIG_FLOAT(1))) ||
                (PCONFIG(7) == 3))
            {
              // reset timer for doubleclick
              PCONFIG(7) = 0;
              PCONFIG_LONG(1) = millis();
            }

            // just to simplify the reading of the code
#define COUNTER PCONFIG(7)
#define DC PCONFIG(4)

            // check settings for doubleclick according to the settings
            if ((COUNTER != 0) || ((COUNTER == 0) && ((DC == 3) || ((DC == 1) && (state == 0)) || ((DC == 2) && (state == 1))))) {
              PCONFIG(7)++;
            }
#undef DC
#undef COUNTER

            // switchstate[event->TaskIndex] = state;
            if ((currentStatus.mode == PIN_MODE_OFFLINE) || (currentStatus.mode == PIN_MODE_UNDEFINED)) { currentStatus.mode = PIN_MODE_INPUT; //
                                                                                                                                               // changed
                                                                                                                                               // from
                                                                                                                                               // offline
                                                                                                                                               // to
                                                                                                                                               // online
            }
            currentStatus.state = state;

            byte output_value;

            // boolean sendState = switchstate[event->TaskIndex];
            boolean sendState = currentStatus.state;

            if (Settings.TaskDevicePin1Inversed[event->TaskIndex]) {
              sendState = !sendState;
            }

            if ((PCONFIG(7) == 3) && (PCONFIG(4) > 0))
            {
              output_value = 3;                 // double click
            } else {
              output_value = sendState ? 1 : 0; // single click
            }

            UserVar[event->BaseVarIndex] = output_value;

            if (loglevelActiveFor(LOG_LEVEL_INFO)) {
              String log = F("PCF  : Port=");
              log += CONFIG_PORT;
              log += F(" State=");
              log += state;
              log += output_value == 3 ? F(" Doubleclick=") : F(" Output value=");
              log += output_value;
              addLog(LOG_LEVEL_INFO, log);
            }
            // send task event
            sendData(event);
            // send monitor event
            if (currentStatus.monitor) sendMonitorEvent(monitorEventString.c_str(), CONFIG_PORT, output_value);

            // Reset forceEvent
            currentStatus.forceEvent = 0;

            savePortStatus(key, currentStatus);
          }
          savePortStatus(key, currentStatus);
        }

        // just to simplify the reading of the code
#define LP PCONFIG(5)
#define FIRED PCONFIG(6)

        // check if LP is enabled and if LP has not fired yet
        else if (!FIRED && ((LP == 3) || ((LP == 1) && (state == 0)) || ((LP == 2) && (state == 1)))) {
#undef LP
#undef FIRED

          /**************************************************************************\
             20181022 - @giig1967g: new longpress logic is:
             if there is no 'state' change, check if longpress interval reached
             When reached send longpress event.
             Returned Event value = state + 10
             So if state = 0 => EVENT longpress = 10
             if state = 1 => EVENT longpress = 11
             So we can trigger longpress for high or low contact
             In rules this can be checked:
             on Button#State=10 do //will fire if longpress when state = 0
             on Button#State=11 do //will fire if longpress when state = 1
          \**************************************************************************/

          // Reset SafeButton counter
          PCONFIG_LONG(3) = 0;

          const unsigned long deltaLP = timePassedSince(PCONFIG_LONG(2));

          if (deltaLP >= (unsigned long)lround(PCONFIG_FLOAT(2)))
          {
            byte output_value;
            PCONFIG(6) = true; // fired = true

            boolean sendState = state;

            if (Settings.TaskDevicePin1Inversed[event->TaskIndex]) {
              sendState = !sendState;
            }

            output_value = sendState ? 1 : 0;
            output_value = output_value + 10;

            UserVar[event->BaseVarIndex] = output_value;

            if (loglevelActiveFor(LOG_LEVEL_INFO)) {
              String log = F("PCF  : LongPress: Port= ");
              log += CONFIG_PORT;
              log += F(" State=");
              log += state ? '1' : '0';
              log += F(" Output value=");
              log += output_value;
              addLog(LOG_LEVEL_INFO, log);
            }
            // send task event
            sendData(event);
            // send monitor event
            if (currentStatus.monitor) sendMonitorEvent(monitorEventString.c_str(), CONFIG_PORT, output_value);

            // reset Userdata so it displays the correct state value in the web page
            UserVar[event->BaseVarIndex] = sendState ? 1 : 0;
          }
        } else {
          if (PCONFIG_LONG(3) == 1) { // Safe Button detected. Send EVENT value = 4
            const byte SAFE_BUTTON_EVENT = 4;

            // Reset SafeButton counter
            PCONFIG_LONG(3) = 0;

            // Create EVENT with value = 4 for SafeButton false positive detection
            const int tempUserVar = round(UserVar[event->BaseVarIndex]);
            UserVar[event->BaseVarIndex] = 4;

            if (loglevelActiveFor(LOG_LEVEL_INFO)) {
              String log = F("PCF : SafeButton: false positive detected. GPIO= ");
              log += CONFIG_PORT;
              log += F(" State=");
              log += tempUserVar;
              addLog(LOG_LEVEL_INFO, log);
            }
            // send task event
            sendData(event);
            // send monitor event
            if (currentStatus.monitor) sendMonitorEvent(monitorEventString.c_str(), CONFIG_PORT, SAFE_BUTTON_EVENT);

            // reset Userdata so it displays the correct state value in the web page
            UserVar[event->BaseVarIndex] = tempUserVar;
          }
        }
      } else if ((state != currentStatus.state) && (state == -1)) {
        // set UserVar and switchState = -1 and send EVENT to notify user
        UserVar[event->BaseVarIndex] = state;

        // switchstate[event->TaskIndex] = state;
        currentStatus.state = state;
        currentStatus.mode  = PIN_MODE_OFFLINE;

        if (loglevelActiveFor(LOG_LEVEL_INFO)) {
          String log = F("PCF  : Port=");
          log += CONFIG_PORT;
          log += F(" is offline (EVENT= -1)");
          addLog(LOG_LEVEL_INFO, log);
        }
        // send task event
        sendData(event);
        // send monitor event
        if (currentStatus.monitor) sendMonitorEvent(monitorEventString.c_str(), CONFIG_PORT, -1);

        savePortStatus(key, currentStatus);
      }
      success = true;
      break;
    }

    // giig1967g: Added EXIT function
    case PLUGIN_EXIT:
    {
      removeTaskFromPort(createKey(PLUGIN_ID_206, CONFIG_PORT));
      break;
    }

    case PLUGIN_READ:
    {
      // We do not actually read the pin state as this is already done 10x/second
      // Instead we just send the last known state stored in Uservar
      if (loglevelActiveFor(LOG_LEVEL_INFO)) {
        String log = F("PCF  : Port= ");
        log += CONFIG_PORT;
        log += F(" State=");
        log += UserVar[event->BaseVarIndex];
        addLog(LOG_LEVEL_INFO, log);
      }
      success = true;
      break;
    }

    case PLUGIN_REQUEST:
    {
      // parseString(string, 1) = device
      // parseString(string, 2) = command
      // parseString(string, 3) = gpio number

      // returns pin value using syntax: [plugin#pcfgpio#pinstate#xx]
      if ((string.length() >= 16) && string.substring(0, 16).equalsIgnoreCase(F("pcfgpio,pinstate")))
      {
        int par1;

        if (validIntFromString(parseString(string, 3), par1)) {
          string = GPIO_PCF_Read(par1);
        }
        success = true;
      }
      break;
    }

    case PLUGIN_WRITE:
    {
      //String log;
      //String command = parseString(string, 1);
      
      break;
    }

    case PLUGIN_TIMER_IN:
    {
      // setPinState(PLUGIN_ID_206, event->Par1, PIN_MODE_OUTPUT, event->Par2);
      portStatusStruct tempStatus;

      // WARNING: operator [] creates an entry in the map if key does not exist
      const uint32_t key = createKey(PLUGIN_ID_206, event->Par1);
      tempStatus = globalMapPortStatus[key];

      tempStatus.state = event->Par2;
      tempStatus.mode  = PIN_MODE_OUTPUT;

      // sp          (tempStatus.monitor) ? tempStatus.forceMonitor = 1 : tempStatus.forceMonitor = 0;
      tempStatus.forceMonitor = 1;
      savePortStatus(key, tempStatus);
      GPIO_PCF_Write(event->Par1, event->Par2);

      break;
    }

    case PLUGIN_ONLY_TIMER_IN:
    {
      // setPinState(PLUGIN_ID_206, event->Par1, PIN_MODE_OUTPUT, event->Par2);
      portStatusStruct tempStatus;

      // WARNING: operator [] creates an entry in the map if key does not exist
      const uint32_t key = createKey(PLUGIN_ID_206, event->Par1);
      tempStatus = globalMapPortStatus[key];

      tempStatus.state                               = event->Par2;
      tempStatus.mode                                = PIN_MODE_OUTPUT;
      (tempStatus.monitor) ? tempStatus.forceMonitor = 1 : tempStatus.forceMonitor = 0; // added to send event for longpulse command
      savePortStatus(key, tempStatus);
      Plugin_206_Write(event->Par1, event->Par2);

      break;
    }
  }
  return success;
}

// ********************************************************************************
// PCF8575 read
// ********************************************************************************
// @giig1967g-20181023: changed to int8_t
int8_t Plugin_206_Read(byte Par1)
{
  int8_t state    = -1;
  byte unit       = (Par1 - 1) / 8;
  byte port       = Par1 - (unit * 8);
  uint8_t address = 0x20 + unit;

  if (unit > 7) { address += 0x10; }

  // get the current pin status
  Wire.requestFrom(address, (uint8_t)0x1);

  if (Wire.available())
  {
    state = ((Wire.read() & _BV(port - 1)) >> (port - 1));
  }
  return state;
}

uint8_t Plugin_206_ReadAllPins(uint8_t address)
{
  uint8_t rawState = 0;

  Wire.requestFrom(address, (uint8_t)0x1);

  if (Wire.available())
  {
    rawState = Wire.read();
  }
  return rawState;
}

// ********************************************************************************
// PCF8575 write
// ********************************************************************************
boolean Plugin_206_Write(byte Par1, byte Par2)
{
  uint8_t unit    = (Par1 - 1) / 8;
  uint8_t port    = Par1 - (unit * 8);
  uint8_t address = 0x20 + unit;

  if (unit > 7) { address += 0x10; }

  // generate bitmask
  int i            = 0;
  uint8_t portmask = 255;

  unit = unit * 8 + 1; // calculate first pin

  uint32_t key;

  for (i = 0; i < 8; i++) {
    key = createKey(PLUGIN_ID_206, unit + i);

    if (existPortStatus(key) && (globalMapPortStatus[key].mode == PIN_MODE_OUTPUT) && (globalMapPortStatus[key].state == 0)) {
      portmask &= ~(1 << i); // set port i = 0
    }
  }

  key = createKey(PLUGIN_ID_206, Par1);

  if (Par2 == 1) {
    portmask |= (1 << (port - 1));
  }
  else {
    portmask &= ~(1 << (port - 1));
  }

  Wire.beginTransmission(address);
  Wire.write(portmask);
  Wire.endTransmission();

  return true;
}

#endif // USES_P206