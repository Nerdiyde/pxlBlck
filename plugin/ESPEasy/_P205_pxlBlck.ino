/*                                 _   _                 _  _               _
                                 | \ | |               | |(_)             | |
  __      ____      ____      __ |  \| |  ___  _ __  __| | _  _   _     __| |  ___
  \ \ /\ / /\ \ /\ / /\ \ /\ / / | . ` | / _ \| '__|/ _` || || | | |   / _` | / _ \
   \ V  V /  \ V  V /  \ V  V /_ | |\  ||  __/| |  | (_| || || |_| | _| (_| ||  __/
    \_/\_/    \_/\_/    \_/\_/(_)|_| \_| \___||_|   \__,_||_| \__, |(_)\__,_| \___|
                                                               __/ |
                                                              |___/
     Plugin 205: pxlBlck by Fabian Steppat
     Infos on https://www.Nerdiy.de/pxlblck

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
     General infos: https://www.Nerdiy.de/pxlblck
     Available Hardware: https://nerdiy.de/tag/pxlblckplattform/
     Installation Infos: https://nerdiy.de/en/howto-pxlblck-das-pxlblck-plugin-installieren-und-konfigurieren/
     Command infos: https://nerdiy.de/howto-pxlblck-befehle-zur-konfiguration-des-pxlblck/
     Configure watch faces: https://nerdiy.de/en/howto-pxlblck-ziffernblaetter-und-bildschirmschoner-konfigurieren/
     Display Animations: https://nerdiy.de/en/howto-pxlblck-animationen-konfigurieren-und-anzeigen/
     Display Icons: https://nerdiy.de/en/howto-pxlblck-icons-designen-auf-den-pxlblck-uebertragen-und-anzeigen/

     Credits:
      -ESPEasy
      - Adafruit
      - RingClock functionality: The RingClock functionality is heavily inspired from this ESPEasy-Plugin:

*/

// == Variable, Constants & Object Defintions ===========================================================================================================================
#define USES_P205
#ifdef USES_P205

#ifdef PLUGIN_BUILD_NORMAL //if this plugin is disabled, change it to PLUGIN_BUILD_NORMAL to re-enable it

#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

//== Defines for plugin-values == Start ============================

#define PLUGIN_205
#define PLUGIN_ID_205         205
#define PLUGIN_NAME_205       "Display - pxlBlck"
#define PLUGIN_VALUENAME1_205 "Enabled"
#define PLUGIN_VALUENAME2_205 "Brightness"
#define PLUGIN_VALUENAME3_205 "Rotation"

//== Defines for plugin-values == End ============================

//== Defines for permanent storage == Start ============================

#ifndef PLUGIN_205_CONFIG
#define PLUGIN_205_CONFIG(n) (Settings.TaskDevicePluginConfig[event->TaskIndex][n])
#endif

#ifndef PXLBLCK_COLOR_PERMANENT_STORAGE
#define PXLBLCK_COLOR_PERMANENT_STORAGE(n) (Settings.TaskDevicePluginConfigLong[event->TaskIndex][n])
#endif

//== Defines for permanent storage == End ============================

//== Defines for Icon-Stuff == Start ============================

#define PXLBLCK_ICON_WIDTH  8
#define PXLBLCK_ICON_HEIGHT  8

#define PXLBLCK_ICON_STATE_START 0
#define PXLBLCK_ICON_STATE_SHOWING 1
#define PXLBLCK_ICON_STATE_END 2
#define PXLBLCK_ICON_STATE_NONE 3

//Show animations
#define PXLBLCK_ICON_ANIM_FADE_IN 1
#define PXLBLCK_ICON_ANIM_INSTANTLY_ON 2
#define PXLBLCK_ICON_ANIM_FLY_IN_FROM_RIGHT 3

//Hide animations
#define PXLBLCK_ICON_ANIM_FADE_OUT 1
#define PXLBLCK_ICON_ANIM_INSTANTLY_OFF 2
#define PXLBLCK_ICON_ANIM_FLY_OUT_TO_LEFT 3

#define PXLBLCK_ICON_ANIM_NONE_ANIMATION 4

#define PXLBLCK_ICON_ID_SPIFFS 6

#define PXLBLCK_ICON_FADE_STEP_SIZE 0.01

//== Defines for Icon-Stuff == End ============================

//== Defines for General stuff == Start ============================

#define PXLBLCK_MATRIX_HEIGHT  Plugin_205_matrixHeight
#define PXLBLCK_MATRIX_WIDTH  Plugin_205_matrixWidth
#define PXLBLCK_LED_COLOR_ORDER Plugin_205_ledColorOrder //NEO_RGBW //NEO_GRB //NEO_GRBW

#define PLUGIN_205_STANDARD_BRIGHTNESS 60
//WARNING: IT's highly recommended to increase this value only if you are sure that the power supply and distribution of the LED-Matrix is capable of the resulting current.
//Remember: A full powered(max. brightness on all colors) RGB LED-Matrix of 10x10 pixels has a total current consumption of aprox. 100x0,06A=6A!

#define PXLBLCK_MAX_SETABLE_BRIGHTNESS 15.0     //maximal possible value of setable brightness-range(should be smaler than 255)
#define PLUGIN_205_STANDARD_MATRIX_ROTATION 0
#define PLUGIN_205_TEXT_WRAPING_ENABLED false //this should stay disabled, otherwise characters of a running text will only be displayed as soon as they fit completely in the matrix area.
#define PLUGIN_205_BRIGHTNESS_STANDARD 10
#define PLUGIN_205_MINIMAL_BRIGHTNESS_STANDARD 10
#define PXLBLCK_HIGHER_COLOR_RESOLUTION_ENABLED_STANDARD_SETTING true
#define PXLBLCK_HOUR_MARK_STANDARD_BRIGHTNESS PLUGIN_205_BRIGHTNESS_STANDARD
#define PXLBLCK_ONE_TILE_ONLY_VALUE 255 //Thats the number value that will be used to define that no tile setting is needed
#define PXLBLCK_MAX_SETABLE_MATRIX_TILES_IN_X_DIR 10
#define PXLBLCK_MAX_SETABLE_MATRIX_TILES_IN_Y_DIR 10

#define PXLBLCK_COMMAND_GENERAL_SETTINGS "pb"
#define PXLBLCK_COMMAND_COLOR_SETTINGS "pbclr"
#define PXLBLCK_COMMAND_RUNNING_TEXT "pbrntxt"
#define PXLBLCK_COMMAND_SHOW_ICON "pbicon"
#define PXLBLCK_COMMAND_FAKE_TV "pbfaketv"
#define PXLBLCK_COMMAND_MATRIX_TEST "pbtest"
#define PXLBLCK_COMMAND_ANIMATION "pbani"
#define PXLBLCK_COMMAND_SET_COLOR_BY_WHEEL "pbclrwhl"
#define PXLBLCK_COMMAND_START_DEMO_MODE "pbdemo"
#define PXLBLCK_COMMAND_SET_BRIGHTNESS "pbbr"
#define PXLBLCK_COMMAND_SET_BOOLEANS "pbbo"
#define PXLBLCK_COMMAND_SET_DIAL "pbdia"

//== Defines for Matrix-Stuff == End ============================

//== Defines for Dial-Stuff == Start ============================

//in case you want to add additional dials:
//create a name for it and define it like PXLBLCK_DIAL_NAME...
//create an id for it and define it like PXLBLCK_DIAL_NAME_..._ID (this should be unique for every dial of course :))
//increase PLUGIN_205_MAX_DIAL_NUM to the number of available dials
//add PXLBLCK_DIAL_NONE to Plugin_205_possibleDialList to fill up unused spaces

#define PLUGIN_205_MAX_DIAL_NUM 12

#define PXLBLCK_DIAL_NONE "-" //Used as a placeholder and marks this space as unused
#define PXLBLCK_DIAL_NONE_ID "0"
#define PXLBLCK_DIAL_NONE_ID_INT 0

#define PXLBLCK_DIAL_NAME_BLANK "Blank" //Used for: All; Kind:Nothing(it's a blank screen)
#define PXLBLCK_DIAL_NAME_BLANK_ID "1"
#define PXLBLCK_DIAL_NAME_BLANK_ID_INT 1

#define PXLBLCK_DIAL_NAME_HR_NM_AND_MN_PNTS "HrNum&MntPoints" //Used for: 8x8; Kind: Dial
#define PXLBLCK_DIAL_NAME_HR_NM_AND_MN_PNTS_ID "2"
#define PXLBLCK_DIAL_NAME_HR_NM_AND_MN_PNTS_ID_INT 2

#define PXLBLCK_DIAL_NAME_RANDOM_PIXELS "RandPxls" //Used for: All; Kind: Screensaver that shows random appearing pixels
#define PXLBLCK_DIAL_NAME_RANDOM_PIXELS_ID "3"
#define PXLBLCK_DIAL_NAME_RANDOM_PIXELS_ID_INT 3

#define PXLBLCK_DIAL_NAME_WANDERING_PIXELS "WndrngPxl" //Used for: All; Kind: Screensaver about a pixel that is wandering along the outer limits of the matrix
#define PXLBLCK_DIAL_NAME_WANDERING_PIXELS_ID "4"
#define PXLBLCK_DIAL_NAME_WANDERING_PIXELS_ID_INT 4

#define PXLBLCK_DIAL_NAME_TV_SIMULATOR "TvSmltr" //Used for: All; Kind: Screensaver that is glowing like a tv simulator
#define PXLBLCK_DIAL_NAME_TV_SIMULATOR_ID "5"
#define PXLBLCK_DIAL_NAME_TV_SIMULATOR_ID_INT 5

#define PXLBLCK_DIAL_NAME_FIBONACCI_CLOCK "fiboClock" //Used for: Fibonacci Clock; Kind: Dial
#define PXLBLCK_DIAL_NAME_FIBONACCI_CLOCK_ID "6"
#define PXLBLCK_DIAL_NAME_FIBONACCI_CLOCK_ID_INT 6

#define PXLBLCK_DIAL_NAME_RINGGCLOCK_DIAL "Ring" //Used for: Ring Clock; Kind: Dial
#define PXLBLCK_DIAL_NAME_RINGGCLOCK_DIAL_ID "7"
#define PXLBLCK_DIAL_NAME_RINGGCLOCK_DIAL_ID_INT 7

#define PXLBLCK_DIAL_NAME_WORDCLOCK_DIAL "wordClock" //Used for: Wordclock; Kind: Dial
#define PXLBLCK_DIAL_NAME_WORDCLOCK_DIAL_ID "8"
#define PXLBLCK_DIAL_NAME_WORDCLOCK_DIAL_ID_INT 8

#define PXLBLCK_DIAL_NAME_DIGIT_CLOCK_DIAL "digitClock" //Used for: Digit Clock; Kind: Dial
#define PXLBLCK_DIAL_NAME_DIGIT_CLOCK_DIAL_ID "9"
#define PXLBLCK_DIAL_NAME_DIGIT_CLOCK_DIAL_ID_INT 9

#define PXLBLCK_DIAL_NAME_HORIZONTAL_NUMBERS_DIAL "horizNmbrs" //Used for: All Matrizes with a width greater 26; Kind: Dial->Normal time view like e.g 23:15
#define PXLBLCK_DIAL_NAME_HORIZONTAL_NUMBERS_DIAL_ID "10"
#define PXLBLCK_DIAL_NAME_HORIZONTAL_NUMBERS_DIAL_ID_INT 10

#define PXLBLCK_DIAL_NAME_HORIZONTAL_MINI_NUMBERS_DIAL "horizMiniNmbrs" //Used for: All Matrizes with a width greater 13; Kind: Dial->Normal time view like e.g 23 15
#define PXLBLCK_DIAL_NAME_HORIZONTAL_MINI_NUMBERS_DIAL_ID "11"
#define PXLBLCK_DIAL_NAME_HORIZONTAL_MINI_NUMBERS_DIAL_ID_INT 11

#define PXLBLCK_DIAL_NAME_VERTICAL_MINI_NUMBERS_DIAL "vertMiniNmbrs" //Used for: All Matrizes with a height greater 9; Kind: Dial->Normal time view like e.g 23 15
#define PXLBLCK_DIAL_NAME_VERTICAL_MINI_NUMBERS_DIAL_ID "12"
#define PXLBLCK_DIAL_NAME_VERTICAL_MINI_NUMBERS_DIAL_ID_INT 12

#define PXLBLCK_DIAL_NAME_DIAGONAL_MINI_NUMBERS_DIAL "diagMiniNmbrs" //Used for: All Matrizes with a height greater 9; Kind: Dial->Normal time view like e.g 23 15
#define PXLBLCK_DIAL_NAME_DIAGONAL_MINI_NUMBERS_DIAL_ID "13"
#define PXLBLCK_DIAL_NAME_DIAGONAL_MINI_NUMBERS_DIAL_ID_INT 13

#define PXLBLCK_DIAL_NAME_CAMP_FIRE "cmpFire" //Used for: All Matrizes with a height and width greater 8
#define PXLBLCK_DIAL_NAME_CAMP_FIRE_ID "14"
#define PXLBLCK_DIAL_NAME_CAMP_FIRE_ID_INT 14

#define PXLBLCK_DIAL_NAME_RUNNING_CLOCK "runningClock" //Used for: All Matrizes with a height and width greater 8
#define PXLBLCK_DIAL_NAME_RUNNING_CLOCK_ID "15"
#define PXLBLCK_DIAL_NAME_RUNNING_CLOCK_ID_INT 15


//in case you want to define the available dials for a matrix kind:
//add them (and of course also the corosponding id) to every matrix kind of Plugin_205_possibleDialList

//This array contains the dials that are available for the selected matrix kind. It contains the name and id of the available dials
String Plugin_205_possibleDialList[][PLUGIN_205_MAX_DIAL_NUM][2] = {
  { //Available dials for 8x8 matrix: PXLBLCK_8X8_MATRIX_ID
    {PXLBLCK_DIAL_NAME_BLANK, PXLBLCK_DIAL_NAME_BLANK_ID},
    {PXLBLCK_DIAL_NAME_HR_NM_AND_MN_PNTS, PXLBLCK_DIAL_NAME_HR_NM_AND_MN_PNTS_ID},
    {PXLBLCK_DIAL_NAME_RANDOM_PIXELS, PXLBLCK_DIAL_NAME_RANDOM_PIXELS_ID},
    {PXLBLCK_DIAL_NAME_WANDERING_PIXELS, PXLBLCK_DIAL_NAME_WANDERING_PIXELS_ID},
    {PXLBLCK_DIAL_NAME_CAMP_FIRE, PXLBLCK_DIAL_NAME_CAMP_FIRE_ID},
    {PXLBLCK_DIAL_NAME_TV_SIMULATOR, PXLBLCK_DIAL_NAME_TV_SIMULATOR_ID},
    {PXLBLCK_DIAL_NAME_RUNNING_CLOCK, PXLBLCK_DIAL_NAME_RUNNING_CLOCK_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID}
  },
  { //Available dials for 10x10 matrix: PXLBLCK_10X10_MATRIX_ID
    {PXLBLCK_DIAL_NAME_BLANK, PXLBLCK_DIAL_NAME_BLANK_ID},
    {PXLBLCK_DIAL_NAME_HR_NM_AND_MN_PNTS, PXLBLCK_DIAL_NAME_HR_NM_AND_MN_PNTS_ID},
    {PXLBLCK_DIAL_NAME_RANDOM_PIXELS, PXLBLCK_DIAL_NAME_RANDOM_PIXELS_ID},
    {PXLBLCK_DIAL_NAME_WANDERING_PIXELS, PXLBLCK_DIAL_NAME_WANDERING_PIXELS_ID},
    {PXLBLCK_DIAL_NAME_TV_SIMULATOR, PXLBLCK_DIAL_NAME_TV_SIMULATOR_ID},
    {PXLBLCK_DIAL_NAME_CAMP_FIRE, PXLBLCK_DIAL_NAME_CAMP_FIRE_ID},
    {PXLBLCK_DIAL_NAME_VERTICAL_MINI_NUMBERS_DIAL, PXLBLCK_DIAL_NAME_VERTICAL_MINI_NUMBERS_DIAL_ID},
    {PXLBLCK_DIAL_NAME_DIAGONAL_MINI_NUMBERS_DIAL, PXLBLCK_DIAL_NAME_DIAGONAL_MINI_NUMBERS_DIAL_ID},
    {PXLBLCK_DIAL_NAME_RUNNING_CLOCK, PXLBLCK_DIAL_NAME_RUNNING_CLOCK_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID}
  },
  { //Available dials for wordclock matrix: PXLBLCK_WORDCLOCK_MATRIX_ID
    {PXLBLCK_DIAL_NAME_BLANK, PXLBLCK_DIAL_NAME_BLANK_ID},
    {PXLBLCK_DIAL_NAME_WORDCLOCK_DIAL, PXLBLCK_DIAL_NAME_WORDCLOCK_DIAL_ID},
    {PXLBLCK_DIAL_NAME_HR_NM_AND_MN_PNTS, PXLBLCK_DIAL_NAME_HR_NM_AND_MN_PNTS_ID},
    {PXLBLCK_DIAL_NAME_RANDOM_PIXELS, PXLBLCK_DIAL_NAME_RANDOM_PIXELS_ID},
    {PXLBLCK_DIAL_NAME_TV_SIMULATOR, PXLBLCK_DIAL_NAME_TV_SIMULATOR_ID},
    {PXLBLCK_DIAL_NAME_CAMP_FIRE, PXLBLCK_DIAL_NAME_CAMP_FIRE_ID},
    {PXLBLCK_DIAL_NAME_HORIZONTAL_MINI_NUMBERS_DIAL, PXLBLCK_DIAL_NAME_HORIZONTAL_MINI_NUMBERS_DIAL_ID},
    {PXLBLCK_DIAL_NAME_VERTICAL_MINI_NUMBERS_DIAL, PXLBLCK_DIAL_NAME_VERTICAL_MINI_NUMBERS_DIAL_ID},
    {PXLBLCK_DIAL_NAME_RUNNING_CLOCK, PXLBLCK_DIAL_NAME_RUNNING_CLOCK_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID}
  },
  { //Available dials for cassette matrix: PXLBLCK_CASSETTE_MATRIX_ID
    {PXLBLCK_DIAL_NAME_BLANK, PXLBLCK_DIAL_NAME_BLANK_ID},
    {PXLBLCK_DIAL_NAME_HR_NM_AND_MN_PNTS, PXLBLCK_DIAL_NAME_HR_NM_AND_MN_PNTS_ID},
    {PXLBLCK_DIAL_NAME_RANDOM_PIXELS, PXLBLCK_DIAL_NAME_RANDOM_PIXELS_ID},
    {PXLBLCK_DIAL_NAME_WANDERING_PIXELS, PXLBLCK_DIAL_NAME_WANDERING_PIXELS_ID},
    {PXLBLCK_DIAL_NAME_TV_SIMULATOR, PXLBLCK_DIAL_NAME_TV_SIMULATOR_ID},
    {PXLBLCK_DIAL_NAME_CAMP_FIRE, PXLBLCK_DIAL_NAME_CAMP_FIRE_ID},
    {PXLBLCK_DIAL_NAME_RUNNING_CLOCK, PXLBLCK_DIAL_NAME_RUNNING_CLOCK_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID}
  },
  { //Available dials for 16x8 matrix: PXLBLCK_16x8_MATRIX_ID
    {PXLBLCK_DIAL_NAME_BLANK, PXLBLCK_DIAL_NAME_BLANK_ID},
    {PXLBLCK_DIAL_NAME_HR_NM_AND_MN_PNTS, PXLBLCK_DIAL_NAME_HR_NM_AND_MN_PNTS_ID},
    {PXLBLCK_DIAL_NAME_RANDOM_PIXELS, PXLBLCK_DIAL_NAME_RANDOM_PIXELS_ID},
    {PXLBLCK_DIAL_NAME_WANDERING_PIXELS, PXLBLCK_DIAL_NAME_WANDERING_PIXELS_ID},
    {PXLBLCK_DIAL_NAME_TV_SIMULATOR, PXLBLCK_DIAL_NAME_TV_SIMULATOR_ID},
    {PXLBLCK_DIAL_NAME_CAMP_FIRE, PXLBLCK_DIAL_NONE_ID},
    {PXLBLCK_DIAL_NAME_HORIZONTAL_MINI_NUMBERS_DIAL, PXLBLCK_DIAL_NAME_HORIZONTAL_MINI_NUMBERS_DIAL_ID},
    {PXLBLCK_DIAL_NAME_VERTICAL_MINI_NUMBERS_DIAL, PXLBLCK_DIAL_NAME_VERTICAL_MINI_NUMBERS_DIAL_ID},
    {PXLBLCK_DIAL_NAME_DIAGONAL_MINI_NUMBERS_DIAL, PXLBLCK_DIAL_NAME_DIAGONAL_MINI_NUMBERS_DIAL_ID},
    {PXLBLCK_DIAL_NAME_RUNNING_CLOCK, PXLBLCK_DIAL_NAME_RUNNING_CLOCK_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID}
  },
  { //Available dials for ringclock matrix: PXLBLCK_RINGCLOCK_MATRIX_ID
    {PXLBLCK_DIAL_NAME_BLANK, PXLBLCK_DIAL_NAME_BLANK_ID},
    {PXLBLCK_DIAL_NAME_RINGGCLOCK_DIAL, PXLBLCK_DIAL_NAME_RINGGCLOCK_DIAL_ID},
    {PXLBLCK_DIAL_NAME_RANDOM_PIXELS, PXLBLCK_DIAL_NAME_RANDOM_PIXELS_ID},
    {PXLBLCK_DIAL_NAME_TV_SIMULATOR, PXLBLCK_DIAL_NAME_TV_SIMULATOR_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID}
  },
  { //Available dials for fiboclock matrix: PXLBLCK_FIBOCLOCK_MATRIX_ID
    {PXLBLCK_DIAL_NAME_BLANK, PXLBLCK_DIAL_NAME_BLANK_ID},
    {PXLBLCK_DIAL_NAME_FIBONACCI_CLOCK, PXLBLCK_DIAL_NAME_FIBONACCI_CLOCK_ID},
    {PXLBLCK_DIAL_NAME_RANDOM_PIXELS, PXLBLCK_DIAL_NAME_RANDOM_PIXELS_ID},
    {PXLBLCK_DIAL_NAME_TV_SIMULATOR, PXLBLCK_DIAL_NAME_TV_SIMULATOR_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID}
  },
  { //Available dials for 16x16 matrix: PXLBLCK_16x16_MATRIX_ID
    {PXLBLCK_DIAL_NAME_BLANK, PXLBLCK_DIAL_NAME_BLANK_ID},
    {PXLBLCK_DIAL_NAME_HR_NM_AND_MN_PNTS, PXLBLCK_DIAL_NAME_HR_NM_AND_MN_PNTS_ID},
    {PXLBLCK_DIAL_NAME_RANDOM_PIXELS, PXLBLCK_DIAL_NAME_RANDOM_PIXELS_ID},
    {PXLBLCK_DIAL_NAME_WANDERING_PIXELS, PXLBLCK_DIAL_NAME_WANDERING_PIXELS_ID},
    {PXLBLCK_DIAL_NAME_TV_SIMULATOR, PXLBLCK_DIAL_NAME_TV_SIMULATOR_ID},
    {PXLBLCK_DIAL_NAME_CAMP_FIRE, PXLBLCK_DIAL_NAME_CAMP_FIRE_ID},
    {PXLBLCK_DIAL_NAME_HORIZONTAL_MINI_NUMBERS_DIAL, PXLBLCK_DIAL_NAME_HORIZONTAL_MINI_NUMBERS_DIAL_ID},
    {PXLBLCK_DIAL_NAME_VERTICAL_MINI_NUMBERS_DIAL, PXLBLCK_DIAL_NAME_VERTICAL_MINI_NUMBERS_DIAL_ID},
    {PXLBLCK_DIAL_NAME_DIAGONAL_MINI_NUMBERS_DIAL, PXLBLCK_DIAL_NAME_DIAGONAL_MINI_NUMBERS_DIAL_ID},
    {PXLBLCK_DIAL_NAME_RUNNING_CLOCK, PXLBLCK_DIAL_NAME_RUNNING_CLOCK_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID}
  },
  { //Available dials for 24x8 matrix: PXLBLCK_24x8_MATRIX_ID
    {PXLBLCK_DIAL_NAME_BLANK, PXLBLCK_DIAL_NAME_BLANK_ID},
    {PXLBLCK_DIAL_NAME_HR_NM_AND_MN_PNTS, PXLBLCK_DIAL_NAME_HR_NM_AND_MN_PNTS_ID},
    {PXLBLCK_DIAL_NAME_RANDOM_PIXELS, PXLBLCK_DIAL_NAME_RANDOM_PIXELS_ID},
    {PXLBLCK_DIAL_NAME_WANDERING_PIXELS, PXLBLCK_DIAL_NAME_WANDERING_PIXELS_ID},
    {PXLBLCK_DIAL_NAME_TV_SIMULATOR, PXLBLCK_DIAL_NAME_TV_SIMULATOR_ID},
    {PXLBLCK_DIAL_NAME_CAMP_FIRE, PXLBLCK_DIAL_NAME_CAMP_FIRE_ID},
    {PXLBLCK_DIAL_NAME_HORIZONTAL_MINI_NUMBERS_DIAL, PXLBLCK_DIAL_NAME_HORIZONTAL_MINI_NUMBERS_DIAL_ID},
    {PXLBLCK_DIAL_NAME_DIAGONAL_MINI_NUMBERS_DIAL, PXLBLCK_DIAL_NAME_DIAGONAL_MINI_NUMBERS_DIAL_ID},
    {PXLBLCK_DIAL_NAME_RUNNING_CLOCK, PXLBLCK_DIAL_NAME_RUNNING_CLOCK_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID}
  },
  { //Available dials for 32x8 matrix: PXLBLCK_32x8_MATRIX_ID
    {PXLBLCK_DIAL_NAME_BLANK, PXLBLCK_DIAL_NAME_BLANK_ID},
    {PXLBLCK_DIAL_NAME_HR_NM_AND_MN_PNTS, PXLBLCK_DIAL_NAME_HR_NM_AND_MN_PNTS_ID},
    {PXLBLCK_DIAL_NAME_RANDOM_PIXELS, PXLBLCK_DIAL_NAME_RANDOM_PIXELS_ID},
    {PXLBLCK_DIAL_NAME_WANDERING_PIXELS, PXLBLCK_DIAL_NAME_WANDERING_PIXELS_ID},
    {PXLBLCK_DIAL_NAME_TV_SIMULATOR, PXLBLCK_DIAL_NAME_TV_SIMULATOR_ID},
    {PXLBLCK_DIAL_NAME_CAMP_FIRE, PXLBLCK_DIAL_NAME_CAMP_FIRE_ID},
    {PXLBLCK_DIAL_NAME_HORIZONTAL_NUMBERS_DIAL, PXLBLCK_DIAL_NAME_HORIZONTAL_NUMBERS_DIAL_ID},
    {PXLBLCK_DIAL_NAME_HORIZONTAL_MINI_NUMBERS_DIAL, PXLBLCK_DIAL_NAME_HORIZONTAL_MINI_NUMBERS_DIAL_ID},
    {PXLBLCK_DIAL_NAME_DIAGONAL_MINI_NUMBERS_DIAL, PXLBLCK_DIAL_NAME_DIAGONAL_MINI_NUMBERS_DIAL_ID},
    {PXLBLCK_DIAL_NAME_RUNNING_CLOCK, PXLBLCK_DIAL_NAME_RUNNING_CLOCK_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID}
  },
  { //Available dials for digit clock matrix: PXLBLCK_DIGIT_CLOCK_MATRIX_ID
    {PXLBLCK_DIAL_NAME_BLANK, PXLBLCK_DIAL_NAME_BLANK_ID},
    {PXLBLCK_DIAL_NAME_DIGIT_CLOCK_DIAL, PXLBLCK_DIAL_NAME_DIGIT_CLOCK_DIAL_ID},
    {PXLBLCK_DIAL_NAME_RANDOM_PIXELS, PXLBLCK_DIAL_NAME_RANDOM_PIXELS_ID},
    {PXLBLCK_DIAL_NAME_TV_SIMULATOR, PXLBLCK_DIAL_NAME_TV_SIMULATOR_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID}
  },
  { //Available dials for pipeLamp matrix: PXLBLCK_PIPE_LAMP_MATRIX_ID
    {PXLBLCK_DIAL_NAME_BLANK, PXLBLCK_DIAL_NAME_BLANK_ID},
    {PXLBLCK_DIAL_NAME_HR_NM_AND_MN_PNTS, PXLBLCK_DIAL_NAME_HR_NM_AND_MN_PNTS_ID},
    {PXLBLCK_DIAL_NAME_RANDOM_PIXELS, PXLBLCK_DIAL_NAME_RANDOM_PIXELS_ID},
    {PXLBLCK_DIAL_NAME_WANDERING_PIXELS, PXLBLCK_DIAL_NAME_WANDERING_PIXELS_ID},
    {PXLBLCK_DIAL_NAME_TV_SIMULATOR, PXLBLCK_DIAL_NAME_TV_SIMULATOR_ID},
    {PXLBLCK_DIAL_NAME_CAMP_FIRE, PXLBLCK_DIAL_NAME_CAMP_FIRE_ID},
    {PXLBLCK_DIAL_NAME_HORIZONTAL_MINI_NUMBERS_DIAL, PXLBLCK_DIAL_NAME_HORIZONTAL_MINI_NUMBERS_DIAL_ID},
    {PXLBLCK_DIAL_NAME_VERTICAL_MINI_NUMBERS_DIAL, PXLBLCK_DIAL_NAME_VERTICAL_MINI_NUMBERS_DIAL_ID},
    {PXLBLCK_DIAL_NAME_DIAGONAL_MINI_NUMBERS_DIAL, PXLBLCK_DIAL_NAME_DIAGONAL_MINI_NUMBERS_DIAL_ID},
    {PXLBLCK_DIAL_NAME_RUNNING_CLOCK, PXLBLCK_DIAL_NAME_RUNNING_CLOCK_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID}
  },
  { //Available dials for MiniFloorLamp: PXLBLCK_MINI_FLOORLAMP_MATRIX_ID
    {PXLBLCK_DIAL_NAME_BLANK, PXLBLCK_DIAL_NAME_BLANK_ID},
    {PXLBLCK_DIAL_NAME_RANDOM_PIXELS, PXLBLCK_DIAL_NAME_RANDOM_PIXELS_ID},
    {PXLBLCK_DIAL_NAME_WANDERING_PIXELS, PXLBLCK_DIAL_NAME_WANDERING_PIXELS_ID},
    {PXLBLCK_DIAL_NAME_TV_SIMULATOR, PXLBLCK_DIAL_NAME_TV_SIMULATOR_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID}
  },
  { //Available dials for CassetteLamp: PXLBLCK_CASSETTE_LAMP_MATRIX_ID
    {PXLBLCK_DIAL_NAME_BLANK, PXLBLCK_DIAL_NAME_BLANK_ID},
    {PXLBLCK_DIAL_NAME_RANDOM_PIXELS, PXLBLCK_DIAL_NAME_RANDOM_PIXELS_ID},
    {PXLBLCK_DIAL_NAME_WANDERING_PIXELS, PXLBLCK_DIAL_NAME_WANDERING_PIXELS_ID},
    {PXLBLCK_DIAL_NAME_TV_SIMULATOR, PXLBLCK_DIAL_NAME_TV_SIMULATOR_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID},
    {PXLBLCK_DIAL_NONE, PXLBLCK_DIAL_NONE_ID}
  }
};

//used for displaying 5x3 digits
#define PLUGIN_205_MAX_PIXELS_PER_DIGIT 13 //normally this should be 15 but we only need max. 13(in case of an eight) so we can save this two array-spaces
#define PLUGIN_205_COORDINATES_PER_PIXEL 2
#define PLUGIN_205_DEFINED_NUMBERS 10
#define PLUGIN_205_5x4_NUMBER_NONE 255 //just a random number that is greater as five which is the highest possible coordinate

#define USED_WIDTH_OF_TWO_DIGITS 8 //defines the number of pixels that are used by two digits in horizontal direction
#define USED_HEIGHT_OF_ONE_DIGIT 5 //defines the number of pixels that are used by a digits in vertical direction

#define DISPLAY_AREA_WIDTH 8
#define DISPLAY_AREA_HEIGHT 8

//#define USED_WIDTH_OF_TWO_DIGITS 8
//#define USED_HEIGHT_OF_ONE_DIGIT 5
//#define USED_WIDTH_OF_TWO_DIGITS 8
//#define USED_HEIGHT_OF_ONE_DIGIT 5

//#define USED_WIDTH_OF_TWO_DIGITS 8

//== Defines for Dial-Stuff == End ============================

//== Matrix Type definitions == Start ============================
/* Info for adding a new kind of matrix:
   Create corosponding infos here and also add them to
   Plugin_205_matrixSizesById
   Plugin_205_matrixNamesById

   deprecated: Additionaly increase PXLBLCK_MATRIX_KIND_NUM for every additional matrix
*/

#define PXLBLCK_8X8_MATRIX_ID 0
#define PXLBLCK_8X8_MATRIX_WIDTH 8
#define PXLBLCK_8X8_MATRIX_HEIGHT 8
#define PXLBLCK_8X8_MATRIX_NAME "8x8"

#define PXLBLCK_10X10_MATRIX_ID 1
#define PXLBLCK_10X10_MATRIX_WIDTH 10
#define PXLBLCK_10X10_MATRIX_HEIGHT 10
#define PXLBLCK_10X10_MATRIX_NAME "10x10"

#define PXLBLCK_WORDCLOCK_MATRIX_ID 2
#define PXLBLCK_WORDCLOCK_MATRIX_WIDTH 11
#define PXLBLCK_WORDCLOCK_MATRIX_HEIGHT 11
#define PXLBLCK_WORDCLOCK_MATRIX_NAME "Wordclock"

#define PXLBLCK_CASSETTE_MATRIX_ID 3
#define PXLBLCK_CASSETTE_MATRIX_WIDTH 12
#define PXLBLCK_CASSETTE_MATRIX_HEIGHT 8
#define PXLBLCK_CASSETTE_MATRIX_NAME "Cassette(12x8)"

#define PXLBLCK_16x8_MATRIX_ID 4
#define PXLBLCK_16x8_MATRIX_WIDTH 16
#define PXLBLCK_16x8_MATRIX_HEIGHT 8
#define PXLBLCK_16x8_MATRIX_NAME "16x8"

#define PXLBLCK_RINGCLOCK_MATRIX_ID 5
#define PXLBLCK_RINGCLOCK_MATRIX_WIDTH 1
#define PXLBLCK_RINGCLOCK_MATRIX_HEIGHT 60
#define PXLBLCK_RINGCLOCK_MATRIX_NAME "Ringclock(1x60)"

#define PXLBLCK_FIBOCLOCK_MATRIX_ID 6
#define PXLBLCK_FIBOCLOCK_MATRIX_WIDTH 1
#define PXLBLCK_FIBOCLOCK_MATRIX_HEIGHT 9
#define PXLBLCK_FIBOCLOCK_MATRIX_NAME "Fiboclock"

#define PXLBLCK_16x16_MATRIX_ID 7
#define PXLBLCK_16x16_MATRIX_WIDTH 16
#define PXLBLCK_16x16_MATRIX_HEIGHT 16
#define PXLBLCK_16x16_MATRIX_NAME "16x16"

#define PXLBLCK_24x8_MATRIX_ID 8
#define PXLBLCK_24x8_MATRIX_WIDTH 24
#define PXLBLCK_24x8_MATRIX_HEIGHT 8
#define PXLBLCK_24x8_MATRIX_NAME "24x8"

#define PXLBLCK_32x8_MATRIX_ID 9
#define PXLBLCK_32x8_MATRIX_WIDTH 32
#define PXLBLCK_32x8_MATRIX_HEIGHT 8
#define PXLBLCK_32x8_MATRIX_NAME "32x8"

#define PXLBLCK_DIGIT_CLOCK_MATRIX_ID 10
#define PXLBLCK_DIGIT_CLOCK_MATRIX_WIDTH 2
#define PXLBLCK_DIGIT_CLOCK_MATRIX_HEIGHT 29
#define PXLBLCK_DIGIT_CLOCK_MATRIX_NAME "digitClock"

#define PXLBLCK_PIPE_LAMP_MATRIX_ID 11
#define PXLBLCK_PIPE_LAMP_MATRIX_WIDTH 18
#define PXLBLCK_PIPE_LAMP_MATRIX_HEIGHT 12
#define PXLBLCK_PIPE_LAMP_MATRIX_NAME "pipeLamp(18x12)"

#define PXLBLCK_MINI_FLOORLAMP_MATRIX_ID 12
#define PXLBLCK_MINI_FLOORLAMP_MATRIX_WIDTH 1
#define PXLBLCK_MINI_FLOORLAMP_MATRIX_HEIGHT 18
#define PXLBLCK_MINI_FLOORLAMP_MATRIX_NAME "miniFloorLamp(1x18)"

#define PXLBLCK_CASSETTE_LAMP_MATRIX_ID 13
#define PXLBLCK_CASSETTE_LAMP_MATRIX_WIDTH 1
#define PXLBLCK_CASSETTE_LAMP_MATRIX_HEIGHT 32
#define PXLBLCK_CASSETTE_LAMP_MATRIX_NAME "cassetteLamp(1x32)"

//== Matrix Type definitions == End ============================

//== Webserver Form definitions == Start ============================

#define PXLBLCK_WEBSERVER_FORM_ID_WORDCLOCK_IT_IS_ENABLED "itsEnbld"
#define PXLBLCK_WEBSERVER_FORM_ID_WORDCLOCK_O_CLOCK_ENABLED "ocEnbld"
#define PXLBLCK_WEBSERVER_FORM_ID_WORDCLOCK_LANGUAGE "lang"

#define PXLBLCK_WEBSERVER_FORM_ID_DISPLAY_ENABLED "dspEn"
#define PXLBLCK_WEBSERVER_FORM_ID_BRIGHTNESS "brgns"
#define PXLBLCK_WEBSERVER_FORM_ID_MINIMAL_BRIGHTNESS "minBrig"
#define PXLBLCK_WEBSERVER_FORM_ID_DIAL "dial"

#define PXLBLCK_WEBSERVER_FORM_ID_MATRIX_TYPE "mtrxTp"
#define PXLBLCK_WEBSERVER_FORM_ID_START_POSITION "strtps"
#define PXLBLCK_WEBSERVER_FORM_ID_MATRIX_ARRANGEMENT "mtrxArr"
#define PXLBLCK_WEBSERVER_FORM_ID_TILE_MATRIX_ARRANGEMENT "mtrxTlArr"
#define PXLBLCK_WEBSERVER_FORM_ID_MATRIX_TILES_WIDTH "mtrxTilWdth"
#define PXLBLCK_WEBSERVER_FORM_ID_MATRIX_TILES_HEIGHT "mtrxTilHght"
#define PXLBLCK_WEBSERVER_FORM_ID_ROTATION "rot"
#define PXLBLCK_WEBSERVER_FORM_ID_LED_TYPE "ldtp"

#define PXLBLCK_WEBSERVER_FORM_ID_RINGCLOCK_LED_OFFSET "rcOff"
#define PXLBLCK_WEBSERVER_FORM_ID_RINGCLOCK_THICK_MARK_ENABLED "rcThckMrk"
#define PXLBLCK_WEBSERVER_FORM_ID_RINGCLOCK_DIR_INVERSED "rcDrInv"
#define PXLBLCK_WEBSERVER_FORM_ID_RINGCLOCK_HOUR_MARK_BRIGHTNESS "rcHrMrkBrg"

#define PXLBLCK_WEBSERVER_FORM_ID_DIGITCLOCK_LEADING_ZEROS_ENABLED "dcLzE"

#define PXLBLCK_WEBSERVER_FORM_ID_COLOR_ONE_PARTS "ClrOne"
#define PXLBLCK_WEBSERVER_FORM_ID_COLOR_TWO_PARTS "ClrTwo"
#define PXLBLCK_WEBSERVER_FORM_ID_COLOR_THREE_PARTS "ClrThree"
#define PXLBLCK_WEBSERVER_FORM_ID_COLOR_FOUR_PARTS "ClrFour"

#define PXLBLCK_WEBSERVER_FORM_COLOR_NOT_USED_VALUE "---"

//== Webserver Form IDs definitions == End ============================

//== Variables for runtime-values == Start ============================

Adafruit_NeoMatrix *Plugin_205_matrix;
boolean Plugin_205_initialDebugOutputDone = false;
boolean Plugin_205_displayEnabled = true;
uint8_t Plugin_205_displayBrightness = PLUGIN_205_BRIGHTNESS_STANDARD;
uint8_t Plugin_205_selectedDial = 0;
uint8_t Plugin_205_matrixRotation = PLUGIN_205_STANDARD_MATRIX_ROTATION;
uint8_t Plugin_205_screensaver_position = 0;
boolean Plugin_205_higherColorResultionEnabled = PXLBLCK_HIGHER_COLOR_RESOLUTION_ENABLED_STANDARD_SETTING;
uint8_t Plugin_205_minimalBrightness = PLUGIN_205_MINIMAL_BRIGHTNESS_STANDARD;
unsigned long Plugin_205_iconShowedTimestamp = 0;

uint8_t Plugin_205_previousHour = 0;
uint8_t Plugin_205_previousMinute = 0;
uint8_t Plugin_205_previousSecond = 0;

uint8_t Plugin_205_matrixLayoutStartPosition = NEO_MATRIX_TOP + NEO_MATRIX_RIGHT;
uint8_t Plugin_205_matrixArrangement = NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE;
uint8_t Plugin_205_matrixTileArrangement = PXLBLCK_ONE_TILE_ONLY_VALUE;
uint8_t Plugin_205_matrixTilesWidth = 0;
uint8_t Plugin_205_matrixTilesHeight = 0;
uint8_t Plugin_205_ledColorOrder = NEO_GRB;

uint8_t Plugin_205_selectedMatrixId = 0;
uint8_t Plugin_205_selectedWordclockLanguage = 0;
boolean Plugin_205_wordclockShowOClockEnabled = true;
boolean Plugin_205_wordclockShowItIsEnabled = true;
uint8_t Plugin_205_wordclockLanguageId = 0;
boolean Plugin_205_diallLeadingZerosEnabled = true;

uint8_t Plugin_205_colorWheelPosition[5] = {0};

String Plugin_205_colorOneName = "";
String Plugin_205_colorTwoName = "";
String Plugin_205_colorThreeName = "";
String Plugin_205_colorFourName = "";

uint32_t Plugin_205_colorOne = 0;
uint32_t Plugin_205_colorTwo = 0;
uint32_t Plugin_205_colorThree = 0;
uint32_t Plugin_205_colorFour = 0;

static uint8_t Plugin_205_matrixSizesById[][2] = {
  {PXLBLCK_8X8_MATRIX_HEIGHT, PXLBLCK_8X8_MATRIX_WIDTH},
  {PXLBLCK_10X10_MATRIX_WIDTH, PXLBLCK_10X10_MATRIX_HEIGHT},
  {PXLBLCK_WORDCLOCK_MATRIX_WIDTH, PXLBLCK_WORDCLOCK_MATRIX_HEIGHT},
  {PXLBLCK_CASSETTE_MATRIX_WIDTH, PXLBLCK_CASSETTE_MATRIX_HEIGHT},
  {PXLBLCK_16x8_MATRIX_WIDTH, PXLBLCK_16x8_MATRIX_HEIGHT},
  {PXLBLCK_RINGCLOCK_MATRIX_WIDTH, PXLBLCK_RINGCLOCK_MATRIX_HEIGHT},
  {PXLBLCK_FIBOCLOCK_MATRIX_WIDTH, PXLBLCK_FIBOCLOCK_MATRIX_HEIGHT},
  {PXLBLCK_16x16_MATRIX_WIDTH, PXLBLCK_16x16_MATRIX_HEIGHT},
  {PXLBLCK_24x8_MATRIX_WIDTH, PXLBLCK_24x8_MATRIX_HEIGHT},
  {PXLBLCK_32x8_MATRIX_WIDTH, PXLBLCK_32x8_MATRIX_HEIGHT},
  {PXLBLCK_DIGIT_CLOCK_MATRIX_WIDTH, PXLBLCK_DIGIT_CLOCK_MATRIX_HEIGHT},
  {PXLBLCK_PIPE_LAMP_MATRIX_WIDTH, PXLBLCK_PIPE_LAMP_MATRIX_HEIGHT},
  {PXLBLCK_MINI_FLOORLAMP_MATRIX_WIDTH, PXLBLCK_MINI_FLOORLAMP_MATRIX_HEIGHT},
  {PXLBLCK_CASSETTE_LAMP_MATRIX_WIDTH, PXLBLCK_CASSETTE_LAMP_MATRIX_HEIGHT}
};

static String Plugin_205_matrixNamesById[] = {
  PXLBLCK_8X8_MATRIX_NAME,
  PXLBLCK_10X10_MATRIX_NAME,
  PXLBLCK_WORDCLOCK_MATRIX_NAME,
  PXLBLCK_CASSETTE_MATRIX_NAME,
  PXLBLCK_16x8_MATRIX_NAME,
  PXLBLCK_RINGCLOCK_MATRIX_NAME,
  PXLBLCK_FIBOCLOCK_MATRIX_NAME,
  PXLBLCK_16x16_MATRIX_NAME,
  PXLBLCK_24x8_MATRIX_NAME,
  PXLBLCK_32x8_MATRIX_NAME,
  PXLBLCK_DIGIT_CLOCK_MATRIX_NAME,
  PXLBLCK_PIPE_LAMP_MATRIX_NAME,
  PXLBLCK_MINI_FLOORLAMP_MATRIX_NAME,
  PXLBLCK_CASSETTE_LAMP_MATRIX_NAME
};

uint8_t Plugin_205_matrixHeight = 0;
uint8_t Plugin_205_matrixWidth = 0;

//== Variables for runtime-values == End ============================

//== Defines for animations == Start ============================

unsigned long Plugin_205_animationExecutedTimestamp = 0;
#define PLUGIN_205_ANIMATION_PIXEL_WIDTH 3 //Pixelwidth needed for the ringanimation
#define PXLBLCK_ANIMATION_COOLDOWN_TIME  1000
#define PXLBLCK_COMMAND_ANIMATION_STANDARD_TIME 20

//== Defines for animations == End ============================

//== Wordclock-specific values == Start ============================

//Wordclock Language Values
#define PLUGIN_205_WORDCLOCK_LANGUAGE_VALUE_GERMAN 0
#define PLUGIN_205_WORDCLOCK_LANGUAGE_NUM 1 //Number of installed/available languages
#define PLUGIN_205_WORDCLOCK_CHARACTER_GROUPS_NUM 26 //Number of character groups 
#define PLUGIN_205_WORDCLOCK_DIAL_NONE (12) //Just a Placeholder for empty spaces in the "character group"-array to mark these fields as unused. The value must be reather then the Wordclock-Matrix-Width to be ignored by the "character-selection"-function.

//Language Values
#define PLUGIN_205_LANGUAGE_ID_GERMAN 0

//== Wordclock-specific values == End ============================

//== Ringclock-specific values == Start ============================

boolean Plugin_205_ringclockThick12markEnabled = false;
boolean Plugin_205_ringclockClockDirInversed = false;
uint8_t Plugin_205_ringclockHourMarksBrightness = PXLBLCK_HOUR_MARK_STANDARD_BRIGHTNESS;   //brightness of the hour marks
uint8_t Plugin_205_ringclockClockTopOffset;    //position of the 12 o'clock LED on the strip

//== Ringclock-specific values == End ============================

//== Fiboclock-specific values == Start ============================

uint8_t Plugin_205_bits[PXLBLCK_FIBOCLOCK_MATRIX_HEIGHT];

//== Fiboclock-specific values == End ============================

//== digitClock-specific values == Start ============================


//== Fiboclock-specific values == End ============================

//== defines for PxlBlckUtils == Start ============================

#define PXLBLCK_RNG_TXT_STRUCT Plugin_205_rngTxtVar
#define PXLBLCK_ICON_STRUCT Plugin_205_iconVar
#define PXLBLCK_INSTANCE Plugin_205_matrix
#define PXLBLCK_FAKE_TV_STRUCT Plugin_205_fakeTV
#define PXLBLCK_DEVICE_NAME "PxlBlck"
#define PXLBLCK_HIGHER_COLOR_RESOLUTION_ENABLED Plugin_205_higherColorResultionEnabled
#define PXLBLCK_MAX_SETABLE_BRIGHTNESS 15.0     //maximal possible value of setable brightness-range(should be smaler than 16)
#define PXLBLCK_MINIMAL_BRIGHTNESS Plugin_205_minimalBrightness
#define PXLBLCK_ICON_COOLDOWN_TIME 5000
#define PXLBLCK_ICON_SHOWED_TIMESTAMP Plugin_205_iconShowedTimestamp

//== defines for PxlBlckUtils == End ============================

//== Variables for running-text == Start ============================

struct Plugin_205_rngTxtStruct
{
  Plugin_205_rngTxtStruct() :
    runtxtColor(0), runtxtBgColor(0), runtxtDelayTime(0), runtxtPassedDelayTime(0), runtxtText(""), runtxtPosition(0) {}
  uint32_t runtxtColor = 0;
  uint32_t runtxtBgColor = 0;
  uint16_t runtxtDelayTime = 0;
  unsigned long runtxtPassedDelayTime = 0;
  String runtxtText = "";
  int16_t runtxtPosition = 0;
} Plugin_205_rngTxtVar;

//== Variables for running-text == End ============================

//== Variables for icon-stuff == Start ============================

struct Plugin_205_iconStruct
{
  Plugin_205_iconStruct() :
    iconPending(false), iconState(PXLBLCK_ICON_STATE_END), inAnimation(PXLBLCK_ICON_ANIM_NONE_ANIMATION), outAnimation(PXLBLCK_ICON_ANIM_NONE_ANIMATION), logo(),
    inDelay(0), outDelay(0), showDelay(0), showDelayTimestamp(0), brightness(100), textThatFollows(""), spiffsIcon(""), repetition(0) {}
  boolean iconPending = false;
  uint8_t iconState = PXLBLCK_ICON_STATE_END;
  uint8_t inAnimation = PXLBLCK_ICON_ANIM_NONE_ANIMATION;
  uint8_t outAnimation = PXLBLCK_ICON_ANIM_NONE_ANIMATION;
  uint8_t logo[3][PXLBLCK_ICON_WIDTH][PXLBLCK_ICON_HEIGHT];
  uint16_t inDelay = 0;
  uint16_t outDelay = 0;
  uint16_t showDelay = 0;
  long unsigned showDelayTimestamp = 0;
  uint8_t brightness = 100;
  String textThatFollows = "";
  String spiffsIcon = "";
  uint8_t repetition = 0;
} Plugin_205_iconVar;

//== Variables for icon-stuff == End ============================

//== Variables for fakeTV == Start ============================

struct Plugin_205_fakeTvStruct
{
  Plugin_205_fakeTvStruct() :
    running(false), executionInterval(0), lastExecution(0), r(0), g(0), b(100), frameNumber(0), framePosition(0) {}
  boolean running = false;
  long unsigned executionInterval = 0;
  long unsigned lastExecution = 0;
  uint8_t r = 0;
  uint8_t g = 0;
  uint8_t b = 0;
  uint32_t frameNumber = 0;
  uint32_t framePosition = 0;
} Plugin_205_fakeTV;

//== Variables for fakeTV == End ============================

//== Array for simple 5x4-pixel numbers == Start ============================

//The following array holds the coordinates of all numbers from 0 to 9. The first index holds the desired number, the second index holds each pixel of the desired number and the third index holds the coordinates of each pixel of each number.
//The coorinates of the pixels are given as x,y. The base of the pixel coordinate-system is on the upper left corner.
const uint8_t Plugin_205_5x4Numbers[PLUGIN_205_DEFINED_NUMBERS][PLUGIN_205_MAX_PIXELS_PER_DIGIT][PLUGIN_205_COORDINATES_PER_PIXEL] =  //15 pixel per number possible, 2 coordinates for each pixel
{
  {{0, 0}, {1, 0}, {2, 0}, {0, 1}, {2, 1}, {0, 2}, {2, 2}, {0, 3}, {2, 3}, {0, 4}, {1, 4}, {2, 4}, {PLUGIN_205_5x4_NUMBER_NONE, PLUGIN_205_5x4_NUMBER_NONE}}, //ZERO
  {{2, 0}, {2, 1}, {2, 2}, {2, 3}, {2, 4}, {PLUGIN_205_5x4_NUMBER_NONE, PLUGIN_205_5x4_NUMBER_NONE}, {PLUGIN_205_5x4_NUMBER_NONE, PLUGIN_205_5x4_NUMBER_NONE}, {PLUGIN_205_5x4_NUMBER_NONE, PLUGIN_205_5x4_NUMBER_NONE}, {PLUGIN_205_5x4_NUMBER_NONE, PLUGIN_205_5x4_NUMBER_NONE}, {PLUGIN_205_5x4_NUMBER_NONE, PLUGIN_205_5x4_NUMBER_NONE}, {PLUGIN_205_5x4_NUMBER_NONE, PLUGIN_205_5x4_NUMBER_NONE}, {PLUGIN_205_5x4_NUMBER_NONE, PLUGIN_205_5x4_NUMBER_NONE}, {PLUGIN_205_5x4_NUMBER_NONE, PLUGIN_205_5x4_NUMBER_NONE}}, //ONE
  {{0, 0}, {1, 0}, {2, 0}, {2, 1}, {0, 2}, {1, 2}, {2, 2}, {0, 3}, {0, 4}, {1, 4}, {2, 4}, {PLUGIN_205_5x4_NUMBER_NONE, PLUGIN_205_5x4_NUMBER_NONE}, {PLUGIN_205_5x4_NUMBER_NONE, PLUGIN_205_5x4_NUMBER_NONE}}, //TWO
  {{0, 0}, {1, 0}, {2, 0}, {2, 1}, {0, 2}, {1, 2}, {2, 2}, {2, 3}, {0, 4}, {1, 4}, {2, 4}, {PLUGIN_205_5x4_NUMBER_NONE, PLUGIN_205_5x4_NUMBER_NONE}, {PLUGIN_205_5x4_NUMBER_NONE, PLUGIN_205_5x4_NUMBER_NONE}}, //THREE
  {{0, 0}, {2, 0}, {0, 1}, {2, 1}, {0, 2}, {1, 2}, {2, 2}, {2, 3}, {2, 4}, {PLUGIN_205_5x4_NUMBER_NONE, PLUGIN_205_5x4_NUMBER_NONE}, {PLUGIN_205_5x4_NUMBER_NONE, PLUGIN_205_5x4_NUMBER_NONE}, {PLUGIN_205_5x4_NUMBER_NONE, PLUGIN_205_5x4_NUMBER_NONE}, {PLUGIN_205_5x4_NUMBER_NONE, PLUGIN_205_5x4_NUMBER_NONE}}, //FOUR
  {{0, 0}, {1, 0}, {2, 0}, {0, 1}, {0, 2}, {1, 2}, {2, 2}, {2, 3}, {0, 4}, {1, 4}, {2, 4}, {PLUGIN_205_5x4_NUMBER_NONE, PLUGIN_205_5x4_NUMBER_NONE}, {PLUGIN_205_5x4_NUMBER_NONE, PLUGIN_205_5x4_NUMBER_NONE}}, //FIVE
  {{0, 0}, {1, 0}, {2, 0}, {0, 1}, {0, 2}, {1, 2}, {2, 2}, {0, 3}, {2, 3}, {0, 4}, {1, 4}, {2, 4}, {PLUGIN_205_5x4_NUMBER_NONE, PLUGIN_205_5x4_NUMBER_NONE}}, //SIX
  {{0, 0}, {1, 0}, {2, 0}, {2, 1}, {2, 2}, {2, 3}, {2, 4}, {PLUGIN_205_5x4_NUMBER_NONE, PLUGIN_205_5x4_NUMBER_NONE}, {PLUGIN_205_5x4_NUMBER_NONE, PLUGIN_205_5x4_NUMBER_NONE}, {PLUGIN_205_5x4_NUMBER_NONE, PLUGIN_205_5x4_NUMBER_NONE}, {PLUGIN_205_5x4_NUMBER_NONE, PLUGIN_205_5x4_NUMBER_NONE}, {PLUGIN_205_5x4_NUMBER_NONE, PLUGIN_205_5x4_NUMBER_NONE}, {PLUGIN_205_5x4_NUMBER_NONE, PLUGIN_205_5x4_NUMBER_NONE}}, //SEVEN
  {{0, 0}, {1, 0}, {2, 0}, {0, 1}, {2, 1}, {0, 2}, {1, 2}, {2, 2}, {0, 3}, {2, 3}, {0, 4}, {1, 4}, {2, 4}}, //EIGHT
  {{0, 0}, {1, 0}, {2, 0}, {0, 1}, {2, 1}, {0, 2}, {1, 2}, {2, 2}, {2, 3}, {0, 4}, {1, 4}, {2, 4}, {PLUGIN_205_5x4_NUMBER_NONE, PLUGIN_205_5x4_NUMBER_NONE}}, //NINE
};

//== Array for simple 5x4-pixel numbers == End ============================

//== Defines for Wordclock-Dial-Stuff == Start ============================

//== LED Groups start ==
//The following assignments define a unique id for every group of leds that can be shown
#define IT_IS  1
#define FIVE_FIRSTONE  2
#define TEN_FIRSTONE  3
#define TWENTY_FIRSTONE  4
#define THREE_FIRSTONE  5
#define QUARTER  6
#define AFTER  7
#define BEFORE  8
#define HALF  9
#define TWELVE  10
#define TWO  11
#define ONE  12
#define SEVEN  13
#define THREE  14
#define FIVE 15
#define ELEVEN  16
#define NINE  17
#define FOUR  18
#define EIGHT  19
#define TEN  20
#define SIX  21
#define OCLOCK  22
#define MINUTE1  23
#define MINUTE2  24
#define MINUTE3  25
#define MINUTE4  26
//== LED Groups end ==

//This array(characters) contains the positions of all leds that should be lit for the specific character group.
//At the first position of every character group is always the row-position of the character group, then followed by all column-positions of the leds belonging to the character group.
const uint8_t Plugin_205_wordclockCharacterGroups[PLUGIN_205_WORDCLOCK_LANGUAGE_NUM][PLUGIN_205_WORDCLOCK_CHARACTER_GROUPS_NUM][12] = {
  {
    {0, 0, 1, 3, 4, 5, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE}, //IT_IS
    {0, 7, 8, 9, 10, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE},//FIVE_FIRSTONE
    {1, 0, 1, 2, 3, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE},//TEN_FIRSTONE
    {1, 4, 5, 6, 7, 8, 9, 10, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE},//TWENTY_FIRSTONE
    {2, 0, 1, 2, 3, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE},//THREE_FIRSTONE
    {2, 4, 5, 6, 7, 8, 9, 10, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE},//QUARTER
    {3, 2, 3, 4, 5, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE},//AFTER
    {3, 6, 7, 8, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE},//BEFORE
    {4, 0, 1, 2, 3, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE},//HALF
    {4, 5, 6, 7, 8, 9, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE},//TWELVE
    {5, 0, 1, 2, 3, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE},//TWO
    {5, 2, 3, 4, 5, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE},//ONE
    {5, 5, 6, 7, 8, 9, 10, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE},//SEVEN
    {6, 1, 2, 3, 4, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE},//THREE
    {6, 7, 8, 9, 10, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE},//FIVE
    {7, 0, 1, 2, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE},//ELEVEN
    {7, 3, 4, 5, 6, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE},//NINE
    {7, 7, 8, 9, 10, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE},//FOUR
    {8, 1, 2, 3, 4, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE},//EIGHT
    {8, 5, 6, 7, 8, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE},//TEN
    {9, 1, 2, 3, 4, 5, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE},//SIX
    {9, 8, 9, 10, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE},//OCLOCK
    {10, 1, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE},//MINUTE1
    {10, 2, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE},//MINUTE2
    {10, 3, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE},//MINUTE3
    {10, 4, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE}//MINUTE4
    /*
      {0, 0, 1, 3, 4, 5, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE}, //IT_IS
      {0, 7, 8, 9, 10, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE},//FIVE_FIRSTONE
      {1, 0, 1, 2, 3, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE},//TEN_FIRSTONE
      {1, 4, 5, 6, 7, 8, 9, 10, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE},//TWENTY_FIRSTONE
      {2, 0, 1, 2, 3, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE},//THREE_FIRSTONE
      {2, 4, 5, 6, 7, 8, 9, 10, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE},//QUARTER
      {3, 2, 3, 4, 5, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE},//AFTER
      {3, 6, 7, 8, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE},//BEFORE
      {4, 0, 1, 2, 3, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE},//HALF
      {4, 5, 6, 7, 8, 9, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE},//TWELVE
      {5, 0, 1, 2, 3, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE},//TWO
      {5, 2, 3, 4, 5, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE},//ONE
      {5, 5, 6, 7, 8, 9, 10, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE},//SEVEN
      {6, 1, 2, 3, 4, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE},//THREE
      {6, 7, 8, 9, 10, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE},//FIVE
      {7, 0, 1, 2, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE},//ELEVEN
      {7, 3, 4, 5, 6, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE},//NINE
      {7, 7, 8, 9, 10, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE},//FOUR
      {8, 1, 2, 3, 4, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE},//EIGHT
      {8, 5, 6, 7, 8, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE},//TEN
      {9, 1, 2, 3, 4, 5, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE},//SIX
      {9, 8, 9, 10, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE},//OCLOCK
      {10, 1, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE},//MINUTE1
      {10, 2, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE},//MINUTE2
      {10, 3, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE},//MINUTE3
      {10, 4, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE, PLUGIN_205_WORDCLOCK_DIAL_NONE}//MINUTE4
    */
  }
};

//== Defines for Wordclock-Dial-Stuff == End ============================



// == Variable, Constants & Object Defintions == End =========================================================================================================================

boolean Plugin_205(byte function, struct EventStruct *event, String& string)
{
  boolean success = false;

  switch (function)
  {

    case PLUGIN_DEVICE_ADD:
      {
        Device[++deviceCount].Number = PLUGIN_ID_205;
        Device[deviceCount].Type = DEVICE_TYPE_SINGLE;
        Device[deviceCount].VType = SENSOR_TYPE_TRIPLE;
        Device[deviceCount].Ports = 0;
        Device[deviceCount].PullUpOption = false;
        Device[deviceCount].InverseLogicOption = false;
        Device[deviceCount].FormulaOption = false;
        Device[deviceCount].ValueCount = 3;
        Device[deviceCount].SendDataOption = false;

        break;
      }

    case PLUGIN_GET_DEVICENAME:
      {
        string = F(PLUGIN_NAME_205);
        break;
      }

    case PLUGIN_GET_DEVICEVALUENAMES:
      {
        strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[0], PSTR(PLUGIN_VALUENAME1_205));
        strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[1], PSTR(PLUGIN_VALUENAME2_205));
        strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[2], PSTR(PLUGIN_VALUENAME3_205));
        break;
      }

    case PLUGIN_GET_DEVICEGPIONAMES:
      {
        event->String1 = formatGpioName_input(F("DigitalIn"));
        break;
      }

    case PLUGIN_WEBFORM_LOAD:
      {
        //Start output of Form-parts
        addFormSubHeader(F("pxlBlck configuration"));

        if (!systemTimePresent()) //Add hint for needed NTP configuration
          pxlBlckUtils_addFormSubHeaderCaution(F("A Time-Source is needed: Enable and configure NTP or RTC!"));

        static int numberOfElementsInArray = (sizeof(Plugin_205_matrixNamesById) / sizeof(Plugin_205_matrixNamesById[0]));

        int possibleMatrixIds[numberOfElementsInArray];
        for (uint8_t i = 0; i < numberOfElementsInArray; i++)
        {
          possibleMatrixIds[i] = i;
        }

        pxlBlckUtils_addHelpButton(F("Help"), F("pxlblck"));
        addFormNote(F("To get more information click on the ?"));

        addFormCheckBox(F("Display enabled"), F(PXLBLCK_WEBSERVER_FORM_ID_DISPLAY_ENABLED), Plugin_205_displayEnabled);
        addFormNote(F("Enable or disable the display output"));

        //addFormSelector(const String& label, const String& id, int optionCount, const String options[], const int indices[], int selectedIndex, bool reloadonchange)
        addFormSelector(F("Matrix type"), F(PXLBLCK_WEBSERVER_FORM_ID_MATRIX_TYPE), numberOfElementsInArray, Plugin_205_matrixNamesById, possibleMatrixIds, Plugin_205_selectedMatrixId, true);
        addFormNote(F("Select Matrix-Type"));

        String possibleLedTypes[3] = {F("1: RGB"), F("2: GRB"), F("3: RGBW")};
        int possibleLedTypeValues[3] = {NEO_RGB, NEO_GRB, NEO_RGBW};
        addFormSelector(F("Led type"), F(PXLBLCK_WEBSERVER_FORM_ID_LED_TYPE), 3, possibleLedTypes, possibleLedTypeValues, PXLBLCK_LED_COLOR_ORDER, true);
        addFormNote(F("Select LED-Color-Order"));

        //in case of led matrix that is not the Ringlock matrix show settings for matrix layout
        if (Plugin_205_selectedMatrixId != PXLBLCK_RINGCLOCK_MATRIX_ID
            && Plugin_205_selectedMatrixId != PXLBLCK_FIBOCLOCK_MATRIX_ID
            && Plugin_205_selectedMatrixId != PXLBLCK_DIGIT_CLOCK_MATRIX_ID)
        {
          //Settings for single LED matrix
          String possibleStartPositions[4] = {F("1: Top-Left"), F("2: Top-Right"), F("3: Bottom-Left"), F("4: Bottom-Right")};
          int possibleStartPositionsValues[4] = {(NEO_MATRIX_TOP + NEO_MATRIX_LEFT), (NEO_MATRIX_TOP + NEO_MATRIX_RIGHT), (NEO_MATRIX_BOTTOM + NEO_MATRIX_LEFT), (NEO_MATRIX_BOTTOM + NEO_MATRIX_RIGHT)};
          addFormSelector(F("Start positions"), F(PXLBLCK_WEBSERVER_FORM_ID_START_POSITION), 4, possibleStartPositions, possibleStartPositionsValues, Plugin_205_matrixLayoutStartPosition, true);
          addFormNote(F("Select the start-position of your LED-Matrix."));

          String possibleArrangement[4] = {F("1: Column-Progressive"), F("2: Column-ZigZag"), F("3: Row-Progressive"), F("4: Row-ZigZag")};
          int possibleArrangementValues[4] = {(NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE), (NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG), (NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE), (NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG)};
          addFormSelector(F("LED-Arrangement"), F(PXLBLCK_WEBSERVER_FORM_ID_MATRIX_ARRANGEMENT), 4, possibleArrangement, possibleArrangementValues, Plugin_205_matrixArrangement, true);
          addFormNote(F("Select the Arrangement of your LED-Matrix."));

          /*
                    //Settings for multiple LED matrices
                    String possibleTileStartPositions[5] = {F("0: OneTileOnly"), F("1: Top-Left"), F("2: Top-Right"), F("3: Bottom-Left"), F("4: Bottom-Right")};
                    int possibleTileStartPositionsValues[5] = {PXLBLCK_ONE_TILE_ONLY_VALUE, (NEO_TILE_TOP + NEO_TILE_LEFT), (NEO_TILE_TOP + NEO_TILE_RIGHT), (NEO_TILE_BOTTOM + NEO_TILE_LEFT), (NEO_TILE_BOTTOM + NEO_TILE_RIGHT)};
                    addFormSelector(F("Tile start position"), F(PXLBLCK_WEBSERVER_FORM_ID_TILE_START_POSITION), 5, possibleTileStartPositions, possibleTileStartPositionsValues, Plugin_205_matrixLayoutTileStartPosition, true);
                    addFormNote(F("Select the start-position of the first tile."));
          */

          String possibleTileArrangement[17] = {
            F("0: OneTileOnly"),

            F("1: Column-Progressive-Top-Left"),
            F("2: Column-ZigZag-Top-Left"),
            F("3: Row-Progressive-Top-Left"),
            F("4: Row-ZigZag-Top-Left"),

            F("5: Column-Progressive-Top-Right"),
            F("6: Column-ZigZag-Top-Right"),
            F("7: Row-Progressive-Top-Right"),
            F("8: Row-ZigZag-Top-Right"),

            F("9: Column-Progressive-Bottom-Left"),
            F("10: Column-ZigZag-Bottom-Left"),
            F("11: Row-Progressive-Bottom-Left"),
            F("12: Row-ZigZag-Bottom-Left"),

            F("13: Column-Progressive-Bottom-Right"),
            F("14: Column-ZigZag-Bottom-Right"),
            F("15: Row-Progressive-Bottom-Right"),
            F("16: Row-ZigZag-Bottom-Right"),
          };
          int possibleTileArrangementValues[17] = {
            PXLBLCK_ONE_TILE_ONLY_VALUE,

            NEO_TILE_COLUMNS + NEO_TILE_PROGRESSIVE + NEO_TILE_TOP + NEO_TILE_LEFT,
            NEO_TILE_COLUMNS + NEO_TILE_ZIGZAG + NEO_TILE_TOP + NEO_TILE_LEFT,
            NEO_TILE_ROWS + NEO_TILE_PROGRESSIVE + NEO_TILE_TOP + NEO_TILE_LEFT,
            NEO_TILE_ROWS + NEO_TILE_ZIGZAG + NEO_TILE_TOP + NEO_TILE_LEFT,

            NEO_TILE_COLUMNS + NEO_TILE_PROGRESSIVE + NEO_TILE_TOP + NEO_TILE_RIGHT,
            NEO_TILE_COLUMNS + NEO_TILE_ZIGZAG + NEO_TILE_TOP + NEO_TILE_RIGHT,
            NEO_TILE_ROWS + NEO_TILE_PROGRESSIVE + NEO_TILE_TOP + NEO_TILE_RIGHT,
            NEO_TILE_ROWS + NEO_TILE_ZIGZAG + NEO_TILE_TOP + NEO_TILE_RIGHT,

            NEO_TILE_COLUMNS + NEO_TILE_PROGRESSIVE + NEO_TILE_BOTTOM + NEO_TILE_LEFT,
            NEO_TILE_COLUMNS + NEO_TILE_ZIGZAG + NEO_TILE_BOTTOM + NEO_TILE_LEFT,
            NEO_TILE_ROWS + NEO_TILE_PROGRESSIVE + NEO_TILE_BOTTOM + NEO_TILE_LEFT,
            NEO_TILE_ROWS + NEO_TILE_ZIGZAG + NEO_TILE_BOTTOM + NEO_TILE_LEFT,

            NEO_TILE_COLUMNS + NEO_TILE_PROGRESSIVE + NEO_TILE_BOTTOM + NEO_TILE_RIGHT,
            NEO_TILE_COLUMNS + NEO_TILE_ZIGZAG + NEO_TILE_BOTTOM + NEO_TILE_RIGHT,
            NEO_TILE_ROWS + NEO_TILE_PROGRESSIVE + NEO_TILE_BOTTOM + NEO_TILE_RIGHT,
            NEO_TILE_ROWS + NEO_TILE_ZIGZAG + NEO_TILE_BOTTOM + NEO_TILE_RIGHT,
          };
          addFormSelector(F("Tile-Arrangement"), F(PXLBLCK_WEBSERVER_FORM_ID_TILE_MATRIX_ARRANGEMENT), 17, possibleTileArrangement, possibleTileArrangementValues, Plugin_205_matrixTileArrangement, true);
          addFormNote(F("Select the Arrangement of your tile-Matrix."));

          addFormNumericBox(F("Matrix tiles in X-direction"), F(PXLBLCK_WEBSERVER_FORM_ID_MATRIX_TILES_WIDTH), Plugin_205_matrixTilesWidth, 1, PXLBLCK_MAX_SETABLE_MATRIX_TILES_IN_X_DIR);
          addFormNote(F("Number of matrix-tiles in X-direction"));

          addFormNumericBox(F("Matrix tiles in Y-direction"), F(PXLBLCK_WEBSERVER_FORM_ID_MATRIX_TILES_HEIGHT), Plugin_205_matrixTilesHeight, 1, PXLBLCK_MAX_SETABLE_MATRIX_TILES_IN_Y_DIR);
          addFormNote(F("Number of matrix-tiles in Y-direction"));

          addFormNumericBox(F("Matrix rotation"), F(PXLBLCK_WEBSERVER_FORM_ID_ROTATION), Plugin_205_matrixRotation, 0, 3);
          addFormNote(F("Increase to rotate clockwise (0-3)"));
        }

        //=== preparing the available dials for the selected Plugin_205_selectedMatrixId === start ==

        //Count all the available dials (that are not equal PXLBLCK_DIAL_NONE) for the actual selected matrix (defined by Plugin_205_selectedMatrixId)
        //Collect all available dials and copy them to the "possibleDials"-array
        int possibleDialValueLength = 0;
        String possibleDials[PLUGIN_205_MAX_DIAL_NUM];
        int possibleDialsValues[PLUGIN_205_MAX_DIAL_NUM];

        for (int i = 0; i < PLUGIN_205_MAX_DIAL_NUM; i++)
        {
          if (!Plugin_205_possibleDialList[Plugin_205_selectedMatrixId][i][0].equals(PXLBLCK_DIAL_NONE))
          {
            possibleDialValueLength++;
            possibleDials[i] = Plugin_205_possibleDialList[Plugin_205_selectedMatrixId][i][0];
            possibleDialsValues[i] = Plugin_205_possibleDialList[Plugin_205_selectedMatrixId][i][1].toInt();
          }
        }

        String log = F(PXLBLCK_DEVICE_NAME);
        addLog(LOG_LEVEL_DEBUG, log);
        log = F("   -Plugin_205_selectedMatrixId=");
        log += String(Plugin_205_selectedMatrixId);
        addLog(LOG_LEVEL_DEBUG, log);
        log = F("   -possibleDialValueLength=");
        log += String(possibleDialValueLength);
        addLog(LOG_LEVEL_DEBUG, log);
        log = "";
        for (int i = 0; i < possibleDialValueLength; i++)
        {
          log += F("   -possibleDials[i]=");
          log += String(possibleDials[i]);
          log += F("; possibleDialsValues[i]=");
          log += String(possibleDialsValues[i]);
        }
        addLog(LOG_LEVEL_DEBUG, log);

        addFormSelector(F("Dial version"), F(PXLBLCK_WEBSERVER_FORM_ID_DIAL), possibleDialValueLength, possibleDials, possibleDialsValues, Plugin_205_selectedDial, true);
        addFormNote(F("Select the dial that will be used to display time."));

        //=== preparing the available dials for the selected Plugin_205_selectedMatrixId === end ==

        //Wordclock specific form parts
        if (Plugin_205_selectedMatrixId == PXLBLCK_WORDCLOCK_MATRIX_ID)
        {
          String possibleWordclockLanguages[1] = {F("German")};
          int wordclockLanguageValues[1] = {PLUGIN_205_WORDCLOCK_LANGUAGE_VALUE_GERMAN};
          addFormSelector(F("Language"), F(PXLBLCK_WEBSERVER_FORM_ID_WORDCLOCK_LANGUAGE), 1, possibleWordclockLanguages, wordclockLanguageValues, (Plugin_205_selectedWordclockLanguage + 1));

          addFormCheckBox(F("Show \"clock\""), F(PXLBLCK_WEBSERVER_FORM_ID_WORDCLOCK_O_CLOCK_ENABLED), Plugin_205_wordclockShowOClockEnabled);
          addFormNote(F("Control the visibility of the \"o'clock\" part"));

          addFormCheckBox(F("Show \"It is\""), F(PXLBLCK_WEBSERVER_FORM_ID_WORDCLOCK_IT_IS_ENABLED), Plugin_205_wordclockShowItIsEnabled);
          addFormNote(F("Control the visibility of the \"It is\" part"));

        }

        //Ringclock specific form parts
        if (Plugin_205_selectedMatrixId == PXLBLCK_RINGCLOCK_MATRIX_ID)
        {
          addFormNumericBox(F("12 o'clock LED position"), F(PXLBLCK_WEBSERVER_FORM_ID_RINGCLOCK_LED_OFFSET), Plugin_205_ringclockClockTopOffset, 0, 59);
          addFormNote(F("Position of the 12 o'clock LED in the strip"));

          addFormCheckBox(F("Thick 12 o'clock mark"), F(PXLBLCK_WEBSERVER_FORM_ID_RINGCLOCK_THICK_MARK_ENABLED), Plugin_205_ringclockThick12markEnabled);
          addFormNote(F("Check to have 3 LEDs marking the 12 o'clock position"));

          addFormCheckBox(F("Direction inversed"), F(PXLBLCK_WEBSERVER_FORM_ID_RINGCLOCK_DIR_INVERSED), Plugin_205_ringclockClockDirInversed);
          addFormNote(F("Inverse 'turn-direction' of clock"));

          addFormNumericBox(F("Hour mark brightness"), F(PXLBLCK_WEBSERVER_FORM_ID_RINGCLOCK_HOUR_MARK_BRIGHTNESS), Plugin_205_ringclockHourMarksBrightness, 1, PXLBLCK_MAX_SETABLE_BRIGHTNESS);
          addFormNote(F("Brightness level of the hour marks (1-15)"));
        }

        //digitClock specific form parts: CHANGED: Made this available for every matrix to make this variable usable for other (normal) dials
        //if (Plugin_205_selectedMatrixId == PXLBLCK_DIGIT_CLOCK_MATRIX_ID)
        //{
        addFormCheckBox(F("Leading zeros enabled"), F(PXLBLCK_WEBSERVER_FORM_ID_DIGITCLOCK_LEADING_ZEROS_ENABLED), Plugin_205_diallLeadingZerosEnabled);
        addFormNote(F("Enabling display of leading Zeros."));
        //}

        //General form parts
        addFormNumericBox(F("Display brightness"), F(PXLBLCK_WEBSERVER_FORM_ID_BRIGHTNESS), Plugin_205_displayBrightness, 1, PXLBLCK_MAX_SETABLE_BRIGHTNESS);
        addFormNote(F("Brightness level of Display (1-15)"));

        addFormNumericBox(F("Minimal brightness"), F(PXLBLCK_WEBSERVER_FORM_ID_MINIMAL_BRIGHTNESS), Plugin_205_minimalBrightness, 3, 255);
        addFormNote(F("Minimal brightness level of Display (3-255)"));

        //Color specific form parts
        Plugin_205_colorOneName = "";
        Plugin_205_colorTwoName = "";
        Plugin_205_colorThreeName = "";
        Plugin_205_colorFourName = "";

        //Depending of the selected dial id the color-names will be set
        switch (Plugin_205_selectedDial)
        {
          case PXLBLCK_DIAL_NAME_BLANK_ID_INT:
            Plugin_205_colorOneName = F(PXLBLCK_WEBSERVER_FORM_COLOR_NOT_USED_VALUE);
            Plugin_205_colorTwoName = F(PXLBLCK_WEBSERVER_FORM_COLOR_NOT_USED_VALUE);
            Plugin_205_colorThreeName = F(PXLBLCK_WEBSERVER_FORM_COLOR_NOT_USED_VALUE);
            Plugin_205_colorFourName = F("Background");
            break;
          case PXLBLCK_DIAL_NAME_HR_NM_AND_MN_PNTS_ID_INT:
            Plugin_205_colorOneName = F("Hour");
            Plugin_205_colorTwoName = F("Minute");
            Plugin_205_colorThreeName = F(PXLBLCK_WEBSERVER_FORM_COLOR_NOT_USED_VALUE);
            Plugin_205_colorFourName = F("Background");
            break;
          case PXLBLCK_DIAL_NAME_RANDOM_PIXELS_ID_INT:
            Plugin_205_colorOneName = F(PXLBLCK_WEBSERVER_FORM_COLOR_NOT_USED_VALUE);
            Plugin_205_colorTwoName = F(PXLBLCK_WEBSERVER_FORM_COLOR_NOT_USED_VALUE);
            Plugin_205_colorThreeName = F(PXLBLCK_WEBSERVER_FORM_COLOR_NOT_USED_VALUE);
            Plugin_205_colorFourName = F("Background");
            break;
          case PXLBLCK_DIAL_NAME_WANDERING_PIXELS_ID_INT:
            Plugin_205_colorOneName = F("Pixel-Color");
            Plugin_205_colorTwoName = F(PXLBLCK_WEBSERVER_FORM_COLOR_NOT_USED_VALUE);
            Plugin_205_colorThreeName = F(PXLBLCK_WEBSERVER_FORM_COLOR_NOT_USED_VALUE);
            Plugin_205_colorFourName = F("Background");
            break;
          case PXLBLCK_DIAL_NAME_TV_SIMULATOR_ID_INT:
            Plugin_205_colorOneName = F(PXLBLCK_WEBSERVER_FORM_COLOR_NOT_USED_VALUE);
            Plugin_205_colorTwoName = F(PXLBLCK_WEBSERVER_FORM_COLOR_NOT_USED_VALUE);
            Plugin_205_colorThreeName = F(PXLBLCK_WEBSERVER_FORM_COLOR_NOT_USED_VALUE);
            Plugin_205_colorFourName = F("Background");
            break;
          case PXLBLCK_DIAL_NAME_FIBONACCI_CLOCK_ID_INT:
            Plugin_205_colorOneName = F("Hour");
            Plugin_205_colorTwoName = F("Minute");
            Plugin_205_colorThreeName = F("Both");
            Plugin_205_colorFourName = F("None");
            break;
          case PXLBLCK_DIAL_NAME_RINGGCLOCK_DIAL_ID_INT:
            Plugin_205_colorOneName = F("Hour");
            Plugin_205_colorTwoName = F("Minute");
            Plugin_205_colorThreeName = F("Second");
            Plugin_205_colorFourName = F("Marks");
            break;
          case PXLBLCK_DIAL_NAME_WORDCLOCK_DIAL_ID_INT:
            Plugin_205_colorOneName = F("Hour");
            Plugin_205_colorTwoName = F("Minute");
            Plugin_205_colorThreeName = F(PXLBLCK_WEBSERVER_FORM_COLOR_NOT_USED_VALUE);
            Plugin_205_colorFourName = F("Background");
            break;
          case PXLBLCK_DIAL_NAME_DIGIT_CLOCK_DIAL_ID_INT:
            Plugin_205_colorOneName = F("Hour");
            Plugin_205_colorTwoName = F("Minute");
            Plugin_205_colorThreeName = F("Dots");
            Plugin_205_colorFourName = F("Background");
            break;
          case PXLBLCK_DIAL_NAME_HORIZONTAL_NUMBERS_DIAL_ID_INT:
            Plugin_205_colorOneName = F("Hour");
            Plugin_205_colorTwoName = F("Minute");
            Plugin_205_colorThreeName = F("Dots");
            Plugin_205_colorFourName = F("Background");
            break;
          case PXLBLCK_DIAL_NAME_HORIZONTAL_MINI_NUMBERS_DIAL_ID_INT:
            Plugin_205_colorOneName = F("Hour");
            Plugin_205_colorTwoName = F("Minute");
            Plugin_205_colorThreeName = F(PXLBLCK_WEBSERVER_FORM_COLOR_NOT_USED_VALUE);
            Plugin_205_colorFourName = F("Background");
            break;
          case PXLBLCK_DIAL_NAME_VERTICAL_MINI_NUMBERS_DIAL_ID_INT:
            Plugin_205_colorOneName = F("Hour");
            Plugin_205_colorTwoName = F("Minute");
            Plugin_205_colorThreeName = F(PXLBLCK_WEBSERVER_FORM_COLOR_NOT_USED_VALUE);
            Plugin_205_colorFourName = F("Background");
            break;
          case PXLBLCK_DIAL_NAME_DIAGONAL_MINI_NUMBERS_DIAL_ID_INT:
            Plugin_205_colorOneName = F("Hour");
            Plugin_205_colorTwoName = F("Minute");
            Plugin_205_colorThreeName = F(PXLBLCK_WEBSERVER_FORM_COLOR_NOT_USED_VALUE);
            Plugin_205_colorFourName = F("Background");
            break;
          case PXLBLCK_DIAL_NAME_RUNNING_CLOCK_ID_INT:
            Plugin_205_colorOneName = F("Text");
            Plugin_205_colorTwoName = F(PXLBLCK_WEBSERVER_FORM_COLOR_NOT_USED_VALUE);
            Plugin_205_colorThreeName = F(PXLBLCK_WEBSERVER_FORM_COLOR_NOT_USED_VALUE);
            Plugin_205_colorFourName = F("Background");
            break;
          default:
            Plugin_205_colorOneName = F("Color1");
            Plugin_205_colorTwoName = F("Color2");
            Plugin_205_colorThreeName = F("Color3");
            Plugin_205_colorFourName = F("Color4");
            break;
        }

        pxlBlckUtils_addColorFormParts(PXLBLCK_WEBSERVER_FORM_ID_COLOR_ONE_PARTS, PXLBLCK_COLOR_PERMANENT_STORAGE(0), Plugin_205_colorOneName);

        pxlBlckUtils_addColorFormParts(PXLBLCK_WEBSERVER_FORM_ID_COLOR_TWO_PARTS, PXLBLCK_COLOR_PERMANENT_STORAGE(1), Plugin_205_colorTwoName);

        pxlBlckUtils_addColorFormParts(PXLBLCK_WEBSERVER_FORM_ID_COLOR_THREE_PARTS, PXLBLCK_COLOR_PERMANENT_STORAGE(2), Plugin_205_colorThreeName);

        pxlBlckUtils_addColorFormParts(PXLBLCK_WEBSERVER_FORM_ID_COLOR_FOUR_PARTS, PXLBLCK_COLOR_PERMANENT_STORAGE(3), Plugin_205_colorFourName);

        success = true;
        break;
      }

    case PLUGIN_WEBFORM_SAVE:
      {

        //== Output received values to log =======================
        String log = F(PXLBLCK_DEVICE_NAME);

        //General settings
        addLog(LOG_LEVEL_DEBUG, log);
        log = F("   enbld=");
        log += isFormItemChecked(F(PXLBLCK_WEBSERVER_FORM_ID_DISPLAY_ENABLED));
        addLog(LOG_LEVEL_DEBUG, log);
        log = F("   brightness=");
        log += getFormItemInt(F(PXLBLCK_WEBSERVER_FORM_ID_BRIGHTNESS));
        addLog(LOG_LEVEL_DEBUG, log);
        log = F("   minimal_brightness=");
        log += getFormItemInt(F(PXLBLCK_WEBSERVER_FORM_ID_MINIMAL_BRIGHTNESS));
        addLog(LOG_LEVEL_DEBUG, log);

        log = F("   dial=");
        log += getFormItemInt(F(PXLBLCK_WEBSERVER_FORM_ID_DIAL));
        addLog(LOG_LEVEL_DEBUG, log);
        log = F("   rot=");
        log += getFormItemInt(F(PXLBLCK_WEBSERVER_FORM_ID_ROTATION));
        addLog(LOG_LEVEL_DEBUG, log);

        //Matrix layout
        log = F("   mtrx_typ=");
        log += getFormItemInt(F(PXLBLCK_WEBSERVER_FORM_ID_MATRIX_TYPE));
        addLog(LOG_LEVEL_DEBUG, log);
        log = F("   strtps=");
        log += getFormItemInt(F(PXLBLCK_WEBSERVER_FORM_ID_START_POSITION));
        addLog(LOG_LEVEL_DEBUG, log);
        log = F("   arrngt=");
        log += getFormItemInt(F(PXLBLCK_WEBSERVER_FORM_ID_MATRIX_ARRANGEMENT));
        addLog(LOG_LEVEL_DEBUG, log);
        log = F("   led_type=");
        log += getFormItemInt(F(PXLBLCK_WEBSERVER_FORM_ID_LED_TYPE));
        addLog(LOG_LEVEL_DEBUG, log);

        //Tile Layout
        log = F("   tileMtrxArr=");
        log += getFormItemInt(F(PXLBLCK_WEBSERVER_FORM_ID_TILE_MATRIX_ARRANGEMENT));
        addLog(LOG_LEVEL_DEBUG, log);
        log = F("   tileWidth=");
        log += getFormItemInt(F(PXLBLCK_WEBSERVER_FORM_ID_MATRIX_TILES_WIDTH));
        addLog(LOG_LEVEL_DEBUG, log);
        log = F("   tileHeight=");
        log += getFormItemInt(F(PXLBLCK_WEBSERVER_FORM_ID_MATRIX_TILES_HEIGHT));
        addLog(LOG_LEVEL_DEBUG, log);

        //Wordclock
        log = F("   its_enbld=");
        log += isFormItemChecked(F(PXLBLCK_WEBSERVER_FORM_ID_WORDCLOCK_IT_IS_ENABLED));
        addLog(LOG_LEVEL_DEBUG, log);
        log = F("   oc_enbld=");
        log += isFormItemChecked(F(PXLBLCK_WEBSERVER_FORM_ID_WORDCLOCK_O_CLOCK_ENABLED));
        addLog(LOG_LEVEL_DEBUG, log);
        log = F("   lang=");
        log += getFormItemInt(F(PXLBLCK_WEBSERVER_FORM_ID_WORDCLOCK_LANGUAGE));
        addLog(LOG_LEVEL_DEBUG, log);

        //Ringclock
        log = F("   rc_offset=");
        log += getFormItemInt(F(PXLBLCK_WEBSERVER_FORM_ID_RINGCLOCK_LED_OFFSET));
        addLog(LOG_LEVEL_DEBUG, log);
        log = F("   rc_thick_mark=");
        log += isFormItemChecked(F(PXLBLCK_WEBSERVER_FORM_ID_RINGCLOCK_THICK_MARK_ENABLED));
        addLog(LOG_LEVEL_DEBUG, log);
        log = F("   rc_clock_inv=");
        log += isFormItemChecked(F(PXLBLCK_WEBSERVER_FORM_ID_RINGCLOCK_DIR_INVERSED));
        addLog(LOG_LEVEL_DEBUG, log);
        log = F("   rc_mark_brightness=");
        log += getFormItemInt(F(PXLBLCK_WEBSERVER_FORM_ID_RINGCLOCK_HOUR_MARK_BRIGHTNESS));
        addLog(LOG_LEVEL_DEBUG, log);

        //digitClock
        log = F("   lead_zero_enbld=");
        log += isFormItemChecked(F(PXLBLCK_WEBSERVER_FORM_ID_DIGITCLOCK_LEADING_ZEROS_ENABLED));
        addLog(LOG_LEVEL_DEBUG, log);



        //Colors
        pxlBlckUtils_add_color_values_to_debug_log(PXLBLCK_WEBSERVER_FORM_ID_COLOR_ONE_PARTS);
        pxlBlckUtils_add_color_values_to_debug_log(PXLBLCK_WEBSERVER_FORM_ID_COLOR_TWO_PARTS);
        pxlBlckUtils_add_color_values_to_debug_log(PXLBLCK_WEBSERVER_FORM_ID_COLOR_THREE_PARTS);
        pxlBlckUtils_add_color_values_to_debug_log(PXLBLCK_WEBSERVER_FORM_ID_COLOR_FOUR_PARTS);

        //== Save formular data to permanent storage and runtime variables =======================

        //General settings
        Plugin_205_displayEnabled = isFormItemChecked(F(PXLBLCK_WEBSERVER_FORM_ID_DISPLAY_ENABLED));
        Plugin_205_displayBrightness = getFormItemInt(F(PXLBLCK_WEBSERVER_FORM_ID_BRIGHTNESS));
        Plugin_205_minimalBrightness = getFormItemInt(F(PXLBLCK_WEBSERVER_FORM_ID_MINIMAL_BRIGHTNESS));
        Plugin_205_selectedDial = getFormItemInt(F(PXLBLCK_WEBSERVER_FORM_ID_DIAL));
        Plugin_205_matrixRotation = getFormItemInt(F(PXLBLCK_WEBSERVER_FORM_ID_ROTATION));
        Plugin_205_diallLeadingZerosEnabled = isFormItemChecked(F(PXLBLCK_WEBSERVER_FORM_ID_DIGITCLOCK_LEADING_ZEROS_ENABLED));

        //Matrix layout
        Plugin_205_selectedMatrixId = getFormItemInt(F(PXLBLCK_WEBSERVER_FORM_ID_MATRIX_TYPE));
        Plugin_205_matrixLayoutStartPosition = getFormItemInt(F(PXLBLCK_WEBSERVER_FORM_ID_START_POSITION));
        Plugin_205_matrixArrangement = getFormItemInt(F(PXLBLCK_WEBSERVER_FORM_ID_MATRIX_ARRANGEMENT));
        Plugin_205_ledColorOrder = getFormItemInt(F(PXLBLCK_WEBSERVER_FORM_ID_LED_TYPE));

        //Tile layout
        Plugin_205_matrixTileArrangement = getFormItemInt(F(PXLBLCK_WEBSERVER_FORM_ID_TILE_MATRIX_ARRANGEMENT));
        Plugin_205_matrixTilesWidth = getFormItemInt(F(PXLBLCK_WEBSERVER_FORM_ID_MATRIX_TILES_WIDTH));
        Plugin_205_matrixTilesHeight = getFormItemInt(F(PXLBLCK_WEBSERVER_FORM_ID_MATRIX_TILES_HEIGHT));

        //Wordclock
        Plugin_205_wordclockShowItIsEnabled = isFormItemChecked(F(PXLBLCK_WEBSERVER_FORM_ID_WORDCLOCK_IT_IS_ENABLED));
        Plugin_205_wordclockShowOClockEnabled = isFormItemChecked(F(PXLBLCK_WEBSERVER_FORM_ID_WORDCLOCK_O_CLOCK_ENABLED));
        Plugin_205_wordclockLanguageId = getFormItemInt(F(PXLBLCK_WEBSERVER_FORM_ID_WORDCLOCK_LANGUAGE));

        //Ringclock
        Plugin_205_ringclockClockTopOffset = getFormItemInt(F(PXLBLCK_WEBSERVER_FORM_ID_RINGCLOCK_LED_OFFSET));
        Plugin_205_ringclockThick12markEnabled = isFormItemChecked(F(PXLBLCK_WEBSERVER_FORM_ID_RINGCLOCK_THICK_MARK_ENABLED));
        Plugin_205_ringclockClockDirInversed = isFormItemChecked(F(PXLBLCK_WEBSERVER_FORM_ID_RINGCLOCK_DIR_INVERSED));
        Plugin_205_ringclockHourMarksBrightness = getFormItemInt(F(PXLBLCK_WEBSERVER_FORM_ID_RINGCLOCK_HOUR_MARK_BRIGHTNESS));

        //Color values
        String newColorOneCp = pxlBlckUtils_getFormItemString(String(PXLBLCK_WEBSERVER_FORM_ID_COLOR_ONE_PARTS) + "CP");
        uint8_t newColorOneR = getFormItemInt(String(PXLBLCK_WEBSERVER_FORM_ID_COLOR_ONE_PARTS) + "R");
        uint8_t newColorOneG = getFormItemInt(String(PXLBLCK_WEBSERVER_FORM_ID_COLOR_ONE_PARTS) + "G");
        uint8_t newColorOneB = getFormItemInt(String(PXLBLCK_WEBSERVER_FORM_ID_COLOR_ONE_PARTS) + "B");

        String newColorTwoCP = pxlBlckUtils_getFormItemString(String(PXLBLCK_WEBSERVER_FORM_ID_COLOR_TWO_PARTS) + "CP");
        uint8_t newColorTwoR = getFormItemInt(String(PXLBLCK_WEBSERVER_FORM_ID_COLOR_TWO_PARTS) + "R");
        uint8_t newColorTwoG = getFormItemInt(String(PXLBLCK_WEBSERVER_FORM_ID_COLOR_TWO_PARTS) + "G");
        uint8_t newColorTwoB = getFormItemInt(String(PXLBLCK_WEBSERVER_FORM_ID_COLOR_TWO_PARTS) + "B");

        String newColorThreeCP = pxlBlckUtils_getFormItemString(String(PXLBLCK_WEBSERVER_FORM_ID_COLOR_THREE_PARTS) + "CP");
        uint8_t newColorThreeR = getFormItemInt(String(PXLBLCK_WEBSERVER_FORM_ID_COLOR_THREE_PARTS) + "R");
        uint8_t newColorThreeG = getFormItemInt(String(PXLBLCK_WEBSERVER_FORM_ID_COLOR_THREE_PARTS) + "G");
        uint8_t newColorThreeB = getFormItemInt(String(PXLBLCK_WEBSERVER_FORM_ID_COLOR_THREE_PARTS) + "B");

        String newColorFourCP = pxlBlckUtils_getFormItemString(String(PXLBLCK_WEBSERVER_FORM_ID_COLOR_FOUR_PARTS) + "CP");
        uint8_t newColorFourR = getFormItemInt(String(PXLBLCK_WEBSERVER_FORM_ID_COLOR_FOUR_PARTS) + "R");
        uint8_t newColorFourG = getFormItemInt(String(PXLBLCK_WEBSERVER_FORM_ID_COLOR_FOUR_PARTS) + "G");
        uint8_t newColorFourB = getFormItemInt(String(PXLBLCK_WEBSERVER_FORM_ID_COLOR_FOUR_PARTS) + "B");

        //Save color values to permanent storage
        //First color value
        uint32_t tempConfigVariable = PXLBLCK_COLOR_PERMANENT_STORAGE(0);

        pxlBlckUtils_handle_and_save_new_color_values(
          newColorOneCp,
          &tempConfigVariable,
          &newColorOneR,
          &newColorOneG,
          &newColorOneB,
          getFormItemInt(String(PXLBLCK_WEBSERVER_FORM_ID_COLOR_ONE_PARTS) + "R"),
          getFormItemInt(String(PXLBLCK_WEBSERVER_FORM_ID_COLOR_ONE_PARTS) + "G"),
          getFormItemInt(String(PXLBLCK_WEBSERVER_FORM_ID_COLOR_ONE_PARTS) + "B"));

        PXLBLCK_COLOR_PERMANENT_STORAGE(0) = tempConfigVariable;
        Plugin_205_colorOne = tempConfigVariable;

        //Second color value
        tempConfigVariable = PXLBLCK_COLOR_PERMANENT_STORAGE(1);

        pxlBlckUtils_handle_and_save_new_color_values(
          newColorTwoCP,
          &tempConfigVariable,
          &newColorTwoR,
          &newColorTwoG,
          &newColorTwoB,
          getFormItemInt(String(PXLBLCK_WEBSERVER_FORM_ID_COLOR_TWO_PARTS) + "R"),
          getFormItemInt(String(PXLBLCK_WEBSERVER_FORM_ID_COLOR_TWO_PARTS) + "G"),
          getFormItemInt(String(PXLBLCK_WEBSERVER_FORM_ID_COLOR_TWO_PARTS) + "B"));

        PXLBLCK_COLOR_PERMANENT_STORAGE(1) = tempConfigVariable;
        Plugin_205_colorTwo = tempConfigVariable;

        //Third color value
        tempConfigVariable = PXLBLCK_COLOR_PERMANENT_STORAGE(2);

        pxlBlckUtils_handle_and_save_new_color_values(
          newColorThreeCP,
          &tempConfigVariable,
          &newColorThreeR,
          &newColorThreeG,
          &newColorThreeB,
          getFormItemInt(String(PXLBLCK_WEBSERVER_FORM_ID_COLOR_THREE_PARTS) + "R"),
          getFormItemInt(String(PXLBLCK_WEBSERVER_FORM_ID_COLOR_THREE_PARTS) + "G"),
          getFormItemInt(String(PXLBLCK_WEBSERVER_FORM_ID_COLOR_THREE_PARTS) + "B"));

        PXLBLCK_COLOR_PERMANENT_STORAGE(2) = tempConfigVariable;
        Plugin_205_colorThree = tempConfigVariable;

        //Fourth color value
        tempConfigVariable = PXLBLCK_COLOR_PERMANENT_STORAGE(3);

        pxlBlckUtils_handle_and_save_new_color_values(
          newColorFourCP,
          &tempConfigVariable,
          &newColorFourR,
          &newColorFourG,
          &newColorFourB,
          getFormItemInt(String(PXLBLCK_WEBSERVER_FORM_ID_COLOR_FOUR_PARTS) + "R"),
          getFormItemInt(String(PXLBLCK_WEBSERVER_FORM_ID_COLOR_FOUR_PARTS) + "G"),
          getFormItemInt(String(PXLBLCK_WEBSERVER_FORM_ID_COLOR_FOUR_PARTS) + "B"));

        PXLBLCK_COLOR_PERMANENT_STORAGE(3) = tempConfigVariable;
        Plugin_205_colorFour = tempConfigVariable;

        //Save all color values to runtime variables
        pxlBlckUtils_save_color_values_to_runtime_variables(
          newColorOneR, newColorOneG, newColorOneB,
          newColorTwoR, newColorTwoG, newColorTwoB,
          newColorThreeR, newColorThreeG, newColorThreeB,
          newColorFourR, newColorFourG, newColorFourB);

        //Save matrix dimensions to working variables
        Plugin_205_matrixHeight = Plugin_205_matrixSizesById[Plugin_205_selectedMatrixId][1];
        Plugin_205_matrixWidth = Plugin_205_matrixSizesById[Plugin_205_selectedMatrixId][0];

        //if led-matrix is already initialized also update the actual matrix-rotation
        if (PXLBLCK_INSTANCE != NULL)
        {
          PXLBLCK_INSTANCE->setRotation(Plugin_205_matrixRotation);
        }

        //This stuff is needed to save up to eight boolean variables in one byte variable.
        PLUGIN_205_CONFIG(0) = pxlBlckUtils_save_boolean_runtime_variables_to_permanent_storage();

        //This stuff is needed to save up to two byte variables in one uint16 variable.
        uint16_t intVariable = PLUGIN_205_CONFIG(1);
        pxlBlckUtils_save_two_bytes_in_uint16(&intVariable, Plugin_205_displayBrightness, Plugin_205_selectedDial);
        PLUGIN_205_CONFIG(1) = intVariable;

        intVariable = PLUGIN_205_CONFIG(2);
        pxlBlckUtils_save_two_bytes_in_uint16(&intVariable, Plugin_205_matrixRotation, Plugin_205_selectedMatrixId);
        PLUGIN_205_CONFIG(2) = intVariable;

        intVariable = PLUGIN_205_CONFIG(3);
        pxlBlckUtils_save_two_bytes_in_uint16(&intVariable, Plugin_205_matrixLayoutStartPosition, Plugin_205_matrixArrangement);
        PLUGIN_205_CONFIG(3) = intVariable;

        intVariable = PLUGIN_205_CONFIG(4);
        pxlBlckUtils_save_two_bytes_in_uint16(&intVariable, Plugin_205_ledColorOrder, Plugin_205_wordclockLanguageId);
        PLUGIN_205_CONFIG(4) = intVariable;

        intVariable = PLUGIN_205_CONFIG(5);
        pxlBlckUtils_save_two_bytes_in_uint16(&intVariable, Plugin_205_ringclockClockTopOffset, Plugin_205_ringclockHourMarksBrightness);
        PLUGIN_205_CONFIG(5) = intVariable;

        intVariable = PLUGIN_205_CONFIG(6);
        pxlBlckUtils_save_two_bytes_in_uint16(&intVariable, Plugin_205_minimalBrightness, Plugin_205_matrixTileArrangement);
        PLUGIN_205_CONFIG(6) = intVariable;

        intVariable = PLUGIN_205_CONFIG(7);
        pxlBlckUtils_save_two_bytes_in_uint16(&intVariable, Plugin_205_matrixTilesWidth, Plugin_205_matrixTilesHeight);
        PLUGIN_205_CONFIG(7) = intVariable;

        //write values to user-vars: so the data showd in the plugin overview will also updated
        pxlBlckUtils_update_user_vars(event, Plugin_205_displayEnabled, Plugin_205_displayBrightness, Plugin_205_matrixRotation);

        success = true;
        break;
      }

    case PLUGIN_INIT:
      {

        //Initialize the "random-number-enerator"
        randomSeed(ESP.getCycleCount());

        //This stuff is needed to get all the two byte variables back from the uint16 variables.
        pxlBlckUtils_return_byte_values_from_uint16(PLUGIN_205_CONFIG(1), &Plugin_205_displayBrightness, &Plugin_205_selectedDial);

        pxlBlckUtils_return_byte_values_from_uint16(PLUGIN_205_CONFIG(2), &Plugin_205_matrixRotation, &Plugin_205_selectedMatrixId);

        pxlBlckUtils_return_byte_values_from_uint16(PLUGIN_205_CONFIG(3), &Plugin_205_matrixLayoutStartPosition, &Plugin_205_matrixArrangement);

        pxlBlckUtils_return_byte_values_from_uint16(PLUGIN_205_CONFIG(4), &Plugin_205_ledColorOrder, &Plugin_205_wordclockLanguageId);

        pxlBlckUtils_return_byte_values_from_uint16(PLUGIN_205_CONFIG(5), &Plugin_205_ringclockClockTopOffset, &Plugin_205_ringclockHourMarksBrightness);

        pxlBlckUtils_return_byte_values_from_uint16(PLUGIN_205_CONFIG(6), &Plugin_205_minimalBrightness, &Plugin_205_matrixTileArrangement);

        pxlBlckUtils_return_byte_values_from_uint16(PLUGIN_205_CONFIG(7), &Plugin_205_matrixTilesWidth, &Plugin_205_matrixTilesHeight);

        //This stuff is needed to get all the boolean variables back from the byte variable.
        uint8_t boolArray[8] = {0};
        uint8_t byteVariable = PLUGIN_205_CONFIG(0);

        pxlBlckUtils_return_bool_values_from_byte(&byteVariable, boolArray);
        Plugin_205_displayEnabled = boolArray[0];
        Plugin_205_wordclockShowItIsEnabled = boolArray[1];
        Plugin_205_wordclockShowOClockEnabled = boolArray[2];
        Plugin_205_ringclockThick12markEnabled = boolArray[3];
        Plugin_205_ringclockClockDirInversed = boolArray[4];
        Plugin_205_diallLeadingZerosEnabled = boolArray[5];

        //Save matrix dimensions to working variables
        Plugin_205_matrixHeight = Plugin_205_matrixSizesById[Plugin_205_selectedMatrixId][1];
        Plugin_205_matrixWidth = Plugin_205_matrixSizesById[Plugin_205_selectedMatrixId][0];
        //int width = Plugin_205_matrixWidth;
        //int height = Plugin_205_matrixHeight;

        PXLBLCK_HIGHER_COLOR_RESOLUTION_ENABLED = PXLBLCK_HIGHER_COLOR_RESOLUTION_ENABLED_STANDARD_SETTING;

        if ((PXLBLCK_LED_COLOR_ORDER != NEO_GRB) && (PXLBLCK_LED_COLOR_ORDER != NEO_RGB) && (PXLBLCK_LED_COLOR_ORDER != NEO_RGBW))
          PXLBLCK_LED_COLOR_ORDER = NEO_GRB;

        //initialize led-matrix
        if (Plugin_205_matrixTileArrangement == PXLBLCK_ONE_TILE_ONLY_VALUE || Plugin_205_matrixWidth == 1)
        {
          PXLBLCK_INSTANCE = new Adafruit_NeoMatrix(
            (int)Plugin_205_matrixWidth,
            (int)Plugin_205_matrixHeight,
            Settings.TaskDevicePin1[event->TaskIndex],
            Plugin_205_matrixLayoutStartPosition +
            Plugin_205_matrixArrangement,
            PXLBLCK_LED_COLOR_ORDER +
            NEO_KHZ800);
          //Serial.println("OLDDDDDD");
        } else
        {
          PXLBLCK_INSTANCE = new Adafruit_NeoMatrix(
            (int)Plugin_205_matrixWidth,
            (int)Plugin_205_matrixHeight,
            Plugin_205_matrixTilesWidth,
            Plugin_205_matrixTilesHeight,
            Settings.TaskDevicePin1[event->TaskIndex],
            Plugin_205_matrixLayoutStartPosition +
            Plugin_205_matrixArrangement,
            PXLBLCK_LED_COLOR_ORDER +
            NEO_KHZ800 +
            Plugin_205_matrixTileArrangement);
          //Serial.println("NEWWWWW");
        }


        PXLBLCK_INSTANCE->begin();
        PXLBLCK_INSTANCE->setBrightness(PLUGIN_205_STANDARD_BRIGHTNESS);
        PXLBLCK_INSTANCE->setRotation(Plugin_205_matrixRotation);
        PXLBLCK_INSTANCE->setTextWrap(PLUGIN_205_TEXT_WRAPING_ENABLED);

        if (Plugin_205_selectedMatrixId == PXLBLCK_WORDCLOCK_MATRIX_ID)
          PXLBLCK_INSTANCE->setRemapFunction(Plugin_205_wordclock_grid_pixel_remap); //Need to care for the special position of the minute leds: more info in remap-function
        else if (Plugin_205_selectedMatrixId == PXLBLCK_DIGIT_CLOCK_MATRIX_ID)
          PXLBLCK_INSTANCE->setRemapFunction(Plugin_205_digitClock_grid_pixel_remap); //Need to care for the special kind of matrix for digitClock: more info in remap-function

        //load permanent saved color-data to the runtime variables
        pxlBlckUtils_save_color_values_to_runtime_variables(
          pxlBlckUtils_return_red_from_config(PXLBLCK_COLOR_PERMANENT_STORAGE(0)), pxlBlckUtils_return_green_from_config(PXLBLCK_COLOR_PERMANENT_STORAGE(0)), pxlBlckUtils_return_blue_from_config(PXLBLCK_COLOR_PERMANENT_STORAGE(0)),
          pxlBlckUtils_return_red_from_config(PXLBLCK_COLOR_PERMANENT_STORAGE(1)), pxlBlckUtils_return_green_from_config(PXLBLCK_COLOR_PERMANENT_STORAGE(1)), pxlBlckUtils_return_blue_from_config(PXLBLCK_COLOR_PERMANENT_STORAGE(1)),
          pxlBlckUtils_return_red_from_config(PXLBLCK_COLOR_PERMANENT_STORAGE(2)), pxlBlckUtils_return_green_from_config(PXLBLCK_COLOR_PERMANENT_STORAGE(2)), pxlBlckUtils_return_blue_from_config(PXLBLCK_COLOR_PERMANENT_STORAGE(2)),
          pxlBlckUtils_return_red_from_config(PXLBLCK_COLOR_PERMANENT_STORAGE(3)), pxlBlckUtils_return_green_from_config(PXLBLCK_COLOR_PERMANENT_STORAGE(3)), pxlBlckUtils_return_blue_from_config(PXLBLCK_COLOR_PERMANENT_STORAGE(3))
        );

        //Disable 24 bit color mode if needed
        if (!PXLBLCK_HIGHER_COLOR_RESOLUTION_ENABLED)
          PXLBLCK_INSTANCE->setPassThruColor();

        //write values to user-vars: so the data showd in the plugin overview will also updated
        pxlBlckUtils_update_user_vars(event, Plugin_205_displayEnabled, Plugin_205_displayBrightness, Plugin_205_matrixRotation);
        pxlBlckUtils_show_start_animation(1);

        success = true;
        break;
      }

    case PLUGIN_ONCE_A_SECOND:
      {
        if (!Plugin_205_initialDebugOutputDone)
        {
          Plugin_205_initialDebugOutputDone = true;
          String log = F(PXLBLCK_DEVICE_NAME);
          log += F(" will be initalized with the following parameters:");
          addLog(LOG_LEVEL_DEBUG, log);
          log = F("   width=");
          log += String(Plugin_205_matrixWidth);
          addLog(LOG_LEVEL_DEBUG, log);
          log = F("   height=");
          log += String(Plugin_205_matrixHeight);
          addLog(LOG_LEVEL_DEBUG, log);
          log = F("   pin=");
          log += String(Settings.TaskDevicePin1[event->TaskIndex]);
          addLog(LOG_LEVEL_DEBUG, log);
          log = F("   matrix-start-position=");
          log += String(NEO_MATRIX_TOP + NEO_MATRIX_RIGHT);
          addLog(LOG_LEVEL_DEBUG, log);
          log = F("   matrix-led-order=");
          log += String(NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE);
          addLog(LOG_LEVEL_DEBUG, log);
          log = F("   led-color-order=");
          log += String(PXLBLCK_LED_COLOR_ORDER);
          addLog(LOG_LEVEL_DEBUG, log);
          log = F("   displayBrightness=");
          log += String(Plugin_205_displayBrightness);
          addLog(LOG_LEVEL_DEBUG, log);
          log = F("   selectedDial=");
          log += String(Plugin_205_selectedDial);
          addLog(LOG_LEVEL_DEBUG, log);
          log = F("   matrixRotation=");
          log += String(Plugin_205_matrixRotation);
          addLog(LOG_LEVEL_DEBUG, log);
          log = F("   selectedMatrixId=");
          log += String(Plugin_205_selectedMatrixId);
          addLog(LOG_LEVEL_DEBUG, log);
          log = F("   matrixLayoutStartPosition=");
          log += String(Plugin_205_matrixLayoutStartPosition);
          addLog(LOG_LEVEL_DEBUG, log);
          log = F("   matrixArrangement=");
          log += String(Plugin_205_matrixArrangement);
          addLog(LOG_LEVEL_DEBUG, log);
          log = F("   wordclockLanguageId=");
          log += String(Plugin_205_wordclockLanguageId);
          addLog(LOG_LEVEL_DEBUG, log);
          log = F("   ringclockClockTopOffset=");
          log += String(Plugin_205_ringclockClockTopOffset);
          addLog(LOG_LEVEL_DEBUG, log);
          log = F("   ringclockHourMarksBrightness=");
          log += String(Plugin_205_ringclockHourMarksBrightness);
          addLog(LOG_LEVEL_DEBUG, log);
          log = F("   displayEnabled=");
          log += String(Plugin_205_displayEnabled);
          addLog(LOG_LEVEL_DEBUG, log);
          log = F("   wordclockShowItIsEnabled=");
          log += String(Plugin_205_wordclockShowItIsEnabled);
          addLog(LOG_LEVEL_DEBUG, log);
          log = F("   wordclockShowOClockEnabled=");
          log += String(Plugin_205_wordclockShowOClockEnabled);
          addLog(LOG_LEVEL_DEBUG, log);
          log = F("   ringclockThick12markEnabled=");
          log += String(Plugin_205_ringclockThick12markEnabled);
          addLog(LOG_LEVEL_DEBUG, log);
          log = F("   ringclockClockDirInversed=");
          log += String(Plugin_205_ringclockClockDirInversed);
          addLog(LOG_LEVEL_DEBUG, log);
        }
        //WifiDisconnect();
        //setAP(false);
        //setSTA(false);

        /*
          if (WiFi.getMode() != WIFI_OFF)
          {
          WiFi.mode(WIFI_OFF);
          #ifdef ESP8266
          WiFi.forceSleepBegin();
          #endif
          delay(1);
          }
          setWifiMode(WIFI_OFF);
        */
        success = true;
        break;
      }

    case PLUGIN_TEN_PER_SECOND:
      {
        Plugin_205_update();
        success = true;
        break;
      }

    case PLUGIN_FIFTY_PER_SECOND:
      {
        pxlBlckUtils_check_running_text();
        pxlBlckUtils_check_multi_colored_icon();
        pxlBlckUtils_check_fakeTV();
        pxlBlckUtils_check_fireSimulation();
        success = true;
        break;
      }

    case PLUGIN_WRITE:
      {
        if (PXLBLCK_INSTANCE == NULL) //to avoid access of matrix variables that are not initalized
          break;

        String command = pxlBlckUtils_parseString(string, 1);
        command.toLowerCase();
        String param1 = pxlBlckUtils_parseString(string, 2);
        String param2 = pxlBlckUtils_parseString(string, 3);
        String param3 = pxlBlckUtils_parseString(string, 4);
        String param4 = pxlBlckUtils_parseString(string, 5);
        String param5 = pxlBlckUtils_parseString(string, 6);
        String param6 = pxlBlckUtils_parseString(string, 7);
        String param7 = pxlBlckUtils_parseString(string, 8);
        String param8 = pxlBlckUtils_parseString(string, 9);
        String param9 = pxlBlckUtils_parseString(string, 10);
        String param10 = pxlBlckUtils_parseString(string, 11);
        String param11 = pxlBlckUtils_parseString(string, 12);
        String param12 = pxlBlckUtils_parseString(string, 13);
        String param13 = pxlBlckUtils_parseString(string, 14);
        String param14 = pxlBlckUtils_parseString(string, 15);
        String param15 = pxlBlckUtils_parseString(string, 16);
        String param16 = pxlBlckUtils_parseString(string, 17);

        //Command debuging routine
        String log = F(PXLBLCK_DEVICE_NAME);
        addLog(LOG_LEVEL_DEBUG, log);
        log = F("   command = ");
        log += command;
        addLog(LOG_LEVEL_DEBUG, log);
        log = F("   param1 = ");
        log += param1;
        addLog(LOG_LEVEL_DEBUG, log);
        log = F("   param2 = ");
        log += param2;
        addLog(LOG_LEVEL_DEBUG, log);
        log = F("   param3 = ");
        log += param3;
        addLog(LOG_LEVEL_DEBUG, log);
        log = F("   param4 = ");
        log += param4;
        addLog(LOG_LEVEL_DEBUG, log);
        log = F("   param5 = ");
        log += param5;
        addLog(LOG_LEVEL_DEBUG, log);
        log = F("   param6 = ");
        log += param6;
        addLog(LOG_LEVEL_DEBUG, log);
        log = F("   param7 = ");
        log += param7;
        addLog(LOG_LEVEL_DEBUG, log);
        log = F("   param8 = ");
        log += param8;
        addLog(LOG_LEVEL_DEBUG, log);
        log = F("   param9 = ");
        log += param9;
        addLog(LOG_LEVEL_DEBUG, log);
        log = F("   param10 = ");
        log += param10;
        addLog(LOG_LEVEL_DEBUG, log);
        log = F("   param11 = ");
        log += param11;
        addLog(LOG_LEVEL_DEBUG, log);
        log = F("   param12 = ");
        log += param12;
        addLog(LOG_LEVEL_DEBUG, log);
        log = F("   param13 = ");
        log += param13;
        addLog(LOG_LEVEL_DEBUG, log);
        log = F("   param14 = ");
        log += param14;
        addLog(LOG_LEVEL_DEBUG, log);
        log = F("   param15 = ");
        log += param15;
        addLog(LOG_LEVEL_DEBUG, log);
        log = F("   param16 = ");
        log += param16;
        addLog(LOG_LEVEL_DEBUG, log);

        if (command == F(PXLBLCK_COMMAND_GENERAL_SETTINGS))//Command: pb,<enabled 1/0>,<brightness 0-255>,
        {
          //command to configure runtime values like brightness,color,etc...
          if (param1 != "")
          {
            uint8_t state = param1.toInt();
            if (state > -1 && state < 2)
            {
              Plugin_205_displayEnabled = state;
              PLUGIN_205_CONFIG(0) = pxlBlckUtils_save_boolean_runtime_variables_to_permanent_storage();
            }
          }
          if (param2 != "")
          {
            uint8_t brightness = param2.toInt();
            if (brightness > -1 && brightness <= PXLBLCK_MAX_SETABLE_BRIGHTNESS)
            {
              Plugin_205_displayBrightness = brightness;
              uint16_t intVariable = PLUGIN_205_CONFIG(1);
              pxlBlckUtils_save_two_bytes_in_uint16(&intVariable, Plugin_205_displayBrightness, Plugin_205_selectedDial);
              PLUGIN_205_CONFIG(1) = intVariable;
            }
          }
          if (param3 != "")
          {
            uint8_t brightness = param3.toInt();
            if (brightness > -1 && brightness <= PXLBLCK_MAX_SETABLE_BRIGHTNESS)
            {
              Plugin_205_ringclockHourMarksBrightness = brightness;
              uint16_t intVariable = PLUGIN_205_CONFIG(5);
              pxlBlckUtils_save_two_bytes_in_uint16(&intVariable, Plugin_205_ringclockClockTopOffset, Plugin_205_ringclockHourMarksBrightness);
              PLUGIN_205_CONFIG(5) = intVariable;
            }
          }
          if (param4 != "")
          {
            uint8_t rotation = param4.toInt();
            if (rotation > -1 && rotation < 4)
            {
              Plugin_205_matrixRotation = rotation;
              uint16_t intVariable = PLUGIN_205_CONFIG(2);
              pxlBlckUtils_save_two_bytes_in_uint16(&intVariable, Plugin_205_matrixRotation, Plugin_205_selectedMatrixId);
              PLUGIN_205_CONFIG(2) = intVariable;

            }
          }
          if (param5 != "")
          {
            uint8_t selectedDial = param5.toInt();
            if (selectedDial > -1 && selectedDial < 16)
            {
              Plugin_205_selectedDial = selectedDial;
              uint16_t intVariable = PLUGIN_205_CONFIG(1);
              pxlBlckUtils_save_two_bytes_in_uint16(&intVariable, Plugin_205_displayBrightness, Plugin_205_selectedDial);
              PLUGIN_205_CONFIG(1) = intVariable;

            }
          }
          if (param6 != "")
          {
            uint8_t wordclockShowItIsEnabled = param6.toInt();
            if (wordclockShowItIsEnabled > -1 && wordclockShowItIsEnabled < 2)
            {
              Plugin_205_wordclockShowItIsEnabled = wordclockShowItIsEnabled;
              PLUGIN_205_CONFIG(0) = pxlBlckUtils_save_boolean_runtime_variables_to_permanent_storage();
            }
          }
          if (param7 != "")
          {
            uint8_t wordclockShowOClockEnabled = param7.toInt();
            if (wordclockShowOClockEnabled > -1 && wordclockShowOClockEnabled < 2)
            {
              Plugin_205_wordclockShowOClockEnabled = wordclockShowOClockEnabled;
              PLUGIN_205_CONFIG(0) = pxlBlckUtils_save_boolean_runtime_variables_to_permanent_storage();
            }
          }
          if (param8 != "")
          {
            uint8_t ringclockClockDirInversed = param8.toInt();
            if (ringclockClockDirInversed > -1 && ringclockClockDirInversed < 2)
            {
              Plugin_205_ringclockClockDirInversed = ringclockClockDirInversed;
              PLUGIN_205_CONFIG(0) = pxlBlckUtils_save_boolean_runtime_variables_to_permanent_storage();
            }
          }
          if (param9 != "")
          {
            uint8_t wordclockLanguageId = param9.toInt();
            if (wordclockLanguageId > 0 && wordclockLanguageId <= PLUGIN_205_WORDCLOCK_LANGUAGE_NUM)
            {
              Plugin_205_wordclockLanguageId = wordclockLanguageId;
              uint16_t intVariable = PLUGIN_205_CONFIG(4);
              pxlBlckUtils_save_two_bytes_in_uint16(&intVariable, Plugin_205_ledColorOrder, Plugin_205_wordclockLanguageId);
              PLUGIN_205_CONFIG(4) = intVariable;
            }
          }
          if (param10 != "")
          {
            uint8_t minimalBrightness = param10.toInt();
            if (minimalBrightness > -1 && minimalBrightness < 255)
            {
              Plugin_205_minimalBrightness = minimalBrightness;
              uint16_t intVariable = PLUGIN_205_CONFIG(6);
              pxlBlckUtils_save_two_bytes_in_uint16(&intVariable, Plugin_205_minimalBrightness, Plugin_205_matrixTileArrangement);
              PLUGIN_205_CONFIG(6) = intVariable;
            }
          }

          success = true;
        } else if (command == F(PXLBLCK_COMMAND_COLOR_SETTINGS))
        {
          uint8_t colorValueId = (param1.toInt() > -1 && param1.toInt() < 256) ? param1.toInt() : 0; //This defines which of the four available color-values will be set.
          uint8_t colorRed = (param2.toInt() > -1 && param2.toInt() < 256) ? param2.toInt() : -1; //brightness-value of red-color that should be set.
          uint8_t colorGreen = (param3.toInt() > -1 && param3.toInt() < 256) ? param3.toInt() : -1; //brightness-value of green-color that should be set.
          uint8_t colorBlue = (param4.toInt() > -1 && param4.toInt() < 256) ? param4.toInt() : -1; //brightness-value of blue-color that should be set.
          uint8_t colorWarmWhite = (param5.toInt() > -1 && param5.toInt() < 256) ? param5.toInt() : -1; //brightness-value of warmWhite-color that should be set.

          String log = F(PXLBLCK_DEVICE_NAME);
          addLog(LOG_LEVEL_DEBUG, log);

          switch (colorValueId)
          {
            case 0: //save color-brightness-values for all colors
              PXLBLCK_COLOR_PERMANENT_STORAGE(0) = pxlBlckUtils_color_value_update(PXLBLCK_COLOR_PERMANENT_STORAGE(0), colorRed, colorGreen, colorBlue, colorWarmWhite);
              Plugin_205_colorOne = PXLBLCK_COLOR_PERMANENT_STORAGE(0);
              PXLBLCK_COLOR_PERMANENT_STORAGE(1) = pxlBlckUtils_color_value_update(PXLBLCK_COLOR_PERMANENT_STORAGE(1), colorRed, colorGreen, colorBlue, colorWarmWhite);
              Plugin_205_colorTwo = PXLBLCK_COLOR_PERMANENT_STORAGE(1);
              PXLBLCK_COLOR_PERMANENT_STORAGE(2) = pxlBlckUtils_color_value_update(PXLBLCK_COLOR_PERMANENT_STORAGE(2), colorRed, colorGreen, colorBlue, colorWarmWhite);
              Plugin_205_colorThree = PXLBLCK_COLOR_PERMANENT_STORAGE(2);
              PXLBLCK_COLOR_PERMANENT_STORAGE(3) = pxlBlckUtils_color_value_update(PXLBLCK_COLOR_PERMANENT_STORAGE(3), colorRed, colorGreen, colorBlue, colorWarmWhite);
              Plugin_205_colorFour = PXLBLCK_COLOR_PERMANENT_STORAGE(3);
              break;
            case 1: //save color-brightness-values for first color
              PXLBLCK_COLOR_PERMANENT_STORAGE(0) = pxlBlckUtils_color_value_update(PXLBLCK_COLOR_PERMANENT_STORAGE(0), colorRed, colorGreen, colorBlue, colorWarmWhite);
              Plugin_205_colorOne = PXLBLCK_COLOR_PERMANENT_STORAGE(0);
              break;
            case 2: //save color-brightness-values for second color
              PXLBLCK_COLOR_PERMANENT_STORAGE(1) = pxlBlckUtils_color_value_update(PXLBLCK_COLOR_PERMANENT_STORAGE(1), colorRed, colorGreen, colorBlue, colorWarmWhite);
              Plugin_205_colorTwo = PXLBLCK_COLOR_PERMANENT_STORAGE(1);
              break;
            case 3: //save color-brightness-values for third color
              PXLBLCK_COLOR_PERMANENT_STORAGE(2) = pxlBlckUtils_color_value_update(PXLBLCK_COLOR_PERMANENT_STORAGE(2), colorRed, colorGreen, colorBlue, colorWarmWhite);
              Plugin_205_colorThree = PXLBLCK_COLOR_PERMANENT_STORAGE(2);

              //write values to user-vars: so the data showd in the plugin overview will also updated
              pxlBlckUtils_update_user_vars(event, Plugin_205_displayEnabled, Plugin_205_displayBrightness, Plugin_205_matrixRotation);
              break;
            case 4: //save color-brightness-values for fourth color
              PXLBLCK_COLOR_PERMANENT_STORAGE(3) = pxlBlckUtils_color_value_update(PXLBLCK_COLOR_PERMANENT_STORAGE(3), colorRed, colorGreen, colorBlue, colorWarmWhite);
              Plugin_205_colorFour = PXLBLCK_COLOR_PERMANENT_STORAGE(3);
              break;
            default:
              break;
          }
          pxlBlckUtils_update_user_vars(event, Plugin_205_displayEnabled, Plugin_205_displayBrightness, Plugin_205_matrixRotation);

          log = F("   - colorOne: ");
          log += Plugin_205_colorOne;
          addLog(LOG_LEVEL_DEBUG, log);
          log = F("   - colorTwo: ");
          log += Plugin_205_colorTwo;
          addLog(LOG_LEVEL_DEBUG, log);
          log = F("   - colorThree: ");
          log += Plugin_205_colorThree;
          addLog(LOG_LEVEL_DEBUG, log);
          log = F("   - colorFour: ");
          log += Plugin_205_colorFour;
          addLog(LOG_LEVEL_DEBUG, log);

          success = true;

          //command to set the boolean values
        } else if (command == F(PXLBLCK_COMMAND_SET_BOOLEANS))
        {
          uint8_t boolValueId = param1.toInt(); //This defines which of the available boolean-values will be set.
          uint8_t boolValue = param2.toInt(); //Boolean-value that should be set. If this is set to two the regarding variable will be toggled

          String log = F(PXLBLCK_DEVICE_NAME);
          addLog(LOG_LEVEL_DEBUG, log);

          if (boolValue >= 0 && boolValue <= 2)
          {
            switch (boolValueId)
            {
              case 1: //save Plugin_205_displayEnabled
                Plugin_205_displayEnabled = ((boolValue == 2) ? !Plugin_205_displayEnabled : boolValue);
                pxlBlckUtils_update_user_vars(event, Plugin_205_displayEnabled, Plugin_205_displayBrightness, Plugin_205_matrixRotation);
                break;
              case 2: //save Plugin_205_wordclockShowOClockEnabled
                Plugin_205_wordclockShowOClockEnabled =  ((boolValue == 2) ? !Plugin_205_wordclockShowOClockEnabled : boolValue);
                break;
              case 3: //save Plugin_205_wordclockShowItIsEnabled
                Plugin_205_wordclockShowItIsEnabled =  ((boolValue == 2) ? !Plugin_205_wordclockShowItIsEnabled : boolValue);
                break;
              case 4: //save Plugin_205_ringclockThick12markEnabled
                Plugin_205_ringclockThick12markEnabled =  ((boolValue == 2) ? !Plugin_205_ringclockThick12markEnabled : boolValue);
                break;
              case 5: //save Plugin_205_ringclockClockDirInversed
                Plugin_205_ringclockClockDirInversed =  ((boolValue == 2) ? !Plugin_205_ringclockClockDirInversed : boolValue);
                break;
              case 6: //save Plugin_205_diallLeadingZerosEnabled
                Plugin_205_diallLeadingZerosEnabled =  ((boolValue == 2) ? !Plugin_205_diallLeadingZerosEnabled : boolValue);
                break;
              default:
                break;
            }
            PLUGIN_205_CONFIG(0) = pxlBlckUtils_save_boolean_runtime_variables_to_permanent_storage();

            log = F("   - boolValue: ");
            log += boolValue;
            addLog(LOG_LEVEL_DEBUG, log);
            log = F("   - boolValueId: ");
            log += boolValueId;
            addLog(LOG_LEVEL_DEBUG, log);

          }  else
          {

            log = F("   - boolValue was not in allowed range and therefore not set.");
            addLog(LOG_LEVEL_DEBUG, log);
            log = F("   - boolValueId: ");
            log += String(boolValueId);
            addLog(LOG_LEVEL_DEBUG, log);
            log = F("   - boolValue: ");
            log += String(boolValue);
            addLog(LOG_LEVEL_DEBUG, log);
          }
          success = true;

          //command to set the brightness values
        } else if (command == F(PXLBLCK_COMMAND_SET_BRIGHTNESS))
        {
          uint8_t brightnessId = param1.toInt(); //this defines which of the available brightness-values will be set. If this is set to zero all brightness-values will be set
          int8_t brightness = ((int8_t)param2.toInt()) > PXLBLCK_MAX_SETABLE_BRIGHTNESS ? PXLBLCK_MAX_SETABLE_BRIGHTNESS : param2.toInt(); //brightness that should be set
          uint8_t absolute = param3.toInt(); //this is some kind of boolean. If this is set the brightness value will be seen as an absolute value. If not it will be seen relative.
          uint8_t bottomBorder = ((param4.length() > 0) ? param4.toInt() : PXLBLCK_MAX_SETABLE_BRIGHTNESS); //this is here for an optional limitation of the max and min values
          uint8_t topBorder = ((param5.length() > 0) ? param5.toInt() : PXLBLCK_MAX_SETABLE_BRIGHTNESS);

          String log = F(PXLBLCK_DEVICE_NAME);
          addLog(LOG_LEVEL_INFO, log);
          log = F("   - displayBrightness(old): ");
          log += Plugin_205_displayBrightness;
          addLog(LOG_LEVEL_INFO, log);
          log = F("   - ringclockHourMarksBrightness(old): ");
          log += Plugin_205_ringclockHourMarksBrightness;
          addLog(LOG_LEVEL_INFO, log);

          if ((absolute == 1 && (brightness >= bottomBorder && brightness <= topBorder)) || absolute == 0) //not really needed here but left for raw filtering of crazy values
          {
            uint8_t displayBrightness = ((absolute == 1) ? brightness : Plugin_205_displayBrightness + brightness);
            uint8_t ringclockHourMarksBrightness = ((absolute == 1) ? brightness : Plugin_205_ringclockHourMarksBrightness + brightness);

            uint16_t intVariable = PLUGIN_205_CONFIG(5); //This is just the helper variable that helps saving multiple booleans in one uint16_t variable
            switch (brightnessId)
            {
              case 0:
                if (displayBrightness >= bottomBorder && displayBrightness <= topBorder)
                {
                  Plugin_205_displayBrightness = displayBrightness;
                  intVariable = PLUGIN_205_CONFIG(1);
                  pxlBlckUtils_save_two_bytes_in_uint16(&intVariable, Plugin_205_displayBrightness, Plugin_205_selectedDial);
                  PLUGIN_205_CONFIG(1) = intVariable;
                  pxlBlckUtils_update_user_vars(event, Plugin_205_displayEnabled, Plugin_205_displayBrightness, Plugin_205_matrixRotation);
                }

                if (ringclockHourMarksBrightness >= bottomBorder && ringclockHourMarksBrightness <= topBorder)
                {
                  Plugin_205_ringclockHourMarksBrightness = ringclockHourMarksBrightness;
                  intVariable = PLUGIN_205_CONFIG(5);
                  pxlBlckUtils_save_two_bytes_in_uint16(&intVariable, Plugin_205_ringclockClockTopOffset, Plugin_205_ringclockHourMarksBrightness);
                  PLUGIN_205_CONFIG(5) = intVariable;
                }

                break;
              case 1: //save standard Plugin_205_displayBrightness

                if (displayBrightness >= bottomBorder && displayBrightness <= topBorder)
                {
                  Plugin_205_displayBrightness = displayBrightness;
                  intVariable = PLUGIN_205_CONFIG(1);
                  pxlBlckUtils_save_two_bytes_in_uint16(&intVariable, Plugin_205_displayBrightness, Plugin_205_selectedDial);
                  PLUGIN_205_CONFIG(1) = intVariable;
                  pxlBlckUtils_update_user_vars(event, Plugin_205_displayEnabled, Plugin_205_displayBrightness, Plugin_205_matrixRotation);
                }
                break;
              case 2: //save Plugin_205_ringclockHourMarksBrightness

                if (ringclockHourMarksBrightness >= bottomBorder && ringclockHourMarksBrightness <= topBorder)
                {
                  Plugin_205_ringclockHourMarksBrightness = ringclockHourMarksBrightness;
                  intVariable = PLUGIN_205_CONFIG(5);
                  pxlBlckUtils_save_two_bytes_in_uint16(&intVariable, Plugin_205_ringclockClockTopOffset, Plugin_205_ringclockHourMarksBrightness);
                  PLUGIN_205_CONFIG(5) = intVariable;
                }
                break;
              default:
                break;
            }

            log = F("   - displayBrightness(new): ");
            log += Plugin_205_displayBrightness;
            addLog(LOG_LEVEL_INFO, log);
            log = F("   - ringclockHourMarksBrightness(new): ");
            log += Plugin_205_ringclockHourMarksBrightness;
            addLog(LOG_LEVEL_INFO, log);
            log = F("   - brightness-id: ");
            log += String(brightnessId);
            addLog(LOG_LEVEL_INFO, log);
            log = F("   - brightness: ");
            log += String(brightness);
            addLog(LOG_LEVEL_INFO, log);
            log = F("   - bottomBorder: ");
            log += String(bottomBorder);
            addLog(LOG_LEVEL_INFO, log);
            log = F("   - topBorder: ");
            log += String(topBorder);
            addLog(LOG_LEVEL_INFO, log);

          } else
          {
            log = F("   - brightness was not in allowed range and therefore not set.");
            addLog(LOG_LEVEL_INFO, log);
            log = F("   - displayBrightness(new): ");
            log += Plugin_205_displayBrightness;
            addLog(LOG_LEVEL_INFO, log);
            log = F("   - ringclockHourMarksBrightness(new): ");
            log += Plugin_205_ringclockHourMarksBrightness;
            addLog(LOG_LEVEL_INFO, log);
            log = F("   - brightness-id: ");
            log += String(brightnessId);
            addLog(LOG_LEVEL_INFO, log);
            log = F("   - brightness: ");
            log += String(brightness);
            addLog(LOG_LEVEL_INFO, log);
            log = F("   - bottomBorder: ");
            log += String(bottomBorder);
            addLog(LOG_LEVEL_INFO, log);
            log = F("   - topBorder: ");
            log += String(topBorder);
            addLog(LOG_LEVEL_INFO, log);
          }
          success = true;

          //command to set the color values by the parameters of a color wheel
        } else if (command == F(PXLBLCK_COMMAND_SET_COLOR_BY_WHEEL))
        {
          if (param1 != "" && param2 != "")
          {
            uint8_t colorId = param1.toInt(); //this defines which of the four available colors will be set. If this is set to 0 all colors will be set
            int16_t degree = param2.toInt(); //degree on the color wheel
            uint8_t absolute = param3.toInt(); //this is some kind of boolean. If this is set the degree value will be seen as an absolute value. if not it will be seen relative

            String log = F(PXLBLCK_DEVICE_NAME);
            addLog(LOG_LEVEL_DEBUG, log);

            if (colorId >= 0 && colorId < 5 && degree > -360 && degree < 360 )
            {

              if (absolute == 1)
              {
                Plugin_205_colorWheelPosition[colorId] = degree;
              } else
              {
                Plugin_205_colorWheelPosition[colorId] = Plugin_205_colorWheelPosition[colorId] + degree;
              }

              uint32_t colorResult = pxlBlckUtils_color_wheel(Plugin_205_colorWheelPosition[colorId]);

              switch (colorId)
              {
                case 0:
                  PXLBLCK_COLOR_PERMANENT_STORAGE(0) = colorResult;
                  PXLBLCK_COLOR_PERMANENT_STORAGE(1) = colorResult;
                  PXLBLCK_COLOR_PERMANENT_STORAGE(2) = colorResult;
                  PXLBLCK_COLOR_PERMANENT_STORAGE(3) = colorResult;
                  Plugin_205_colorOne = colorResult;
                  Plugin_205_colorTwo = colorResult;
                  Plugin_205_colorThree = colorResult;
                  Plugin_205_colorFour = colorResult;
                  break;
                case 1:
                  PXLBLCK_COLOR_PERMANENT_STORAGE(0) = colorResult;
                  Plugin_205_colorOne = colorResult;
                  break;
                case 2:
                  PXLBLCK_COLOR_PERMANENT_STORAGE(1) = colorResult;
                  Plugin_205_colorTwo = colorResult;
                  break;
                case 3:
                  PXLBLCK_COLOR_PERMANENT_STORAGE(2) = colorResult;
                  Plugin_205_colorThree = colorResult;
                  break;
                case 4:
                  PXLBLCK_COLOR_PERMANENT_STORAGE(3) = colorResult;
                  Plugin_205_colorFour = colorResult;
                  break;
                default:
                  break;
              }

              log = F("   - color ");
              log += colorId;
              log += F(" set to color-wheel-degree ");
              log += String(Plugin_205_colorWheelPosition[colorId]);
              log += F("=color-value:  ");
              log += String(colorResult);
              addLog(LOG_LEVEL_DEBUG, log);
              log = F("   - red: ");
              log += String(pxlBlckUtils_return_red_from_config(PXLBLCK_COLOR_PERMANENT_STORAGE(colorId - 1)));
              log += F(" ,green: ");
              log += String(pxlBlckUtils_return_green_from_config(PXLBLCK_COLOR_PERMANENT_STORAGE(colorId - 1)));
              log += F(" ,blue: ");
              log += String(pxlBlckUtils_return_blue_from_config(PXLBLCK_COLOR_PERMANENT_STORAGE(colorId - 1)));
              addLog(LOG_LEVEL_DEBUG, log);

            } else
            {

              log = F("   - color-wheel-color was not set. Value is invalid.");
              addLog(LOG_LEVEL_DEBUG, log);
              log = F("   - color-id: ");
              log += String(colorId);
              addLog(LOG_LEVEL_DEBUG, log);
              log = F("   - degree: ");
              log += String(Plugin_205_colorWheelPosition[colorId]);
              addLog(LOG_LEVEL_DEBUG, log);
            }
          }
          success = true;
          //command to start a running text.
        } else if (command == F(PXLBLCK_COMMAND_RUNNING_TEXT)) //Command: pbrntxt,<text color r(0-255)>,<text color g(0-255)>,<text color b(0-255)>,<background color r(0-255)>,<background color g(0-255)>,<background color b(0-255)>,<delay time(0-1000ms)>,<start position(0-matrix.width)>,
        {
          if (Plugin_205_selectedMatrixId != PXLBLCK_RINGCLOCK_MATRIX_ID && Plugin_205_selectedMatrixId != PXLBLCK_FIBOCLOCK_MATRIX_ID)
          {
            uint8_t txtColorR, txtColorG, txtColorB, bgColorR, bgColorG, bgColorB;
            uint16_t delayTime = 0;
            uint8_t startPos = PXLBLCK_MATRIX_WIDTH;

            if (param1 != "")
            {
              uint8_t color = param1.toInt();
              if (color > -1 && color < 256)
              {
                txtColorR = color;
              }
            }
            if (param2 != "")
            {
              uint8_t color = param2.toInt();
              if (color > -1 && color < 256)
              {
                txtColorG = color;
              }
            }
            if (param3 != "")
            {
              uint8_t color = param3.toInt();
              if (color > -1 && color < 256)
              {
                txtColorB = color;
              }
            }
            if (param4 != "")
            {
              uint8_t color = param4.toInt();
              if (color > -1 && color < 256)
              {
                bgColorR = color;
              }
            }
            if (param5 != "")
            {
              uint8_t color = param5.toInt();
              if (color > -1 && color < 256)
              {
                bgColorG = color;
              }
            }
            if (param6 != "")
            {
              uint8_t color = param6.toInt();
              if (color > -1 && color < 256)
              {
                bgColorB = color;
              }
            }
            if (param7 != "")
            {
              uint16_t delayTimeTemp = param7.toInt();
              if (delayTimeTemp > -1 && delayTimeTemp <= 1000)
              {
                delayTime = delayTimeTemp;
              }
            }
            if (param8 != "")
            {
              uint16_t startPosTemp = param8.toInt();
              if (startPosTemp > -1 && startPosTemp < PXLBLCK_MATRIX_WIDTH)
              {
                startPos = startPosTemp;
              }
            }
            uint32_t txtColor = pxlBlckUtils_convert_color_values_to_32bit(txtColorR, txtColorG, txtColorB);
            uint32_t bgColor = pxlBlckUtils_convert_color_values_to_32bit(bgColorR, bgColorG, bgColorB);

            pxlBlckUtils_prepare_runing_text(param9, txtColor, bgColor, delayTime, startPos);

          }
          success = true;

          //command to start a icon animation.
        } else if (command == F(PXLBLCK_COMMAND_SHOW_ICON)) //Command: pbicon,<selected icon(0-255)>,<in animation(0-255)>,<out animation(0-255)>,<inAnimation duration(0-1000ms)>,<display duration(0-10000ms)>,<outAnim duration(0-1000ms)>,<brightness (0-100)>, <opt: running text>,<repetition (0-10)>,<opt: spiffsIcon Filename>,
        {
          if (Plugin_205_selectedMatrixId != PXLBLCK_RINGCLOCK_MATRIX_ID && Plugin_205_selectedMatrixId != PXLBLCK_FIBOCLOCK_MATRIX_ID)
          {
            //Prepare variables and load standard values
            uint8_t inAnim = PXLBLCK_ICON_ANIM_INSTANTLY_ON;
            uint8_t outAnim = PXLBLCK_ICON_ANIM_INSTANTLY_OFF;
            uint16_t inDelay = 2000;
            uint16_t showDelay = 2000;
            uint16_t outDelay = 2000;
            uint8_t brightness = 100;
            String txt = F("No text :'(");
            String spiffsIcon = "";
            uint8_t repetition = 0;


            if (param1 != "")
            {
              uint8_t inAnimTemp = param1.toInt();
              if (inAnimTemp > -1 && inAnimTemp < 256)
              {
                inAnim = inAnimTemp;
              }
            }

            if (param2 != "")
            {
              uint8_t outAnimTemp = param2.toInt();
              if (outAnimTemp > -1 && outAnimTemp < 256)
              {
                outAnim = outAnimTemp;
              }
            }

            if (param3 != "")
            {
              uint16_t inDelayTemp = param3.toInt();
              if (inDelayTemp <= 2000)
              {
                inDelay = inDelayTemp;
              }
            }

            if (param4 != "")
            {
              uint16_t showDelayTemp = param4.toInt();
              if (showDelayTemp <= 10000)
              {
                showDelay = showDelayTemp;
              }
            }

            if (param5 != "")
            {
              uint16_t outDelayTemp = param5.toInt();
              if (outDelayTemp <= 2000)
              {
                outDelay = outDelayTemp;
              }
            }

            if (param6 != "")
            {
              uint8_t bright = param6.toInt();
              if (bright <= 100)
              {
                brightness = bright;
              }
            }

            if (param7 != "")
            {
              txt = param7;
            }

            if (param8 != "")
            {
              spiffsIcon = param8;
            }

            if (param9 != "")
            {
              uint8_t rep = param9.toInt();
              if (rep <= 10)
              {
                repetition = rep;
              }
            }

            pxlBlckUtils_prepare_multi_colored_icon(
              event,
              inAnim,
              outAnim,
              inDelay,
              showDelay,
              outDelay,
              brightness,
              txt,
              spiffsIcon,
              repetition);

          }

          success = true;
        } else if (command == F(PXLBLCK_COMMAND_FAKE_TV))
        {

          uint8_t state = param1.toInt();
          pxlBlckUtils_switch_fakeTV_onOff(state);

          success = true;
        } else if (command == F(PXLBLCK_COMMAND_MATRIX_TEST))
        {
          pxlBlckUtils_test_matrix();

          success = true;
        } else if (command == F(PXLBLCK_COMMAND_ANIMATION))
        {
          if (pxlBlckUtils_execute_if_interval_passed(&Plugin_205_animationExecutedTimestamp, PXLBLCK_ANIMATION_COOLDOWN_TIME))
          {
            uint8_t anim_red_on, anim_green_on, anim_blue_on;
            uint8_t anim_red_off, anim_green_off, anim_blue_off;
            uint8_t anim_mode = 1;
            uint16_t anim_delay = PXLBLCK_COMMAND_ANIMATION_STANDARD_TIME;
            if (param1 != "")
            {
              uint8_t mode = param1.toInt();
              if (mode > 0 && mode < 6)
              {
                anim_mode = mode;
              } else
              {
                anim_mode = 1;
              }
            }
            if (param2 != "")
            {
              int r = param2.toInt();
              if (r > -1 && r < 255)
              {
                anim_red_on = r;
              } else
              {
                anim_red_on = 255;
              }
            }
            if (param3 != "")
            {
              int g = param3.toInt();
              if (g > -1 && g < 255)
              {
                anim_green_on = g;
              } else
              {
                anim_green_on = 255;
              }
            }
            if (param4 != "")
            {
              int b = param4.toInt();
              if (b > -1 && b < 255)
              {
                anim_blue_on = b;
              } else
              {
                anim_blue_on = 255;
              }
            }

            if (param5 != "")
            {
              int r = param5.toInt();
              if (r > -1 && r < 255)
              {
                anim_red_off = r;
              } else
              {
                anim_red_off = 255;
              }
            }
            if (param6 != "")
            {
              int g = param6.toInt();
              if (g > -1 && g < 255)
              {
                anim_green_off = g;
              } else
              {
                anim_green_off = 255;
              }
            }
            if (param7 != "")
            {
              int b = param7.toInt();
              if (b > -1 && b < 255)
              {
                anim_blue_off = b;
              } else
              {
                anim_blue_off = 255;
              }
            }
            if (param8 != "")
            {
              int b = param8.toInt();
              if (b > 10 && b < 500)
              {
                anim_delay = b;
              } else
              {
                anim_delay = 500;
              }
            }


            //pxlBlckUtils_fill_matrix(pxlBlckUtils_convert_color_values_to_32bit(100, 100, 100, 0));

            switch (anim_mode)
            {
              case 1:
                {
                  //this animation shows a continously filling ring that starts at the left side and goes back to the right after it was completely filled =================================================================================================
                  pxlBlckUtils_clear_matrix();

                  for (int16_t i = 0; i < PXLBLCK_MATRIX_HEIGHT; i++)
                  {
                    pxlBlckUtils_draw_horizontal_bar(i, pxlBlckUtils_convert_color_values_to_32bit(anim_red_on, anim_green_on, anim_blue_on));
                    delay(anim_delay);
                  }

                  for (int16_t i = (PXLBLCK_MATRIX_HEIGHT - 1); i >= 0; i--)
                  {
                    pxlBlckUtils_draw_horizontal_bar(i, pxlBlckUtils_convert_color_values_to_32bit(anim_red_off, anim_green_off, anim_blue_off));
                    delay(anim_delay);
                  }

                  pxlBlckUtils_clear_matrix();
                  pxlBlckUtils_update_matrix();
                }
                break;
              case 2:
                {
                  //this animation shows a continously filling ring that starts at the right side and goes back to the left after it was completely filled =================================================================================================
                  pxlBlckUtils_clear_matrix();

                  for (int16_t i = (PXLBLCK_MATRIX_HEIGHT - 1); i >= 0; i--)
                  {
                    pxlBlckUtils_draw_horizontal_bar(i, pxlBlckUtils_convert_color_values_to_32bit(anim_red_on, anim_green_on, anim_blue_on));
                    delay(anim_delay);
                  }

                  for (int16_t i = 0; i < PXLBLCK_MATRIX_HEIGHT; i++)
                  {
                    pxlBlckUtils_draw_horizontal_bar(i, pxlBlckUtils_convert_color_values_to_32bit(anim_red_off, anim_green_off, anim_blue_off));
                    delay(anim_delay);
                  }

                  pxlBlckUtils_clear_matrix();
                  pxlBlckUtils_update_matrix();
                }
                break;
              case 3:
                {
                  //this animation shows a wandering line from the left side to the right side =================================================================================================
                  pxlBlckUtils_clear_matrix();

                  uint8_t animationWidthHalf = PLUGIN_205_ANIMATION_PIXEL_WIDTH / 2;

                  for (int16_t i = (0 - animationWidthHalf); i < (PXLBLCK_MATRIX_HEIGHT + animationWidthHalf); i++)
                  {
                    if ((i - animationWidthHalf) >= 0)
                    {
                      pxlBlckUtils_draw_horizontal_bar((i - animationWidthHalf), pxlBlckUtils_convert_color_values_to_32bit(anim_red_on, anim_green_on, anim_blue_on));
                    }

                    if (i >= 0 && i < PXLBLCK_MATRIX_HEIGHT)
                    {
                      pxlBlckUtils_draw_horizontal_bar(i, pxlBlckUtils_convert_color_values_to_32bit(anim_red_on, anim_green_on, anim_blue_on));
                    }

                    if ((i + animationWidthHalf) < PXLBLCK_MATRIX_HEIGHT)
                    {
                      pxlBlckUtils_draw_horizontal_bar((i + animationWidthHalf), pxlBlckUtils_convert_color_values_to_32bit(anim_red_on, anim_green_on, anim_blue_on));
                    }

                    delay(anim_delay);
                    pxlBlckUtils_clear_matrix();
                  }

                  for (int16_t i = (PXLBLCK_MATRIX_HEIGHT + animationWidthHalf); i >= (0 - animationWidthHalf); i--)
                  {
                    if ((i - animationWidthHalf) >= 0)
                    {
                      pxlBlckUtils_draw_horizontal_bar((i - animationWidthHalf), pxlBlckUtils_convert_color_values_to_32bit(anim_red_off, anim_green_off, anim_blue_off));
                    }

                    if (i >= 0 && i < PXLBLCK_MATRIX_HEIGHT)
                    {
                      pxlBlckUtils_draw_horizontal_bar(i, pxlBlckUtils_convert_color_values_to_32bit(anim_red_off, anim_green_off, anim_blue_off));
                    }

                    if ((i + animationWidthHalf) < PXLBLCK_MATRIX_HEIGHT)
                    {
                      pxlBlckUtils_draw_horizontal_bar(i + animationWidthHalf, pxlBlckUtils_convert_color_values_to_32bit(anim_red_off, anim_green_off, anim_blue_off));
                    }

                    delay(anim_delay);
                    pxlBlckUtils_clear_matrix();
                  }

                  pxlBlckUtils_clear_matrix();
                  pxlBlckUtils_update_matrix();
                }
                break;
              case 4:
                {
                  //this animation shows a wandering line from the right side to the left side =================================================================================================
                  pxlBlckUtils_clear_matrix();

                  uint8_t animationWidthHalf = PLUGIN_205_ANIMATION_PIXEL_WIDTH / 2;

                  for (int16_t i = (PXLBLCK_MATRIX_HEIGHT + animationWidthHalf); i >= (0 - animationWidthHalf); i--)
                  {
                    if ((i - animationWidthHalf) >= 0)
                    {
                      pxlBlckUtils_draw_horizontal_bar((i - animationWidthHalf), pxlBlckUtils_convert_color_values_to_32bit(anim_red_off, anim_green_off, anim_blue_off));
                    }

                    if (i >= 0 && i < PXLBLCK_MATRIX_HEIGHT)
                    {
                      pxlBlckUtils_draw_horizontal_bar(i, pxlBlckUtils_convert_color_values_to_32bit(anim_red_off, anim_green_off, anim_blue_off));
                    }

                    if ((i + animationWidthHalf) < PXLBLCK_MATRIX_HEIGHT)
                    {
                      pxlBlckUtils_draw_horizontal_bar(i + animationWidthHalf, pxlBlckUtils_convert_color_values_to_32bit(anim_red_off, anim_green_off, anim_blue_off));
                    }

                    delay(anim_delay);
                    pxlBlckUtils_clear_matrix();
                  }

                  for (int16_t i = (0 - animationWidthHalf); i < (PXLBLCK_MATRIX_HEIGHT + animationWidthHalf); i++)
                  {
                    if ((i - animationWidthHalf) >= 0)
                    {
                      pxlBlckUtils_draw_horizontal_bar((i - animationWidthHalf), pxlBlckUtils_convert_color_values_to_32bit(anim_red_on, anim_green_on, anim_blue_on));
                    }

                    if (i >= 0 && i < PXLBLCK_MATRIX_HEIGHT)
                    {
                      pxlBlckUtils_draw_horizontal_bar(i, pxlBlckUtils_convert_color_values_to_32bit(anim_red_on, anim_green_on, anim_blue_on));
                    }

                    if ((i + animationWidthHalf) < PXLBLCK_MATRIX_HEIGHT)
                    {
                      pxlBlckUtils_draw_horizontal_bar((i + animationWidthHalf), pxlBlckUtils_convert_color_values_to_32bit(anim_red_on, anim_green_on, anim_blue_on));
                    }

                    delay(anim_delay);
                    pxlBlckUtils_clear_matrix();
                  }

                  pxlBlckUtils_clear_matrix();
                  pxlBlckUtils_update_matrix();
                }
                break;
              case 5:
                {
                  //this animation shows a wandering line comming from both sides =================================================================================================

                  pxlBlckUtils_clear_matrix();

                  for (int16_t i = 0; i <= (PXLBLCK_MATRIX_HEIGHT / 2) - 1; i++)
                  {
                    pxlBlckUtils_draw_horizontal_bar(i, pxlBlckUtils_convert_color_values_to_32bit(anim_red_on, anim_green_on, anim_blue_on));
                    pxlBlckUtils_draw_horizontal_bar(((PXLBLCK_MATRIX_HEIGHT - 1) - i), pxlBlckUtils_convert_color_values_to_32bit(anim_red_on, anim_green_on, anim_blue_on));
                    //pxlBlckUtils_update_matrix();
                    delay(anim_delay);
                  }

                  delay(anim_delay);

                  for (int16_t i = (PXLBLCK_MATRIX_HEIGHT / 2) - 1; i >= 0; i--)
                  {
                    pxlBlckUtils_draw_horizontal_bar(i, pxlBlckUtils_convert_color_values_to_32bit(anim_red_off, anim_green_off, anim_blue_off));
                    pxlBlckUtils_draw_horizontal_bar(((PXLBLCK_MATRIX_HEIGHT - 1) - i), pxlBlckUtils_convert_color_values_to_32bit(anim_red_off, anim_green_off, anim_blue_off));
                    delay(anim_delay);
                  }

                  pxlBlckUtils_clear_matrix();
                  pxlBlckUtils_update_matrix();
                }
                break;
              case 6:
                {
                  //this animation shows falling level on both sides of the ring =================================================================================================
                  pxlBlckUtils_clear_matrix();

                  for (int16_t i = (PXLBLCK_MATRIX_HEIGHT / 2) - 1; i >= 0; i--)
                  {
                    pxlBlckUtils_draw_horizontal_bar(i, pxlBlckUtils_convert_color_values_to_32bit(anim_red_off, anim_green_off, anim_blue_off));
                    pxlBlckUtils_draw_horizontal_bar(((PXLBLCK_MATRIX_HEIGHT - 1) - i), pxlBlckUtils_convert_color_values_to_32bit(anim_red_off, anim_green_off, anim_blue_off));
                    delay(anim_delay);
                  }

                  delay(anim_delay);

                  for (int16_t i = 0; i <= (PXLBLCK_MATRIX_HEIGHT / 2) - 1; i++)
                  {
                    pxlBlckUtils_draw_horizontal_bar(i, pxlBlckUtils_convert_color_values_to_32bit(anim_red_on, anim_green_on, anim_blue_on));
                    pxlBlckUtils_draw_horizontal_bar(((PXLBLCK_MATRIX_HEIGHT - 1) - i), pxlBlckUtils_convert_color_values_to_32bit(anim_red_on, anim_green_on, anim_blue_on));
                    //pxlBlckUtils_update_matrix();
                    delay(anim_delay);
                  }

                  pxlBlckUtils_clear_matrix();
                  pxlBlckUtils_update_matrix();

                }
                break;
            }
            Plugin_205_update();
          } else
          {
            String log = F(PXLBLCK_DEVICE_NAME);
            addLog(LOG_LEVEL_DEBUG, log);
            log = F("   - Animation not executed. Cool-down-time not passed.");
            addLog(LOG_LEVEL_DEBUG, log);

            log = F("pxlBlck: Animation not executed. Cool-down-time not passed.");
            SendStatus(event->Source, log);

          }

          success = true;
        } else if (command == F(PXLBLCK_COMMAND_START_DEMO_MODE))
        {
          pxlBlckUtils_start_demo_mode();
          success = true;
        } else if (command == F(PXLBLCK_COMMAND_SET_DIAL))
        {
          int8_t dialId = param1.toInt(); //This defines which of the available dials will be activated.
          boolean isAbsoluteValue = param2.toInt() == 1; //defines if the received dial id should be interpreted as absolute or relative value

          String log = F(PXLBLCK_DEVICE_NAME);
          addLog(LOG_LEVEL_DEBUG, log);

          uint8_t maxPossibleDials = 0;
          int possibleDialsValues[PLUGIN_205_MAX_DIAL_NUM];

          //count available digits of actual selected matrix
          for (int i = 0; i < PLUGIN_205_MAX_DIAL_NUM; i++)
          {
            if (!Plugin_205_possibleDialList[Plugin_205_selectedMatrixId][i][0].equals(PXLBLCK_DIAL_NONE))
            {
              maxPossibleDials++;
              possibleDialsValues[i] = Plugin_205_possibleDialList[Plugin_205_selectedMatrixId][i][1].toInt();
            }
          }

          log = F("   - maxPossibleDials: ");
          log += maxPossibleDials;
          addLog(LOG_LEVEL_DEBUG, log);

          int8_t newDialId = 0;

          if (isAbsoluteValue)
          {
            Plugin_205_selectedDial = (dialId < maxPossibleDials && dialId >= 0) ? possibleDialsValues[dialId] : Plugin_205_selectedDial;
          } else
          {

            //get the position of the value "Plugin_205_selectedDial" in "Plugin_205_possibleDialList"
            int8_t actualDialPositionInArray = -1;
            for (int i = 0; i < PLUGIN_205_MAX_DIAL_NUM; i++)
            {
              if (Plugin_205_possibleDialList[Plugin_205_selectedMatrixId][i][1].toInt() == Plugin_205_selectedDial)
              {
                actualDialPositionInArray = i;
                break;
              }
            }

            newDialId = actualDialPositionInArray + dialId;

            log = F("   - newDialId(before boundariy check): ");
            log += newDialId;
            addLog(LOG_LEVEL_DEBUG, log);

            if (newDialId >= maxPossibleDials)
            {
              newDialId = 0;
            } else if (newDialId < 0)
            {
              newDialId = maxPossibleDials - 1;
            }

            log = F("   - newDialId: ");
            log += newDialId;
            addLog(LOG_LEVEL_DEBUG, log);

            Plugin_205_selectedDial = (newDialId < maxPossibleDials && newDialId >= 0) ? possibleDialsValues[newDialId] : Plugin_205_selectedDial;
          }

          uint16_t intVariable = PLUGIN_205_CONFIG(1);
          pxlBlckUtils_save_two_bytes_in_uint16(&intVariable, Plugin_205_displayBrightness, Plugin_205_selectedDial);
          PLUGIN_205_CONFIG(1) = intVariable;

          log = F("   - dialId: ");
          log += dialId;
          addLog(LOG_LEVEL_DEBUG, log);
          log = F("   - isAbsoluteValue: ");
          log += isAbsoluteValue;
          addLog(LOG_LEVEL_DEBUG, log);
          log = F("   - selectedDial set to: ");
          log += String(Plugin_205_possibleDialList[Plugin_205_selectedMatrixId][newDialId][0]);
          log += F("(ID:");
          log += String(Plugin_205_selectedDial);
          log += F(")");
          addLog(LOG_LEVEL_DEBUG, log);

          log = F("pxlBlck: selectedDial set to ");
          log += String(Plugin_205_possibleDialList[Plugin_205_selectedMatrixId][newDialId][0]);
          log += F("(ID:");
          log += String(Plugin_205_selectedDial);
          log += F(")");
          SendStatus(event->Source, log);

          success = true;

        }

        break;
      }

    case PLUGIN_EXIT:
      {
        if (PXLBLCK_INSTANCE != NULL)
        {
          delete PXLBLCK_INSTANCE;
          PXLBLCK_INSTANCE = NULL;
        }
        break;
      }

  }
  return success;
}

void Plugin_205_update()
{
  checkTime(); //updating systemtime-variables first
  uint8_t hours = hour();
  uint8_t minutes = minute();
  int seconds = second();

  if (Plugin_205_previousSecond != seconds || Plugin_205_previousMinute != minutes || Plugin_205_previousHour != hours)
  {

    String log = F(PXLBLCK_DEVICE_NAME);
    addLog(LOG_LEVEL_DEBUG, log);
    log = F("   -Time=");
    log += hours;
    log += ":";
    log += minutes;
    log += ":";
    log += seconds;
    addLog(LOG_LEVEL_DEBUG, log);
    log = F("   -ColorOne=");
    log += String(Plugin_205_colorOne);
    addLog(LOG_LEVEL_DEBUG, log);
    log = F("   -ColorTwo=");
    log += String(Plugin_205_colorTwo);
    addLog(LOG_LEVEL_DEBUG, log);
    log = F("   -ColorThree=");
    log += String(Plugin_205_colorThree);
    addLog(LOG_LEVEL_DEBUG, log);
    log = F("   -ColorFour=");
    log += String(Plugin_205_colorFour);
    addLog(LOG_LEVEL_DEBUG, log);
    log = F("  -displayBrightness = ");
    log += Plugin_205_displayBrightness;
    addLog(LOG_LEVEL_DEBUG, log);

    if (PXLBLCK_INSTANCE != NULL)
    {
      if (Plugin_205_displayEnabled && PXLBLCK_RNG_TXT_STRUCT.runtxtDelayTime == 0 && !PXLBLCK_ICON_STRUCT.iconPending && !PXLBLCK_FAKE_TV_STRUCT.running)
      { //time should only be displayed if there is no running text already "on the run"(this is the case if the PXLBLCK_RNG_TXT_STRUCT.runtxtDelayTime is set)

        uint32_t colorOneTemp = pxlBlckUtils_add_brightness_to_color(Plugin_205_displayBrightness, Plugin_205_minimalBrightness, Plugin_205_colorOne);
        uint32_t colorTwoTemp = pxlBlckUtils_add_brightness_to_color(Plugin_205_displayBrightness, Plugin_205_minimalBrightness, Plugin_205_colorTwo);
        uint32_t colorThreeTemp = pxlBlckUtils_add_brightness_to_color(Plugin_205_displayBrightness, Plugin_205_minimalBrightness, Plugin_205_colorThree);
        uint32_t colorFourTemp = pxlBlckUtils_add_brightness_to_color(Plugin_205_displayBrightness, Plugin_205_minimalBrightness, Plugin_205_colorFour);

        switch (Plugin_205_selectedDial)
        {
          case PXLBLCK_DIAL_NAME_BLANK_ID_INT:
            {
              //blank
              pxlBlckUtils_fill_matrix(colorFourTemp);
              pxlBlckUtils_update_matrix();
            }
            break;
          case PXLBLCK_DIAL_NAME_HR_NM_AND_MN_PNTS_ID_INT:
            {
              pxlBlckUtils_clear_matrix();
              Plugin_205_show_dial_hourNumberAndMinutePoints(hours, minutes, colorOneTemp, colorTwoTemp, colorFourTemp, Plugin_205_diallLeadingZerosEnabled);
              pxlBlckUtils_update_matrix();
            }
            break;
          case PXLBLCK_DIAL_NAME_RANDOM_PIXELS_ID_INT:
            {
              //this dial shows every second a new pixel with a random color at a radnom position
              Plugin_205_show_rand_pixels_screensaver();
            }
            break;
          case PXLBLCK_DIAL_NAME_WANDERING_PIXELS_ID_INT:
            {
              //this dial shows a wandering white pixel along the outer border of the matrix
              Plugin_205_wandering_pixel_screensaver(colorOneTemp, colorFourTemp);
            }
            break;
          case PXLBLCK_DIAL_NAME_TV_SIMULATOR_ID_INT:
            {
              //this dial activates the tv simulator
              //Plugin_205_wandering_pixel_screensaver(Plugin_205_colorOne);
            }
            break;
          case PXLBLCK_DIAL_NAME_WORDCLOCK_DIAL_ID_INT:
            {
              //this dial shows the wordclock dial
              //uint8_t Plugin_205_wordclockCharacterGroupsToShow[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25};
              uint8_t Plugin_205_wordclockCharacterGroupsToShow[PLUGIN_205_WORDCLOCK_CHARACTER_GROUPS_NUM] = {0};
              //Must be initiated with 0 because otherwise its possible that the initial value is representing an id of one of the characterGroups.
              //This would lead to a falsepositive for the specific characterGroup-item.

              pxlBlckUtils_clear_matrix();
              Plugin_205_estimate_characterGroupsToShow(hours, minutes, Plugin_205_wordclockCharacterGroupsToShow);
              Plugin_205_show_selected_character_groups(colorOneTemp, colorTwoTemp, colorThreeTemp, Plugin_205_wordclockCharacterGroupsToShow);

              pxlBlckUtils_update_matrix();
            }
            break;
          case PXLBLCK_DIAL_NAME_RINGGCLOCK_DIAL_ID_INT:
            {
              //this dial shows the ringclock dial
              pxlBlckUtils_clear_matrix();
              Plugin_205_show_dial_ringClock(hours, minutes, seconds);
              pxlBlckUtils_update_matrix();
            }
            break;
          case PXLBLCK_DIAL_NAME_FIBONACCI_CLOCK_ID_INT:
            {
              //this dial shows the fiboclock dial
              if (Plugin_205_previousHour != hours || (Plugin_205_previousMinute / 5) != (minutes / 5))
              {
                pxlBlckUtils_clear_matrix();
                Plugin_205_show_dial_fibonacciClock(hours, minutes);
                pxlBlckUtils_update_matrix();
              }
            }
            break;
          case PXLBLCK_DIAL_NAME_DIGIT_CLOCK_DIAL_ID_INT:
            {
              //this dial shows the digitClock dial
              pxlBlckUtils_clear_matrix();
              Plugin_205_show_dial_digitClock(hours, minutes, colorOneTemp, colorTwoTemp, colorThreeTemp, colorFourTemp, Plugin_205_diallLeadingZerosEnabled);
              pxlBlckUtils_update_matrix();
            }
            break;
          case PXLBLCK_DIAL_NAME_HORIZONTAL_NUMBERS_DIAL_ID_INT:
            {
              //this dial shows the time with horizontal numbers using the standard font
              pxlBlckUtils_clear_matrix();
              Plugin_205_show_dial_numbersHorizontal(hours, minutes, colorOneTemp, colorTwoTemp, colorThreeTemp, colorFourTemp, false);
              pxlBlckUtils_update_matrix();
            }
            break;
          case PXLBLCK_DIAL_NAME_HORIZONTAL_MINI_NUMBERS_DIAL_ID_INT:
            {
              //this dial shows the time with the mini digits
              pxlBlckUtils_clear_matrix();
              //Plugin_205_show_dial_numbersHorizontal(hours, minutes, colorOneTemp, colorTwoTemp, colorThreeTemp, colorFourTemp, false);
              Plugin_205_show_dial_horizontalMiniNumbers(hours, minutes, colorOneTemp, colorTwoTemp, colorFourTemp, Plugin_205_diallLeadingZerosEnabled);
              pxlBlckUtils_update_matrix();
            }
            break;
          case PXLBLCK_DIAL_NAME_VERTICAL_MINI_NUMBERS_DIAL_ID_INT:
            {
              //this dial shows the time with the mini digits
              pxlBlckUtils_clear_matrix();
              Plugin_205_show_dial_verticalMiniNumbers(hours, minutes, colorOneTemp, colorTwoTemp, colorFourTemp, Plugin_205_diallLeadingZerosEnabled);
              pxlBlckUtils_update_matrix();
            }
            break;
          case PXLBLCK_DIAL_NAME_DIAGONAL_MINI_NUMBERS_DIAL_ID_INT:
            {
              //this dial shows the time with the diagonal mini digits
              pxlBlckUtils_clear_matrix();
              Plugin_205_show_dial_diagonalMiniNumbers(hours, minutes, colorOneTemp, colorTwoTemp, colorFourTemp, Plugin_205_diallLeadingZerosEnabled);
              pxlBlckUtils_update_matrix();
            }
            break;
          case PXLBLCK_DIAL_NAME_RUNNING_CLOCK_ID_INT:
            {
              if (Plugin_205_previousMinute != minutes)
              {
                Plugin_205_show_dial_runningClock(hours, minutes, colorOneTemp, colorFourTemp, Plugin_205_diallLeadingZerosEnabled);
              }
            }
            break;
          default:
            {
              String log = F(PXLBLCK_DEVICE_NAME);
              addLog(LOG_LEVEL_DEBUG, log);
              log += F("   -show-dial-routine called with wrong dial-id: ");
              log += Plugin_205_selectedDial;
              addLog(LOG_LEVEL_DEBUG, log);
            }
            break;
        }
      } else if (!Plugin_205_displayEnabled)
      {
        //display is disabled so we clear the display
        pxlBlckUtils_clear_matrix();
        pxlBlckUtils_update_matrix();
      }

    } else
    {
      String log = F(PXLBLCK_DEVICE_NAME);
      addLog(LOG_LEVEL_DEBUG, log);
      log = F("   - Status: Not yet initiated. :'(");
      addLog(LOG_LEVEL_DEBUG, log);

    }

    Plugin_205_previousHour = hours;
    Plugin_205_previousMinute = minutes;
    Plugin_205_previousSecond = seconds;
  }
}

// == pxlBlck helper functions ===========================================================================================================================

// == pxlBlck general dial functions and screensavers == start ===========================================================================================================================

void Plugin_205_show_dial_runningClock(uint8_t hours, uint8_t minutes, uint32_t timeColor, uint32_t bgColor, boolean leadingZerosEnabled)
{
  String hoursString = (leadingZerosEnabled && (hours < 10))  ? "0" + String(hours) : String(hours) ;
  String minutesString = (leadingZerosEnabled && (minutes < 10))  ? "0" + String(minutes) : String(minutes) ;
  String currentTime = hoursString + ":" + minutesString;
  pxlBlckUtils_prepare_runing_text(currentTime, timeColor, bgColor, 100, -1);
}

void Plugin_205_wandering_pixel_screensaver(uint32_t color, uint32_t backgroundColor)
{
  //this dial shows a wandering white pixel along the outer border of the matrix

  uint8_t x = Plugin_205_screensaver_position / 10;
  uint8_t y = Plugin_205_screensaver_position % 10;

  float brightness = ((float)Plugin_205_displayBrightness / PXLBLCK_MAX_SETABLE_BRIGHTNESS);


  uint8_t red = pxlBlckUtils_return_red_from_config(color);
  uint8_t green = pxlBlckUtils_return_green_from_config(color);
  uint8_t blue = pxlBlckUtils_return_blue_from_config(color);

  pxlBlckUtils_fill_matrix(backgroundColor);

  for (float i = 0.0; i < brightness; i += 0.01)
  {
    if (PXLBLCK_HIGHER_COLOR_RESOLUTION_ENABLED)
    {
      pxlBlckUtils_draw_pixel(x, y, pxlBlckUtils_convert_color_values_to_32bit(i * red, i * green, i * blue));
    } else
    {
      pxlBlckUtils_draw_pixel(x, y, PXLBLCK_INSTANCE->Color(i * red, i * green, i * blue));
    }

    pxlBlckUtils_update_matrix();
    delay(5);
  }

  if (y == 0 && x == 0)
  {
    x++;
  } else if (y == 0 && x > 0) //pixel is wandering at the upper side of the matrix
  {
    if (x == (PXLBLCK_MATRIX_WIDTH - 1))
    {
      y = 1;
    } else
    {
      x++;
    }
  } else if (x == (PXLBLCK_MATRIX_WIDTH - 1) && y > 0) //pixel is wandering at the left sied of the matrix
  {
    if (y == (PXLBLCK_MATRIX_HEIGHT - 1))
    {
      x = PXLBLCK_MATRIX_WIDTH - 2;
    } else
    {
      y++;
    }
  } else if (y == (PXLBLCK_MATRIX_HEIGHT - 1) && x > 0) //pixel is wandering at the bottom side of the matrix
  {
    if (x == 0)
    {
      y = PXLBLCK_MATRIX_HEIGHT - 2;
    } else
    {
      x--;
    }
  } else if (x == 0 && y > 0) //pixel is wandering on the left side of the matrix
  {
    if (y == 0)
    {
      x = 1;
    } else
    {
      y--;
    }
  }

  Plugin_205_screensaver_position = x * 10 + y; //we save both coordinates in one file


}

void Plugin_205_show_rand_pixels_screensaver()
{
  uint8_t red = random(1, 255);
  uint8_t green = random(1, 255);
  uint8_t blue = random(1, 255);
  uint8_t x = random(0, PXLBLCK_MATRIX_WIDTH);
  uint8_t y = random(0, PXLBLCK_MATRIX_HEIGHT);
  float brightness = ((float)Plugin_205_displayBrightness / PXLBLCK_MAX_SETABLE_BRIGHTNESS);

  for (float i = 0.0; i < brightness; i += 0.01)
  {
    if (PXLBLCK_HIGHER_COLOR_RESOLUTION_ENABLED)
    {
      pxlBlckUtils_draw_pixel(x, y, pxlBlckUtils_convert_color_values_to_32bit(i * red, i * green, i * blue));
    } else
    {
      pxlBlckUtils_draw_pixel(x, y, PXLBLCK_INSTANCE->Color(i * red, i * green, i * blue));
    }
    pxlBlckUtils_update_matrix();
    delay(3);
  }

}

void Plugin_205_show_dial_diagonalMiniNumbers(uint8_t hours, uint8_t minutes, uint32_t hourColor, uint32_t minuteColor, uint32_t bgColor, boolean leadingZerosEnabled)
{
  pxlBlckUtils_fill_matrix(bgColor);

  //prepare show of digits: select all the pixels that need to be switched on. PLUGIN_205_MAX_PIXELS_PER_DIGIT*2 because we want to save the coordinates of two digits per array
  uint8_t pixelsToShowHour[PLUGIN_205_MAX_PIXELS_PER_DIGIT * 2][PLUGIN_205_COORDINATES_PER_PIXEL];
  uint8_t pixelsToShowMinute[PLUGIN_205_MAX_PIXELS_PER_DIGIT * 2][PLUGIN_205_COORDINATES_PER_PIXEL];

  //first write PLUGIN_205_5x4_NUMBER_NONE to all spaces of pixelsToShowHour to mark them as "unused"
  for (uint8_t i = 0; i < (PLUGIN_205_MAX_PIXELS_PER_DIGIT * 2); i++)
  {
    pixelsToShowHour[i][0] = PLUGIN_205_5x4_NUMBER_NONE;
    pixelsToShowHour[i][1] = PLUGIN_205_5x4_NUMBER_NONE;
    pixelsToShowMinute[i][0] = PLUGIN_205_5x4_NUMBER_NONE;
    pixelsToShowMinute[i][1] = PLUGIN_205_5x4_NUMBER_NONE;
  }

  uint8_t hourPixelsToShowCounter = 0; // this variable tells us how many pixels were added to hourPixelsToShowCounter
  uint8_t minutePixelsToShowCounter = 0; // this variable tells us how many pixels were added to minutePixelsToShowCounter

  uint8_t xMinuteStartPos = (!leadingZerosEnabled && hours < 10 ) ? (PXLBLCK_MATRIX_WIDTH - (USED_WIDTH_OF_TWO_DIGITS / 2)) + 1 : (PXLBLCK_MATRIX_WIDTH - USED_WIDTH_OF_TWO_DIGITS);

  uint8_t yMinuteStartPos = PXLBLCK_MATRIX_HEIGHT - USED_HEIGHT_OF_ONE_DIGIT;

  Plugin_205_add_number_to_pixelsToShow(hours, 0, 0, pixelsToShowHour, &hourPixelsToShowCounter, leadingZerosEnabled); //hour is written in the upper left corner of the led matrix
  Plugin_205_add_number_to_pixelsToShow(minutes, xMinuteStartPos, yMinuteStartPos, pixelsToShowMinute, &minutePixelsToShowCounter, leadingZerosEnabled);

  //Writes the hour pixels that are saved in pixelsToShowHour to the display
  Plugin_205_write_prepared_pixels_to_display(pixelsToShowHour, &hourPixelsToShowCounter, &hourColor);

  //Writes the minute pixels that are saved in pixelsToShowMinute to the display
  Plugin_205_write_prepared_pixels_to_display(pixelsToShowMinute, &minutePixelsToShowCounter, &minuteColor);

}

void Plugin_205_show_dial_verticalMiniNumbers(uint8_t hours, uint8_t minutes, uint32_t hourColor, uint32_t minuteColor, uint32_t bgColor, boolean leadingZerosEnabled)
{
  pxlBlckUtils_fill_matrix(bgColor);

  //prepare show of digits: select all the pixels that need to be switched on. PLUGIN_205_MAX_PIXELS_PER_DIGIT*2 because we want to save the coordinates of two digits per array
  uint8_t pixelsToShowHour[PLUGIN_205_MAX_PIXELS_PER_DIGIT * 2][PLUGIN_205_COORDINATES_PER_PIXEL];
  uint8_t pixelsToShowMinute[PLUGIN_205_MAX_PIXELS_PER_DIGIT * 2][PLUGIN_205_COORDINATES_PER_PIXEL];

  //first write PLUGIN_205_5x4_NUMBER_NONE to all spaces of pixelsToShowHour to mark them as "unused"
  for (uint8_t i = 0; i < (PLUGIN_205_MAX_PIXELS_PER_DIGIT * 2); i++)
  {
    pixelsToShowHour[i][0] = PLUGIN_205_5x4_NUMBER_NONE;
    pixelsToShowHour[i][1] = PLUGIN_205_5x4_NUMBER_NONE;
    pixelsToShowMinute[i][0] = PLUGIN_205_5x4_NUMBER_NONE;
    pixelsToShowMinute[i][1] = PLUGIN_205_5x4_NUMBER_NONE;
  }

  uint8_t hourPixelsToShowCounter = 0; // this variable tells us how many pixels were added to hourPixelsToShowCounter
  uint8_t minutePixelsToShowCounter = 0; // this variable tells us how many pixels were added to minutePixelsToShowCounter

  uint8_t xStartPos = (!leadingZerosEnabled && hours < 10 ) ? (PXLBLCK_MATRIX_WIDTH / 2) - (USED_WIDTH_OF_TWO_DIGITS / 4) + 1 : (PXLBLCK_MATRIX_WIDTH / 2) - (USED_WIDTH_OF_TWO_DIGITS / 2);

  uint8_t yStartPos = PXLBLCK_MATRIX_WIDTH > 10 ? 1 : 0;
  uint8_t yOffsetForMinutes = USED_HEIGHT_OF_ONE_DIGIT;


  Plugin_205_add_number_to_pixelsToShow(hours, xStartPos, yStartPos, pixelsToShowHour, &hourPixelsToShowCounter, leadingZerosEnabled);
  Plugin_205_add_number_to_pixelsToShow(minutes, xStartPos, yStartPos + yOffsetForMinutes, pixelsToShowMinute, &minutePixelsToShowCounter, leadingZerosEnabled);

  //Writes the hour pixels that are saved in pixelsToShowHour to the display
  Plugin_205_write_prepared_pixels_to_display(pixelsToShowHour, &hourPixelsToShowCounter, &hourColor);

  //Writes the minute pixels that are saved in pixelsToShowMinute to the display
  Plugin_205_write_prepared_pixels_to_display(pixelsToShowMinute, &minutePixelsToShowCounter, &minuteColor);

}

void Plugin_205_show_dial_horizontalMiniNumbers(uint8_t hours, uint8_t minutes, uint32_t hourColor, uint32_t minuteColor, uint32_t bgColor, boolean leadingZerosEnabled)
{

  pxlBlckUtils_fill_matrix(bgColor);

  //prepare show of digits: select all the pixels that need to be switched on. PLUGIN_205_MAX_PIXELS_PER_DIGIT*2 because we want to save the coordinates of two digits per array
  uint8_t pixelsToShowHour[PLUGIN_205_MAX_PIXELS_PER_DIGIT * 2][PLUGIN_205_COORDINATES_PER_PIXEL];
  uint8_t pixelsToShowMinute[PLUGIN_205_MAX_PIXELS_PER_DIGIT * 2][PLUGIN_205_COORDINATES_PER_PIXEL];

  //first write PLUGIN_205_5x4_NUMBER_NONE to all spaces of pixelsToShowHour to mark them as "unused"
  for (uint8_t i = 0; i < (PLUGIN_205_MAX_PIXELS_PER_DIGIT * 2); i++)
  {
    pixelsToShowHour[i][0] = PLUGIN_205_5x4_NUMBER_NONE;
    pixelsToShowHour[i][1] = PLUGIN_205_5x4_NUMBER_NONE;
    pixelsToShowMinute[i][0] = PLUGIN_205_5x4_NUMBER_NONE;
    pixelsToShowMinute[i][1] = PLUGIN_205_5x4_NUMBER_NONE;
  }

  uint8_t hourPixelsToShowCounter = 0; // this variable tells us how many pixels were added to hourPixelsToShowCounter
  uint8_t minutePixelsToShowCounter = 0; // this variable tells us how many pixels were added to minutePixelsToShowCounter

  uint8_t xStartPos = (!leadingZerosEnabled && hours < 10 ) ? (PXLBLCK_MATRIX_WIDTH / 2) - (USED_WIDTH_OF_TWO_DIGITS / 2) + 1 : (PXLBLCK_MATRIX_WIDTH / 2) - (USED_WIDTH_OF_TWO_DIGITS);
  //uint8_t xOffsetForMinutes = (!leadingZerosEnabled && minutes < 10 ) ? 5 : 8; //4 beacuse this is the width of one digit. 7 is the width of two digits
  uint8_t xOffsetForMinutes = (!leadingZerosEnabled && minutes < 10 ) ? 4 : 8; //4 beacuse this is the width of one digit (including one space between the numbers).

  uint8_t yStartPos = (PXLBLCK_MATRIX_HEIGHT / 2) - (USED_HEIGHT_OF_ONE_DIGIT / 2) - 1; //-1 because to lift the whole thing one more pixel


  Plugin_205_add_number_to_pixelsToShow(hours, xStartPos, yStartPos, pixelsToShowHour, &hourPixelsToShowCounter, leadingZerosEnabled);
  Plugin_205_add_number_to_pixelsToShow(minutes, xStartPos + xOffsetForMinutes, yStartPos, pixelsToShowMinute, &minutePixelsToShowCounter, leadingZerosEnabled);

  //Writes the hour pixels that are saved in pixelsToShowHour to the display
  Plugin_205_write_prepared_pixels_to_display(pixelsToShowHour, &hourPixelsToShowCounter, &hourColor);

  //Writes the minute pixels that are saved in pixelsToShowMinute to the display
  Plugin_205_write_prepared_pixels_to_display(pixelsToShowMinute, &minutePixelsToShowCounter, &minuteColor);

}

void Plugin_205_write_prepared_pixels_to_display(uint8_t pixelsToShow[][PLUGIN_205_COORDINATES_PER_PIXEL], uint8_t *pixelsToShowCounter, uint32_t *color)
{
  //Writes the minute pixels that are saved in pixelsToShowMinute to the display
  for (uint8_t i = 0; i < *pixelsToShowCounter; i++)
  {
    if (pixelsToShow[i][0] < PLUGIN_205_5x4_NUMBER_NONE &&
        pixelsToShow[i][1] < PLUGIN_205_5x4_NUMBER_NONE )
    {
      pxlBlckUtils_draw_pixel(pixelsToShow[i][0], pixelsToShow[i][1], *color);
    }
  }
}

void Plugin_205_show_dial_hourNumberAndMinutePoints(uint8_t hours, uint8_t minutes, uint32_t hourColor, uint32_t minuteColor, uint32_t bgColor, boolean leadingZerosEnabled)
{
  /*
    String hoursOut = String(hours);
    String minutesOut = String(minutes);

    if (hours < 10) {
      hoursOut = "0" + String(hours);
    }

    if (minutes < 10) {
      minutesOut = "0" + String(minutes);
    }
  */
  pxlBlckUtils_fill_matrix(bgColor);

  //the following part should set the pixels within in a specific area
  uint8_t setPixels = 0;

  //uint8_t xDrawStartPosition = ((PXLBLCK_MATRIX_WIDTH / 2) - (DISPLAY_AREA_WIDTH / 2));
  //uint8_t xDrawStartPosition = ((PXLBLCK_MATRIX_WIDTH / 2) - (DISPLAY_AREA_WIDTH / 2));

  uint8_t xOffsetFromLeftMatrixBorder = ((PXLBLCK_MATRIX_WIDTH / 2) - (DISPLAY_AREA_WIDTH / 2));
  //uint8_t yOffsetFromTopMatrixBorder = ((PXLBLCK_MATRIX_HEIGHT / 2) - (DISPLAY_AREA_WIDTH / 2));

  uint8_t xCoordinateOfLeftBorderOfDisplayArea = xOffsetFromLeftMatrixBorder;
  uint8_t xCoordinateOfRightBorderOfDisplayArea = (DISPLAY_AREA_WIDTH + xOffsetFromLeftMatrixBorder) - 1;
  uint8_t yCoordinateOfTopBorderOfDisplayArea = PXLBLCK_MATRIX_HEIGHT - DISPLAY_AREA_HEIGHT;
  uint8_t yCoordinateOfBottomBorderOfDisplayArea = PXLBLCK_MATRIX_HEIGHT - 1;
  /*
    Serial.print("xLeft:");
    Serial.print(xCoordinateOfLeftBorderOfDisplayArea);
    Serial.print("; xRight:");
    Serial.print(xCoordinateOfRightBorderOfDisplayArea);
    Serial.print("; yTop:");
    Serial.print(yCoordinateOfTopBorderOfDisplayArea);
    Serial.print("; yBottom:");
    Serial.println(yCoordinateOfBottomBorderOfDisplayArea);*/

  for (int row = (PXLBLCK_MATRIX_HEIGHT - 1); row >= yCoordinateOfTopBorderOfDisplayArea; row--)
  {
    if (setPixels >= minutes)
      break;

    for (int column = xCoordinateOfLeftBorderOfDisplayArea; column <= xCoordinateOfRightBorderOfDisplayArea; column++)
    {
      if (!(row == yCoordinateOfTopBorderOfDisplayArea && column == xCoordinateOfLeftBorderOfDisplayArea) && //top left corner
          !(row == yCoordinateOfTopBorderOfDisplayArea && column == xCoordinateOfRightBorderOfDisplayArea) && //top right corner
          !(row == yCoordinateOfBottomBorderOfDisplayArea && column == xCoordinateOfLeftBorderOfDisplayArea) && //bottom left corner
          !(row == yCoordinateOfBottomBorderOfDisplayArea && column == xCoordinateOfRightBorderOfDisplayArea)) //bottom right corner
      {
        /*
                Serial.print("row:");
                Serial.print(row);
                Serial.print("; column:");
                Serial.print(column);
                Serial.print("; setPixels:");
                Serial.println(setPixels);
        */
        setPixels++;
        pxlBlckUtils_draw_pixel(column, row, minuteColor);
      }
      if (setPixels >= minutes)
        break;
    }
  }

  //prepare show of hour number: select all the pixels that need to be switched on. PLUGIN_205_MAX_PIXELS_PER_DIGIT*2 because we want to save the coordinates of two digits in this array
  uint8_t pixelsToShowHour[PLUGIN_205_MAX_PIXELS_PER_DIGIT * 2][PLUGIN_205_COORDINATES_PER_PIXEL];

  //first write PLUGIN_205_5x4_NUMBER_NONE to all spaces of pixelsToShowHour to mark them as "unused"
  for (uint8_t i = 0; i < (PLUGIN_205_MAX_PIXELS_PER_DIGIT * 2); i++)
  {
    pixelsToShowHour[i][0] = PLUGIN_205_5x4_NUMBER_NONE;
    pixelsToShowHour[i][1] = PLUGIN_205_5x4_NUMBER_NONE;
  }

  uint8_t pixelsToShowCounter = 0; // this variable tells us how many pixels were added to pixelsToShowHour
  uint8_t xStartPos = (!leadingZerosEnabled && hours < 10 ) ? (PXLBLCK_MATRIX_WIDTH / 2) - (USED_WIDTH_OF_TWO_DIGITS / 4) + 1 : (PXLBLCK_MATRIX_WIDTH / 2) - (USED_WIDTH_OF_TWO_DIGITS / 2);

  Plugin_205_add_number_to_pixelsToShow(hours, xStartPos, 1, pixelsToShowHour, &pixelsToShowCounter, leadingZerosEnabled);
  //Plugin_205_show_selected_pixels(hourColor, pixelsToShowHour);

  //Writes the pixels that are saved in pixelsToShow to the display
  for (uint8_t i = 0; i < pixelsToShowCounter; i++)
  {
    if (pixelsToShowHour[i][0] < PLUGIN_205_5x4_NUMBER_NONE &&
        pixelsToShowHour[i][1] < PLUGIN_205_5x4_NUMBER_NONE )
    {
      pxlBlckUtils_draw_pixel(pixelsToShowHour[i][0], pixelsToShowHour[i][1], hourColor);

    }
  }

}

void Plugin_205_add_number_to_pixelsToShow(uint8_t numberToShow, uint8_t x_pos, uint8_t y_pos, uint8_t pixelsToShow[][PLUGIN_205_COORDINATES_PER_PIXEL], uint8_t *pixelsToShowCounter, boolean addLeaddingZero)
{
  //numberToShow: Holds the number that should be added to the list of the digits that should be shown on the display later
  //x_pos: x-coordinate of the upper left corner of the digit
  //y_pos: y-coordinate of the upper left corner of the digit

  if (numberToShow < 10) //if the desired number is smaller than nine we only need to add one digit to the pixelsToShow-list
  {
    for (uint8_t pixelI = 0; pixelI < PLUGIN_205_MAX_PIXELS_PER_DIGIT ; pixelI++) //Iterates through all possible positions of one defined number and adds the coordinates to the pixelsToShow-list
    {
      if (addLeaddingZero && Plugin_205_5x4Numbers[0][pixelI][0] < PLUGIN_205_5x4_NUMBER_NONE) //adds a zero as leading number if activated
      {
        pixelsToShow[*pixelsToShowCounter][0] = Plugin_205_5x4Numbers[0][pixelI][0] + x_pos; //add x coordinate of selected number to pixelsToShow-list
        pixelsToShow[*pixelsToShowCounter][1] = Plugin_205_5x4Numbers[0][pixelI][1] + y_pos; //add y coordinate of selected number to pixelsToShow-list
        *pixelsToShowCounter = *pixelsToShowCounter + 1;
      }
      if (Plugin_205_5x4Numbers[numberToShow][pixelI][0] < PLUGIN_205_5x4_NUMBER_NONE) //if coordinates at actual index(pixelI) valid/needed for the desired number add them to the list
      {
        pixelsToShow[*pixelsToShowCounter][0] = Plugin_205_5x4Numbers[numberToShow][pixelI][0] + x_pos + (addLeaddingZero ? 5 : 0); //add x coordinate of selected number to pixelsToShow-list
        pixelsToShow[*pixelsToShowCounter][1] = Plugin_205_5x4Numbers[numberToShow][pixelI][1] + y_pos; //add y coordinate of selected number to pixelsToShow-list
        *pixelsToShowCounter = *pixelsToShowCounter + 1;
      }
    }
  } else //number is greater than 9
  {
    uint8_t xPosSecondDigit = x_pos;
    if (numberToShow < 20) // in case that the number starts wih a one we shift the whole number one pixel to the left. So it looks more centralized.
    {
      x_pos -= 1;

      //in this case of course the second digits needs to be shifted one pixel further to the right to stay on the same place as before
      xPosSecondDigit = x_pos + 1;
    }

    for (uint8_t pixelI = 0; pixelI < PLUGIN_205_MAX_PIXELS_PER_DIGIT ; pixelI++) //Iterates through all possible positions of one defined number and adds the coordinates to the pixelsToShow-list
    {
      if (Plugin_205_5x4Numbers[numberToShow % 10][pixelI][0] < PLUGIN_205_5x4_NUMBER_NONE &&
          Plugin_205_5x4Numbers[numberToShow % 10][pixelI][1] < PLUGIN_205_5x4_NUMBER_NONE) //if coordinates at actual index(pixelI) valid/needed for the desired number add them to the list
      {
        pixelsToShow[*pixelsToShowCounter][0] = Plugin_205_5x4Numbers[numberToShow % 10][pixelI][0] + xPosSecondDigit + 5; //add x coordinate of selected number to pixelsToShow-list, +5 to shift the digit over by five pixels (to not write over the first digit)
        pixelsToShow[*pixelsToShowCounter][1] = Plugin_205_5x4Numbers[numberToShow % 10][pixelI][1] + y_pos; //add y coordinate of selected number to pixelsToShow-list
        *pixelsToShowCounter = *pixelsToShowCounter + 1;
      }

      if (Plugin_205_5x4Numbers[numberToShow / 10][pixelI][0] < PLUGIN_205_5x4_NUMBER_NONE &&
          Plugin_205_5x4Numbers[numberToShow / 10][pixelI][1] < PLUGIN_205_5x4_NUMBER_NONE) //if coordinates at actual index(pixelI) valid/needed for the desired number add them to the list
      {
        pixelsToShow[*pixelsToShowCounter][0] = Plugin_205_5x4Numbers[numberToShow / 10][pixelI][0] + x_pos; //add x coordinate of selected number to pixelsToShow-list
        pixelsToShow[*pixelsToShowCounter][1] = Plugin_205_5x4Numbers[numberToShow / 10][pixelI][1] + y_pos; //add y coordinate of selected number to pixelsToShow-list
        *pixelsToShowCounter = *pixelsToShowCounter + 1;
      }
    }
  }
}

// == pxlBlck general dial functions and screensavers == end ================================================================================================================

// == pxlBlckRingclock dial functions == start ========================================================================================================

void Plugin_205_show_dial_ringClock(int hours, int minutes, int seconds)
{
  //First calculate mark positions
  uint8_t markPositionsList[14];
  for (int i = 0; i < 12; i++)
  {
    markPositionsList[i] = 5 * i + (Plugin_205_ringclockClockTopOffset % 5);
  }
  if (Plugin_205_ringclockThick12markEnabled) {
    if (Plugin_205_ringclockClockTopOffset == 0)
    {
      markPositionsList[12] = 1;
      markPositionsList[13] = 59;
    }
    else if (Plugin_205_ringclockClockTopOffset == 59)
    {
      markPositionsList[12] = 0;
      markPositionsList[13] = 58;
    }
    else
    {
      markPositionsList[12] = Plugin_205_ringclockClockTopOffset + 1;
      markPositionsList[13] = Plugin_205_ringclockClockTopOffset - 1;
    }
  } else
  {
    markPositionsList[12] = 255;
    markPositionsList[13] = 255;
  }

  //Now calculate digit positions
  if (hours > 11)
    hours = hours - 12;

  //make the hour hand move each 12 minutes and apply the offset
  if (Plugin_205_ringclockClockDirInversed)
    hours = map((hours * 5 + (minutes / 12)), 0, 60, PXLBLCK_MATRIX_HEIGHT, 0) + Plugin_205_ringclockClockTopOffset;
  else
    hours = map((hours * 5 + (minutes / 12)), 0, 60, 0, PXLBLCK_MATRIX_HEIGHT) + Plugin_205_ringclockClockTopOffset;

  if (hours > 59)
    hours = hours - 60;

  //apply offset to minutes
  if (Plugin_205_ringclockClockDirInversed)
    minutes = map(minutes, 0, 60, PXLBLCK_MATRIX_HEIGHT, 0) + Plugin_205_ringclockClockTopOffset;
  else
    minutes = map(minutes, 0, 60, 0, PXLBLCK_MATRIX_HEIGHT) + Plugin_205_ringclockClockTopOffset;

  if (minutes > 59)
    minutes =  minutes - 60;

  //apply offset to seconds
  if (Plugin_205_ringclockClockDirInversed)
    seconds = map(seconds, 0, 60, PXLBLCK_MATRIX_HEIGHT, 0) + Plugin_205_ringclockClockTopOffset;
  else
    seconds = map(seconds, 0, 60, 0, PXLBLCK_MATRIX_HEIGHT) + Plugin_205_ringclockClockTopOffset;

  if (seconds > 59)
    seconds = seconds - 60;

  //calculate color values of marks back from 24/32bit color value and add brigtness

  uint32_t Plugin_205_marksColor_temp = pxlBlckUtils_add_brightness_to_color(Plugin_205_ringclockHourMarksBrightness, Plugin_205_minimalBrightness, Plugin_205_colorFour);

  for (int i = 0 ; i < 14; i ++) //set the hour marks
  {
    if ((markPositionsList[i] != hours) && (markPositionsList[i] != minutes) && (markPositionsList[i] != seconds) && (markPositionsList[i] != 255)) //do not draw a mark there is a clock hand in that position
    {
      //pxlBlckUtils_draw_pixel(0, markPositionsList[i], Plugin_205_marksColor_temp);
      pxlBlckUtils_draw_pixel(0, markPositionsList[i], Plugin_205_marksColor_temp);
    }
  }

  uint32_t hr_color_temp = pxlBlckUtils_add_brightness_to_color(Plugin_205_displayBrightness, Plugin_205_minimalBrightness, Plugin_205_colorOne);
  uint32_t minute_color_temp = pxlBlckUtils_add_brightness_to_color(Plugin_205_displayBrightness, Plugin_205_minimalBrightness, Plugin_205_colorTwo);
  uint32_t second_color_temp = pxlBlckUtils_add_brightness_to_color(Plugin_205_displayBrightness, Plugin_205_minimalBrightness, Plugin_205_colorThree);

  for (int i = 0; i < PXLBLCK_MATRIX_HEIGHT; i++) //draw the clock hands
  {
    if (i == hours)
    {
      pxlBlckUtils_draw_pixel(0, i, hr_color_temp);
    }
    if (i == minutes)
    {
      pxlBlckUtils_draw_pixel(0, i, minute_color_temp);
    }
    if (i == seconds && second_color_temp > 0)
    {
      pxlBlckUtils_draw_pixel(0, i, second_color_temp);
    }
  }
}

// == pxlBlckRingclock dial functions == end ===================================================================================================================

// == pxlBlckWordclock dial functions == start =================================================================================================================

void Plugin_205_add_element_to_array(uint8_t container[], uint8_t *index, uint8_t itemToAdd)
{
  container[*index] = itemToAdd;
  *index = *index + 1;
}

void Plugin_205_estimate_characterGroupsToShow(uint8_t hours, uint8_t minutes, uint8_t Plugin_205_wordclockCharacterGroupsToShow[])
{
  //This function extimates which character groups have to be added to Plugin_205_wordclockCharacterGroupsToShow[] regarding the actual hour and minute

  /*
    #define IT_IS  1
    #define FIVE_FIRSTONE  2
    #define TEN_FIRSTONE  3
    #define TWENTY_FIRSTONE  4
    #define THREE_FIRSTONE  5
    #define QUARTER  6
    #define AFTER  7
    #define BEFORE  8
    #define HALF  9
    #define TWELVE  10
    #define TWO  11
    #define ONE  12
    #define SEVEN  13
    #define THREE  14
    #define FIVE 15
    #define ELEVEN  16
    #define NINE  17
    #define FOUR  18
    #define EIGHT  19
    #define TEN  20
    #define SIX  21
    #define OCLOCK  22
    #define MINUTE1  23
    #define MINUTE2  24
    #define MINUTE3  25
    #define MINUTE4  26
  */

  if (minutes >= 25)//after 24 minutes the hour-value must be increased by one because after that the time is displayed in relation to the next hour
    hours++;

  if (hours >= 12) //to care for 24h mode and calculate it back to 12h mode
    hours = hours - 12;

  String log = F("   -wordclockTime=");
  log += hours;
  log += ":";
  log += minutes;
  addLog(LOG_LEVEL_DEBUG, log);

  uint8_t index = 0;

  if (Plugin_205_wordclockShowItIsEnabled)
  {
    Plugin_205_add_element_to_array(Plugin_205_wordclockCharacterGroupsToShow, &index, IT_IS);
  }

  if (Plugin_205_wordclockShowOClockEnabled && minutes < 5)
  {
    Plugin_205_add_element_to_array(Plugin_205_wordclockCharacterGroupsToShow, &index, OCLOCK);
  }

  switch (hours)
  {
    case 0:
      Plugin_205_add_element_to_array(Plugin_205_wordclockCharacterGroupsToShow, &index, TWELVE);
      break;
    case 1:
      Plugin_205_add_element_to_array(Plugin_205_wordclockCharacterGroupsToShow, &index, ONE);
      break;
    case 2:
      Plugin_205_add_element_to_array(Plugin_205_wordclockCharacterGroupsToShow, &index, TWO);
      break;
    case 3:
      Plugin_205_add_element_to_array(Plugin_205_wordclockCharacterGroupsToShow, &index, THREE);
      break;
    case 4:
      Plugin_205_add_element_to_array(Plugin_205_wordclockCharacterGroupsToShow, &index, FOUR);
      break;
    case 5:
      Plugin_205_add_element_to_array(Plugin_205_wordclockCharacterGroupsToShow, &index, FIVE);
      break;
    case 6:
      Plugin_205_add_element_to_array(Plugin_205_wordclockCharacterGroupsToShow, &index, SIX);
      break;
    case 7:
      Plugin_205_add_element_to_array(Plugin_205_wordclockCharacterGroupsToShow, &index, SEVEN);
      break;
    case 8:
      Plugin_205_add_element_to_array(Plugin_205_wordclockCharacterGroupsToShow, &index, EIGHT);
      break;
    case 9:
      Plugin_205_add_element_to_array(Plugin_205_wordclockCharacterGroupsToShow, &index, NINE);
      break;
    case 10:
      Plugin_205_add_element_to_array(Plugin_205_wordclockCharacterGroupsToShow, &index, TEN);
      break;
    case 11:
      Plugin_205_add_element_to_array(Plugin_205_wordclockCharacterGroupsToShow, &index, ELEVEN);
      break;
    case 12:
      Plugin_205_add_element_to_array(Plugin_205_wordclockCharacterGroupsToShow, &index, TWELVE);
      break;
    default:
      break;
  }

  switch (minutes) {
    case 5 ... 9:
      Plugin_205_add_element_to_array(Plugin_205_wordclockCharacterGroupsToShow, &index, FIVE_FIRSTONE);
      Plugin_205_add_element_to_array(Plugin_205_wordclockCharacterGroupsToShow, &index, AFTER);
      break;
    case 10 ... 14:
      Plugin_205_add_element_to_array(Plugin_205_wordclockCharacterGroupsToShow, &index, TEN_FIRSTONE);
      Plugin_205_add_element_to_array(Plugin_205_wordclockCharacterGroupsToShow, &index, AFTER);
      break;
    case 15 ... 19:
      Plugin_205_add_element_to_array(Plugin_205_wordclockCharacterGroupsToShow, &index, QUARTER);
      Plugin_205_add_element_to_array(Plugin_205_wordclockCharacterGroupsToShow, &index, AFTER);
      break;
    case 20 ... 24:
      Plugin_205_add_element_to_array(Plugin_205_wordclockCharacterGroupsToShow, &index, TWENTY_FIRSTONE);
      Plugin_205_add_element_to_array(Plugin_205_wordclockCharacterGroupsToShow, &index, AFTER);
      break;
    case 25 ... 29:
      Plugin_205_add_element_to_array(Plugin_205_wordclockCharacterGroupsToShow, &index, FIVE_FIRSTONE);
      Plugin_205_add_element_to_array(Plugin_205_wordclockCharacterGroupsToShow, &index, BEFORE);
      Plugin_205_add_element_to_array(Plugin_205_wordclockCharacterGroupsToShow, &index, HALF);
      break;
    case 30 ... 34:
      Plugin_205_add_element_to_array(Plugin_205_wordclockCharacterGroupsToShow, &index, HALF);
      break;
    case 35 ... 39:
      Plugin_205_add_element_to_array(Plugin_205_wordclockCharacterGroupsToShow, &index, FIVE_FIRSTONE);
      Plugin_205_add_element_to_array(Plugin_205_wordclockCharacterGroupsToShow, &index, AFTER);
      Plugin_205_add_element_to_array(Plugin_205_wordclockCharacterGroupsToShow, &index, HALF);
      break;
    case 40 ... 44:
      Plugin_205_add_element_to_array(Plugin_205_wordclockCharacterGroupsToShow, &index, TWENTY_FIRSTONE);
      Plugin_205_add_element_to_array(Plugin_205_wordclockCharacterGroupsToShow, &index, BEFORE);
      break;
    case 45 ... 49:
      Plugin_205_add_element_to_array(Plugin_205_wordclockCharacterGroupsToShow, &index, QUARTER);
      Plugin_205_add_element_to_array(Plugin_205_wordclockCharacterGroupsToShow, &index, BEFORE);
      break;
    case 50 ... 54:
      Plugin_205_add_element_to_array(Plugin_205_wordclockCharacterGroupsToShow, &index, TEN_FIRSTONE);
      Plugin_205_add_element_to_array(Plugin_205_wordclockCharacterGroupsToShow, &index, BEFORE);
      break;
    case 55 ... 59:
      Plugin_205_add_element_to_array(Plugin_205_wordclockCharacterGroupsToShow, &index, FIVE_FIRSTONE);
      Plugin_205_add_element_to_array(Plugin_205_wordclockCharacterGroupsToShow, &index, BEFORE);
      break;
    default:
      break;
  }

  switch (minutes % 5)
  {
    case 4:
      Plugin_205_add_element_to_array(Plugin_205_wordclockCharacterGroupsToShow, &index, MINUTE1);
    case 3:
      Plugin_205_add_element_to_array(Plugin_205_wordclockCharacterGroupsToShow, &index, MINUTE2);
    case 2:
      Plugin_205_add_element_to_array(Plugin_205_wordclockCharacterGroupsToShow, &index, MINUTE3);
    case 1:
      Plugin_205_add_element_to_array(Plugin_205_wordclockCharacterGroupsToShow, &index, MINUTE4);
      break;
    default:
      break;
  }
}

void Plugin_205_show_selected_character_groups(uint32_t hourColor, uint32_t minuteColor, uint32_t bgColor, uint8_t Plugin_205_wordclockCharacterGroupsToShow[])
{
  //This function displays the selected character groups that are stored in Plugin_205_wordclockCharacterGroupsToShow[] on the led matrix


  hourColor = pxlBlckUtils_add_brightness_to_color(Plugin_205_displayBrightness, Plugin_205_minimalBrightness, hourColor);
  minuteColor = pxlBlckUtils_add_brightness_to_color(Plugin_205_displayBrightness, Plugin_205_minimalBrightness, minuteColor);
  bgColor = pxlBlckUtils_add_brightness_to_color(Plugin_205_displayBrightness, Plugin_205_minimalBrightness, bgColor);
  pxlBlckUtils_fill_matrix(bgColor);

  for (uint8_t i = 0; i < PLUGIN_205_WORDCLOCK_CHARACTER_GROUPS_NUM; i++) //iterates through all selected character groups that we would like to switch on. Only in the case that we want to switch all available character groups on this has to run PLUGIN_205_WORDCLOCK_CHARACTER_GROUPS_NUM-times
  {
    for (uint8_t j = 1; j < PXLBLCK_MATRIX_WIDTH; j++) //iterate thorugh all columns that should switched on of the selected character group
    {
      //i holds the number of the actual characterGroup we would like to display
      //Plugin_205_wordclockCharacterGroupsToShow[i] holds the id of the characterGroup we would like to display
      //Plugin_205_wordclockCharacterGroups[Plugin_205_wordclockLanguageId][Plugin_205_wordclockCharacterGroupsToShow[i]][j] holds the coordinates of the single pixels(leds) stored in each characterGroup. The first stored value at position [0] is always the row.
      //The values from [1...PXLBLCK_MATRIX_WIDTH] represent the column values of the single pixels per characterGroup.

      if (Plugin_205_wordclockCharacterGroups[Plugin_205_wordclockLanguageId][Plugin_205_wordclockCharacterGroupsToShow[i] - 1][j] < PXLBLCK_MATRIX_WIDTH && Plugin_205_wordclockCharacterGroupsToShow[i] > 0)
      {
        uint8_t y = Plugin_205_wordclockCharacterGroups[Plugin_205_wordclockLanguageId][Plugin_205_wordclockCharacterGroupsToShow[i] - 1][0];
        uint32_t outputColor = y == 10 ? minuteColor : hourColor; //in case we are actually writing to the row that contains the minute leds we have to use the minuteColor

        //add only Plugin_205_wordclockCharacterGroups with a higher id as zero(valid id's start at one) and only Plugin_205_wordclockCharacterGroups with x-coordinates lower than PXLBLCK_MATRIX_WIDTH(unused spaces in Plugin_205_wordclockCharacterGroups are filled with values that are greater than PXLBLCK_MATRIX_WIDTH)
        pxlBlckUtils_draw_pixel(Plugin_205_wordclockCharacterGroups[Plugin_205_wordclockLanguageId][Plugin_205_wordclockCharacterGroupsToShow[i] - 1][j], y, outputColor);

      }
    }
  }
}

uint16_t Plugin_205_wordclock_grid_pixel_remap(uint16_t x, uint16_t y)
{
  //Minute LED's that are physically on the first four positions of the LED-Strip are virtually remaped by this function. They are remaped to the 11. row and first four columns. So:
  //MinuteLED1 is at Position: 1x11
  //MinuteLED2 is at Position: 2x11
  //MinuteLED3 is at Position: 3x11
  //MinuteLED4 is at Position: 4x11
  if (y == 10)
  {
    switch (x)
    {
      case 1:
        return 0;
      case 2:
        return 1;
      case 3:
        return 2;
      case 4:
        return 3;
    }
  }

  if (y & 1)
    return ((y + 1) * PXLBLCK_MATRIX_WIDTH - 1 - x) + 4;
  else
    return  (y      * PXLBLCK_MATRIX_WIDTH     + x) + 4;

}

// == pxlBlckWordclock dial functions == end ===================================================================================================================


// == pxlBlckFiboClock dial functions == start ===========================================================================================================================

void Plugin_205_show_dial_fibonacciClock(uint8_t hours, uint8_t minutes)
{
  if (hours == 0)
    hours = 12; // 12 midnight
  else if (hours > 12)
    hours -= 12;

  for (int i = 0; i < PXLBLCK_FIBOCLOCK_MATRIX_HEIGHT; i++)
    Plugin_205_bits[i] = 0;

  setBits(hours, 0x01);
  setBits(minutes / 5, 0x02);

  for (int i = 0; i < PXLBLCK_FIBOCLOCK_MATRIX_HEIGHT; i++)
  {
    set_fibonacci_pixel(i, Plugin_205_color_value_from_pallete(Plugin_205_bits[i]));
    //Plugin_205_update_strip();
  }

}

void set_fibonacci_pixel(byte pixel, uint32_t color)
{
  //sets the desired led-area(that represents one of the fibonacci numbers) with the desired color
  switch (pixel)
  {
    case 0:
      //sets the first led-area that represents the fibonacci number 1 to the desired color
      pxlBlckUtils_draw_pixel(0, 0, color);
      break;
    case 1:
      //sets the second led-area that represents the fibonacci number 1 to the desired color
      pxlBlckUtils_draw_pixel(0, 1, color);
      break;
    case 2:
      //sets the led-area that represents the fibonacci number 2 to the desired color
      pxlBlckUtils_draw_pixel(0, 2, color);
      break;
    case 3:
      //sets the led-area that represents the fibonacci number 3 to the desired color
      pxlBlckUtils_draw_pixel(0, 3, color);
      pxlBlckUtils_draw_pixel(0, 4, color);
      break;
    case 4:
      //sets the led-area that represents the fibonacci number 5 to the desired color
      pxlBlckUtils_draw_pixel(0, 5, color);
      pxlBlckUtils_draw_pixel(0, 6, color);
      pxlBlckUtils_draw_pixel(0, 7, color);
      pxlBlckUtils_draw_pixel(0, 8, color);
      break;
  };
}

void setBits(byte value, byte offset)
{
  switch (value)
  {
    case 1:
      switch (random(2))
      {
        case 0:
          Plugin_205_bits[0] |= offset;
          break;
        case 1:
          Plugin_205_bits[1] |= offset;
          break;
      }
      break;
    case 2:
      switch (random(2))
      {
        case 0:
          Plugin_205_bits[2] |= offset;
          break;
        case 1:
          Plugin_205_bits[0] |= offset;
          Plugin_205_bits[1] |= offset;
          break;
      }
      break;
    case 3:
      switch (random(3))
      {
        case 0:
          Plugin_205_bits[3] |= offset;
          break;
        case 1:
          Plugin_205_bits[0] |= offset;
          Plugin_205_bits[2] |= offset;
          break;
        case 2:
          Plugin_205_bits[1] |= offset;
          Plugin_205_bits[2] |= offset;
          break;
      }
      break;
    case 4:
      switch (random(3))
      {
        case 0:
          Plugin_205_bits[0] |= offset;
          Plugin_205_bits[3] |= offset;
          break;
        case 1:
          Plugin_205_bits[1] |= offset;
          Plugin_205_bits[3] |= offset;
          break;
        case 2:
          Plugin_205_bits[0] |= offset;
          Plugin_205_bits[1] |= offset;
          Plugin_205_bits[2] |= offset;
          break;
      }
      break;
    case 5:
      switch (random(3))
      {
        case 0:
          Plugin_205_bits[4] |= offset;
          break;
        case 1:
          Plugin_205_bits[2] |= offset;
          Plugin_205_bits[3] |= offset;
          break;
        case 2:
          Plugin_205_bits[0] |= offset;
          Plugin_205_bits[1] |= offset;
          Plugin_205_bits[3] |= offset;
          break;
      }
      break;
    case 6:
      switch (random(4))
      {
        case 0:
          Plugin_205_bits[0] |= offset;
          Plugin_205_bits[4] |= offset;
          break;
        case 1:
          Plugin_205_bits[1] |= offset;
          Plugin_205_bits[4] |= offset;
          break;
        case 2:
          Plugin_205_bits[0] |= offset;
          Plugin_205_bits[2] |= offset;
          Plugin_205_bits[3] |= offset;
          break;
        case 3:
          Plugin_205_bits[1] |= offset;
          Plugin_205_bits[2] |= offset;
          Plugin_205_bits[3] |= offset;
          break;
      }
      break;
    case 7:
      switch (random(3))
      {
        case 0:
          Plugin_205_bits[2] |= offset;
          Plugin_205_bits[4] |= offset;
          break;
        case 1:
          Plugin_205_bits[0] |= offset;
          Plugin_205_bits[1] |= offset;
          Plugin_205_bits[4] |= offset;
          break;
        case 2:
          Plugin_205_bits[0] |= offset;
          Plugin_205_bits[1] |= offset;
          Plugin_205_bits[2] |= offset;
          Plugin_205_bits[3] |= offset;
          break;
      }
      break;
    case 8:
      switch (random(3))
      {
        case 0:
          Plugin_205_bits[3] |= offset;
          Plugin_205_bits[4] |= offset;
          break;
        case 1:
          Plugin_205_bits[0] |= offset;
          Plugin_205_bits[2] |= offset;
          Plugin_205_bits[4] |= offset;
          break;
        case 2:
          Plugin_205_bits[1] |= offset;
          Plugin_205_bits[2] |= offset;
          Plugin_205_bits[4] |= offset;
          break;
      }
      break;
    case 9:
      switch (random(2))
      {
        case 0:
          Plugin_205_bits[0] |= offset;
          Plugin_205_bits[3] |= offset;
          Plugin_205_bits[4] |= offset;
          break;
        case 1:
          Plugin_205_bits[1] |= offset;
          Plugin_205_bits[3] |= offset;
          Plugin_205_bits[4] |= offset;
          break;
      }
      break;
    case 10:
      switch (random(2))
      {
        case 0:
          Plugin_205_bits[2] |= offset;
          Plugin_205_bits[3] |= offset;
          Plugin_205_bits[4] |= offset;
          break;
        case 1:
          Plugin_205_bits[0] |= offset;
          Plugin_205_bits[1] |= offset;
          Plugin_205_bits[3] |= offset;
          Plugin_205_bits[4] |= offset;
          break;
      }
      break;
    case 11:
      switch (random(2))
      {
        case 0:
          Plugin_205_bits[0] |= offset;
          Plugin_205_bits[2] |= offset;
          Plugin_205_bits[3] |= offset;
          Plugin_205_bits[4] |= offset;
          break;
        case 1:
          Plugin_205_bits[1] |= offset;
          Plugin_205_bits[2] |= offset;
          Plugin_205_bits[3] |= offset;
          Plugin_205_bits[4] |= offset;
          break;
      }

      break;
    case 12:
      Plugin_205_bits[0] |= offset;
      Plugin_205_bits[1] |= offset;
      Plugin_205_bits[2] |= offset;
      Plugin_205_bits[3] |= offset;
      Plugin_205_bits[4] |= offset;

      break;
  }
}

uint32_t Plugin_205_color_value_from_pallete(uint8_t bitNumber)
{
  //converts the rgb values which are saved in colors-array to uint32_t-color value
  //if (Plugin_205_useCustomColor)
  //{
  switch (bitNumber)
  {
    case 0:
      return 0;
      break;
    case 1:
      return Plugin_205_colorOne;
      break;
    case 2:
      return Plugin_205_colorTwo;
      break;
    case 3:
      return Plugin_205_colorThree;
      break;
    default:
      return 0;
      break;
  }
  //} else
  //{
  //  float brightness   = (float)Plugin_205_displayBrightness / PXLBLCK_MAX_SETABLE_BRIGHTNESS;
  //  return Plugin_205_ledStrip->Color(brightness * Plugin_205_colors[palettenNummer][bitNumber][0], brightness * Plugin_205_colors[palettenNummer][bitNumber][1], brightness * Plugin_205_colors[palettenNummer][bitNumber][2]);
  //}

}

// == pxlBlckFiboClock dial functions == end ===========================================================================================================================

// == pxlBlck24x8 dial functions == start ===========================================================================================================================

void Plugin_205_show_dial_numbersHorizontal(uint8_t hours, uint8_t minutes, uint32_t hourColor, uint32_t minuteColor, uint32_t dotColor, uint32_t bgColor, bool includingDots)
{
  String hoursOut = String(hours);
  String minutesOut = String(minutes);

  if (hours < 10) {
    hoursOut = "0" + String(hours);
  }

  if (minutes < 10) {
    minutesOut = "0" + String(minutes);
  }

  pxlBlckUtils_fill_matrix(bgColor);


  if (PXLBLCK_HIGHER_COLOR_RESOLUTION_ENABLED)
    PXLBLCK_INSTANCE->setPassThruColor(hourColor);
  else
    PXLBLCK_INSTANCE->setTextColor(hourColor);

  PXLBLCK_INSTANCE->setCursor(0, 0);
  PXLBLCK_INSTANCE->print(hoursOut);

  if (includingDots)
  {

    if (PXLBLCK_HIGHER_COLOR_RESOLUTION_ENABLED)
      PXLBLCK_INSTANCE->setPassThruColor(dotColor);
    else
      PXLBLCK_INSTANCE->setTextColor(dotColor);

    PXLBLCK_INSTANCE->setCursor(9, 0);
    PXLBLCK_INSTANCE->print(":");

    PXLBLCK_INSTANCE->setCursor(10, 0);
    PXLBLCK_INSTANCE->print(":");
  }

  if (PXLBLCK_HIGHER_COLOR_RESOLUTION_ENABLED)
    PXLBLCK_INSTANCE->setPassThruColor(minuteColor);
  else
    PXLBLCK_INSTANCE->setTextColor(minuteColor);

  PXLBLCK_INSTANCE->setCursor(13, 0);
  PXLBLCK_INSTANCE->print(minutesOut);
}



// == pxlBlck24x8 dial functions == end ===========================================================================================================================


// == pxlBlckdigiClock dial functions == start ===========================================================================================================================


void Plugin_205_show_dial_digitClock(uint8_t hours, uint8_t minutes, uint32_t hourColor, uint32_t  minuteColor, uint32_t dotColor, uint32_t  bgColor, boolean inclLeadingZeros)
{

  pxlBlckUtils_fill_matrix(bgColor);
  if ((hours / 10) > 0 || inclLeadingZeros)
    Plugin_205_display_digitClock_digit(hours / 10, 0, hourColor); //0 is the y coordinate of the matrix that represents the bottom segment of the first digit
  Plugin_205_display_digitClock_digit(hours % 10, 7, hourColor); //7 is the y coordinate of the matrix that represents the bottom segment of the second digit

  pxlBlckUtils_draw_horizontal_bar_no_update(14, dotColor); //dots: Regarding the digitClock matrix pattern the dots are located in row 14

  if ((minutes / 10) > 0 || inclLeadingZeros)
    Plugin_205_display_digitClock_digit(minutes / 10, 15, minuteColor); //15 is the y coordinate of the matrix that represents the bottom segment of the third digit (here is the offset of the dots included.)
  Plugin_205_display_digitClock_digit(minutes % 10, 22, minuteColor); //22 is the y coordinate of the matrix that represents the bottom segment of the fourth digit

}

void Plugin_205_display_digitClock_digit(uint8_t number, uint8_t baseSegmentOffset, uint32_t color)
{
  switch (number)
  {
    case 0:
      pxlBlckUtils_draw_horizontal_bar_no_update(baseSegmentOffset, color); //bottom segment
      pxlBlckUtils_draw_horizontal_bar_no_update(baseSegmentOffset + 1, color); //bottom right segment
      pxlBlckUtils_draw_horizontal_bar_no_update(baseSegmentOffset + 2, color); //top right segment
      pxlBlckUtils_draw_horizontal_bar_no_update(baseSegmentOffset + 3, color); //top segment
      pxlBlckUtils_draw_horizontal_bar_no_update(baseSegmentOffset + 4, color); //top left segment
      pxlBlckUtils_draw_horizontal_bar_no_update(baseSegmentOffset + 6, color); //bottom left segment
      break;
    case 1:
      pxlBlckUtils_draw_horizontal_bar_no_update(baseSegmentOffset + 1, color); //bottom right segment
      pxlBlckUtils_draw_horizontal_bar_no_update(baseSegmentOffset + 2, color); //top right segment
      break;
    case 2:
      pxlBlckUtils_draw_horizontal_bar_no_update(baseSegmentOffset, color); //bottom segment
      pxlBlckUtils_draw_horizontal_bar_no_update(baseSegmentOffset + 6, color); //bottom left segment
      pxlBlckUtils_draw_horizontal_bar_no_update(baseSegmentOffset + 5, color); //middle segment
      pxlBlckUtils_draw_horizontal_bar_no_update(baseSegmentOffset + 2, color); //top right segment
      pxlBlckUtils_draw_horizontal_bar_no_update(baseSegmentOffset + 3, color); //top segment
      break;
    case 3:
      pxlBlckUtils_draw_horizontal_bar_no_update(baseSegmentOffset, color); //bottom segment
      pxlBlckUtils_draw_horizontal_bar_no_update(baseSegmentOffset + 1, color); //bottom right segment
      pxlBlckUtils_draw_horizontal_bar_no_update(baseSegmentOffset + 2, color); //top right segment
      pxlBlckUtils_draw_horizontal_bar_no_update(baseSegmentOffset + 3, color); //top segment
      pxlBlckUtils_draw_horizontal_bar_no_update(baseSegmentOffset + 5, color); //middle segment
      break;
    case 4:
      pxlBlckUtils_draw_horizontal_bar_no_update(baseSegmentOffset + 1, color); //bottom right segment
      pxlBlckUtils_draw_horizontal_bar_no_update(baseSegmentOffset + 2, color); //top right segment
      pxlBlckUtils_draw_horizontal_bar_no_update(baseSegmentOffset + 4, color); //top left segment
      pxlBlckUtils_draw_horizontal_bar_no_update(baseSegmentOffset + 5, color); //middle segment
      break;
    case 5:
      pxlBlckUtils_draw_horizontal_bar_no_update(baseSegmentOffset, color); //bottom segment
      pxlBlckUtils_draw_horizontal_bar_no_update(baseSegmentOffset + 1, color); //bottom right segment
      pxlBlckUtils_draw_horizontal_bar_no_update(baseSegmentOffset + 3, color); //top segment
      pxlBlckUtils_draw_horizontal_bar_no_update(baseSegmentOffset + 4, color); //top left segment
      pxlBlckUtils_draw_horizontal_bar_no_update(baseSegmentOffset + 5, color); //middle segment
      break;
    case 6:
      pxlBlckUtils_draw_horizontal_bar_no_update(baseSegmentOffset, color); //bottom segment
      pxlBlckUtils_draw_horizontal_bar_no_update(baseSegmentOffset + 1, color); //bottom right segment
      pxlBlckUtils_draw_horizontal_bar_no_update(baseSegmentOffset + 3, color); //top segment
      pxlBlckUtils_draw_horizontal_bar_no_update(baseSegmentOffset + 4, color); //top left segment
      pxlBlckUtils_draw_horizontal_bar_no_update(baseSegmentOffset + 5, color); //middle segment
      pxlBlckUtils_draw_horizontal_bar_no_update(baseSegmentOffset + 6, color); //bottom left segment
      break;
    case 7:
      pxlBlckUtils_draw_horizontal_bar_no_update(baseSegmentOffset + 1, color); //bottom right segment
      pxlBlckUtils_draw_horizontal_bar_no_update(baseSegmentOffset + 2, color); //top right segment
      pxlBlckUtils_draw_horizontal_bar_no_update(baseSegmentOffset + 3, color); //top segment
      break;
    case 8:
      pxlBlckUtils_draw_horizontal_bar_no_update(baseSegmentOffset, color); //bottom segment
      pxlBlckUtils_draw_horizontal_bar_no_update(baseSegmentOffset + 1, color); //bottom right segment
      pxlBlckUtils_draw_horizontal_bar_no_update(baseSegmentOffset + 2, color); //top right segment
      pxlBlckUtils_draw_horizontal_bar_no_update(baseSegmentOffset + 3, color); //top segment
      pxlBlckUtils_draw_horizontal_bar_no_update(baseSegmentOffset + 4, color); //top left segment
      pxlBlckUtils_draw_horizontal_bar_no_update(baseSegmentOffset + 5, color); //middle segment
      pxlBlckUtils_draw_horizontal_bar_no_update(baseSegmentOffset + 6, color); //bottom left segment
      break;
    case 9:
      pxlBlckUtils_draw_horizontal_bar_no_update(baseSegmentOffset, color); //bottom segment
      pxlBlckUtils_draw_horizontal_bar_no_update(baseSegmentOffset + 1, color); //bottom right segment
      pxlBlckUtils_draw_horizontal_bar_no_update(baseSegmentOffset + 2, color); //top right segment
      pxlBlckUtils_draw_horizontal_bar_no_update(baseSegmentOffset + 3, color); //top segment
      pxlBlckUtils_draw_horizontal_bar_no_update(baseSegmentOffset + 4, color); //top left segment
      pxlBlckUtils_draw_horizontal_bar_no_update(baseSegmentOffset + 5, color); //middle segment
      break;
  }
}

uint16_t Plugin_205_digitClock_grid_pixel_remap(uint16_t x, uint16_t y)
{
  /*The WS2812 LED "lineup" always starts ats the bottom segment and contunious the following way: bottom right, top right, top, top left, middle, bottom left. The next digits
    continue in the same pattern. The seperator-dots are wired between the second and the third digit.
    The virtual remap of these led strip should represent a 2x29pixel matrix. This means a matrix with the width two and the height 29. Each line corospends to one segment.
    Notes for developing of "algorithm":
    y=0;x=0 : 0=y*2+x
    y=0;x=1 : 1=y*2+x
    y=1;x=0 : 2=y*2+x
    y=1;x=1 : 3=y*2+x
    y=2;x=0 : 4=y*2+x
    y=2;x=1 : 5=y*2+x
  */

  return y * 2 + x;
}

// == pxlBlckdigiClock dial functions == end ===========================================================================================================================



void pxlBlckUtils_check_fireSimulation()
{
  if (Plugin_205_selectedDial == PXLBLCK_DIAL_NAME_CAMP_FIRE_ID_INT)
  {
    pxlBlckUtils_clear_matrix();
    pxlBlckUtils_update_matrix();
  }
}
#endif

//== Variables for fakeTV-data == Start ============================

const uint8_t PROGMEM pxlBlckUtils_fakeTVcolors[] = {
  0X8C, 0XD8, 0X8C, 0XF9, 0X8C, 0XD8, 0X84, 0X98, 0X7C, 0X77, 0X64, 0X16,
  0X43, 0X94, 0X4B, 0XB4, 0X43, 0X32, 0X42, 0XF1, 0X53, 0X51, 0X5B, 0X70,
  0X94, 0XF7, 0X8C, 0X94, 0X7C, 0X10, 0X84, 0X31, 0X8C, 0X72, 0X8C, 0X72,
  0X94, 0X92, 0X5A, 0XEC, 0X3A, 0X29, 0X42, 0X4A, 0X42, 0XCD, 0X5B, 0X91,
  0X74, 0X55, 0X74, 0X95, 0X7C, 0XD5, 0X7C, 0XD5, 0X74, 0X75, 0X6C, 0X34,
  0X6C, 0X34, 0X6C, 0X34, 0X7C, 0X53, 0X8C, 0X52, 0X7C, 0X33, 0X10, 0XC3,
  0X63, 0X50, 0X5B, 0X4F, 0X53, 0X2E, 0X42, 0X6A, 0X08, 0X84, 0X19, 0X8B,
  0X19, 0X8B, 0X21, 0X8B, 0X3A, 0X6E, 0X64, 0X12, 0X53, 0X91, 0X2A, 0X90,
  0X53, 0X70, 0X7C, 0X30, 0X74, 0X0F, 0X74, 0X30, 0X7B, 0XCD, 0X83, 0X49,
  0X83, 0X49, 0X83, 0XAC, 0X7C, 0X50, 0X64, 0X12, 0X63, 0XF2, 0X6C, 0X12,
  0X64, 0X12, 0X63, 0XD2, 0X7B, 0XCF, 0XCC, 0X86, 0X8C, 0X50, 0X94, 0X91,
  0X94, 0X92, 0X94, 0XB2, 0X8C, 0XF3, 0X74, 0X73, 0X83, 0XED, 0XAC, 0X2A,
  0X74, 0X31, 0X84, 0X50, 0X84, 0X91, 0X84, 0X71, 0X84, 0X70, 0X84, 0X70,
  0X84, 0X70, 0X7C, 0X30, 0X7C, 0X51, 0X84, 0X51, 0X94, 0X90, 0X9C, 0XF0,
  0X9C, 0XF0, 0X9C, 0XF0, 0X94, 0XF1, 0X94, 0XF1, 0X84, 0XB1, 0X84, 0XD1,
  0X84, 0XB2, 0X85, 0X13, 0X8C, 0XF2, 0X8D, 0X33, 0X95, 0X33, 0X95, 0X12,
  0X94, 0XF1, 0X8C, 0XF1, 0X94, 0XF1, 0XAC, 0XEE, 0X9D, 0X10, 0XA5, 0X2F,
  0X94, 0XAF, 0X94, 0XEF, 0X9C, 0XCE, 0X9C, 0X8E, 0X8C, 0X0C, 0X94, 0X4C,
  0X94, 0X2C, 0X94, 0X4C, 0X94, 0X2C, 0XA4, 0X0B, 0X93, 0XAA, 0X8B, 0XCA,
  0X7C, 0X52, 0X74, 0X35, 0X42, 0X8D, 0X83, 0XCB, 0X9C, 0X4C, 0X5A, 0XAB,
  0X42, 0X6C, 0X63, 0X8F, 0X63, 0X8F, 0X73, 0XF1, 0X6B, 0X6E, 0X4A, 0XCC,
  0X42, 0X8B, 0X52, 0XCC, 0X5A, 0XCC, 0X5A, 0XCC, 0X62, 0XCC, 0X5A, 0XCC,
  0X63, 0X6C, 0X6B, 0X8C, 0X6B, 0X8C, 0X6B, 0XCF, 0X74, 0X52, 0X6C, 0X11,
  0X74, 0X91, 0X63, 0XF0, 0X52, 0XEC, 0X52, 0XEC, 0X4A, 0XCC, 0X52, 0XEC,
  0X53, 0X0D, 0X5B, 0XCF, 0X5B, 0XCF, 0X5B, 0X8E, 0X5B, 0XAE, 0X63, 0XAE,
  0X63, 0XCD, 0X4B, 0X0C, 0X3A, 0X4C, 0X32, 0X0C, 0X3A, 0XAE, 0X32, 0X6D,
  0X3A, 0X8D, 0X3A, 0X6E, 0X29, 0XCB, 0X29, 0XCB, 0X31, 0XEC, 0X31, 0XEB,
  0X29, 0XEC, 0X3A, 0X8B, 0X42, 0XAC, 0X3A, 0XAC, 0X3A, 0XAC, 0X4A, 0XCC,
  0X42, 0XAA, 0X42, 0XAA, 0X4A, 0XAA, 0X4A, 0XAA, 0X3A, 0X6B, 0X32, 0X0D,
  0X32, 0X0D, 0X32, 0X0C, 0X52, 0XEC, 0X63, 0X2C, 0X5B, 0X2E, 0X63, 0X70,
  0X63, 0XB1, 0X63, 0XD3, 0X64, 0X14, 0X6C, 0X14, 0X74, 0X36, 0X6B, 0XF3,
  0X5B, 0X0B, 0X63, 0X6E, 0X84, 0X93, 0X7C, 0X52, 0X7C, 0X52, 0X84, 0X31,
  0X84, 0X53, 0X8C, 0X93, 0X62, 0XEE, 0X5A, 0XEE, 0X63, 0X6F, 0X5B, 0X0E,
  0X42, 0X4B, 0X53, 0X2E, 0X5B, 0X90, 0X6C, 0X12, 0X52, 0XCD, 0X42, 0X2B,
  0X63, 0X4F, 0X63, 0X6F, 0X63, 0X6F, 0X52, 0XAC, 0X6B, 0XD1, 0X7C, 0X94,
  0X7C, 0X73, 0X6B, 0XB0, 0X6B, 0X90, 0X73, 0XF1, 0X6B, 0XAF, 0X73, 0XF0,
  0X4A, 0XAC, 0X29, 0X88, 0X63, 0X0D, 0X6B, 0X6E, 0X6B, 0X4D, 0X6B, 0X2D,
  0X6B, 0X2D, 0X6B, 0X4E, 0X73, 0X6F, 0X83, 0XF1, 0X83, 0XD1, 0X83, 0XD1,
  0X83, 0XD1, 0X83, 0XD1, 0X83, 0XF1, 0X83, 0XF1, 0X83, 0XD1, 0X8C, 0X53,
  0X94, 0X73, 0X8C, 0X32, 0X8C, 0X32, 0X6B, 0X6E, 0X6B, 0X6D, 0X6B, 0X6D,
  0X6B, 0X6D, 0X6B, 0X6D, 0X6B, 0X6D, 0X6B, 0X6D, 0X6B, 0X6D, 0X6B, 0X6F,
  0X6B, 0X6F, 0X73, 0XB0, 0X73, 0XB0, 0X73, 0XB1, 0X6B, 0X4F, 0X73, 0XB0,
  0X7B, 0XF0, 0X7B, 0XF0, 0X7B, 0XF0, 0X7B, 0XD0, 0X83, 0XCF, 0X84, 0X11,
  0X8C, 0X93, 0X8C, 0X93, 0X8C, 0X93, 0X8C, 0X93, 0X95, 0X15, 0X94, 0XF4,
  0X84, 0X72, 0X84, 0X52, 0X84, 0X52, 0X7B, 0XAF, 0X73, 0X6D, 0X73, 0XAF,
  0X7C, 0X10, 0X94, 0XF5, 0X94, 0XF5, 0X94, 0XF5, 0X7B, 0XF0, 0X73, 0X6D,
  0X73, 0X6D, 0X7B, 0XAF, 0X73, 0XAE, 0X73, 0X6D, 0X73, 0XAF, 0X73, 0X8F,
  0X73, 0X6D, 0X73, 0X6D, 0X6B, 0X4C, 0X62, 0XEB, 0X62, 0XCA, 0X6A, 0XCA,
  0X62, 0X89, 0X84, 0X10, 0X94, 0XD3, 0X94, 0X92, 0X9C, 0XB2, 0X9C, 0XF4,
  0X9D, 0X15, 0X9C, 0XF4, 0XA5, 0X15, 0X5B, 0X2E, 0X32, 0X2B, 0X39, 0XE9,
  0X42, 0X2A, 0X5B, 0X90, 0X63, 0XB1, 0X53, 0X0E, 0X39, 0XC9, 0X4A, 0X8B,
  0X63, 0X6E, 0X63, 0X4E, 0X4A, 0X6B, 0X5A, 0XED, 0X4A, 0X8C, 0X3A, 0X0A,
  0X3A, 0X0A, 0X3A, 0X2B, 0X5B, 0X0D, 0X4A, 0XAD, 0X42, 0X4C, 0X42, 0X4B,
  0X42, 0X8C, 0X52, 0XAD, 0X52, 0XCD, 0X52, 0XED, 0X52, 0XEE, 0X4A, 0X8B,
  0X4A, 0X6B, 0X32, 0X09, 0X2A, 0X09, 0X29, 0XE9, 0X32, 0X2B, 0X08, 0XC5,
  0X5A, 0XEA, 0X94, 0XD1, 0X7B, 0X8C, 0X6A, 0XA9, 0X6A, 0X8B, 0X6A, 0X6B,
  0X62, 0XAB, 0X6A, 0XAB, 0X5A, 0X8A, 0X62, 0XAB, 0X73, 0X2B, 0X73, 0X0A,
  0X73, 0X6E, 0X7C, 0X13, 0X6B, 0X0D, 0X73, 0X0B, 0X7B, 0X2B, 0X84, 0X11,
  0X8C, 0X30, 0X83, 0XEF, 0X73, 0X8E, 0X73, 0XAE, 0X63, 0X4C, 0X5B, 0X0B,
  0X73, 0X8E, 0X7B, 0XCF, 0X7B, 0XCF, 0X7B, 0XEF, 0X7B, 0XAE, 0X7B, 0XEE,
  0X7C, 0X30, 0X7C, 0X0F, 0X8C, 0X0E, 0X94, 0X4F, 0X8C, 0X50, 0X8C, 0X70,
  0X8C, 0X50, 0X84, 0X4F, 0X8C, 0X70, 0X84, 0X91, 0X73, 0XAD, 0X84, 0X70,
  0X84, 0X0E, 0X83, 0XCD, 0X8B, 0XEE, 0X84, 0X2F, 0X7C, 0X4F, 0X6B, 0XEF,
  0X84, 0X2F, 0X94, 0X8F, 0X8B, 0XCA, 0X8B, 0XEB, 0X84, 0X0E, 0X84, 0X2F,
  0X83, 0XED, 0X7B, 0XEF, 0X7B, 0XEF, 0X83, 0XEF, 0X83, 0XEF, 0X84, 0X0F,
  0X84, 0X0F, 0X84, 0X10, 0X84, 0X10, 0X84, 0X30, 0X8C, 0X30, 0X84, 0X30,
  0X94, 0X4E, 0X9C, 0X2C, 0X84, 0XD4, 0X7C, 0X30, 0X73, 0XAE, 0X73, 0XF0,
  0X73, 0X8D, 0X7B, 0XEE, 0X7B, 0XCE, 0X7B, 0XEF, 0X8C, 0XF4, 0X8C, 0X70,
  0X73, 0XCE, 0X73, 0XF0, 0X84, 0X0F, 0X8B, 0XEE, 0X93, 0XCD, 0X93, 0XED,
  0X8C, 0XD4, 0X84, 0XB4, 0X7C, 0X52, 0X7C, 0X73, 0X73, 0XF1, 0X7C, 0X10,
  0X84, 0X73, 0X8C, 0XF4, 0X63, 0X4E, 0X29, 0X66, 0X39, 0XE8, 0X31, 0XA7,
  0X4A, 0X6B, 0X4A, 0X8B, 0X31, 0X88, 0X31, 0XA8, 0X31, 0XA9, 0X31, 0XCB,
  0X31, 0XCB, 0X52, 0X6C, 0X52, 0XAD, 0X6B, 0X0C, 0X4A, 0X6C, 0X3A, 0XB0,
  0X6B, 0X31, 0X62, 0X6D, 0X4B, 0X32, 0X53, 0X32, 0X4B, 0X33, 0X53, 0X32,
  0X73, 0X51, 0X63, 0X52, 0X4B, 0X54, 0X4B, 0X53, 0X4B, 0X54, 0X4B, 0X54,
  0X4B, 0X54, 0X4B, 0X54, 0X6B, 0X12, 0X9A, 0XD0, 0X83, 0XB3, 0X7B, 0X32,
  0X72, 0XB1, 0X8A, 0X8F, 0X62, 0XD0, 0X52, 0X90, 0X52, 0XB0, 0X52, 0XB0,
  0X32, 0X51, 0X08, 0X83, 0X00, 0X20, 0X21, 0X68, 0X3A, 0XF3, 0X3B, 0X33,
  0X33, 0X33, 0X3B, 0X53, 0X3B, 0X53, 0X3B, 0X33, 0X33, 0X13, 0X2A, 0XF3,
  0X33, 0X13, 0X32, 0XF3, 0X33, 0X13, 0X2A, 0XF3, 0X22, 0X93, 0X22, 0X73,
  0X1A, 0X53, 0X1A, 0X53, 0X22, 0X53, 0X39, 0XF0, 0X59, 0X29, 0X59, 0X29,
  0X59, 0X29, 0X59, 0X29, 0X59, 0X29, 0X59, 0X29, 0X59, 0X29, 0X61, 0X29,
  0X61, 0X29, 0X59, 0X08, 0X61, 0X09, 0X61, 0X8B, 0X51, 0X8A, 0X51, 0X69,
  0X49, 0X49, 0X59, 0X8B, 0X61, 0XCD, 0X59, 0X8C, 0X41, 0X2A, 0X38, 0XE8,
  0X30, 0XC8, 0X38, 0XC9, 0X40, 0XC9, 0X38, 0XC8, 0X48, 0XE9, 0X59, 0X4A,
  0X69, 0XAD, 0X82, 0X0E, 0X8A, 0X4F, 0X81, 0XED, 0X71, 0X6B, 0X39, 0X07,
  0X20, 0XE6, 0X29, 0X06, 0X29, 0X27, 0X20, 0XA5, 0X28, 0XC6, 0X31, 0X08,
  0X39, 0X08, 0X39, 0X07, 0X28, 0XC5, 0X30, 0XA6, 0X30, 0XA6, 0X30, 0XA6,
  0X38, 0XC6, 0X61, 0XEB, 0X82, 0X8E, 0X72, 0X2C, 0X72, 0X2B, 0X62, 0X2A,
  0X51, 0XC9, 0X49, 0X8A, 0X41, 0X68, 0X59, 0XA9, 0X79, 0XEA, 0X81, 0XE9,
  0X81, 0XE9, 0X89, 0XC7, 0X89, 0XE7, 0X89, 0XE7, 0X81, 0XC7, 0X81, 0XC7,
  0X81, 0XC7, 0X71, 0X86, 0X69, 0X66, 0X89, 0X47, 0X89, 0X67, 0X81, 0X47,
  0X70, 0XE6, 0X58, 0XE6, 0X40, 0XE6, 0X38, 0XC7, 0X50, 0XC8, 0X68, 0XC9,
  0X68, 0XC9, 0X68, 0X88, 0X68, 0XA7, 0X68, 0XE7, 0X60, 0XC7, 0X60, 0XC7,
  0X58, 0XC7, 0X50, 0XC7, 0X50, 0XA7, 0X50, 0XC7, 0X58, 0XC8, 0X60, 0XC9,
  0X60, 0XA9, 0X30, 0X44, 0X00, 0X00, 0X08, 0X01, 0X20, 0X85, 0X42, 0XB3,
  0X42, 0XD5, 0X5A, 0X8F, 0X92, 0X2D, 0X7A, 0X4B, 0X79, 0X88, 0X82, 0X08,
  0X82, 0X27, 0X81, 0XC8, 0X8A, 0X4D, 0X7A, 0XAC, 0X82, 0XEE, 0X84, 0X12,
  0X83, 0X50, 0X79, 0XCB, 0X81, 0XEA, 0X89, 0XC8, 0X91, 0XC8, 0X89, 0XE9,
  0X7A, 0X4D, 0X82, 0X4C, 0X92, 0X29, 0X92, 0X29, 0X99, 0XEB, 0XA1, 0XCB,
  0XA1, 0X8B, 0X91, 0XCD, 0X91, 0XCC, 0X9A, 0X49, 0X9A, 0X0D, 0X9A, 0X0C,
  0X9A, 0X49, 0X9A, 0X4A, 0X9A, 0X6A, 0X9A, 0X2A, 0XAA, 0X2A, 0XB2, 0X2B,
  0X8A, 0X4D, 0X62, 0X4A, 0X62, 0X4B, 0X41, 0X46, 0X28, 0XE3, 0X10, 0XA2,
  0X39, 0X44, 0X31, 0X45, 0X18, 0XC2, 0X18, 0XC2, 0X18, 0XC2, 0X18, 0XC3,
  0X10, 0XC3, 0X18, 0XC4, 0X29, 0X09, 0X10, 0XA8, 0X18, 0XC7, 0X31, 0X06,
  0X41, 0X67, 0X20, 0XE4, 0X10, 0XA3, 0X28, 0XE5, 0X18, 0XE5, 0X21, 0X06,
  0X31, 0X47, 0X20, 0XE4, 0X20, 0XC5, 0X31, 0X47, 0X18, 0XC6, 0X10, 0XA5,
  0X5A, 0X4B, 0X83, 0XB1, 0X73, 0X70, 0X7B, 0X0D, 0X72, 0X8B, 0X7A, 0XAB,
  0X83, 0X0C, 0X8A, 0XEC, 0X82, 0XCB, 0X82, 0XCC, 0X82, 0X69, 0X82, 0X49,
  0X8A, 0X6A, 0X82, 0X6A, 0X82, 0X8B, 0X6A, 0XCE, 0X83, 0X4E, 0X8B, 0X8F,
  0X7B, 0X2D, 0X73, 0X0C, 0X73, 0X0C, 0X83, 0X2C, 0X93, 0X4D, 0X82, 0XEB,
  0X7A, 0XEA, 0X8B, 0X0B, 0X6A, 0XCB, 0X7A, 0XCB, 0X83, 0X0B, 0X92, 0X69,
  0X9A, 0X27, 0X82, 0XAA, 0X82, 0XAA, 0X7A, 0XCC, 0X7B, 0X0D, 0X6A, 0X2A,
  0X72, 0X09, 0X82, 0X8C, 0X83, 0X0D, 0X7B, 0X0E, 0X7B, 0X0E, 0X7A, 0XEC,
  0X72, 0XCC, 0X6A, 0XCC, 0X72, 0XCD, 0X6A, 0XCD, 0X6A, 0XAB, 0X6A, 0X8B,
  0X62, 0X8B, 0X62, 0X8B, 0X6A, 0X8A, 0X6A, 0X8A, 0X6A, 0X8A, 0X62, 0X6A,
  0X5A, 0X6A, 0X5A, 0X4A, 0X62, 0X4A, 0X5A, 0X4A, 0X5A, 0X49, 0X5A, 0X6A,
  0X62, 0X8A, 0X62, 0X8A, 0X62, 0X69, 0X62, 0X6A, 0X62, 0X6A, 0X62, 0X69,
  0X62, 0X48, 0X62, 0X28, 0X6A, 0X69, 0X6A, 0XAB, 0X62, 0X6B, 0X5A, 0X4A,
  0X62, 0X4B, 0X62, 0X69, 0X62, 0X6A, 0X62, 0X4A, 0X6A, 0X6A, 0X72, 0X6A,
  0X6A, 0X89, 0X62, 0X49, 0X5A, 0X48, 0X5A, 0X29, 0X52, 0X08, 0X5A, 0X49,
  0X62, 0X8B, 0X5A, 0X6B, 0X5A, 0X8B, 0X62, 0X6A, 0X62, 0X6A, 0X5A, 0X2A,
  0X5A, 0X2A, 0X5A, 0X4A, 0X5A, 0X6A, 0X52, 0X29, 0X5A, 0X29, 0X62, 0X4A,
  0X62, 0X6B, 0X62, 0XAC, 0X6A, 0XAC, 0X6A, 0XCC, 0X6A, 0X6A, 0X62, 0X6A,
  0X6A, 0X8B, 0X72, 0X8A, 0X7A, 0XAA, 0X72, 0XAA, 0X7A, 0XCB, 0X72, 0XA8,
  0X72, 0X67, 0X72, 0X67, 0X72, 0X89, 0X7A, 0XAA, 0X7A, 0XA9, 0X72, 0XA9,
  0X72, 0XCA, 0X72, 0XAA, 0X7A, 0XA9, 0X8B, 0X2A, 0X6A, 0XAA, 0X5A, 0X2A,
  0X62, 0X6A, 0X7A, 0XC9, 0X72, 0XC9, 0X82, 0XEA, 0X82, 0XC9, 0X82, 0XA9,
  0X82, 0XA9, 0X72, 0XAB, 0X72, 0XAB, 0X7A, 0XAB, 0X72, 0XAB, 0X6A, 0XAA,
  0X9A, 0XED, 0XA3, 0X2E, 0X69, 0XE8, 0X49, 0X65, 0X51, 0XC7, 0X6A, 0X8A,
  0X6A, 0X8A, 0X6A, 0X8A, 0X6A, 0X8A, 0X6A, 0X8A, 0X6A, 0X8A, 0X6A, 0XAA,
  0X6A, 0X8B, 0X72, 0X8A, 0X72, 0X6B, 0X72, 0XAA, 0X72, 0XAB, 0X6A, 0X8B,
  0X6A, 0XAB, 0X72, 0XAC, 0X72, 0XAC, 0X72, 0XCC, 0X6A, 0XAA, 0X6A, 0X47,
  0X62, 0X27, 0X62, 0X28, 0X6A, 0X68, 0X6A, 0X6A, 0X6A, 0X6C, 0X6A, 0XAD,
  0X72, 0X8C, 0X72, 0XAE, 0X62, 0X4C, 0X6A, 0XAC, 0X72, 0XEE, 0X62, 0X6B,
  0X62, 0X2A, 0X79, 0XAA, 0X71, 0X8C, 0X29, 0XAF, 0X11, 0X6E, 0X4A, 0X6A,
  0X6A, 0XA8, 0X51, 0XC8, 0X51, 0X26, 0X31, 0X25, 0X19, 0X25, 0X19, 0X26,
  0X21, 0X66, 0X10, 0X63, 0X18, 0X84, 0X20, 0X80, 0X28, 0XE0, 0X31, 0XA1,
  0X29, 0X81, 0X48, 0X81, 0X58, 0X00, 0X58, 0X00, 0X60, 0X00, 0X58, 0X00,
  0X50, 0XA3, 0X58, 0XC3, 0X68, 0X00, 0X68, 0X00, 0X70, 0XE3, 0X69, 0X86,
  0X68, 0XA3, 0X61, 0XA7, 0X48, 0X82, 0X50, 0X81, 0X51, 0X05, 0X62, 0X28,
  0X49, 0XA8, 0X39, 0X86, 0X51, 0XC8, 0X82, 0X2C, 0X82, 0X0A, 0X69, 0XC9,
  0X61, 0XA9, 0X59, 0XE7, 0X62, 0X29, 0X62, 0X49, 0X49, 0XA7, 0X51, 0XC8,
  0X51, 0XC7, 0X49, 0XA6, 0X5A, 0X8A, 0X5A, 0X6A, 0X4A, 0X2C, 0X42, 0X4F,
  0X42, 0X70, 0X4A, 0X91, 0X5A, 0XB0, 0X5A, 0X8F, 0X52, 0X50, 0X5A, 0XAF,
  0X62, 0XCF, 0X49, 0XEF, 0X41, 0XCE, 0X49, 0XEF, 0X52, 0X0F, 0X52, 0X10,
  0X52, 0X2F, 0X52, 0X6D, 0X5A, 0X6D, 0X5A, 0X2D, 0X4A, 0X0C, 0X52, 0X2C,
  0X49, 0XCF, 0X49, 0XB0, 0X49, 0XAF, 0X3A, 0X4D, 0X32, 0XAC, 0X32, 0X8D,
  0X32, 0XAD, 0X3A, 0XCC, 0X32, 0X2A, 0X2A, 0X2A, 0X32, 0X4C, 0X42, 0X2E,
  0X52, 0X50, 0X62, 0XF0, 0X62, 0XCF, 0X4A, 0X4E, 0X4A, 0X2F, 0X4A, 0X2F,
  0X4A, 0X2F, 0X41, 0XEF, 0X39, 0XCF, 0X5A, 0X8F, 0X52, 0X8F, 0X7A, 0X64,
  0X92, 0XA0, 0X8A, 0X81, 0X8A, 0X80, 0X92, 0X80, 0X8A, 0X80, 0X9A, 0XC1,
  0XAB, 0X04, 0X41, 0XCE, 0X4A, 0X2C, 0X52, 0X29, 0X4A, 0X0A, 0X4A, 0X2C,
  0X41, 0XC9, 0X41, 0XE2, 0X4A, 0X02, 0X4A, 0X22, 0X29, 0X22, 0X18, 0XC1,
  0X31, 0X82, 0X39, 0XC2, 0X42, 0X02, 0X41, 0XE2, 0X42, 0X02, 0X41, 0XE2,
  0X42, 0X02, 0X31, 0X62, 0X21, 0X02, 0X18, 0XC1, 0X21, 0X02, 0X39, 0XA4,
  0X31, 0X87, 0X31, 0XCC, 0X29, 0X87, 0X18, 0XE2, 0X18, 0XC2, 0X18, 0XE2,
  0X18, 0XC2, 0X08, 0X61, 0X31, 0X47, 0X41, 0XCA, 0X21, 0X04, 0X10, 0X81,
  0X18, 0XE2, 0X18, 0XC1, 0X28, 0XE7, 0X39, 0X6E, 0X39, 0X69, 0X39, 0X8A,
  0X31, 0X4C, 0X5A, 0X6B, 0X52, 0X0B, 0X41, 0X6B, 0X49, 0XCC, 0X52, 0X2E,
  0X52, 0X0E, 0X4A, 0X0D, 0X4A, 0X0D, 0X52, 0X2D, 0X72, 0XEC, 0X6A, 0X28,
  0X83, 0X67, 0X6B, 0XE4, 0X4B, 0X6C, 0X43, 0X4E, 0X43, 0X4D, 0X43, 0X4D,
  0X4B, 0X4E, 0X6A, 0XAF, 0X6A, 0XAE, 0X6A, 0XB0, 0X6A, 0XF0, 0X79, 0XEC,
  0X79, 0X0A, 0X79, 0X0A, 0X89, 0XEB, 0X5B, 0XCF, 0X63, 0X86, 0X63, 0X24,
  0X63, 0X65, 0X82, 0X22, 0X6A, 0XAB, 0X43, 0XF2, 0X43, 0XB1, 0X4B, 0XD2,
  0X4B, 0XD2, 0X4B, 0XD2, 0X4B, 0XF0, 0X5A, 0X4A, 0X80, 0XE7, 0XA2, 0X45,
  0X93, 0X07, 0X8A, 0XEA, 0X41, 0X6E, 0X39, 0X8E, 0X8C, 0X71, 0X8C, 0X30,
  0X7B, 0X6C, 0X8B, 0XCD, 0X83, 0X8D, 0X9B, 0XEE, 0X94, 0X2F, 0X83, 0X4D,
  0X5C, 0XFA, 0X65, 0X1A, 0X64, 0XFA, 0X85, 0X12, 0X84, 0X8D, 0X7C, 0X4D,
  0X84, 0X2C, 0X8C, 0X8F, 0X8C, 0XF0, 0X8C, 0XD0, 0X94, 0X8F, 0X93, 0XAC,
  0X93, 0XAC, 0X94, 0X4E, 0X8C, 0XD0, 0X8C, 0XAF, 0X8C, 0XD0, 0X8C, 0XF0,
  0X8C, 0XF2, 0X84, 0XF7, 0X7C, 0XB4, 0X7C, 0X92, 0X7C, 0X92, 0X7C, 0X92,
  0X8B, 0XF1, 0X93, 0XB0, 0X93, 0XB0, 0X9C, 0X11, 0XA4, 0X52, 0XAC, 0X71,
  0XA4, 0X73, 0XA4, 0X32, 0X8C, 0X94, 0X8C, 0XB4, 0X8C, 0X94, 0X8C, 0X94,
  0X8C, 0X93, 0X8C, 0X93, 0X7C, 0X95, 0X7C, 0XB6, 0X74, 0XB7, 0X84, 0X51,
  0X84, 0X30, 0X8C, 0X10, 0X94, 0X50, 0X8C, 0X50, 0X8C, 0XCC, 0X84, 0XCA,
  0X7C, 0XCE, 0X74, 0XF9, 0X74, 0XD9, 0X6C, 0XB7, 0X74, 0XF7, 0X75, 0X7B,
  0X94, 0X30, 0X7C, 0XD2, 0X7C, 0XCD, 0X74, 0XD7, 0X6C, 0XD7, 0X74, 0XD2,
  0X7D, 0X56, 0X7D, 0X35, 0X84, 0XEA, 0X64, 0X94, 0X64, 0X96, 0X6C, 0XB6,
  0X6C, 0XD6, 0X6C, 0XB6, 0X6D, 0X39, 0X6C, 0XD7, 0X84, 0X93, 0XAC, 0X8D,
  0XAC, 0X6C, 0XB4, 0XAD, 0XB4, 0XEE, 0XAC, 0XEF, 0XAC, 0XAF, 0XA4, 0X2E,
  0XB4, 0X4B, 0XBC, 0XAA, 0XCD, 0X4B, 0XCD, 0X4B, 0XBD, 0X2D, 0XB5, 0X0E,
  0XD3, 0XE9, 0XB4, 0X6A, 0XA5, 0X4D, 0XBD, 0X0D, 0XBC, 0XCC, 0XBC, 0XCD,
  0XB4, 0X8B, 0X9D, 0X4F, 0X8D, 0X0F, 0X95, 0X0F, 0X8D, 0X2F, 0XA5, 0X4B,
  0XC5, 0X46, 0XBD, 0X07, 0XBC, 0XE7, 0XBD, 0X07, 0XAC, 0XE8, 0XBC, 0X2C,
  0XAB, 0XED, 0XAC, 0XCF, 0X9D, 0X72, 0X9D, 0X31, 0X9C, 0XAF, 0X94, 0XD0,
  0X75, 0X14, 0X7C, 0X90, 0XA3, 0XE6, 0XB4, 0X65, 0XC4, 0X83, 0XC4, 0XA4,
  0XA3, 0XA8, 0X52, 0X8E, 0X10, 0X63, 0X00, 0X00, 0X00, 0X00, 0X08, 0X85,
  0X29, 0X4A, 0X29, 0X4A, 0X29, 0X6A, 0X29, 0X4A, 0X31, 0X2A, 0X41, 0X2A,
  0X41, 0X4A, 0X39, 0X09, 0X08, 0X22, 0X00, 0X00, 0X00, 0X00, 0X00, 0X20,
  0X08, 0X62, 0X08, 0X62, 0X08, 0X62, 0X08, 0X62, 0X08, 0X82, 0X10, 0XA3,
  0X10, 0XA4, 0X10, 0XC4, 0X10, 0XE5, 0X19, 0X47, 0X11, 0X27, 0X19, 0X68,
  0X2A, 0X0A, 0X3A, 0XCD, 0X53, 0X90, 0X6C, 0X52, 0X74, 0X93, 0X84, 0XF3,
  0X8D, 0X12, 0X7C, 0XB0, 0X7C, 0X4F, 0X7C, 0X6F, 0X74, 0X6E, 0X63, 0XED,
  0X6B, 0XAD, 0X6B, 0X8D, 0X6B, 0X8E, 0X73, 0XCF, 0X7C, 0X30, 0X7C, 0X72,
  0X6C, 0X33, 0X5B, 0XD3, 0X4B, 0XD5, 0X4C, 0X18, 0X3B, 0XD7, 0X33, 0X76,
  0X2B, 0X15, 0X2A, 0XD3, 0X2A, 0XD3, 0X3A, 0XF4, 0X3A, 0XF4, 0X2A, 0X73,
  0X22, 0X12, 0X1A, 0X13, 0X19, 0XF5, 0X19, 0XF6, 0X19, 0XD5, 0X19, 0XB3,
  0X19, 0X71, 0X19, 0X91, 0X19, 0X2F, 0X00, 0X23, 0X00, 0X21, 0X09, 0X08,
  0X09, 0X48, 0X11, 0X67, 0X09, 0X48, 0X08, 0XEA, 0X08, 0XAD, 0X19, 0X10,
  0X29, 0XF2, 0X29, 0XF2, 0X21, 0XD1, 0X21, 0XD1, 0X21, 0XB1, 0X21, 0XD1,
  0X31, 0XD1, 0X49, 0X8F, 0X69, 0X8E, 0X81, 0XAD, 0X91, 0XED, 0X9A, 0X0D,
  0X91, 0XEC, 0X89, 0XCC, 0X8A, 0X0C, 0X99, 0XCE, 0XB1, 0XF4, 0XBA, 0X77,
  0XA2, 0X35, 0X81, 0XF2, 0X69, 0XD0, 0X51, 0XAE, 0X41, 0X8E, 0X29, 0X6C,
  0X19, 0X4B, 0X11, 0X6B, 0X11, 0X4B, 0X11, 0X2B, 0X09, 0X0B, 0X08, 0XC9,
  0X08, 0X89, 0X00, 0X24, 0X00, 0X01, 0X00, 0X01, 0X00, 0X01, 0X00, 0X01,
  0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X10, 0X20,
  0X69, 0X24, 0X79, 0X45, 0X79, 0X44, 0X81, 0X63, 0X81, 0X83, 0X81, 0XA3,
  0X81, 0X83, 0X89, 0XA5, 0X92, 0X07, 0X69, 0X65, 0X08, 0X20, 0X00, 0X00,
  0X4A, 0X27, 0X62, 0XEA, 0X62, 0XEA, 0X6B, 0X0B, 0X6A, 0XEA, 0X73, 0X4C,
  0X73, 0X2C, 0X73, 0X2B, 0X6A, 0XCA, 0X52, 0X27, 0X52, 0X48, 0X6B, 0X4B,
  0X73, 0XCC, 0X6B, 0X4A, 0X6B, 0X49, 0X6B, 0X0A, 0X62, 0XEC, 0X62, 0XEC,
  0X62, 0XEC, 0X62, 0XEC, 0X5A, 0XCC, 0X5A, 0XAB, 0X5A, 0XCB, 0X5A, 0XCC,
  0X62, 0X8C, 0X62, 0XAC, 0X5A, 0XAB, 0X5A, 0X49, 0X73, 0X0C, 0X9C, 0X30,
  0X7B, 0X6D, 0X5A, 0X69, 0X5A, 0X6A, 0X7B, 0X4D, 0XA4, 0X6F, 0X8C, 0X30,
  0X7B, 0XEC, 0X6B, 0X29, 0X5A, 0X89, 0X62, 0XA9, 0X52, 0X68, 0X6B, 0X0A,
  0X94, 0X0F, 0X8B, 0XEE, 0X93, 0XEE, 0X94, 0X2F, 0X7B, 0X0A, 0X7B, 0XCC,
  0X83, 0XCA, 0X73, 0X0B, 0X73, 0X0B, 0X62, 0XEB, 0X5A, 0XA9, 0X94, 0X0F,
  0X94, 0X2F, 0X8B, 0XCE, 0X8B, 0XAD, 0X8C, 0X10, 0X8C, 0X72, 0X8C, 0X72,
  0X8C, 0X72, 0X8C, 0X93, 0X8C, 0X6F, 0X8C, 0X8F, 0X84, 0X50, 0X84, 0X4F,
  0X84, 0X4E, 0X7B, 0XEB, 0X7B, 0X8C, 0X62, 0XAA, 0X6A, 0XA9, 0X7B, 0X6C,
  0X83, 0X6B, 0X93, 0XAA, 0X7B, 0X2A, 0X5A, 0X68, 0X73, 0X4C, 0X8C, 0XB3,
  0X84, 0X0F, 0X84, 0X0F, 0X83, 0XCF, 0X7B, 0X4A, 0X7B, 0X6A, 0X73, 0X8A,
  0X73, 0XCD, 0X7B, 0XEE, 0X7B, 0XCC, 0X9C, 0X4E, 0X9B, 0XCB, 0X83, 0XAC,
  0X83, 0XAC, 0X93, 0X08, 0X93, 0X09, 0X93, 0X29, 0X93, 0X09, 0X72, 0XEA,
  0X6A, 0XEB, 0X6A, 0XEB, 0X73, 0X0B, 0X83, 0X09, 0X72, 0XAC, 0X7A, 0XCD,
  0X7A, 0XCC, 0X6A, 0X6D, 0X72, 0X8D, 0X72, 0X8C, 0X7A, 0XEE, 0X52, 0X29,
  0X31, 0X86, 0X31, 0X86, 0X29, 0X65, 0X31, 0X86, 0X39, 0XA7, 0X39, 0XA7,
  0X41, 0XE7, 0X4A, 0X07, 0X4A, 0X07, 0X41, 0XE7, 0X4A, 0X08, 0X41, 0XE7,
  0X41, 0XE8, 0X4A, 0X08, 0X41, 0XE7, 0X41, 0XE7, 0X31, 0XA7, 0X62, 0X49,
  0X5A, 0X49, 0X52, 0X6A, 0X8B, 0XAD, 0X83, 0XAC, 0X8B, 0XED, 0X94, 0X0E,
  0X9B, 0XCE, 0X94, 0X0E, 0X94, 0X0E, 0X9C, 0X2F, 0X94, 0X0F, 0X82, 0XE9,
  0X7A, 0X88, 0X83, 0X2B, 0X84, 0X30, 0X94, 0X0F, 0X94, 0X2F, 0X8C, 0X0F,
  0X94, 0X50, 0X8C, 0X10, 0X8B, 0XB0, 0X93, 0XD0, 0X93, 0XB1, 0X93, 0XF0,
  0X94, 0X0F, 0X8B, 0XEF, 0X83, 0X8D, 0X83, 0X8C, 0X83, 0X8C, 0X83, 0X8C,
  0X6B, 0X4C, 0X6B, 0X6D, 0X6B, 0X6D, 0X63, 0X4E, 0X6B, 0X4D, 0X63, 0X0C,
  0X73, 0X6D, 0X62, 0X8A, 0X4A, 0X28, 0X52, 0X48, 0X52, 0X28, 0X5A, 0X69,
  0X83, 0XAF, 0X6A, 0XAA, 0X7B, 0X6D, 0X83, 0XCE, 0X83, 0XAE, 0X7B, 0XAF,
  0X83, 0XEE, 0X83, 0XEF, 0X83, 0XEE, 0X8B, 0XEE, 0X8C, 0X51, 0X8C, 0X0F,
  0X8B, 0X4C, 0X6B, 0X4C, 0X6B, 0X4D, 0X63, 0X0B, 0X5A, 0XA7, 0X73, 0X2D,
  0X93, 0XF2, 0X73, 0X4E, 0X4A, 0X09, 0X83, 0X71, 0X83, 0X8F, 0X7B, 0X8F,
  0X7B, 0X8F, 0X7B, 0X8F, 0X7B, 0XAE, 0X62, 0XCB, 0X62, 0X6A, 0X83, 0X8D,
  0X8C, 0X30, 0X83, 0XEE, 0X83, 0XCE, 0X8C, 0X0F, 0X83, 0XCE, 0X84, 0X2F,
  0X73, 0X6D, 0X72, 0XCA, 0X72, 0XCB, 0X6A, 0XEB, 0X6B, 0X4B, 0X6B, 0X6C,
  0X63, 0X2B, 0X52, 0XAA, 0X52, 0X89, 0X42, 0X28, 0X3A, 0X07, 0X3A, 0X07,
  0X4A, 0X68, 0X52, 0XA9, 0X52, 0X89, 0X42, 0X27, 0X39, 0XE6, 0X39, 0XC6,
  0X42, 0X07, 0X4A, 0X48, 0X31, 0XC7, 0X29, 0XC8, 0X32, 0X2B, 0X32, 0X2B,
  0X2A, 0X0A, 0X32, 0X0A, 0X3A, 0X4A, 0X3A, 0X4B, 0X3A, 0X4B, 0X3A, 0X2A,
  0X3A, 0X2A, 0X3A, 0X4A, 0X32, 0X4A, 0X32, 0X4A, 0X32, 0X6B, 0X3A, 0X8B,
  0X32, 0X4B, 0X21, 0XA8, 0X00, 0X63, 0X00, 0X84, 0X00, 0X00, 0X00, 0X00,
  0X00, 0X00, 0X39, 0X86, 0X5A, 0X69, 0X5A, 0X6A, 0X5A, 0X8C, 0X52, 0X8C,
  0X52, 0X8D, 0X52, 0XAD, 0X52, 0X8C, 0X52, 0XAD, 0X32, 0X0A, 0X29, 0XE9,
  0X10, 0XC4, 0X00, 0X00, 0X20, 0X63, 0X81, 0XEF, 0X81, 0XEF, 0X81, 0XEE,
  0X81, 0XCE, 0X69, 0X6B, 0X59, 0X4A, 0X61, 0X6B, 0X69, 0X8B, 0X71, 0X8B,
  0X71, 0XAB, 0X69, 0XAB, 0X61, 0XAB, 0X69, 0X6B, 0X71, 0X6C, 0X79, 0X8D,
  0X79, 0X8D, 0X71, 0X6D, 0X71, 0X6C, 0X71, 0X6C, 0X71, 0X6C, 0X71, 0X6C,
  0X71, 0X6C, 0X71, 0X6C, 0X71, 0X6C, 0X71, 0X4C, 0X71, 0X8D, 0X51, 0X6D,
  0X41, 0XCE, 0X41, 0XAE, 0X41, 0XAE, 0X41, 0XAD, 0X41, 0XAD, 0X41, 0XAD,
  0X41, 0X8D, 0X39, 0X6D, 0X39, 0X6C, 0X39, 0X6C, 0X39, 0X6C, 0X39, 0X6C,
  0X39, 0X6C, 0X39, 0X4C, 0X39, 0X4C, 0X39, 0X4C, 0X49, 0XAC, 0X6A, 0XEF,
  0X73, 0X2F, 0X4A, 0X8E, 0X22, 0X0B, 0X22, 0X0B, 0X22, 0X0B, 0X22, 0X0B,
  0X2A, 0X4D, 0X32, 0X6E, 0X32, 0X6E, 0X2A, 0X4E, 0X2A, 0X4E, 0X2A, 0X6E,
  0X2A, 0X4E, 0X32, 0X6F, 0X3A, 0X2D, 0X32, 0X0C, 0X32, 0X0C, 0X32, 0X0C,
  0X32, 0X0C, 0X32, 0X0C, 0X31, 0XEC, 0X31, 0XEC, 0X32, 0X0C, 0X42, 0X8E,
  0X3A, 0X8E, 0X3A, 0X6E, 0X3A, 0X6E, 0X3A, 0X6D, 0X42, 0X8E, 0X4A, 0X8E,
  0X4A, 0XAF, 0X42, 0X6E, 0X29, 0X8B, 0X21, 0X4B, 0X29, 0X6C, 0X29, 0X6C,
  0X29, 0X6C, 0X29, 0X6C, 0X29, 0X6C, 0X29, 0X6C, 0X29, 0X6C, 0X29, 0X6C,
  0X29, 0X6C, 0X29, 0X6C, 0X29, 0X6C, 0X29, 0X6C, 0X29, 0X6C, 0X21, 0X2B,
  0X21, 0X0C, 0X21, 0X4D, 0X29, 0X4D, 0X29, 0X4D, 0X21, 0X4D, 0X29, 0X4D,
  0X21, 0X4C, 0X21, 0X4C, 0X29, 0X4D, 0X29, 0X4D, 0X29, 0X2D, 0X29, 0X4D,
  0X29, 0X4D, 0X21, 0X2C, 0X21, 0X4C, 0X29, 0X4C, 0X29, 0X4B, 0X21, 0X2B,
  0X21, 0X6D, 0X21, 0X4D, 0X19, 0X0C, 0X10, 0XCA, 0X10, 0XA9, 0X08, 0X88,
  0X00, 0X66, 0X00, 0X01, 0X00, 0X00, 0X00, 0X00, 0X00, 0X41, 0X08, 0X62,
  0X10, 0XA3, 0X4A, 0X29, 0X52, 0X8A, 0X52, 0X6A, 0X52, 0X6A, 0X52, 0X6A,
  0X5A, 0X8A, 0X18, 0XA2, 0X30, 0XE5, 0XAA, 0XCB, 0XCA, 0XC5, 0XD3, 0X64,
  0XD3, 0X64, 0XD3, 0X64, 0XC3, 0X64, 0XC3, 0X47, 0XC2, 0X4B, 0X80, 0XE8,
  0X30, 0XA7, 0X39, 0X27, 0X79, 0XE8, 0X71, 0XC8, 0X79, 0XE9, 0XA2, 0X88,
  0XAA, 0XE5, 0XBB, 0X65, 0X92, 0XC4, 0X18, 0XA5, 0X08, 0X86, 0X29, 0X26,
  0XAC, 0X84, 0X4B, 0XA9, 0X1A, 0X09, 0X2A, 0X48, 0X32, 0X88, 0X5B, 0X47,
  0X63, 0X06, 0X5A, 0X85, 0X31, 0X81, 0X51, 0X62, 0XD3, 0XE6, 0XD4, 0X05,
  0XE3, 0XA6, 0XA1, 0X23, 0X40, 0XA5, 0X31, 0X4B, 0X31, 0X6C, 0X51, 0X07,
  0X58, 0XA3, 0X00, 0X00, 0X29, 0X6D, 0X53, 0X38, 0X31, 0X0C, 0X41, 0X27,
  0X6A, 0X26, 0X49, 0X44, 0X1A, 0X26, 0X3C, 0X6C, 0X3A, 0X89, 0X4A, 0X29,
  0X21, 0X24, 0X00, 0X43, 0X11, 0X07, 0X08, 0XE5, 0X08, 0XE6, 0X11, 0X27,
  0X32, 0X2B, 0X32, 0X6C, 0X2A, 0X2B, 0X21, 0XEB, 0X21, 0XAA, 0X19, 0X26,
  0X08, 0XA4, 0X08, 0XA4, 0X08, 0XC4, 0X08, 0X84, 0X08, 0XA5, 0X11, 0X28,
  0X21, 0XAB, 0X29, 0X88, 0X31, 0X67, 0X21, 0X25, 0X32, 0X0B, 0X42, 0XAF,
  0X4B, 0X11, 0X42, 0XF1, 0X3A, 0X0A, 0X4A, 0X4A, 0X4A, 0X29, 0X62, 0XCA,
  0X42, 0X4B, 0X42, 0X49, 0X29, 0X04, 0X62, 0X6F, 0X6A, 0X6D, 0X6A, 0X8E,
  0X5A, 0X2B, 0X39, 0X24, 0X49, 0X66, 0X61, 0XCA, 0X69, 0XE9, 0X51, 0X64,
  0X49, 0X47, 0X51, 0X67, 0X59, 0XA5, 0X59, 0X85, 0X69, 0XE5, 0X6A, 0X05,
  0X5A, 0X49, 0X4A, 0XCC, 0X4A, 0XCC, 0X6A, 0X27, 0X51, 0X64, 0X39, 0X04,
  0X41, 0X25, 0X39, 0X26, 0X41, 0X87, 0X41, 0X87, 0X41, 0X66, 0X49, 0X43,
  0X20, 0XA2, 0X53, 0X0E, 0X63, 0X90, 0X53, 0X51, 0X3A, 0XF1, 0X3A, 0X8F,
  0X4A, 0XAD, 0X4A, 0XAD, 0X4A, 0XAD, 0X5B, 0X71, 0X53, 0X50, 0X42, 0XD1,
  0X29, 0XCA, 0X2A, 0X0B, 0X3A, 0X8E, 0X3A, 0X6D, 0X52, 0XCC, 0X5A, 0XCD,
  0X5A, 0XCD, 0X5A, 0XCD, 0X5B, 0X0F, 0X4A, 0XEF, 0X53, 0X30, 0X63, 0X0E,
  0X72, 0XEC, 0X5B, 0X4F, 0X6A, 0XED, 0X72, 0XCC, 0X6A, 0XCC, 0X6A, 0XEC,
  0X6A, 0XEC, 0X73, 0X0C, 0X63, 0X2E, 0X53, 0X0F, 0X4A, 0XEF, 0X42, 0X8E,
  0X4A, 0XAD, 0X5B, 0X70, 0X7C, 0X72, 0X5B, 0X2F, 0X53, 0X2F, 0X42, 0XCF,
  0X3A, 0XAF, 0X4B, 0X2F, 0X4B, 0X0F, 0X63, 0X0E, 0X63, 0X71, 0X5B, 0X4F,
  0X63, 0X4F, 0X63, 0X4F, 0X4B, 0X30, 0X53, 0X71, 0X5B, 0XF2, 0X63, 0XB1,
  0X63, 0X4F, 0X63, 0X4F, 0X63, 0X4F, 0X5B, 0X2E, 0X63, 0X4F, 0X63, 0X2E,
  0X5B, 0X0E, 0X5B, 0X4F, 0X5B, 0XB0, 0X5B, 0X70, 0X5B, 0X70, 0X5B, 0X90,
  0X5B, 0X90, 0X4B, 0X30, 0X42, 0XAF, 0X5B, 0X50, 0X53, 0X0F, 0X5B, 0X2F,
  0X5B, 0X50, 0X63, 0X70, 0X5B, 0X0F, 0X29, 0X66, 0X42, 0X2A, 0X39, 0XC7,
  0X39, 0XC7, 0X39, 0XC7, 0X31, 0X86, 0X29, 0X24, 0X29, 0X44, 0X29, 0X44,
  0X21, 0X45, 0X29, 0X66, 0X42, 0X6A, 0X42, 0X69, 0X6B, 0XF1, 0X63, 0X6F,
  0X63, 0X0D, 0X7B, 0X4F, 0X83, 0X0E, 0X82, 0XED, 0X92, 0X89, 0X8A, 0X89,
  0X8A, 0X89, 0X8A, 0XA9, 0X8A, 0XA9, 0X8A, 0XA9, 0X8B, 0X0C, 0X74, 0X12,
  0X73, 0XB1, 0X73, 0XB0, 0X73, 0XB0, 0X74, 0X12, 0X5B, 0X4F, 0X31, 0XEB,
  0X39, 0XEB, 0X3A, 0X0B, 0X42, 0X2C, 0X42, 0X6D, 0X3A, 0X4C, 0X3A, 0X4B,
  0X3A, 0X2C, 0X41, 0XA5, 0X52, 0XCC, 0X4A, 0XAE, 0X52, 0XCE, 0X5B, 0X30,
  0X53, 0X50, 0X4A, 0XCE, 0X4A, 0XCE, 0X3A, 0X6C, 0X42, 0X6C, 0X42, 0XCE,
  0X4A, 0XEF, 0X4A, 0XEF, 0X52, 0XEE, 0X42, 0XAE, 0X4A, 0X8D, 0X52, 0XEE,
  0X53, 0X0F, 0X4A, 0XAD, 0X4A, 0X8D, 0X52, 0XCF, 0X52, 0XCF, 0X52, 0XCF,
  0X52, 0XAE, 0X5A, 0XCE, 0X5A, 0XF0, 0X52, 0XF0, 0X53, 0X10, 0X4A, 0XF0,
  0X53, 0X11, 0X53, 0X11, 0X53, 0X31, 0X4A, 0XF0, 0X4A, 0XF0, 0X4B, 0X31,
  0X43, 0X11, 0X3B, 0X31, 0X3B, 0X32, 0X3B, 0X10, 0X3A, 0XF0, 0X3B, 0X10,
  0X43, 0X51, 0X43, 0X10, 0X42, 0XEF, 0X42, 0XF0, 0X3A, 0XD0, 0X3A, 0XAF,
  0X3A, 0XAE, 0X3A, 0X8D, 0X42, 0XEF, 0X53, 0X71, 0X7C, 0XB5, 0X6C, 0X33,
  0X4B, 0X10, 0X5B, 0X50, 0X5A, 0X8C, 0X62, 0X6B, 0X6B, 0X0E, 0X73, 0X6D,
  0X6B, 0X6D, 0X7B, 0XAE, 0X41, 0XE8, 0X42, 0X0A, 0X63, 0X0E, 0X5A, 0XEE,
  0X52, 0XCE, 0X3A, 0XD0, 0X3A, 0XB0, 0X3A, 0XAE, 0X4A, 0XCE, 0X5A, 0XEF,
  0X5A, 0XF0, 0X52, 0XCF, 0X5B, 0X30, 0X73, 0X90, 0X6B, 0XB0, 0X7B, 0X90,
  0X7B, 0X90, 0X83, 0XD1, 0X73, 0X4F, 0X6B, 0X4F, 0X83, 0XD1, 0X83, 0X6F,
  0X72, 0X09, 0X69, 0XE9, 0X72, 0XAC, 0X6A, 0X6B, 0X5A, 0X4A, 0X6B, 0X2F,
  0X6B, 0X70, 0X73, 0X0E, 0X72, 0XCC, 0X73, 0X91, 0X73, 0XB2, 0X73, 0X51,
  0X6A, 0XCE, 0X62, 0XCD, 0X73, 0XF3, 0X73, 0XF2, 0X7B, 0XF2, 0X62, 0XCE,
  0X72, 0XED, 0X62, 0XCD, 0X52, 0XAE, 0X5A, 0XCE, 0X5A, 0XCE, 0X6B, 0X0E,
  0X6B, 0X0E, 0X5A, 0XEE, 0X63, 0X0F, 0X63, 0X70, 0X63, 0X92, 0X63, 0X71,
  0X63, 0X4F, 0X63, 0X91, 0X63, 0X92, 0X5B, 0X51, 0X6B, 0X93, 0X6B, 0X92,
  0X5B, 0X31, 0X5B, 0X11, 0X5A, 0XF0, 0X6B, 0X51, 0X7B, 0X2F, 0X7B, 0X2F,
  0X7B, 0X0F, 0X73, 0X2F, 0X3A, 0X8C, 0X42, 0XCB, 0X4B, 0X0B, 0X32, 0X6D,
  0X2A, 0X6C, 0X2A, 0X4C, 0X31, 0XE9, 0X22, 0X2D, 0X3A, 0XCE, 0X2A, 0X6B,
  0X2A, 0X09, 0X3A, 0XA9, 0X21, 0XE9, 0X2A, 0X0A, 0X29, 0XE9, 0X29, 0XC9,
  0X21, 0XE9, 0X21, 0XE9, 0X19, 0XC9, 0X21, 0XC9, 0X32, 0X29, 0X3A, 0XCD,
  0X2A, 0X4C, 0X2A, 0X2B, 0X2B, 0X51, 0X2B, 0X30, 0X1A, 0X2A, 0X3A, 0X6B,
  0X31, 0XE9, 0X11, 0X46, 0X19, 0X66, 0X19, 0X66, 0X19, 0X86, 0X22, 0X0A,
  0X22, 0X8D, 0X2A, 0X4B, 0X21, 0XC7, 0X22, 0X08, 0X21, 0XC7, 0X22, 0X4B,
  0X3B, 0X73, 0X2A, 0X29, 0X22, 0X09, 0X22, 0X29, 0X19, 0XA6, 0X21, 0X25,
  0X39, 0XC8, 0X29, 0XC9, 0X3A, 0XAE, 0X32, 0X8C, 0X21, 0XC7, 0X19, 0XA7,
  0X29, 0X86, 0X32, 0XCD, 0X32, 0XCD, 0X32, 0X6B, 0X21, 0XE9, 0X22, 0X0A,
  0X32, 0X2A, 0X3A, 0X8B, 0X42, 0X8B, 0X32, 0XAD, 0X32, 0X8E, 0X32, 0XEF,
  0X2A, 0XAE, 0X2A, 0X4B, 0X53, 0X4F, 0X53, 0X70, 0X22, 0X2A, 0X3A, 0XEE,
  0X53, 0X50, 0X53, 0X71, 0X4B, 0X71, 0X53, 0X72, 0X53, 0X51, 0X53, 0X51,
  0X53, 0X52, 0X53, 0X51, 0X5B, 0X30, 0X72, 0XAE, 0X7A, 0X8E, 0X7A, 0X8E,
  0X7A, 0XAE, 0X52, 0X0A, 0X42, 0X0B, 0X42, 0X4D, 0X09, 0XED, 0X43, 0X0E,
  0X3A, 0X2A, 0X10, 0X83, 0X00, 0X00, 0X08, 0X41, 0X21, 0X46, 0X22, 0XCF,
  0X23, 0XF5, 0X4B, 0X71, 0X52, 0X6A, 0X42, 0X6B, 0X42, 0X6B, 0X3A, 0X8B,
  0X11, 0XA9, 0X00, 0XA6, 0X00, 0X21, 0X08, 0X62, 0X08, 0XA4, 0X10, 0XC4,
  0X10, 0XC4, 0X10, 0XC4, 0X10, 0XC4, 0X08, 0X84, 0X18, 0XC4, 0X29, 0X46,
  0X10, 0XA3, 0X10, 0X62, 0X10, 0XA2, 0X08, 0X62, 0X08, 0X62, 0X42, 0X6B,
  0X19, 0X26, 0X63, 0X6F, 0X7C, 0X32, 0X10, 0XE2, 0X19, 0X84, 0X32, 0X45,
  0X3A, 0X85, 0X3A, 0XA7, 0X32, 0X87, 0X32, 0X86, 0X32, 0X46, 0X42, 0XA6,
  0X42, 0XA6, 0X3A, 0X48, 0X21, 0X04, 0X28, 0XE5, 0X41, 0X4B, 0X52, 0X27,
  0XA3, 0X65, 0XA1, 0XE2, 0X58, 0X82, 0X08, 0XA4, 0X21, 0X66, 0X21, 0X25,
  0X21, 0X04, 0X2A, 0X2F, 0X43, 0X35, 0X43, 0X14, 0X3B, 0X34, 0X33, 0X15,
  0X42, 0X11, 0X59, 0X07, 0X59, 0X45, 0X30, 0XE5, 0X41, 0X25, 0X59, 0X46,
  0X41, 0XA8, 0X29, 0X66, 0X29, 0X45, 0X29, 0X46, 0X18, 0XC6, 0X18, 0XE6,
  0X21, 0X28, 0X28, 0XE3, 0X31, 0X25, 0X39, 0X46, 0X30, 0XA3, 0X20, 0XA5,
  0X20, 0XA4, 0X28, 0XE4, 0X21, 0X29, 0X21, 0X4C, 0X10, 0XAA, 0X19, 0X31,
  0X18, 0XA8, 0X30, 0XC3, 0X10, 0XA4, 0X11, 0X28, 0X19, 0X69, 0X21, 0X88,
  0X31, 0XC7, 0X31, 0X25, 0X29, 0XAC, 0X62, 0X67, 0X69, 0X25, 0X60, 0XA4,
  0X61, 0X24, 0X29, 0X67, 0X21, 0X26, 0X51, 0XA9, 0X59, 0X46, 0X10, 0X64,
  0X29, 0X06, 0X73, 0X71, 0X7B, 0XD3, 0X08, 0X62, 0X00, 0X20, 0X11, 0XA8,
  0X1A, 0X6B, 0X22, 0X4B, 0X22, 0X6B, 0X22, 0X8B, 0X1A, 0X2A, 0X21, 0XC9,
  0X2A, 0X0A, 0X10, 0XC4, 0X18, 0X81, 0X30, 0XE2, 0X41, 0XA6, 0X5A, 0XAB,
  0X5A, 0XCB, 0X62, 0XEC, 0X62, 0XEC, 0X5A, 0XCB, 0X6B, 0X4E, 0X73, 0XAF,
  0X6B, 0X4D, 0X63, 0X0C, 0X62, 0XEC, 0X5A, 0XCB, 0X5A, 0XEB, 0X5A, 0XEB,
  0X63, 0X2D, 0X63, 0X2D, 0X63, 0X2D, 0X63, 0X0D, 0X6B, 0X2D, 0X6B, 0X4E,
  0X73, 0X8F, 0X73, 0X8F, 0X7B, 0XB0, 0X7B, 0X6F, 0X7B, 0X90, 0X83, 0XD1,
  0X8B, 0XF1, 0X94, 0X12, 0X94, 0X52, 0X8B, 0XF0, 0X73, 0X4C, 0X5B, 0X0B,
  0X52, 0XEA, 0X52, 0XCA, 0X52, 0XCA, 0X4A, 0XC9, 0X4A, 0XC9, 0X4A, 0XC9,
  0X4A, 0XEB, 0X4A, 0XCA, 0X42, 0X89, 0X31, 0XC4, 0X31, 0XE5, 0X42, 0X05,
  0X39, 0XE4, 0X31, 0XC4, 0X19, 0X43, 0X3B, 0X0C, 0X53, 0X8E, 0X53, 0X8E,
  0X43, 0X0B, 0X31, 0XA4, 0X3A, 0X68, 0X43, 0X0B, 0X32, 0X88, 0X4C, 0X32,
  0X3B, 0X0B, 0X29, 0XC4, 0X29, 0XE4, 0X29, 0XC4, 0X3A, 0X05, 0X3A, 0X25,
  0X3A, 0X26, 0X3A, 0X25, 0X31, 0XE4, 0X32, 0X46, 0X42, 0XA9, 0X5B, 0X6C,
  0X5B, 0X4B, 0X53, 0X2A, 0X5B, 0X4B, 0X4A, 0XE9, 0X42, 0X67, 0X42, 0X47,
  0X42, 0X26, 0X42, 0X26, 0X42, 0X67, 0X42, 0X87, 0X4A, 0XA7, 0X52, 0XE9,
  0X52, 0XC9, 0X4A, 0XA8, 0X52, 0XA8, 0X73, 0X4B, 0X62, 0XE9, 0X3A, 0X25,
  0X63, 0X09, 0X73, 0X29, 0X52, 0XC8, 0X42, 0XA8, 0X3A, 0X26, 0X32, 0X05,
  0X32, 0X05, 0X31, 0XE5, 0X39, 0XC6, 0X42, 0X07, 0X39, 0XC7, 0X6A, 0XE9,
  0X93, 0X89, 0X9B, 0X68, 0X9B, 0X48, 0X9B, 0X47, 0X9B, 0X27, 0X93, 0X26,
  0X52, 0X87, 0X42, 0X47, 0X3A, 0X47, 0X3A, 0X27, 0X4A, 0XA8, 0X52, 0XA8,
  0X4A, 0X87, 0X3A, 0X66, 0X3A, 0X66, 0X32, 0X87, 0X2B, 0X08, 0X42, 0X47,
  0X6A, 0XCB, 0XA4, 0X70, 0XB5, 0X09, 0X64, 0X2D, 0X54, 0X0F, 0X54, 0X0E,
  0X53, 0XEE, 0X53, 0XCD, 0X4B, 0X8C, 0X4B, 0XAD, 0X4B, 0XEF, 0X4B, 0XEF,
  0X4B, 0XEF, 0X4B, 0XCE, 0X4B, 0XCE, 0X4B, 0XCE, 0X53, 0XCE, 0X54, 0X0F,
  0X53, 0XEF, 0X53, 0X6C, 0X42, 0XC9, 0X4B, 0X8C, 0X3A, 0XE8, 0X32, 0X86,
  0X2A, 0X67, 0X2A, 0X87, 0X2A, 0XA8, 0X32, 0XC8, 0X32, 0XC7, 0X32, 0X87,
  0X32, 0X66, 0X3A, 0X46, 0X8C, 0X06, 0X8C, 0X06, 0X5A, 0XA6, 0X39, 0X86,
  0X39, 0XCA, 0X42, 0X6D, 0X42, 0XAF, 0X4A, 0XCD, 0X42, 0XCC, 0X43, 0X0B,
  0X62, 0X69, 0X6A, 0X69, 0X8A, 0XE9, 0XA3, 0X48, 0XB4, 0X48, 0XAC, 0XE8,
  0X4A, 0XA6, 0X2A, 0X67, 0X32, 0XC8, 0X32, 0X67, 0X29, 0XE6, 0X3A, 0X66,
  0X3A, 0XA6, 0X32, 0X87, 0X32, 0X67, 0X2A, 0X87, 0X2A, 0XA7, 0X2A, 0X67,
  0X32, 0X67, 0X32, 0X67, 0X32, 0X86, 0X3A, 0XE8, 0X42, 0XA8, 0X42, 0X67,
  0X42, 0X47, 0X42, 0X47, 0X52, 0X88, 0X3A, 0XC8, 0X32, 0XC8, 0X3A, 0XC8,
  0X42, 0XC8, 0X3A, 0XC8, 0X32, 0X87, 0X32, 0X06, 0X3A, 0X06, 0X41, 0XE6,
  0X4A, 0X87, 0X52, 0XA8, 0X42, 0X27, 0X4A, 0X26, 0X42, 0X67, 0X32, 0XE9,
  0X32, 0XA9, 0X3A, 0X89, 0X3A, 0X89, 0X3A, 0X89, 0X29, 0XC7, 0X21, 0X45,
  0X21, 0X46, 0X21, 0X45, 0X10, 0XC3, 0X08, 0X41, 0X19, 0X66, 0X3A, 0X8C,
  0X32, 0X09, 0X29, 0XC8, 0X19, 0X45, 0X19, 0X25, 0X19, 0X25, 0X19, 0X45,
  0X08, 0XA2, 0X00, 0X41, 0X08, 0X41, 0X00, 0X41, 0X29, 0X85, 0X3A, 0X47,
  0X3A, 0X27, 0X42, 0X46, 0X4A, 0X46, 0X42, 0X46, 0X42, 0X26, 0X39, 0XA5,
  0X39, 0XA5, 0X41, 0XE6, 0X39, 0X85, 0X18, 0XC4, 0X19, 0X04, 0X21, 0X45,
  0X29, 0X45, 0X31, 0X85, 0X51, 0XE6, 0X59, 0XE6, 0X51, 0XE5, 0X51, 0XE5,
  0X51, 0XE5, 0X51, 0XC5, 0X49, 0XA5, 0X29, 0X44, 0X29, 0X24, 0X29, 0X85,
  0X29, 0X85, 0X29, 0XA5, 0X2A, 0X05, 0X29, 0XE5, 0X29, 0X65, 0X29, 0XE5,
  0X31, 0XE5, 0X4A, 0X06, 0X29, 0X65, 0X21, 0X04, 0X21, 0X04, 0X21, 0X03,
  0X21, 0X24, 0X21, 0X25, 0X31, 0X66, 0X31, 0X25, 0X5B, 0X0C, 0X4A, 0X49,
  0X28, 0XE4, 0X39, 0XC7, 0X53, 0X0C, 0X4A, 0X69, 0X39, 0XC6, 0X39, 0XC6,
  0X41, 0XE7, 0X49, 0XE6, 0X52, 0X28, 0X52, 0X27, 0X49, 0XE6, 0X52, 0X07,
  0X52, 0X06, 0X41, 0XA5, 0X41, 0XA5, 0X41, 0XA5, 0X41, 0XA5, 0X41, 0XE6,
  0X3A, 0X07, 0X52, 0X69, 0X4A, 0X89, 0X4A, 0X48, 0X49, 0XE6, 0X52, 0X48,
  0X4A, 0XCA, 0X4A, 0X07, 0X4A, 0X28, 0X52, 0X28, 0X52, 0X48, 0X52, 0X48,
  0X4A, 0X27, 0X4A, 0X69, 0X4A, 0XAA, 0X4A, 0X48, 0X4A, 0X27, 0X4A, 0X07,
  0X4A, 0X07, 0X49, 0XC6, 0X39, 0X85, 0X41, 0XA6, 0X49, 0XE7, 0X49, 0XE7,
  0X31, 0XE6, 0X29, 0XC6, 0X29, 0XE6, 0X29, 0X85, 0X29, 0X24, 0X31, 0XC7,
  0X43, 0X2C, 0X3A, 0XCB, 0X19, 0XC8, 0X19, 0XA9, 0X19, 0XA8, 0X21, 0X45,
  0X31, 0X44, 0X39, 0X86, 0X29, 0X87, 0X21, 0X87, 0X19, 0X88, 0X21, 0XC8,
  0X3A, 0XEA, 0X43, 0X29, 0X43, 0XA9, 0X43, 0XC9, 0X4B, 0XC9, 0X4B, 0XC9,
  0X4B, 0XCA, 0X43, 0XCA, 0X4B, 0XA9, 0X53, 0X28, 0X52, 0XE8, 0X53, 0X08,
  0X52, 0XC8, 0X42, 0X26, 0X4A, 0X26, 0X52, 0X66, 0X4A, 0X27, 0X41, 0XC6,
  0X32, 0X05, 0X3A, 0X46, 0X4A, 0X46, 0X4A, 0X66, 0X42, 0X87, 0X52, 0XA8,
  0X6A, 0XEA, 0X62, 0XA9, 0X5A, 0X68, 0X5A, 0X88, 0X62, 0XEA, 0X5A, 0X89,
  0X31, 0XE6, 0X31, 0XA5, 0X31, 0XA5, 0X29, 0X84, 0X42, 0X26, 0X42, 0X47,
  0X39, 0XE7, 0X5A, 0X69, 0X3A, 0X47, 0X32, 0X06, 0X42, 0X47, 0X42, 0XAA,
  0X42, 0X27, 0X39, 0XE6, 0X39, 0XA6, 0X4A, 0X07, 0X52, 0X68, 0X52, 0X88,
  0X52, 0X68, 0X4A, 0X47, 0X3A, 0X06, 0X32, 0X06, 0X3A, 0X26, 0X4A, 0X88,
  0X4A, 0X68, 0X3A, 0X69, 0X4A, 0XAA, 0X4A, 0X68, 0X4A, 0X68, 0X4A, 0X48,
  0X42, 0X67, 0X32, 0X26, 0X4A, 0X47, 0X62, 0XC8, 0X42, 0X27, 0X2A, 0X06,
  0X32, 0X06, 0X32, 0X26, 0X31, 0XE6, 0X31, 0XC6, 0X29, 0X85, 0X19, 0X03,
  0X19, 0X84, 0X19, 0X45, 0X19, 0XA7, 0X21, 0XC7, 0X21, 0XC8, 0X2A, 0X4B,
  0X32, 0XAE, 0X21, 0X88, 0X21, 0X88, 0X29, 0X63, 0X7B, 0XA8, 0X73, 0X87,
  0X7B, 0X88, 0X62, 0XA7, 0X39, 0XA7, 0X52, 0XAB, 0X3A, 0XEB, 0X2A, 0X49,
  0X31, 0XC7, 0X31, 0XC8, 0X21, 0X86, 0X29, 0X86, 0X73, 0X2D, 0X6B, 0X4E,
  0X52, 0XCC, 0X4A, 0X6A, 0X52, 0X8B, 0X52, 0X8B, 0X63, 0X2D, 0X5B, 0X0E,
  0X63, 0X0D, 0X63, 0X2D, 0X63, 0X2D, 0X5B, 0X0D, 0X63, 0X0D, 0X6B, 0X2D,
  0X5B, 0X0D, 0X63, 0X0C, 0X73, 0X4D, 0X6B, 0X0D, 0X5A, 0XCC, 0X63, 0X0C,
  0X62, 0XCC, 0X62, 0XCB, 0X6A, 0XEC, 0X6B, 0X0D, 0X6A, 0XEC, 0X63, 0X0C,
  0X5A, 0XCC, 0X73, 0X2D, 0X73, 0X6E, 0X73, 0X6E, 0X73, 0X4D, 0X73, 0X2D,
  0X7B, 0X8E, 0X83, 0XAE, 0X83, 0XAE, 0X83, 0XAE, 0X73, 0X2C, 0X6B, 0X0C,
  0X63, 0X0C, 0X63, 0X0D, 0X63, 0X0D, 0X63, 0X0D, 0X63, 0X0D, 0X63, 0X0D,
  0X6B, 0X2E, 0X6B, 0X2E, 0X6B, 0X0E, 0X6B, 0X0D, 0X6B, 0X0D, 0X6B, 0X4F,
  0X73, 0X91, 0X7B, 0XD2, 0X7B, 0XB2, 0X7B, 0XF3, 0X8C, 0X54, 0X94, 0X95,
  0X94, 0X95, 0X84, 0X53, 0X84, 0X53, 0X84, 0X32, 0X84, 0X52, 0X7C, 0X32,
  0X73, 0XD0, 0X52, 0XED, 0X3A, 0X0A, 0X31, 0XC9, 0X21, 0X05, 0X00, 0X21,
  0X08, 0X41, 0X10, 0XA4, 0X10, 0XE6, 0X11, 0X07, 0X19, 0X28, 0X19, 0X28,
  0X19, 0X48, 0X19, 0X89, 0X21, 0XEA, 0X21, 0XEA, 0X22, 0X0A, 0X22, 0X0A,
  0X19, 0XCA, 0X19, 0XEA, 0X22, 0X0B, 0X22, 0X4C, 0X2A, 0X8D, 0X11, 0X05,
  0X10, 0XE3, 0X32, 0XAB, 0X3A, 0XCC, 0X53, 0X4E, 0X53, 0X6E, 0X53, 0X8E,
  0X53, 0X6E, 0X43, 0X0C, 0X43, 0X0C, 0X43, 0X0C, 0X42, 0XCB, 0X4B, 0X0C,
  0X3A, 0XEC, 0X3A, 0XEC, 0X32, 0XEC, 0X3B, 0X4D, 0X43, 0X8D, 0X3A, 0XAB,
  0X32, 0XAB, 0X22, 0X2B, 0X2A, 0X4B, 0X2A, 0X2B, 0X2A, 0X4B, 0X43, 0X0E,
  0X43, 0X4E, 0X6C, 0X51, 0X7C, 0X92, 0X74, 0X51, 0X42, 0XAB, 0X64, 0X10,
  0X7D, 0X34, 0X74, 0X71, 0X6B, 0XEE, 0X63, 0X8E, 0X5A, 0XEC, 0X63, 0X6E,
  0X32, 0X08, 0X11, 0X04, 0X19, 0X25, 0X63, 0X6E, 0X3A, 0X49, 0X11, 0X45,
  0X53, 0X2E, 0X63, 0XD2, 0X4B, 0X2E, 0X63, 0XF0, 0X53, 0X4E, 0X19, 0X45,
  0X08, 0X20, 0X10, 0XC4, 0X19, 0X88, 0X19, 0X68, 0X19, 0X47, 0X19, 0X24,
  0X11, 0X04, 0X19, 0X45, 0X4B, 0X0C, 0X4B, 0X0C, 0X4B, 0X2C, 0X53, 0XAE,
  0X64, 0X10, 0X42, 0XCB, 0X3A, 0X49, 0X3A, 0X49, 0X3A, 0X49, 0X2A, 0X8C,
  0X2A, 0XEE, 0X2A, 0XEE, 0X43, 0X2D, 0X5B, 0X0C, 0X73, 0XEE, 0X52, 0XEB,
  0X2A, 0X8C, 0X3A, 0XEC, 0X5B, 0X0C, 0X6B, 0XAF, 0X5B, 0X2D, 0X43, 0X4E,
  0X4B, 0X6F, 0X53, 0XAE, 0X4A, 0XAA, 0X4A, 0XAA, 0X4A, 0XAA, 0X4A, 0XCA,
  0X19, 0X04, 0X4A, 0X25, 0X62, 0XE7, 0X5A, 0XC6, 0X52, 0XE8, 0X53, 0X09,
  0X5A, 0XC7, 0X52, 0X85, 0X5A, 0XC7, 0X42, 0XAB, 0X4A, 0X86, 0X52, 0XA5,
  0X52, 0X86, 0X52, 0X66, 0X4A, 0X65, 0X52, 0X85, 0X5A, 0X84, 0X42, 0X25,
  0X42, 0X25, 0X4A, 0X66, 0X4A, 0X66, 0X4A, 0X66, 0X52, 0XA6, 0X6B, 0X06,
  0X4A, 0XA9, 0X4A, 0X88, 0X4A, 0X87, 0X42, 0XA9, 0X3A, 0XCB, 0X3A, 0X89,
  0X3A, 0X69, 0X2A, 0X4B, 0X22, 0X8D, 0X22, 0XAE, 0X22, 0XCF, 0X22, 0X6D,
  0X19, 0X47, 0X19, 0X26, 0X11, 0X26, 0X19, 0X26, 0X29, 0X86, 0X29, 0X87,
  0X19, 0X67, 0X19, 0X86, 0X21, 0XC9, 0X32, 0X4C, 0X32, 0X4B, 0X32, 0X4B,
  0X21, 0XEA, 0X09, 0X48, 0X11, 0X89, 0X11, 0XCA, 0X11, 0XA9, 0X11, 0XA9,
  0X1A, 0X0B, 0X11, 0XCB, 0X1A, 0X2B, 0X22, 0XCE, 0X22, 0X8D, 0X19, 0X88,
  0X29, 0XC9, 0X29, 0XEA, 0X2A, 0X4C, 0X3A, 0XCF, 0X3A, 0XAE, 0X29, 0XC9,
  0X21, 0X89, 0X21, 0XA9, 0X11, 0XA9, 0X11, 0XA9, 0X19, 0X89, 0X19, 0X68,
  0X11, 0X69, 0X11, 0XCB, 0X11, 0XEC, 0X1A, 0X2D, 0X22, 0X8E, 0X1A, 0X2C,
  0X1A, 0X2C, 0X22, 0X8E, 0X22, 0X4C, 0X19, 0XCC, 0X22, 0X4E, 0X22, 0X8F,
  0X19, 0XAA, 0X2A, 0XEF, 0X2B, 0X11, 0X19, 0XCB, 0X22, 0X2C, 0X22, 0X4D,
  0X19, 0XA9, 0X19, 0XA9, 0X1A, 0X0B, 0X11, 0XA9, 0X19, 0XA9, 0X19, 0XA9,
  0X19, 0X89, 0X21, 0XCA, 0X22, 0X0B, 0X21, 0XEA, 0X19, 0X68, 0X1A, 0X0B,
  0X19, 0XEA, 0X19, 0XAA, 0X19, 0XCB, 0X22, 0X8E, 0X1A, 0X2D, 0X11, 0XA9,
  0X11, 0X68, 0X11, 0X47, 0X11, 0X05, 0X11, 0X26, 0X19, 0X25, 0X19, 0X25,
  0X19, 0X04, 0X19, 0X05, 0X19, 0X05, 0X19, 0X25, 0X19, 0XA8, 0X21, 0XC9,
  0X21, 0XA8, 0X21, 0XA8, 0X1A, 0X2B, 0X1A, 0X8E, 0X1A, 0X6D, 0X1A, 0X4D,
  0X42, 0XAD, 0X52, 0XCD, 0X5A, 0XCD, 0X5A, 0X8C, 0X4A, 0X2B, 0X4A, 0X0A,
  0X6A, 0XCC, 0X62, 0XAC, 0X4A, 0X8B, 0X52, 0X8C, 0X52, 0XAC, 0X52, 0XCC,
  0X3A, 0X09, 0X08, 0X41, 0X08, 0X62, 0X08, 0X83, 0X08, 0XA4, 0X08, 0XA4,
  0X10, 0X83, 0X08, 0X83, 0X08, 0X83, 0X10, 0XC4, 0X19, 0X46, 0X19, 0X46,
  0X19, 0X46, 0X10, 0XE4, 0X08, 0X62, 0X08, 0X62, 0X08, 0X62, 0X00, 0X21,
  0X00, 0X00, 0X08, 0X42, 0X08, 0X63, 0X08, 0X83, 0X10, 0XA3, 0X10, 0XA4,
  0X10, 0XC4, 0X18, 0XE4, 0X18, 0XE4, 0X18, 0XE3, 0X20, 0XE2, 0X29, 0X22,
  0X62, 0X24, 0X72, 0X65, 0X7A, 0XA5, 0X8A, 0XE5, 0X7A, 0XC4, 0X72, 0XA5,
  0X72, 0XA4, 0X8A, 0XC5, 0X72, 0XA5, 0X49, 0XC4, 0X51, 0XE4, 0X5A, 0X04,
  0X51, 0XE4, 0X31, 0X85, 0X29, 0X85, 0X19, 0X25, 0X11, 0X05, 0X10, 0XC5,
  0X19, 0X25, 0X29, 0XC6, 0X42, 0X47, 0X42, 0X67, 0X21, 0X86, 0X08, 0XC5,
  0X08, 0XA5, 0X08, 0XA4, 0X08, 0X84, 0X08, 0X84, 0X08, 0X84, 0X08, 0XA4,
  0X08, 0XA4, 0X08, 0X83, 0X08, 0X83, 0X10, 0XC4, 0X10, 0XE4, 0X08, 0XC4,
  0X39, 0XC7, 0X42, 0X29, 0X4A, 0X4A, 0X42, 0X29, 0X2A, 0X08, 0X3A, 0X29,
  0X42, 0X29, 0X32, 0X29, 0X2A, 0X08, 0X3A, 0X08, 0X42, 0X4A, 0X3A, 0XAB,
  0X3A, 0X6A, 0X42, 0X29, 0X42, 0X08, 0X31, 0XA6, 0X21, 0X24, 0X21, 0X24,
  0X08, 0XA3, 0X11, 0X04, 0X29, 0XA6, 0X31, 0XA6, 0X39, 0XC6, 0X29, 0X65,
  0X19, 0X25, 0X41, 0XC7, 0X52, 0X49, 0X21, 0X46, 0X19, 0X26, 0X19, 0X66,
  0X5A, 0X89, 0X51, 0XE7, 0X31, 0X25, 0X39, 0X45, 0X39, 0XA7, 0X73, 0X0B,
  0X4A, 0X08, 0X62, 0XAA, 0X6A, 0XEB, 0X6A, 0XAA, 0X6A, 0XCB, 0X6A, 0XCB,
  0X73, 0X0C, 0X73, 0X0B, 0X5A, 0X89, 0X41, 0XC7, 0X72, 0XC9, 0X6A, 0XA9,
  0X42, 0X08, 0X31, 0X86, 0X39, 0XC7, 0X4A, 0X28, 0X41, 0XE7, 0X4A, 0X28,
  0X5A, 0XCA, 0X5A, 0XA9, 0X52, 0X89, 0X4A, 0X69, 0X42, 0X27, 0X39, 0XE7,
  0X29, 0XA8, 0X29, 0XC8, 0X19, 0X67, 0X19, 0X87, 0X29, 0XE9, 0X29, 0XE9,
  0X52, 0XCC, 0X52, 0X8A, 0X08, 0XA4, 0X10, 0XE5, 0X21, 0X26, 0X41, 0XCA,
  0X4A, 0X2B, 0X4A, 0X4B, 0X42, 0XAE, 0X39, 0XCA, 0X39, 0XAA, 0X41, 0XEB,
  0X39, 0XAB, 0X4A, 0X4B, 0X52, 0X4C, 0X41, 0XEA, 0X31, 0XE7, 0X42, 0X68,
  0X62, 0X8A, 0X52, 0X69, 0X5A, 0X8A, 0X63, 0X0B, 0X73, 0X8D, 0X53, 0X0C,
  0X5A, 0XCB, 0X52, 0X6A, 0X4A, 0X0B, 0X4A, 0X4B, 0X63, 0X0B, 0X6A, 0XEC,
  0X73, 0X2B, 0X73, 0X2B, 0X62, 0XAB, 0X62, 0XAB, 0X6A, 0XAB, 0X31, 0X66,
  0X29, 0X45, 0X31, 0X86, 0X31, 0X25, 0X29, 0X45, 0X31, 0X86, 0X52, 0X69,
  0X52, 0X69, 0X42, 0X08, 0X42, 0X49, 0X42, 0X4A, 0X4A, 0X6A, 0X42, 0X09,
  0X39, 0XE8, 0X32, 0X2B, 0X29, 0XC9, 0X31, 0X45, 0X4A, 0X29, 0X42, 0X08,
  0X4A, 0X28, 0X41, 0XA6, 0X41, 0XC7, 0X39, 0XE8, 0X39, 0XE8, 0X31, 0XE9,
  0X4A, 0X69, 0X4A, 0X69, 0X42, 0X29, 0X42, 0X28, 0X4A, 0X48, 0X42, 0X49,
  0X42, 0X4A, 0X4A, 0X6A, 0X4A, 0X6B, 0X4A, 0X6B, 0X4A, 0X6B, 0X4A, 0X8A,
  0X4A, 0XAA, 0X4A, 0X49, 0X42, 0X29, 0X4A, 0X8A, 0X4A, 0X8A, 0X4A, 0XAA,
  0X5A, 0XCB, 0X52, 0X8A, 0X5A, 0XAA, 0X5A, 0XAA, 0X52, 0X89, 0X4A, 0X48,
  0X29, 0X65, 0X31, 0X85, 0X5A, 0XCA, 0X52, 0XAB, 0X6B, 0X0B, 0X7B, 0X4A,
  0X41, 0XC6, 0X72, 0XE9, 0X73, 0X2B, 0X29, 0X66, 0X10, 0X83, 0X10, 0X82,
  0X52, 0X48, 0X7B, 0X2B, 0X6B, 0X0A, 0X73, 0X2B, 0X52, 0X89, 0X5A, 0XCA,
  0X5A, 0XAA, 0X6A, 0XC9, 0X73, 0X0A, 0X5A, 0XE9, 0X73, 0X4B, 0X41, 0XC6,
  0X62, 0X88, 0X8B, 0X6B, 0X52, 0X27, 0X41, 0XC6, 0X72, 0XE9, 0X6A, 0XA9,
  0X6A, 0XEA, 0X8B, 0X6A, 0X8B, 0X4A, 0X83, 0X09, 0X7B, 0X0A, 0X83, 0X4A,
  0X8B, 0XAB, 0X83, 0X4A, 0X73, 0X4C, 0X6B, 0X2B, 0X62, 0XCA, 0X6B, 0X0B,
  0X6B, 0X0C, 0X52, 0X48, 0X6B, 0X0C, 0X7B, 0X2A, 0X8B, 0XAB, 0X73, 0X2C,
  0X62, 0XCA, 0X4A, 0X68, 0X52, 0X89, 0X4A, 0X47, 0X41, 0XC7, 0X52, 0X48,
  0X4A, 0X67, 0X62, 0XEA, 0X72, 0XE9, 0X6A, 0XE9, 0X52, 0X69, 0X62, 0XA9,
  0X6A, 0XC8, 0X5A, 0XA9, 0X63, 0X0B, 0X52, 0XAB, 0X5A, 0XA9, 0X63, 0X0B,
  0X62, 0XEA, 0X62, 0XEA, 0X62, 0XEA, 0X62, 0XCA, 0X6B, 0X0B, 0X6B, 0X2B,
  0X7B, 0X6C, 0X83, 0XCE, 0X7B, 0X6B, 0X7B, 0X2B, 0X41, 0XE7, 0X4A, 0X48,
  0X7B, 0XD0, 0X7B, 0XD0, 0X62, 0XEB, 0X6C, 0X13, 0X74, 0X75, 0X73, 0X8D,
  0X7B, 0X2A, 0X51, 0XA5, 0X5A, 0X27, 0X62, 0X88, 0X6A, 0XA9, 0X83, 0X4B,
  0X72, 0XA8, 0X6A, 0X28, 0X8B, 0X4E, 0X6A, 0X89, 0X62, 0X48, 0X83, 0X8D,
  0X62, 0X89, 0X62, 0X88, 0X62, 0X26, 0X5A, 0X68, 0X52, 0XCB, 0X52, 0X8A,
  0X4A, 0X28, 0X73, 0X2B, 0X94, 0X2F, 0X39, 0XA6, 0X4A, 0X48, 0X7B, 0X6C,
  0X73, 0X6C, 0X6B, 0X2B, 0X83, 0XCC, 0X8B, 0XCC, 0X8B, 0XCC, 0X73, 0X4B,
  0X62, 0XEB, 0X62, 0XEB, 0X62, 0XEA, 0X73, 0X4B, 0X83, 0XCD, 0X6B, 0X2B,
  0X7B, 0X8C, 0X83, 0XCD, 0X73, 0X4C, 0X83, 0XAD, 0X63, 0X0B, 0X6B, 0X2B,
  0X52, 0XA9, 0X73, 0X2A, 0X6B, 0X2B, 0X4A, 0X8A, 0X5A, 0X8A, 0X5A, 0X8A,
  0X52, 0X6A, 0X73, 0X4C, 0X6B, 0X2B, 0X6B, 0X0C, 0X7B, 0X2D, 0X41, 0XC8,
  0X31, 0XC8, 0X5A, 0XCB, 0X7B, 0X8C, 0X63, 0X0B, 0X7B, 0X8C, 0X5A, 0XA9,
  0X73, 0X4C, 0X8B, 0XCE, 0X73, 0X4C, 0X4A, 0X28, 0X31, 0XE9, 0X52, 0XAB,
  0X5B, 0X2D, 0X52, 0XEC, 0X6B, 0X4E, 0X6B, 0X4E, 0X4A, 0X89, 0X4A, 0X8A,
  0X42, 0X4A, 0X4A, 0X49, 0X4A, 0X69, 0X4A, 0X69, 0X52, 0X8A, 0X6B, 0X6D,
  0X52, 0X08, 0X5A, 0X49, 0X5A, 0X6A, 0X5A, 0X6A, 0X5A, 0X69, 0X62, 0XAB,
  0X73, 0X0C, 0X6A, 0XEC, 0X5A, 0X6A, 0X5A, 0X8A, 0X5A, 0X8A, 0X52, 0X6A,
  0X6B, 0X2C, 0X5A, 0XCB, 0X42, 0X29, 0X6B, 0X0B, 0X63, 0X2D, 0X53, 0X0D,
  0X4A, 0X8A, 0X39, 0XC7, 0X42, 0X29, 0X4A, 0XCC, 0X42, 0X8B, 0X5A, 0XEB,
  0X52, 0XAA, 0X32, 0X29, 0X42, 0X8B, 0X3A, 0X6C, 0X3A, 0X0A, 0X3A, 0X6D,
  0X3A, 0X8E, 0X32, 0X8A, 0X3A, 0X6B, 0X42, 0X6C, 0X42, 0X6C, 0X2A, 0X0D,
  0X42, 0X4D, 0X52, 0X8D, 0X52, 0X2A, 0X52, 0X2A, 0X5A, 0X6A, 0X62, 0XCB,
  0X6A, 0XCB, 0X6B, 0X6F, 0X6B, 0X4E, 0X73, 0X4E, 0X4A, 0X0A, 0X39, 0X87,
  0X62, 0XAB, 0X6A, 0XCC, 0X6B, 0X0C, 0X6B, 0X2D, 0X63, 0X4F, 0X73, 0X2F,
  0X6A, 0XED, 0X6A, 0XCC, 0X63, 0X2F, 0X73, 0X6F, 0X73, 0X2D, 0X83, 0XAF,
  0X6A, 0XED, 0X62, 0XCD, 0X4A, 0X09, 0X5A, 0X8C, 0X6B, 0X71, 0X5B, 0X32,
  0X6B, 0X73, 0X5A, 0XEF, 0X5A, 0XCF, 0X5B, 0X95, 0X5A, 0X4D, 0X5A, 0X2B,
  0X5A, 0X4B, 0X5A, 0X6B, 0X5A, 0X6C, 0X52, 0X0B, 0X52, 0X2B, 0X49, 0XCA,
  0X41, 0X8A, 0X49, 0XAA, 0X5A, 0X4C, 0X53, 0X10, 0X5B, 0X11, 0X62, 0XCF,
  0X7B, 0X8F, 0X63, 0X2E, 0X63, 0X2D, 0X83, 0XF0, 0X94, 0X93, 0X52, 0XAD,
  0X21, 0X08, 0X5A, 0XAF, 0X31, 0XA6, 0X21, 0X44, 0X21, 0X25, 0X29, 0X46,
  0X29, 0X26, 0X21, 0X06, 0X20, 0XE6, 0X29, 0X26, 0X29, 0X26, 0X29, 0X26,
  0X21, 0X06, 0X19, 0X06, 0X29, 0X68, 0X31, 0X89, 0X31, 0X68, 0X29, 0X67,
  0X31, 0XAA, 0X31, 0XA9, 0X31, 0X68, 0X31, 0X89, 0X29, 0X67, 0X29, 0X68,
  0X29, 0X69, 0X29, 0X27, 0X29, 0X07, 0X29, 0X27, 0X29, 0X69, 0X29, 0X67,
  0X29, 0X67, 0X29, 0X67, 0X29, 0X68, 0X31, 0X89, 0X31, 0X89, 0X29, 0X67,
  0X29, 0X67, 0X29, 0X47, 0X00, 0X00, 0X21, 0X88, 0X32, 0X4D, 0X32, 0X4D,
  0X32, 0X90, 0X42, 0XCF, 0X6B, 0X0D, 0X6B, 0X0C, 0X42, 0X2B, 0X29, 0X68,
  0X59, 0XC4, 0X59, 0XA3, 0X59, 0XC4, 0X51, 0X83, 0X49, 0X63, 0X49, 0X63,
  0X49, 0X43, 0X51, 0X84, 0X5A, 0X06, 0X49, 0X63, 0X49, 0X42, 0X41, 0X63,
  0X41, 0X64, 0X39, 0X64, 0X39, 0X64, 0X39, 0X64, 0X39, 0X85, 0X39, 0X85,
  0X39, 0X85, 0X41, 0XC6, 0X4A, 0X07, 0X41, 0XE7, 0X4A, 0X07, 0X41, 0XC5,
  0X49, 0XA4, 0X49, 0X84, 0X41, 0XC6, 0X41, 0XE7, 0X52, 0X47, 0X62, 0XA9,
  0X62, 0XC9, 0X49, 0XE5, 0X4A, 0X06, 0X4A, 0X06, 0X39, 0X43, 0X52, 0X26,
  0X52, 0X26, 0X49, 0X83, 0X49, 0X83, 0X52, 0X26, 0X49, 0X84, 0X49, 0XA4,
  0X52, 0X46, 0X52, 0X26, 0X52, 0X67, 0X52, 0X47, 0X52, 0X67, 0X4A, 0X06,
  0X39, 0X84, 0X41, 0X84, 0X49, 0XC6, 0X49, 0XC6, 0X49, 0XE6, 0X41, 0X85,
  0X41, 0XE6, 0X42, 0X07, 0X4A, 0X07, 0X52, 0X06, 0X52, 0X06, 0X52, 0X06,
  0X18, 0XE3, 0X2A, 0X08, 0X32, 0X28, 0X32, 0X08, 0X32, 0X29, 0X4A, 0X28,
  0X42, 0X07, 0X42, 0X07, 0X42, 0X07, 0X31, 0XC5, 0X4A, 0X8A, 0X5B, 0X4E,
  0X5B, 0X2E, 0X3A, 0X8C, 0X32, 0X8B, 0X3B, 0X2C, 0X33, 0X4C, 0X2B, 0X2B,
  0X22, 0XEB, 0X1A, 0XAA, 0X22, 0X28, 0X22, 0X28, 0X32, 0X8A, 0X3A, 0XCB,
  0X3A, 0X4A, 0X3A, 0X6A, 0X42, 0X6A, 0X29, 0X68, 0X3A, 0X2A, 0X10, 0XE5,
  0X10, 0X41, 0X20, 0X42, 0X20, 0X64, 0X18, 0X64, 0X10, 0X64, 0X18, 0X84,
  0X30, 0X63, 0X38, 0X63, 0X30, 0X62, 0X18, 0X63, 0X10, 0XA4, 0X10, 0XA4,
  0X18, 0XA3, 0X30, 0X82, 0X39, 0X05, 0X22, 0X69, 0X22, 0X69, 0X2A, 0X89,
  0X2A, 0XEB, 0X33, 0X0C, 0X42, 0X49, 0X6A, 0XEA, 0X5B, 0X0B, 0X4A, 0XAB,
  0X3A, 0X4A, 0X32, 0X6A, 0X3A, 0X89, 0X4A, 0XAB, 0X4A, 0XAB, 0X2A, 0X69,
  0X42, 0XCB, 0X3A, 0XA9, 0X5B, 0X2D, 0X6B, 0X8E, 0X7C, 0X31, 0X7B, 0XEE,
  0X7C, 0X51, 0X73, 0XF0, 0X63, 0X6E, 0X6B, 0X6E, 0X6B, 0X6E, 0X63, 0X8E,
  0X53, 0X4C, 0X52, 0XCB, 0X5B, 0X2D, 0X5A, 0XA9, 0X52, 0X8A, 0X31, 0XC7,
  0X63, 0X6E, 0X6B, 0XAF, 0X73, 0X8E, 0X6B, 0X6E, 0X63, 0X2E, 0X6C, 0X31,
  0X53, 0X2A, 0X4B, 0X0A, 0X4B, 0X09, 0X4A, 0XEA, 0X5A, 0XED, 0X5B, 0X0C,
  0X5A, 0XEA, 0X4B, 0X2B, 0X4A, 0XAA, 0X63, 0X4F, 0X5B, 0X0A, 0X5B, 0X2B,
  0X63, 0X6E, 0X63, 0X2E, 0X63, 0X2E, 0X63, 0X6E, 0X63, 0X8E, 0X53, 0X4D,
  0X3A, 0XAA, 0X53, 0X0C, 0X4A, 0X68, 0X4A, 0XAA, 0X6B, 0X0B, 0X63, 0X6E,
  0X6C, 0X52, 0X6B, 0X8E, 0X6A, 0XCA, 0X73, 0XCF, 0X21, 0XE9, 0X5B, 0X0E,
  0X73, 0XAF, 0X4A, 0X29, 0X39, 0XC7, 0X62, 0XEB, 0X5A, 0X49, 0X39, 0XA5,
  0X52, 0X08, 0X39, 0XA7, 0X41, 0XE7, 0X49, 0XE7, 0X4A, 0X49, 0X6A, 0XEB,
  0X51, 0XE7, 0X5A, 0XEC, 0X4A, 0X49, 0X4A, 0X08, 0X4A, 0X09, 0X62, 0XCA,
  0X7B, 0X2A, 0X5A, 0XAA, 0X5A, 0XAA, 0X5A, 0XA9, 0X5A, 0X88, 0X5A, 0X68,
  0X62, 0XA8, 0X62, 0XC9, 0X52, 0X48, 0X41, 0XA6, 0X4A, 0X27, 0X52, 0X88,
  0X39, 0XA7, 0X41, 0XC8, 0X8B, 0X8B, 0X83, 0X4A, 0X41, 0X88, 0X5A, 0X49,
  0X83, 0X8B, 0X6B, 0X0A, 0X4A, 0X69, 0X63, 0X0C, 0X62, 0XA9, 0X83, 0X6B,
  0X8B, 0X8C, 0X94, 0X0F, 0X9C, 0X4F, 0X4A, 0X4A, 0X62, 0XCA, 0XA4, 0X6F,
  0X6A, 0XEA, 0X5A, 0X67, 0X8B, 0XCD, 0X8B, 0XEE, 0X52, 0X49, 0X73, 0X2B,
  0X5A, 0X67, 0X62, 0XE9, 0X6B, 0X0A, 0X39, 0X86, 0X39, 0X85, 0X62, 0XEA,
  0X8B, 0XAC, 0X73, 0X0A, 0X7B, 0X4A, 0X83, 0X6B, 0X93, 0X8B, 0X5A, 0X68,
  0X29, 0X87, 0X19, 0X26, 0X10, 0XE5, 0X18, 0XA4, 0X18, 0XC3, 0X19, 0X47,
  0X10, 0XC4, 0X18, 0XE4, 0X21, 0X45, 0X63, 0X4C, 0X73, 0XAE, 0X73, 0XCE,
  0X9C, 0X70, 0X73, 0X4C, 0X18, 0XE4, 0X63, 0X0C, 0X84, 0X2F, 0X6B, 0X8E,
  0X6B, 0XAF, 0X7C, 0X0F, 0X53, 0X0E, 0X32, 0X4B, 0X53, 0X2D, 0X6B, 0XAE,
  0X5B, 0X0C, 0X42, 0X49, 0X42, 0X29, 0X5B, 0X0C, 0X29, 0XC7, 0X21, 0XE8,
  0X3B, 0X0D, 0X53, 0X0F, 0X53, 0X0F, 0X63, 0X0E, 0X62, 0XEB, 0X53, 0X0D,
  0X43, 0X2F, 0X3A, 0X8B, 0X3A, 0X6A, 0X2A, 0X4A, 0X52, 0XEC, 0X42, 0XEC,
  0X63, 0X6F, 0X63, 0X6E, 0X63, 0X0D, 0X6B, 0X0D, 0X6B, 0X4E, 0X4A, 0XEC,
  0X63, 0X4D, 0X5B, 0X2D, 0X53, 0X0C, 0X4A, 0XEC, 0X53, 0X0C, 0X6B, 0X8E,
  0X73, 0XAE, 0X3A, 0XAC, 0X39, 0XA4, 0X5A, 0X67, 0X6B, 0X0C, 0X4A, 0XEC,
  0X33, 0X4F, 0X3A, 0XED, 0X5A, 0XCB, 0X42, 0X8A, 0X4A, 0XCB, 0X42, 0XCB,
  0X43, 0X2E, 0X32, 0X8A, 0X32, 0X6A, 0X31, 0XC9, 0X32, 0X4B, 0X31, 0XEA,
  0X3A, 0X8B, 0X53, 0X2C, 0X42, 0XEC, 0X42, 0XEC, 0X31, 0XC9, 0X39, 0XA9,
  0X3A, 0X8C, 0X3A, 0X8B, 0X4A, 0XCB, 0X3A, 0X0A, 0X42, 0X09, 0X32, 0X0A,
  0X42, 0X6A, 0X6B, 0XAE, 0X5B, 0X4F, 0X5B, 0X4D, 0X6B, 0X6F, 0X53, 0XF1,
  0X6B, 0XF0, 0X6B, 0X6F, 0X6B, 0X4E, 0X4A, 0X8A, 0X4A, 0XAB, 0X43, 0XAF,
  0X3A, 0XCC, 0X53, 0X0C, 0X5B, 0X2D, 0X53, 0X6E, 0X5B, 0X90, 0X5B, 0X6F,
  0X63, 0X8D, 0X5B, 0XF0, 0X54, 0X31, 0X5B, 0XCF, 0X63, 0X2C, 0X5C, 0X31,
  0X63, 0X4D, 0X5B, 0X8E, 0X63, 0XAE, 0X52, 0XCA, 0X52, 0X6A, 0X6B, 0XF1,
  0X74, 0X54, 0X63, 0X90, 0X4A, 0XAC, 0X62, 0X8C, 0X7B, 0X4F, 0X93, 0X0F,
  0X9A, 0XEE, 0X9B, 0X0F, 0XA2, 0XF0, 0XA3, 0X10, 0X93, 0X4F, 0X7A, 0XCE,
  0X4A, 0X0A, 0X21, 0X04, 0X5A, 0X4B, 0X7A, 0XAD, 0X7B, 0X0E, 0XB4, 0X53,
  0X69, 0XC7, 0X59, 0X66, 0X49, 0XA8, 0X49, 0X66, 0X42, 0X0A, 0X42, 0X6B,
  0X4A, 0X2A, 0X52, 0X29, 0X5A, 0X8A, 0X4A, 0X09, 0X41, 0XC7, 0X62, 0XCB,
  0X5A, 0X4A, 0X41, 0XA7, 0X39, 0XA5, 0X39, 0XC5, 0X49, 0XC7, 0X5A, 0X0A,
  0X49, 0XA8, 0X49, 0XA8, 0X8A, 0X6E, 0X8A, 0X8E, 0X62, 0X2C, 0X39, 0XEA,
  0X5A, 0X8C, 0X5A, 0X6B, 0X52, 0XAD, 0X3B, 0X31, 0X32, 0XCF, 0X19, 0XCB,
  0X2A, 0X4E, 0X42, 0XF0, 0X3A, 0XAE, 0X42, 0XCF, 0X32, 0X4C, 0X09, 0X25,
  0X09, 0X05, 0X31, 0X65, 0X39, 0X85, 0X21, 0X04, 0X08, 0X41, 0X29, 0X86,
  0X4A, 0XAB, 0X42, 0X8A, 0X4A, 0X8A, 0X42, 0X2A, 0X31, 0XA8, 0X29, 0X88,
  0X21, 0X47, 0X31, 0X87, 0X5A, 0X6A, 0X31, 0X88, 0X21, 0X48, 0X3A, 0X2B,
  0X29, 0X88, 0X10, 0X62, 0X08, 0X62, 0X10, 0X82, 0X08, 0X41, 0X08, 0XE5,
  0X11, 0X48, 0X08, 0XA3, 0X08, 0X62, 0X08, 0X62, 0X08, 0X62, 0X18, 0XE3,
  0X21, 0X24, 0X31, 0X65, 0X31, 0X87, 0X31, 0X24, 0X31, 0X65, 0X39, 0XA7,
  0X41, 0X87, 0X29, 0X04, 0X31, 0X25, 0X39, 0X66, 0X41, 0X67, 0X20, 0XC4,
  0X31, 0XC7, 0X5A, 0X8A, 0X52, 0X6A, 0X39, 0XE9, 0X3A, 0X08, 0X21, 0X67,
  0X21, 0X87, 0X39, 0XE9, 0X42, 0X2A, 0X42, 0X09, 0X42, 0X4B, 0X41, 0XEA,
  0X42, 0X4A, 0X42, 0X2A, 0X31, 0X88, 0X39, 0XEA, 0X42, 0X4B, 0X4A, 0X4A,
  0X4A, 0X09, 0X4A, 0X2A, 0X4A, 0X29, 0X42, 0X0A, 0X42, 0X8A, 0X42, 0XAA,
  0X42, 0X4A, 0X42, 0X29, 0X41, 0XC7, 0X39, 0X47, 0X52, 0X08, 0X49, 0XA7,
  0X62, 0X07, 0X59, 0XE7, 0X4A, 0X49, 0X39, 0XE8, 0X39, 0XE8, 0X52, 0X8A,
  0X31, 0XC8, 0X29, 0X67, 0X4A, 0X6A, 0X39, 0XC8, 0X10, 0XA3, 0X10, 0XA4,
  0X08, 0XA4, 0X10, 0XA4, 0X10, 0XE5, 0X00, 0XC5, 0X00, 0XC4, 0X00, 0XC4,
  0X00, 0XC4, 0X00, 0XC4, 0X00, 0XC4, 0X00, 0XC5, 0X08, 0XC5, 0X08, 0XE5,
  0X08, 0XE5, 0X08, 0XE5, 0X19, 0X05, 0X19, 0X05, 0X11, 0X05, 0X11, 0X05,
  0X10, 0XE5, 0X10, 0XC5, 0X10, 0XE6, 0X10, 0XE6, 0X10, 0XE7, 0X11, 0X07,
  0X19, 0X27, 0X19, 0X28, 0X29, 0XA9, 0X31, 0XEB, 0X3A, 0X2C, 0X3A, 0X2E,
  0X29, 0XAC, 0X21, 0X09, 0X18, 0XC8, 0X18, 0XA7, 0X20, 0XE8, 0X29, 0X29,
  0X29, 0X29, 0X29, 0X29, 0X29, 0X29, 0X29, 0X29, 0X31, 0X4A, 0X29, 0X09,
  0X29, 0X29, 0X29, 0X29, 0X29, 0X29, 0X21, 0X49, 0X29, 0X6A, 0X29, 0X8B,
  0X29, 0X6A, 0X21, 0X4A, 0X21, 0X6A, 0X21, 0X08, 0X29, 0X29, 0X29, 0X6A,
  0X29, 0X49, 0X21, 0X07, 0X21, 0X48, 0X29, 0X08, 0X10, 0XE4, 0X10, 0XA3,
  0X10, 0XC4, 0X10, 0XA3, 0X10, 0XA3, 0X18, 0XE4, 0X21, 0X66, 0X19, 0X46,
  0X11, 0X26, 0X19, 0X25, 0X19, 0X66, 0X19, 0X25, 0X19, 0X46, 0X19, 0X67,
  0X11, 0X05, 0X19, 0X46, 0X19, 0X47, 0X19, 0X46, 0X21, 0X66, 0X19, 0X25,
  0X18, 0XE4, 0X31, 0X86, 0X31, 0X86, 0X31, 0X86, 0X41, 0XA7, 0X39, 0X66,
  0X29, 0X66, 0X29, 0X45, 0X29, 0X04, 0X31, 0X65, 0X31, 0X65, 0X31, 0X67,
  0X39, 0X87, 0X41, 0X87, 0X31, 0X45, 0X29, 0X25, 0X31, 0X45, 0X31, 0X45,
  0X31, 0X66, 0X39, 0X87, 0X41, 0X87, 0X29, 0X45, 0X21, 0X25, 0X19, 0X04,
  0X21, 0X05, 0X31, 0XA8, 0X21, 0X26, 0X21, 0X05, 0X31, 0XA7, 0X31, 0XC8,
  0X39, 0XE9, 0X39, 0XE9, 0X39, 0XC7, 0X29, 0XA8, 0X39, 0XA7, 0X29, 0X87,
  0X29, 0X66, 0X31, 0X86, 0X41, 0XC6, 0X41, 0XC7, 0X49, 0XE8, 0X41, 0XE9,
  0X41, 0XC7, 0X49, 0XE8, 0X4A, 0X29, 0X42, 0X09, 0X4A, 0X08, 0X4A, 0X29,
  0X31, 0X87, 0X39, 0XC8, 0X41, 0XC7, 0X31, 0XA7, 0X29, 0X66, 0X29, 0X87,
  0X29, 0XA7, 0X41, 0XC8, 0X41, 0XC7, 0X29, 0X66, 0X31, 0X65, 0X41, 0XC7,
  0X39, 0XA7, 0X39, 0XC7, 0X52, 0X07, 0X51, 0XE8, 0X49, 0XE7, 0X49, 0XE8,
  0X4A, 0X49, 0X52, 0X69, 0X4A, 0X28, 0X4A, 0X29, 0X52, 0X48, 0X5A, 0X89,
  0X4A, 0X08, 0X39, 0X45, 0X4A, 0X07, 0X52, 0X28, 0X5A, 0X8A, 0X4A, 0X29,
  0X29, 0X45, 0X39, 0XA7, 0X4A, 0X08, 0X4A, 0X08, 0X41, 0XE7, 0X41, 0XE8,
  0X41, 0XE8, 0X4A, 0X2A, 0X5A, 0XAA, 0X4A, 0X09, 0X41, 0XE8, 0X41, 0XE8,
  0X41, 0XE8, 0X41, 0XE8, 0X49, 0XE7, 0X4A, 0X09, 0X52, 0X6B, 0X52, 0X29,
  0X49, 0XE7, 0X51, 0XC7, 0X41, 0XA6, 0X31, 0X45, 0X31, 0X87, 0X39, 0XA6,
  0X39, 0X44, 0X31, 0X25, 0X31, 0X25, 0X31, 0X45, 0X31, 0X45, 0X41, 0XA6,
  0X5A, 0X6A, 0X6A, 0XCC, 0X52, 0X29, 0X49, 0XA7, 0X41, 0X86, 0X41, 0X66,
  0X31, 0X46, 0X31, 0X46, 0X41, 0X66, 0X31, 0X66, 0X52, 0X4A, 0X6A, 0XAB,
  0X5A, 0X08, 0X39, 0X66, 0X21, 0X47, 0X29, 0XA9, 0X29, 0XC9, 0X21, 0XCA,
  0X22, 0X0B, 0X21, 0XCA, 0X31, 0X46, 0X29, 0XA8, 0X42, 0X09, 0X31, 0X87,
  0X29, 0X46, 0X29, 0X47, 0X19, 0X46, 0X11, 0X27, 0X29, 0X88, 0X5A, 0X48,
  0X49, 0XE7, 0X31, 0XA8, 0X31, 0X87, 0X49, 0XC8, 0X39, 0XC9, 0X41, 0XE8,
  0X29, 0X66, 0X21, 0X45, 0X29, 0X67, 0X31, 0XA8, 0X29, 0X88, 0X29, 0X66,
  0X39, 0XE8, 0X5B, 0X0C, 0X4A, 0X49, 0X39, 0X86, 0X52, 0XA9, 0X4A, 0XAA,
  0X32, 0X29, 0X52, 0XAA, 0X52, 0XAA, 0X4A, 0X8A, 0X42, 0X69, 0X42, 0X6A,
  0X42, 0X6A, 0X52, 0XAB, 0X3A, 0X07, 0X3A, 0X27, 0X29, 0XE7, 0X19, 0X66,
  0X21, 0XC8, 0X21, 0XC7, 0X2A, 0X09, 0X29, 0XE9, 0X29, 0XC8, 0X53, 0X2C,
  0X73, 0XAC, 0X63, 0X09, 0X32, 0X29, 0X2A, 0X2A, 0X2A, 0X09, 0X32, 0X08,
  0X2A, 0X09, 0X32, 0X6B, 0X42, 0X6A, 0X32, 0X4A, 0X4A, 0XEC, 0XAC, 0X8B,
  0XA4, 0X4A, 0X5A, 0XEA, 0X32, 0X6A, 0X53, 0X0B, 0X32, 0X4A, 0X32, 0X4B,
  0X4A, 0X89, 0X42, 0XCE, 0X83, 0X49, 0XAB, 0XA5, 0XAB, 0X85, 0XB3, 0XA6,
  0XCB, 0XE5, 0XC4, 0X46, 0XAB, 0XA8, 0XAB, 0X87, 0XA3, 0XA7, 0X52, 0XEA,
  0X83, 0X26, 0XBB, 0XE5, 0XCC, 0X26, 0X7B, 0X07, 0X4A, 0X27, 0XEC, 0X45,
  0X59, 0X82, 0X00, 0X00, 0X10, 0XE5, 0X19, 0X47, 0X11, 0X26, 0X08, 0XC5,
  0X20, 0XE4, 0X31, 0X04, 0X30, 0XE4, 0X41, 0X03, 0X41, 0X43, 0X41, 0X64,
  0X41, 0X44, 0X31, 0X24, 0X41, 0X23, 0X41, 0X23, 0X41, 0X44, 0X39, 0X24,
  0X39, 0X25, 0X31, 0X25, 0X39, 0X46, 0X39, 0X25, 0X49, 0X24, 0X41, 0X65,
  0X41, 0X25, 0X39, 0X45, 0X31, 0X25, 0X39, 0X25, 0X31, 0X45, 0X41, 0X44,
  0X31, 0X24, 0X41, 0X45, 0X49, 0X66, 0X29, 0X46, 0X39, 0X46, 0X41, 0X67,
  0X29, 0X68, 0X29, 0X47, 0X29, 0X47, 0X29, 0X47, 0X29, 0X27, 0X31, 0X26,
  0X41, 0X66, 0X49, 0XC8, 0X41, 0X87, 0X29, 0X46, 0X29, 0X46, 0X29, 0X67,
  0X39, 0XA8, 0X49, 0X86, 0X49, 0X87, 0X41, 0XC9, 0X41, 0XA9, 0X39, 0X68,
  0X49, 0XC8, 0X5A, 0X08, 0X5A, 0X29, 0X41, 0XEA, 0X4A, 0X4B, 0X4A, 0X4A,
  0X4A, 0X08, 0X51, 0XA5, 0X52, 0X4A, 0X52, 0XAC, 0X52, 0X8B, 0X39, 0XCA,
  0X52, 0X8B, 0X52, 0X4A, 0X49, 0XA6, 0X41, 0X87, 0X41, 0X88, 0X41, 0X87,
  0X41, 0X67, 0X31, 0X25, 0X39, 0X45, 0X51, 0X85, 0X59, 0X85, 0X49, 0X87,
  0X49, 0XA8, 0X51, 0X86, 0X49, 0X66, 0X51, 0XA6, 0X49, 0XA6, 0X49, 0XA5,
  0X49, 0X86, 0X49, 0X85, 0X39, 0X24, 0X39, 0X45, 0X39, 0X46, 0X39, 0X45,
  0X41, 0X45, 0X51, 0X65, 0X51, 0X65, 0X49, 0X45, 0X49, 0XC8, 0X4A, 0X0B,
  0X49, 0XC9, 0X41, 0X66, 0X39, 0X45, 0X29, 0X46, 0X21, 0X27, 0X19, 0X28,
  0X19, 0X69, 0X21, 0X48, 0X21, 0X47, 0X29, 0X26, 0X19, 0X26, 0X08, 0XE6,
  0X08, 0XA4, 0X11, 0X26, 0X29, 0XA9, 0X11, 0X06, 0X19, 0X27, 0X19, 0X26,
  0X18, 0XE4, 0X21, 0X05, 0X21, 0X25, 0X21, 0X46, 0X21, 0X25, 0X21, 0X25,
  0X21, 0X25, 0X21, 0X05, 0X21, 0X46, 0X18, 0XE5, 0X18, 0XE4, 0X18, 0XE4,
  0X18, 0XE4, 0X21, 0X05, 0X21, 0X25, 0X21, 0X26, 0X21, 0X25, 0X21, 0X25,
  0X19, 0X06, 0X19, 0X27, 0X19, 0X47, 0X19, 0X47, 0X29, 0XC9, 0X29, 0XCA,
  0X29, 0XEA, 0X19, 0X47, 0X11, 0X26, 0X11, 0X06, 0X11, 0X06, 0X10, 0XE5,
  0X11, 0X06, 0X10, 0XE5, 0X19, 0X26, 0X31, 0XA8, 0X31, 0XA8, 0X21, 0X25,
  0X21, 0X25, 0X19, 0X05, 0X19, 0X05, 0X21, 0X25, 0X19, 0X04, 0X21, 0X05,
  0X21, 0X26, 0X19, 0X05, 0X21, 0X26, 0X18, 0XE5, 0X18, 0XE5, 0X18, 0XE4,
  0X11, 0X05, 0X10, 0XE5, 0X39, 0X25, 0X31, 0X86, 0X29, 0X25, 0X39, 0X45,
  0X30, 0XE4, 0X31, 0X04, 0X39, 0X24, 0X41, 0X24, 0X41, 0X44, 0X49, 0X85,
  0X41, 0X65, 0X41, 0X86, 0X41, 0X87, 0X31, 0X26, 0X41, 0X45, 0X41, 0X45,
  0X51, 0X85, 0X49, 0X66, 0X31, 0X05, 0X41, 0X45, 0X41, 0X44, 0X31, 0X03,
  0X41, 0X44, 0X41, 0X65, 0X41, 0X64, 0X51, 0X85, 0X61, 0XA5, 0X59, 0XA4,
  0X51, 0XA5, 0X61, 0XA5, 0X41, 0X85, 0X51, 0X85, 0X49, 0X64, 0X49, 0X85,
  0X51, 0XA6, 0X49, 0X85, 0X49, 0X86, 0X39, 0X25, 0X31, 0X05, 0X28, 0XC4,
  0X29, 0X67, 0X10, 0XC4, 0X00, 0X00, 0X00, 0X00, 0X40, 0XE3, 0X81, 0XC5,
  0X79, 0XA5, 0X69, 0XA5, 0X72, 0X26, 0X6A, 0X27, 0X9A, 0XC6, 0X7A, 0X46,
  0X51, 0XC7, 0X7A, 0X26, 0X7A, 0X67, 0X7A, 0X88, 0X92, 0XA7, 0XB2, 0XC6,
  0X59, 0XA5, 0X39, 0X45, 0X41, 0X65, 0X39, 0X45, 0X39, 0X45, 0X39, 0X65,
  0X41, 0X86, 0X41, 0X86, 0X41, 0X86, 0X41, 0X86, 0X41, 0X87, 0X49, 0X65,
  0X51, 0X85, 0X59, 0XC7, 0X5A, 0X0A, 0X59, 0XE9, 0X59, 0XC8, 0X51, 0XC9,
  0X59, 0X87, 0X59, 0X87, 0X69, 0XA6, 0X61, 0X86, 0X41, 0XC8, 0X41, 0X87,
  0X31, 0X46, 0X39, 0X88, 0X6A, 0X26, 0X82, 0X65, 0X7A, 0X45, 0X7A, 0X65,
  0X59, 0XC5, 0X39, 0X67, 0X41, 0XA7, 0X39, 0X86, 0X39, 0XC8, 0X08, 0X41,
  0X18, 0XA1, 0X62, 0X87, 0X5A, 0X46, 0X5A, 0X67, 0X62, 0XA7, 0X62, 0XA7,
  0X62, 0XA7, 0X62, 0X86, 0X62, 0XA6, 0X6A, 0XA6, 0X6A, 0XC6, 0X6A, 0XC7,
  0X5A, 0X45, 0X52, 0X47, 0X52, 0X67, 0X52, 0X68, 0X5A, 0XE8, 0X63, 0X48,
  0X63, 0X48, 0X52, 0XA7, 0X6B, 0X28, 0X7B, 0XA8, 0X7B, 0XA9, 0X7B, 0XA9,
  0X93, 0XE8, 0X83, 0XA8, 0X73, 0X69, 0X7B, 0X89, 0X6B, 0X29, 0X8B, 0XCA,
  0X9C, 0X2A, 0X9C, 0X2A, 0X94, 0X09, 0X83, 0X88, 0X83, 0X88, 0X83, 0XA9,
  0X83, 0XA9, 0X83, 0X88, 0X83, 0X68, 0X83, 0X88, 0X7B, 0X6B, 0X7B, 0X6A,
  0X8B, 0XC9, 0X94, 0X0B, 0X94, 0X0B, 0X8B, 0XCB, 0X93, 0XC9, 0X8B, 0XAA,
  0X83, 0X68, 0X7B, 0X47, 0X7B, 0X88, 0X72, 0XC8, 0X52, 0X26, 0X52, 0XE7,
  0X6B, 0X2A, 0X63, 0X2A, 0X6B, 0X29, 0X6B, 0X28, 0X41, 0XE6, 0X31, 0XA5,
  0X32, 0X29, 0X32, 0X8B, 0X32, 0X6A, 0X32, 0X6A, 0X32, 0X6A, 0X32, 0X6B,
  0X22, 0X4B, 0X2A, 0XAC, 0X32, 0XCC, 0X32, 0XAC, 0X43, 0X0E, 0X32, 0XAD,
  0X2A, 0X8D, 0X32, 0XAD, 0X3A, 0XCD, 0X3A, 0XED, 0X2A, 0XCC, 0X32, 0XEC,
  0X4B, 0X8E, 0X42, 0XCC, 0X42, 0XAB, 0X32, 0X6A, 0X32, 0X6A, 0X43, 0X0C,
  0X43, 0X4E, 0X3B, 0X0E, 0X32, 0X8B, 0X32, 0XAC, 0X32, 0XEE, 0X32, 0XCD,
  0X32, 0XCC, 0X32, 0XCC, 0X2A, 0XCD, 0X2B, 0X0E, 0X2B, 0X0E, 0X3B, 0X6F,
  0X33, 0X2E, 0X33, 0X0E, 0X3A, 0XCE, 0X32, 0X6C, 0X3A, 0X6A, 0X42, 0XAB,
  0X43, 0X2D, 0X43, 0X2D, 0X43, 0X6E, 0X4B, 0X6F, 0X4B, 0X2D, 0X3A, 0X8A,
  0X42, 0X8A, 0X4A, 0XCB, 0X4A, 0XCB, 0X42, 0X89, 0X53, 0XCF, 0X54, 0X71,
  0X54, 0X2F, 0X4C, 0X0F, 0X3B, 0X4D, 0X3B, 0X4D, 0X3B, 0X0C, 0X3B, 0X2D,
  0X3A, 0XEC, 0X22, 0X08, 0X33, 0X11, 0X43, 0XD8, 0X43, 0XB7, 0X3B, 0X33,
  0X3A, 0XEA, 0X53, 0XAD, 0X3B, 0X4D, 0X33, 0X4D, 0X32, 0X29, 0X21, 0XC8,
  0X32, 0X4A, 0X32, 0X4A, 0X2A, 0X49, 0X11, 0X65, 0X08, 0XA2, 0X08, 0XC3,
  0X11, 0X66, 0X22, 0X68, 0X33, 0X6B, 0X3B, 0XAB, 0X19, 0XE7, 0X08, 0XC3,
  0X08, 0X61, 0X29, 0XE9, 0X32, 0X2B, 0X31, 0XC8, 0X3A, 0X29, 0X42, 0X6B,
  0X32, 0X09, 0X29, 0XC8, 0X29, 0XE8, 0X32, 0X8A, 0X32, 0X6A, 0X1A, 0X09,
  0X32, 0XAB, 0X32, 0XCC, 0X32, 0XAD, 0X2A, 0X4A, 0X21, 0XC8, 0X19, 0XC8,
  0X19, 0XC8, 0X19, 0XA6, 0X22, 0X09, 0X22, 0X09, 0X22, 0X2A, 0X22, 0X2A,
  0X12, 0X09, 0X1A, 0X2C, 0X21, 0XEB, 0X19, 0XCA, 0X22, 0X4D, 0X1A, 0XAE,
  0X22, 0X2A, 0X2A, 0X2B, 0X2A, 0X6C, 0X22, 0X09, 0X2A, 0X4A, 0X2A, 0X6A,
  0X32, 0X29, 0X2A, 0X08, 0X2A, 0X49, 0X2A, 0X49, 0X2A, 0X89, 0X2A, 0X8A,
  0X2A, 0X4A, 0X22, 0X09, 0X21, 0XC7, 0X21, 0XC9, 0X21, 0XC9, 0X22, 0X0A,
  0X22, 0X2A, 0X21, 0XA8, 0X21, 0XE9, 0X29, 0X86, 0X29, 0XE8, 0X22, 0X29,
  0X22, 0X4A, 0X2A, 0X49, 0X21, 0XA7, 0X32, 0X4A, 0X32, 0X4A, 0X21, 0XC8,
  0X19, 0X65, 0X31, 0XA6, 0X2A, 0X6A, 0X2A, 0X0A, 0X2A, 0X6A, 0X32, 0X09,
  0X2A, 0X49, 0X2A, 0X2A, 0X22, 0X4B, 0X2A, 0X4B, 0X6B, 0X2C, 0X83, 0X8A,
  0X73, 0X2B, 0X73, 0X4C, 0X6A, 0XEA, 0X6B, 0X4F, 0X73, 0X8F, 0X53, 0X0C,
  0X3A, 0XAA, 0X53, 0XB0, 0X5B, 0XB2, 0X63, 0X70, 0X5B, 0X90, 0X2A, 0XCD,
  0X32, 0XAC, 0X32, 0X8B, 0X22, 0X8B, 0X2A, 0X48, 0X22, 0X6A, 0X7C, 0X70,
  0XA4, 0X4D, 0X63, 0XCD, 0X4B, 0XD0, 0X4B, 0XAF, 0X74, 0X0F, 0X5B, 0X4D,
  0X4A, 0XAA, 0X42, 0XA9, 0X63, 0X2A, 0X63, 0X2A, 0X4A, 0XA9, 0X32, 0X06,
  0X4A, 0X8A, 0X42, 0X48, 0X3A, 0X27, 0X42, 0X27, 0X42, 0X68, 0X4A, 0XAA,
  0X42, 0X89, 0X3A, 0XAA, 0X3A, 0XAA, 0X3A, 0XCA, 0X3A, 0XA9, 0X3A, 0X49,
  0X4A, 0XEA, 0X43, 0X29, 0X42, 0XEB, 0X4B, 0X4B, 0X43, 0X6A, 0X43, 0X2B,
  0X43, 0X0B, 0X5B, 0XEC, 0X5B, 0XAD, 0X5B, 0XAD, 0X53, 0X8C, 0X4B, 0X8B,
  0X4B, 0X4A, 0X4A, 0XA9, 0X4A, 0X8B, 0X39, 0XC8, 0X39, 0XA8, 0X41, 0XE7,
  0X4A, 0X07, 0X41, 0XE9, 0X4A, 0X07, 0X52, 0X49, 0X5A, 0X6A, 0X4A, 0X08,
  0X39, 0XA8, 0X31, 0X86, 0X31, 0X65, 0X18, 0XA2, 0X08, 0X41, 0X29, 0X65,
  0X29, 0X65, 0X31, 0X65, 0X31, 0X85, 0X29, 0X24, 0X21, 0X04, 0X21, 0X04,
  0X29, 0X66, 0X29, 0X86, 0X29, 0X66, 0X21, 0X25, 0X31, 0X86, 0X31, 0XA9,
  0X31, 0XA8, 0X29, 0X44, 0X29, 0X66, 0X29, 0X66, 0X29, 0X46, 0X31, 0X87,
  0X31, 0X66, 0X29, 0X66, 0X31, 0XA7, 0X31, 0X86, 0X31, 0X86, 0X31, 0X86,
  0X29, 0X66, 0X29, 0X46, 0X21, 0X25, 0X29, 0X45, 0X21, 0X26, 0X29, 0X46,
  0X31, 0XA8, 0X31, 0X87, 0X31, 0X87, 0X31, 0XA7, 0X31, 0X67, 0X31, 0XA8,
  0X31, 0X87, 0X31, 0XA8, 0X32, 0X0A, 0X31, 0XA7, 0X31, 0X87, 0X31, 0XC8,
  0X39, 0XC8, 0X31, 0X86, 0X29, 0X45, 0X42, 0X29, 0X39, 0XC7, 0X31, 0X86,
  0X31, 0X65, 0X10, 0X82, 0X00, 0X00, 0X21, 0X86, 0X32, 0X69, 0X32, 0X69,
  0X3A, 0XCA, 0X43, 0X6C, 0X4B, 0XCF, 0X54, 0XF8, 0X54, 0XF8, 0X5C, 0XF6,
  0X5C, 0X92, 0X64, 0XD6, 0X64, 0XF3, 0X5D, 0X37, 0X54, 0XB7, 0X5C, 0XB6,
  0X54, 0X96, 0X54, 0X96, 0X54, 0X95, 0X54, 0X75, 0X64, 0XD6, 0X54, 0X75,
  0X64, 0X34, 0X6C, 0XD8, 0X4B, 0X90, 0X42, 0X47, 0X6C, 0X95, 0X6D, 0X58,
  0X44, 0X12, 0X33, 0X2B, 0X43, 0XEE, 0X4C, 0X2F, 0X5B, 0X8D, 0X4C, 0X11,
  0X5C, 0X0F, 0X64, 0X0F, 0X5B, 0XEE, 0X64, 0X0E, 0X6C, 0XB3, 0X64, 0XF6,
  0X5C, 0XB6, 0X4B, 0XF1, 0X53, 0XF0, 0X5C, 0X10, 0X6C, 0X94, 0X6C, 0X52,
  0X6B, 0XD0, 0X64, 0X72, 0X5C, 0X53, 0X63, 0XF2, 0X5B, 0XF1, 0X43, 0XAC,
  0X4C, 0X50, 0X63, 0XEF, 0X6B, 0XEF, 0X6C, 0X11, 0X6B, 0XEF, 0X5C, 0X30,
  0X64, 0X30, 0X64, 0X0F, 0X6C, 0X30, 0X53, 0X91, 0X4B, 0X92, 0X4B, 0X0D,
  0X3B, 0X0B, 0X5C, 0X11, 0X64, 0X31, 0X64, 0X12, 0X63, 0XF1, 0X63, 0XF1,
  0X5C, 0X11, 0X6B, 0X2D, 0X63, 0X0B, 0X53, 0X6D, 0X53, 0X2B, 0X5C, 0XB4,
  0X5C, 0X71, 0X64, 0X70, 0X6C, 0X0F, 0X64, 0X72, 0X64, 0X31, 0X6B, 0X6C,
  0X4B, 0X4C, 0X43, 0X2B, 0X4B, 0X6C, 0X53, 0X6C, 0X5B, 0X4C, 0X63, 0XAE,
  0X53, 0XEF, 0X4B, 0XED, 0X4B, 0X8D, 0X5C, 0X31, 0X54, 0X6F, 0X65, 0X76,
  0X65, 0X78, 0X65, 0X54, 0X65, 0X97, 0X65, 0X78, 0X65, 0X79, 0X65, 0X16,
  0X53, 0XCD, 0X53, 0XAC, 0X43, 0X2B, 0X4C, 0X32, 0X4C, 0XB4, 0X4B, 0XEF,
  0X4B, 0X8B, 0X53, 0X07, 0X53, 0X49, 0X5C, 0X0F, 0X64, 0XB2, 0X6C, 0XB4,
  0X6C, 0XD2, 0X4B, 0X6E, 0X5C, 0X12, 0X64, 0X32, 0X4A, 0XEA, 0X4A, 0XA9,
  0X53, 0X4B, 0X75, 0X34, 0X74, 0XF5, 0X53, 0X6B, 0X4A, 0XC9, 0X53, 0XEF,
  0X54, 0X10, 0X5C, 0X4F, 0X74, 0X8C, 0X53, 0X8A, 0X64, 0X4D, 0X64, 0X8E,
  0X64, 0X2D, 0X63, 0XEC, 0X64, 0X0D, 0X64, 0X0D, 0X63, 0XED, 0X5B, 0XCC,
  0X63, 0XEC, 0X5B, 0XAC, 0X5B, 0XEE, 0X63, 0XEF, 0X6C, 0X2D, 0X6C, 0X4C,
  0X6C, 0X6D, 0X6C, 0X0E, 0X6B, 0XEE, 0X6C, 0X0E, 0X6C, 0X0E, 0X6C, 0X4E,
  0X74, 0XEE, 0X64, 0X6E, 0X64, 0X2F, 0X64, 0X2E, 0X5C, 0X0D, 0X5C, 0X0C,
  0X5C, 0X0E, 0X6C, 0X4D, 0X5B, 0XCC, 0X6B, 0XEB, 0X53, 0X4B, 0X63, 0XAB,
  0X6B, 0XCC, 0X63, 0X4A, 0X5B, 0X8A, 0X54, 0X4F, 0X64, 0X91, 0X64, 0XD1,
  0X5B, 0XCB, 0X5B, 0XCD, 0X5C, 0X2F, 0X53, 0XCC, 0X5B, 0XAD, 0X53, 0X09,
  0X4A, 0XE9, 0X5B, 0X8C, 0X08, 0XE6, 0X08, 0XC5, 0X08, 0XA4, 0X08, 0XC5,
  0X08, 0XE6, 0X00, 0XA4, 0X09, 0X06, 0X09, 0X06, 0X09, 0X06, 0X08, 0XC5,
  0X08, 0XE5, 0X10, 0XC4, 0X11, 0X26, 0X11, 0X89, 0X09, 0X27, 0X08, 0XE5,
  0X08, 0XE5, 0X09, 0X27, 0X09, 0X27, 0X09, 0X47, 0X09, 0X48, 0X08, 0XE6,
  0X11, 0X27, 0X08, 0XE5, 0X08, 0XC4, 0X08, 0XC4, 0X11, 0X26, 0X11, 0X06,
  0X08, 0XE7, 0X08, 0XC5, 0X08, 0XC5, 0X08, 0XC5, 0X08, 0XC5, 0X08, 0XC5,
  0X08, 0XC5, 0X08, 0XE6, 0X09, 0X07, 0X08, 0XC5, 0X10, 0XE5, 0X08, 0XC5,
  0X08, 0XA5, 0X08, 0XC5, 0X11, 0X28, 0X11, 0X28, 0X11, 0X07, 0X11, 0X06,
  0X11, 0X07, 0X11, 0X07, 0X11, 0X07, 0X11, 0X26, 0X11, 0X06, 0X11, 0X07,
  0X11, 0X07, 0X11, 0X07, 0X11, 0X07, 0X11, 0X06, 0X10, 0XE6, 0X10, 0XE6,
  0X10, 0XE6, 0X10, 0XE6, 0X11, 0X07, 0X11, 0X07, 0X11, 0X07, 0X09, 0X08,
  0X09, 0X29, 0X08, 0XE7, 0X08, 0XE6, 0X09, 0X08, 0X09, 0X28, 0X11, 0X48,
  0X11, 0X27, 0X10, 0XE6, 0X11, 0X07, 0X11, 0X06, 0X11, 0X05, 0X11, 0X07,
  0X11, 0X07, 0X11, 0X07, 0X10, 0XE7, 0X10, 0XE6, 0X11, 0X07, 0X10, 0XE7,
  0X10, 0XE6, 0X11, 0X07, 0X11, 0X28, 0X08, 0XE6, 0X09, 0X09, 0X09, 0X08,
  0X09, 0X09, 0X09, 0X09, 0X09, 0X2A, 0X08, 0XE8, 0X08, 0XE8, 0X09, 0X09,
  0X09, 0X29, 0X09, 0X29, 0X09, 0X2A, 0X08, 0XC7, 0X08, 0XC7, 0X08, 0XE8,
  0X09, 0X09, 0X08, 0XC7, 0X08, 0XC5, 0X00, 0XC8, 0X08, 0XC8, 0X08, 0XC7,
  0X11, 0X07, 0X11, 0X27, 0X11, 0X06, 0X08, 0XC5, 0X08, 0XC6, 0X08, 0XC6,
  0X08, 0XA5, 0X10, 0XC4, 0X10, 0XC5, 0X10, 0XE6, 0X10, 0XC6, 0X08, 0XE7,
  0X08, 0XE9, 0X08, 0XE9, 0X09, 0X09, 0X08, 0XE7, 0X08, 0XE8, 0X08, 0XE8,
  0X08, 0X84, 0X08, 0X84, 0X00, 0XA6, 0X00, 0XC7, 0X00, 0XC7, 0X08, 0XE8,
  0X08, 0XE8, 0X09, 0X08, 0X08, 0XE8, 0X00, 0X63, 0X00, 0X00, 0X00, 0X00,
  0X08, 0X41, 0X5A, 0XCD, 0X6B, 0X2E, 0X63, 0X0C, 0X5A, 0XAB, 0X52, 0X8A,
  0X5A, 0XAC, 0X52, 0X6B, 0X52, 0X6A, 0X52, 0X6B, 0X4A, 0X6B, 0X52, 0X8C,
  0X63, 0X2E, 0X63, 0X2B, 0X73, 0X6D, 0X5A, 0XCB, 0X63, 0X4D, 0X63, 0X6D,
  0X63, 0X2C, 0X6B, 0X4D, 0X63, 0X0C, 0X52, 0XA9, 0X6B, 0X06, 0X5A, 0X86,
  0X41, 0XE4, 0X62, 0XC6, 0X72, 0XE8, 0X6A, 0XA8, 0X6A, 0XA8, 0X6A, 0XC8,
  0X6A, 0XA8, 0X6A, 0XA9, 0X6A, 0XC9, 0X72, 0XEA, 0X7B, 0X2B, 0X7B, 0X2B,
  0X73, 0X0A, 0X73, 0X0A, 0X62, 0XA9, 0X62, 0X88, 0X62, 0X89, 0X62, 0X69,
  0X62, 0XA9, 0X6B, 0X0A, 0X6A, 0XEA, 0X72, 0XE9, 0X7B, 0X29, 0X72, 0XE9,
  0X6A, 0XA9, 0X6A, 0XC9, 0X62, 0X89, 0X5A, 0X69, 0X5A, 0X69, 0X5A, 0X48,
  0X5A, 0X8A, 0X5A, 0XAA, 0X4A, 0X07, 0X5A, 0XAA, 0X5A, 0XAA, 0X5A, 0XA9,
  0X5A, 0XA9, 0X52, 0X68, 0X52, 0X48, 0X5A, 0X69, 0X49, 0XE6, 0X49, 0XC6,
  0X4A, 0X07, 0X5A, 0X8A, 0X62, 0XCA, 0X7B, 0X4C, 0X73, 0X4C, 0X5A, 0XAA,
  0X5A, 0X89, 0X62, 0XEB, 0X62, 0XEB, 0X5A, 0XCA, 0X52, 0XAA, 0X52, 0X68,
  0X4A, 0X47, 0X52, 0X88, 0X6B, 0X6E, 0X6B, 0X4D, 0X62, 0XEB, 0X6B, 0X2C,
  0X6B, 0X2C, 0X6B, 0X2C, 0X6B, 0X4D, 0X73, 0X4D, 0X6B, 0X4D, 0X73, 0X6D,
  0X73, 0X6D, 0X73, 0X6D, 0X73, 0X4D, 0X73, 0X4D, 0X73, 0X6E, 0X73, 0X6E,
  0X83, 0XAF, 0X83, 0XD0, 0X83, 0XD0, 0X83, 0XCF, 0X83, 0X8E, 0X7B, 0X6E,
  0X7B, 0X6E, 0X83, 0XAE, 0X83, 0X8E, 0X62, 0X8A, 0X6B, 0X0C, 0X52, 0X27,
  0X52, 0X27, 0X5A, 0X28, 0X5A, 0X68, 0X62, 0XEC, 0X7B, 0X6E, 0X73, 0X2C,
  0X62, 0XCB, 0X62, 0XCB, 0X62, 0XCB, 0X52, 0X8A, 0X5A, 0XE9, 0X73, 0X6C,
  0X73, 0X4E, 0X7B, 0XAF, 0X7B, 0XF0, 0X7B, 0XAF, 0X73, 0X4D, 0X7B, 0X8E,
  0X7B, 0X8E, 0X7B, 0X6E, 0X7B, 0XAE, 0X62, 0XEB, 0X62, 0XCA, 0X6A, 0XEB,
  0X5A, 0XAA, 0X4A, 0X29, 0X4A, 0X08, 0X4A, 0X28, 0X52, 0X49, 0X52, 0X6A,
  0X52, 0X4A, 0X4A, 0X08, 0X5A, 0XAB, 0X6B, 0X0D, 0X5A, 0X8A, 0X5A, 0X8A,
  0X62, 0X8A, 0X6A, 0XEA, 0X62, 0XA9, 0X41, 0XE6, 0X4A, 0X47, 0X6B, 0X2A,
  0X62, 0XCA, 0X73, 0X6E, 0X6B, 0X0C, 0X5A, 0X89, 0X4A, 0X28, 0X49, 0XE7,
  0X39, 0X85, 0X5A, 0XCA, 0X52, 0X48, 0X4A, 0X27, 0X5A, 0XA9, 0X73, 0X6D,
  0X63, 0X0C, 0X73, 0X8D, 0X7C, 0X0F, 0X6B, 0X0A, 0X42, 0X66, 0X4A, 0XA7,
  0X4A, 0X65, 0X42, 0X86, 0X3A, 0X69, 0X3A, 0X28, 0X32, 0X27, 0X4A, 0XC8,
  0X42, 0X89, 0X29, 0XA6, 0X4A, 0XA7, 0X5B, 0X28, 0X52, 0X88, 0X5B, 0X28,
  0X5B, 0X6A, 0X4B, 0X0E, 0X4B, 0X0D, 0X4B, 0X0D, 0X4A, 0XEC, 0X5B, 0X6A,
  0X53, 0X49, 0X6B, 0X8B, 0X6B, 0X49, 0X6B, 0X69, 0X62, 0XC6, 0X62, 0XE7,
  0X6A, 0XE8, 0X73, 0X09, 0X72, 0XEA, 0X73, 0X0A, 0X6B, 0X0A, 0X6B, 0X2A,
  0X73, 0X69, 0X63, 0X07, 0X6B, 0X4A, 0X6B, 0X4B, 0X63, 0X29, 0X63, 0X29,
  0X63, 0X29, 0X5B, 0X08, 0X4A, 0XC7, 0X4A, 0X88, 0X83, 0X8D, 0X5A, 0XC9,
  0X29, 0XC5, 0X31, 0XE5, 0X29, 0XC5, 0X31, 0XC4, 0X00, 0X61, 0X08, 0XC2,
  0X42, 0X46, 0X5A, 0XE9, 0X6B, 0X4A, 0X6B, 0X6A, 0X63, 0X29, 0X5B, 0X6C,
  0X5B, 0XCD, 0X4A, 0XC7, 0X4A, 0XA7, 0X4A, 0XC7, 0X4A, 0XA7, 0X4A, 0XA7,
  0X5B, 0X6B, 0X63, 0XCE, 0X63, 0XCE, 0X5B, 0X29, 0X53, 0X08, 0X4A, 0XE8,
  0X4A, 0XE8, 0X4A, 0XE8, 0X3A, 0XC8, 0X4B, 0X08, 0X42, 0XE8, 0X5B, 0X28,
  0X63, 0X29, 0X6B, 0X49, 0X6B, 0X69, 0X7B, 0XAA, 0X6B, 0X28, 0X52, 0XA7,
  0X6B, 0X48, 0X73, 0X8A, 0X7B, 0XC9, 0X84, 0X0A, 0X73, 0XA9, 0X5A, 0XE7,
  0X5B, 0X28, 0X6B, 0X6B, 0X6B, 0X6A, 0X7B, 0XCB, 0X63, 0X69, 0X5B, 0X69,
  0X5B, 0X89, 0X5B, 0X49, 0X7B, 0X8B, 0X63, 0X2A, 0X63, 0X8D, 0X63, 0X4D,
  0X52, 0XAA, 0X52, 0XA9, 0X5A, 0XE9, 0X63, 0X0A, 0X6B, 0X4A, 0X7C, 0X0C,
  0X6B, 0X4A, 0X5B, 0X08, 0X63, 0X4C, 0X6B, 0XAD, 0X73, 0XEC, 0X63, 0X2C,
  0X73, 0XAC, 0X73, 0X6A, 0X63, 0X4A, 0X6B, 0X6B, 0X63, 0X29, 0X73, 0X49,
  0X73, 0XCA, 0X5B, 0X49, 0X6B, 0X4A, 0X6B, 0X2A, 0X6B, 0X4A, 0X6B, 0X4A,
  0X6B, 0X4A, 0X6B, 0X4A, 0X63, 0X69, 0X5B, 0X08, 0X42, 0X05, 0X52, 0X88,
  0X62, 0XE8, 0X5A, 0X87, 0X5A, 0XA7, 0X63, 0X07, 0X63, 0X09, 0X52, 0XAA,
  0X5A, 0X87, 0X6A, 0XE7, 0X62, 0XC7, 0X6B, 0X08, 0X7B, 0XCB, 0X7C, 0X2B,
  0X73, 0XEB, 0X73, 0XEB, 0X6B, 0XAA, 0X63, 0X69, 0X63, 0X6A, 0X73, 0XEB,
  0X7C, 0X2B, 0X74, 0X2B, 0X74, 0X0B, 0X73, 0XEA, 0X5B, 0X08, 0X63, 0X29,
  0X73, 0XCB, 0X6B, 0XAA, 0X6B, 0XAA, 0X6B, 0XAA, 0X63, 0X49, 0X52, 0XE8,
  0X5B, 0X28, 0X5B, 0X28, 0X73, 0XC9, 0X7B, 0XEC, 0X5B, 0X08, 0X53, 0X07,
  0X53, 0XAA, 0X6C, 0X2C, 0X7C, 0X2B, 0X5B, 0X07, 0X52, 0XA6, 0X52, 0XC7,
  0X52, 0X67, 0X52, 0X46, 0X49, 0XE5, 0X52, 0XA8, 0X4A, 0X26, 0X5B, 0X0A,
  0X5A, 0XA8, 0X73, 0X06, 0X7B, 0X67, 0X63, 0X26, 0X6B, 0X46, 0X63, 0X26,
  0X63, 0X26, 0X6B, 0X46, 0X6B, 0X46, 0X6B, 0XA9, 0X6B, 0X68, 0X6B, 0X68,
  0X5B, 0X28, 0X63, 0X48, 0X52, 0XE7, 0X53, 0X07, 0X63, 0X69, 0X5B, 0X49,
  0X53, 0X48, 0X53, 0X28, 0X5B, 0XCA, 0X5B, 0X27, 0X5B, 0XAA, 0X5B, 0XAA,
  0X5B, 0X8A, 0X4B, 0X08, 0X4A, 0XC7, 0X53, 0X08, 0X63, 0X68, 0X53, 0X69,
  0X4B, 0X69, 0X53, 0X69, 0X63, 0XA9, 0X6C, 0X4A, 0X6C, 0X4A, 0X7C, 0X48,
  0X6C, 0X09, 0X6B, 0XE9, 0X6C, 0X09, 0X6B, 0X88, 0X63, 0X07, 0X5B, 0X87,
  0X63, 0XA7, 0X63, 0X87, 0X63, 0XA7, 0X73, 0XE9, 0X8C, 0X8C, 0X52, 0XE7,
  0X39, 0XE4, 0X52, 0XC6, 0X42, 0X05, 0X41, 0XC5, 0X49, 0XE5, 0X52, 0X46,
  0X53, 0X09, 0X5B, 0X8C, 0X53, 0X4B, 0X52, 0XEB, 0X52, 0XEB, 0X4A, 0XAB,
  0X4A, 0XEC, 0X53, 0X2E, 0X53, 0X0C, 0X5B, 0X29, 0X63, 0XEC, 0X64, 0X0C,
  0X74, 0X2F, 0X7C, 0X31, 0X6B, 0XCF, 0X52, 0XEB, 0X5A, 0XEA, 0X73, 0XED,
  0X6B, 0X4B, 0X62, 0XCA, 0X6B, 0X0B, 0X7B, 0XAD, 0X6B, 0X6C, 0X63, 0X2C,
  0X5B, 0X2D, 0X52, 0XEC, 0X52, 0XC8, 0X4A, 0X87, 0X73, 0X89, 0X7B, 0XAB,
  0X52, 0XEB, 0X52, 0XC9, 0X42, 0X08, 0X29, 0X25, 0X29, 0X65, 0X4A, 0X8A,
  0X5B, 0X0B, 0X4A, 0XCB, 0X3A, 0X6A, 0X4A, 0XA8, 0X2A, 0X4A, 0X29, 0XE9,
  0X43, 0X0A, 0X6C, 0X4B, 0X63, 0XEB, 0X5B, 0X4C, 0X42, 0X8A, 0X3A, 0X48,
  0X5B, 0X69, 0X5B, 0X6D, 0X3A, 0X8B, 0X29, 0XE9, 0X4B, 0X0C, 0X6C, 0X51,
  0X6B, 0XAC, 0X6B, 0X29, 0X6B, 0XCC, 0X73, 0XCC, 0X6B, 0X2A, 0X6B, 0X4A,
  0X73, 0XCB, 0X84, 0X6D, 0X7B, 0XAC, 0X63, 0X49, 0X63, 0X49, 0X73, 0XEB,
  0X73, 0X89, 0X63, 0X29, 0X7B, 0XEA, 0X7C, 0X2B, 0X7C, 0X4B, 0X83, 0XAC,
  0X73, 0X6A, 0X7B, 0XEB, 0X8D, 0X13, 0X8C, 0X91, 0X7B, 0X6B, 0X9C, 0X92,
  0X8C, 0X51, 0X7C, 0X10, 0X5B, 0X2D, 0X6A, 0XCA, 0X62, 0X48, 0X7C, 0X10,
  0X7C, 0X0F, 0X5A, 0X89, 0X83, 0X6C, 0X83, 0X6D, 0X73, 0X0A, 0X6B, 0X2B,
  0X52, 0XC9, 0X73, 0XCD, 0X74, 0X32, 0X74, 0X33, 0X6B, 0XD0, 0X5B, 0X2D,
  0X5B, 0X2D, 0X63, 0X8F, 0X74, 0X31, 0X74, 0X10, 0X84, 0X73, 0X84, 0X73,
  0X7C, 0XB3, 0X7C, 0XB3, 0X74, 0X53, 0X74, 0X74, 0X5A, 0XCB, 0X5A, 0X47,
  0X6A, 0XC9, 0X7A, 0XE9, 0X7B, 0X09, 0X7B, 0X4B, 0X7B, 0X6B, 0X7A, 0XEA,
  0X72, 0XEA, 0X72, 0XC9, 0X73, 0X6E, 0X73, 0XF0, 0X7B, 0XAA, 0X7B, 0X49,
  0X7A, 0XE9, 0X7B, 0X0A, 0X7C, 0X0D, 0X8B, 0XCD, 0X8B, 0XAC, 0X83, 0X6C,
  0X7A, 0XCA, 0X72, 0X89, 0X72, 0XAA, 0X6A, 0X88, 0X6A, 0XA9, 0X72, 0XA9,
  0X72, 0X89, 0X6A, 0XA8, 0X6A, 0XE9, 0X7B, 0X2A, 0X62, 0X88, 0X62, 0X67,
  0X7B, 0X4A, 0X7B, 0X0B, 0X6A, 0XC8, 0X6A, 0XC8, 0X6A, 0XC9, 0X7A, 0XA9,
  0X72, 0XEA, 0X72, 0XE9, 0X72, 0XC9, 0X72, 0XA9, 0X72, 0XA9, 0X7A, 0XEA,
  0X6A, 0XCA, 0X72, 0XA9, 0X7A, 0XA9, 0X7A, 0XC9, 0X6A, 0XA9, 0X73, 0X0A,
  0X72, 0XEA, 0X6A, 0X68, 0X73, 0X0A, 0X83, 0X4A, 0X8B, 0X8A, 0X8B, 0X6A,
  0X93, 0X8A, 0X8B, 0XAB, 0X93, 0XCB, 0X8B, 0X4A, 0X8B, 0X29, 0X93, 0XEC,
  0X93, 0X8A, 0X73, 0X69, 0X53, 0X6A, 0X7B, 0X6A, 0X6B, 0XAA, 0X73, 0X6A,
  0X63, 0X6D, 0X5B, 0XB0, 0X53, 0X4D, 0X63, 0XD1, 0X64, 0X12, 0X63, 0XAD,
  0X6B, 0XCC, 0X6B, 0XCC, 0X6B, 0X4A, 0X72, 0XE9, 0X83, 0X6C, 0X83, 0X4B,
  0X7A, 0X88, 0X72, 0X07, 0X7A, 0X27, 0X7A, 0X88, 0X7A, 0XEA, 0X72, 0XC9,
  0X7A, 0XC9, 0X6B, 0X0A, 0X62, 0XA8, 0X52, 0X26, 0X62, 0XC8, 0X73, 0X0A,
  0X62, 0XA9, 0X5A, 0X67, 0X52, 0X48, 0X4A, 0X28, 0X52, 0X48, 0X52, 0X69,
  0X5A, 0X69, 0X62, 0XCA, 0X52, 0X48, 0X4A, 0X48, 0X4A, 0X28, 0X4A, 0X48,
  0X4A, 0X89, 0X52, 0XA9, 0X5A, 0XA9, 0X4A, 0X06, 0X4A, 0X06, 0X62, 0X88,
  0X72, 0XE9, 0X73, 0X0A, 0X72, 0XC9, 0X6A, 0XA8, 0X6A, 0XE9, 0X73, 0X0B,
  0X6A, 0XA8, 0X6A, 0X47, 0X62, 0XE9, 0X6A, 0XE8, 0X6A, 0XE8, 0X6A, 0XE8,
  0X62, 0XC8, 0X6A, 0XA8, 0X7B, 0X2B, 0X73, 0X09, 0X72, 0XE9, 0X72, 0XE9,
  0X7B, 0X09, 0X7B, 0X69, 0X62, 0XE8, 0X6B, 0X0A, 0X6B, 0X0A, 0X73, 0X2A,
  0X62, 0X68, 0X62, 0X88, 0X7B, 0X29, 0X6A, 0X67, 0X6A, 0X87, 0X83, 0X29,
  0X6A, 0XA8, 0X5A, 0X67, 0X5A, 0X47, 0X6A, 0XC9, 0X6A, 0XEA, 0X6A, 0XEA,
  0X7B, 0X09, 0X73, 0X29, 0X72, 0XE9, 0X7B, 0X2A, 0X83, 0X2A, 0X83, 0X4A,
  0X83, 0X8B, 0X83, 0X8A, 0X6B, 0X08, 0X6B, 0X6A, 0X6B, 0X0A, 0X6A, 0XE9,
  0X73, 0X49, 0X6B, 0X6A, 0X5A, 0XA7, 0X4A, 0X26, 0X52, 0X67, 0X42, 0X26,
  0X31, 0XA5, 0X52, 0X47, 0X4A, 0X6B, 0X5B, 0X50, 0X6B, 0X6C, 0X73, 0X8D,
  0X62, 0XEA, 0X5B, 0X0E, 0X5B, 0X0D, 0X5B, 0X0B, 0X6B, 0X4D, 0X63, 0X0B,
  0X52, 0XC8, 0X5A, 0XE9, 0X63, 0X49, 0X19, 0X03, 0X00, 0X00, 0X00, 0X41,
  0X10, 0XE5, 0X11, 0X06, 0X19, 0X27, 0X19, 0X27, 0X19, 0X48, 0X11, 0X06,
  0X08, 0XA4, 0X08, 0XA4, 0X08, 0XC4, 0X11, 0X07, 0X11, 0X27, 0X08, 0XE7,
  0X10, 0XE6, 0X10, 0XE5, 0X19, 0X67, 0X21, 0X87, 0X19, 0X88, 0X11, 0X49,
  0X11, 0X29, 0X19, 0X69, 0X11, 0X49, 0X11, 0X29, 0X19, 0X67, 0X19, 0X87,
  0X19, 0X67, 0X11, 0X07, 0X11, 0X27, 0X11, 0X47, 0X19, 0X67, 0X19, 0X66,
  0X19, 0X87, 0X21, 0X87, 0X19, 0X67, 0X19, 0X66, 0X19, 0X45, 0X10, 0XC4,
  0X10, 0XC5, 0X11, 0X06, 0X11, 0X06, 0X11, 0X06, 0X11, 0X06, 0X10, 0XE6,
  0X11, 0X06, 0X11, 0X06, 0X10, 0XE6, 0X11, 0X06, 0X10, 0XE6, 0X08, 0XC5,
  0X11, 0X06, 0X19, 0X69, 0X19, 0X48, 0X19, 0X48, 0X19, 0X48, 0X11, 0X48,
  0X11, 0X28, 0X11, 0X28, 0X11, 0X06, 0X10, 0XC5, 0X08, 0XE6, 0X08, 0XC6,
  0X08, 0XC5, 0X08, 0XC5, 0X08, 0XC6, 0X08, 0XA5, 0X08, 0XC5, 0X08, 0XE6,
  0X08, 0XC6, 0X08, 0XC6, 0X08, 0XE6, 0X08, 0XE6, 0X08, 0XE6, 0X08, 0XA5,
  0X08, 0XA4, 0X08, 0XA5, 0X08, 0XC5, 0X08, 0XC5, 0X08, 0XC5, 0X08, 0XE6,
  0X08, 0XE6, 0X08, 0XC6, 0X08, 0XC5, 0X08, 0XC6, 0X08, 0XC5, 0X08, 0XC5,
  0X08, 0XC5, 0X08, 0XA5, 0X08, 0XA4, 0X08, 0XC4, 0X10, 0XE6, 0X10, 0XE6,
  0X10, 0XE6, 0X11, 0X07, 0X19, 0X47, 0X19, 0X67, 0X21, 0XC9, 0X29, 0XC9,
  0X21, 0X87, 0X29, 0XA7, 0X29, 0XC7, 0X21, 0XA7, 0X31, 0XA6, 0X29, 0XA6,
  0X19, 0X86, 0X21, 0XC7, 0X29, 0XA6, 0X29, 0XA5, 0X29, 0XA6, 0X29, 0XE8,
  0X32, 0X29, 0X32, 0X08, 0X42, 0XA9, 0X4A, 0XC9, 0X21, 0X43, 0X31, 0XE5,
  0X42, 0X88, 0X29, 0XA5, 0X29, 0X84, 0X3A, 0X06, 0X3A, 0X26, 0X3A, 0X05,
  0X2A, 0X04, 0X29, 0XC4, 0X29, 0X44, 0X3A, 0X26, 0X3A, 0X06, 0X3A, 0X06,
  0X3A, 0X05, 0X53, 0X2A, 0X64, 0X0D, 0X53, 0X6B, 0X4A, 0XA8, 0X42, 0XE9,
  0X2A, 0X46, 0X21, 0X63, 0X42, 0XA8, 0X63, 0XAD, 0X4B, 0X09, 0X3A, 0X65,
  0X3A, 0X85, 0X53, 0X26, 0X63, 0X88, 0X5B, 0X88, 0X4A, 0XE8, 0X4B, 0X08,
  0X4B, 0X08, 0X52, 0X88, 0X5B, 0X08, 0X5B, 0X47, 0X42, 0X86, 0X53, 0X06,
  0X5B, 0X47, 0X5B, 0X47, 0X63, 0X67, 0X4B, 0X08, 0X42, 0XC7, 0X5B, 0X66,
  0X53, 0X27, 0X52, 0XC9, 0X6B, 0XCC, 0X6B, 0XEC, 0X6C, 0X0D, 0X74, 0X0D,
  0X6B, 0XEC, 0X74, 0X0D, 0X53, 0X4A, 0X4B, 0X2B, 0X63, 0XAB, 0X6B, 0XAC,
  0X5A, 0XEA, 0X6B, 0X6A, 0X6B, 0X8A, 0X6B, 0X8A, 0X6B, 0X69, 0X52, 0X87,
  0X53, 0X4C, 0X5B, 0X8D, 0X4B, 0X0A, 0X52, 0XC8, 0X52, 0XC8, 0X5B, 0X2A,
  0X59, 0XEB, 0X49, 0XAF, 0X51, 0XED, 0X52, 0X0F, 0X52, 0X0F, 0X62, 0X2C,
  0X72, 0XAB, 0X72, 0XC9, 0X5A, 0X0B, 0X51, 0XEA, 0X51, 0XEF, 0X59, 0XED,
  0X59, 0XEE, 0X52, 0X0E, 0X5A, 0X0B, 0X5A, 0X0B, 0X62, 0X2C, 0X5A, 0X0D,
  0X5A, 0X0D, 0X5A, 0X4D, 0X5A, 0XCB, 0X5A, 0XEA, 0X5B, 0X0C, 0X63, 0X4D,
  0X6B, 0X8C, 0X63, 0X6D, 0X63, 0X8D, 0X73, 0X2C, 0X6B, 0X8D, 0X63, 0XCE,
  0X4A, 0XAA, 0X73, 0XB2, 0X63, 0X50, 0X52, 0X8B, 0X52, 0X8A, 0X4A, 0X48,
  0X5A, 0XEA, 0X5A, 0XAC, 0X5A, 0XCD, 0X6B, 0X2B, 0X6B, 0X0C, 0X7B, 0XD4,
  0X73, 0X92, 0X5A, 0XEB, 0X63, 0X91, 0X63, 0X0F, 0X73, 0X92, 0X83, 0X51,
  0X82, 0XF1, 0X7B, 0X10, 0X72, 0XAE, 0X6A, 0X8E, 0X62, 0XCD, 0X83, 0X0F,
  0X8B, 0X0F, 0X8B, 0X0D, 0X8A, 0XCE, 0X82, 0XCE, 0X82, 0XCE, 0X8B, 0X71,
  0X83, 0X4F, 0X7B, 0X0F, 0X82, 0XCF, 0X72, 0XB2, 0X6A, 0XD0, 0X62, 0X74,
  0X6A, 0X94, 0X51, 0XEA, 0X41, 0X68, 0X6A, 0X6D, 0X73, 0X0C, 0X5A, 0X4C,
  0X31, 0X27, 0X41, 0X88, 0X39, 0X88, 0X41, 0X89, 0X41, 0XAB, 0X49, 0XAB,
  0X6A, 0X2B, 0X6A, 0X2B, 0X72, 0X2A, 0X39, 0X47, 0X31, 0X48, 0X41, 0XAA,
  0X39, 0X89, 0X41, 0X68, 0X41, 0X68, 0X41, 0X66, 0X39, 0X66, 0X41, 0XC8,
  0X41, 0XE9, 0X4A, 0X0B, 0X5A, 0X6D, 0X4A, 0XCF, 0X4A, 0X8E, 0X41, 0XC9,
  0X49, 0XE9, 0X51, 0XEA, 0X59, 0XCA, 0X8A, 0XCE, 0X82, 0XCE, 0X49, 0X88,
  0X41, 0X46, 0X72, 0X4C, 0X5A, 0X0C, 0X31, 0X05, 0X39, 0X47, 0X41, 0XCA,
  0X41, 0XC9, 0X39, 0X67, 0X39, 0X68, 0X41, 0X87, 0X41, 0X66, 0X51, 0XE7,
  0X62, 0X2C, 0X41, 0X6C, 0X39, 0X4A, 0X28, 0XE4, 0X4A, 0X07, 0X6B, 0X0B,
  0X6A, 0XA7, 0X72, 0XA7, 0X6A, 0XCC, 0X6A, 0XCB, 0X6A, 0XAB, 0X7B, 0X4E,
  0X5B, 0X0D, 0X5B, 0X0B, 0X6B, 0X8B, 0X7B, 0XED, 0X84, 0X2D, 0X63, 0X2E,
  0X5A, 0XCE, 0X63, 0X0D, 0X73, 0X0B, 0X6B, 0X2F, 0X63, 0X90, 0X73, 0X2C,
  0X8B, 0X6D, 0X73, 0X4C, 0X7B, 0X0B, 0X7B, 0X0D, 0X73, 0X2E, 0X72, 0X92,
  0X49, 0X8C, 0X62, 0X4C, 0X5A, 0X34, 0X42, 0X2D, 0X62, 0XEA, 0X5A, 0XB2,
  0X4A, 0XAF, 0X6A, 0XCC, 0X6B, 0X0C, 0X52, 0XAB, 0X42, 0X8A, 0X42, 0XAB,
  0X63, 0X2B, 0X52, 0XEA, 0X4B, 0X09, 0X53, 0X4A, 0X43, 0X2A, 0X53, 0X2B,
  0X53, 0X0A, 0X53, 0X2A, 0X53, 0X4A, 0X5B, 0X6B, 0X42, 0X68, 0X00, 0X20,
  0X4A, 0X06, 0X5A, 0X68, 0X52, 0X48, 0X52, 0XA9, 0X52, 0XAA, 0X5A, 0XA9,
  0X6A, 0XAA, 0X6A, 0XA9, 0X62, 0XA8, 0X5A, 0X89, 0X52, 0X69, 0X5A, 0X89,
  0X6A, 0XC9, 0X62, 0X89, 0X72, 0XEB, 0X7B, 0X0C, 0X6A, 0XCB, 0X5A, 0X8A,
  0X62, 0XAA, 0X5A, 0X69, 0X52, 0X07, 0X41, 0XC6, 0X39, 0X65, 0X49, 0XE7,
  0X62, 0X8A, 0X72, 0XCB, 0X7A, 0XCB, 0X8A, 0XEC, 0X83, 0X0C, 0X8B, 0X0D,
  0X52, 0X09, 0X51, 0XE8, 0X52, 0X08, 0X49, 0XE7, 0X41, 0XE7, 0X5A, 0X29,
  0X62, 0X4A, 0X62, 0X6B, 0X41, 0XC7, 0X52, 0X08, 0X6A, 0X8A, 0X62, 0X69,
  0X49, 0XC7, 0X5A, 0X29, 0X62, 0X69, 0X6A, 0X8A, 0X62, 0X29, 0X6A, 0X69,
  0X72, 0X6B, 0X72, 0X29, 0X6A, 0X6A, 0X6A, 0X6B, 0X83, 0X0B, 0X72, 0XAA,
  0X5A, 0X2A, 0X62, 0X2A, 0X49, 0XE8, 0X39, 0XA7, 0X39, 0XC8, 0X5A, 0XAA,
  0X62, 0XCA, 0X5A, 0X48, 0X41, 0XC7, 0X49, 0XE7, 0X49, 0XE7, 0X41, 0X87,
  0X41, 0XA7, 0X49, 0XE8, 0X5A, 0X6A, 0X49, 0XE8, 0X41, 0XA6, 0X4A, 0X09,
  0X5A, 0X6B, 0X5A, 0X48, 0X5A, 0X28, 0X52, 0X49, 0X52, 0X28, 0X51, 0XE7,
  0X4A, 0X09, 0X49, 0XE9, 0X52, 0X08, 0X52, 0X08, 0X52, 0X08, 0X4A, 0X08,
  0X52, 0X49, 0X4A, 0X08, 0X49, 0XE8, 0X49, 0XE9, 0X49, 0XE8, 0X52, 0X09,
  0X52, 0X0A, 0X52, 0X4A, 0X49, 0XE8, 0X49, 0XE9, 0X52, 0X09, 0X52, 0X09,
  0X52, 0X08, 0X6A, 0XAA, 0X5A, 0X69, 0X5A, 0X48, 0X5A, 0X69, 0X62, 0X69,
  0X62, 0X69, 0X5A, 0X49, 0X5A, 0X68, 0X52, 0X07, 0X5A, 0X28, 0X5A, 0X28,
  0X52, 0X28, 0X5A, 0X48, 0X62, 0X89, 0X4A, 0X08, 0X42, 0X08, 0X52, 0X28,
  0X62, 0X87, 0X62, 0X67, 0X62, 0X67, 0X7A, 0XA9, 0X5A, 0X48, 0X42, 0X08,
  0X4A, 0X07, 0X6A, 0X68, 0X62, 0X48, 0X5A, 0X28, 0X5A, 0X6A, 0X6A, 0X89,
  0X62, 0X69, 0X62, 0X69, 0X5A, 0X28, 0X52, 0X08, 0X52, 0X08, 0X41, 0XE7,
  0X41, 0XC8, 0X52, 0X29, 0X5A, 0X69, 0X5A, 0X6A, 0X52, 0X28, 0X52, 0X48,
  0X63, 0X0B, 0X5A, 0X69, 0X49, 0XC8, 0X18, 0X83, 0X21, 0X04, 0X62, 0XCB,
  0X49, 0XE7, 0X4A, 0X07, 0X63, 0X8E, 0X74, 0X51, 0X7C, 0X92, 0X84, 0XD3,
  0X7C, 0X30, 0X8C, 0XB2, 0X8C, 0XD2, 0X85, 0X17, 0X7C, 0XB5, 0X7C, 0X30,
  0X85, 0X18, 0X84, 0X53, 0X84, 0XB4, 0X5B, 0X4D, 0X5A, 0X69, 0X52, 0X48,
  0X6B, 0X4D, 0X7C, 0X52, 0X73, 0XD0, 0X5A, 0X68, 0X6C, 0X30, 0X6C, 0XB2,
  0X6B, 0XAE, 0X62, 0X8B, 0X6A, 0XAB, 0X74, 0X52, 0X6C, 0X32, 0X74, 0X11,
  0X7C, 0X70, 0X7C, 0X91, 0X73, 0XEF, 0X62, 0XEA, 0X52, 0X88, 0X52, 0X88,
  0X4A, 0X88, 0X4A, 0X67, 0X5A, 0XE9, 0X5B, 0X2A, 0X5B, 0X09, 0X5A, 0XC9,
  0X31, 0XC6, 0X5B, 0X07, 0X5A, 0XC7, 0X5A, 0XE9, 0X5B, 0X2C, 0X4B, 0XF5,
  0X4A, 0XC8, 0X52, 0XEA, 0X53, 0X09, 0X52, 0XE8, 0X52, 0XA8, 0X5A, 0XC8,
  0X6B, 0X29, 0X5A, 0XA9, 0X52, 0X48, 0X5A, 0XCB, 0X7B, 0XF3, 0X73, 0XD3,
  0X73, 0XF3, 0X73, 0XD3, 0X73, 0XD4, 0X5A, 0X8A, 0X62, 0XCB, 0X84, 0X12,
  0X6B, 0X09, 0X4A, 0X48, 0X73, 0X71, 0X83, 0XF3, 0X7C, 0X14, 0X63, 0X4E,
  0X39, 0XE5, 0X5A, 0XAA, 0X7B, 0X6F, 0X5A, 0X6A, 0X5A, 0X8B, 0X5A, 0X69,
  0X5A, 0X68, 0X62, 0X89, 0X6A, 0XCB, 0X5A, 0XAB, 0X3A, 0X47, 0X39, 0XC6,
  0X39, 0XA5, 0X5A, 0X48, 0X62, 0X89, 0X6B, 0X8F, 0X73, 0XB0, 0X52, 0XEB,
  0X42, 0X67, 0X5B, 0X09, 0X4A, 0XA7, 0X63, 0X0A, 0X83, 0XAF, 0X73, 0XB0,
  0X84, 0X75, 0X5B, 0X2C, 0X4A, 0X87, 0X52, 0X89, 0X52, 0XCA, 0X64, 0X54,
  0X64, 0X96, 0X5B, 0X2C, 0X52, 0X87, 0X6B, 0X4F, 0X5B, 0X0D, 0X4A, 0XEB,
  0X53, 0X0C, 0X53, 0X0B, 0X52, 0XA9, 0X52, 0X87, 0X52, 0XC8, 0X63, 0X2A,
  0X63, 0X09, 0X63, 0X29, 0X63, 0X2B, 0X63, 0X0A, 0X63, 0X4D, 0X62, 0XC9,
  0X5A, 0X87, 0X5A, 0XE8, 0X73, 0XF0, 0X7C, 0X10, 0X7B, 0XCB, 0X6B, 0XAB,
  0X6B, 0XCD, 0X6B, 0XAD, 0X73, 0X8C, 0X73, 0XAB, 0X6B, 0X8A, 0X73, 0XAA,
  0X5A, 0XE9, 0X42, 0X28, 0X42, 0X27, 0X42, 0X07, 0X52, 0X68, 0X52, 0X08,
  0X52, 0X48, 0X52, 0X68, 0X52, 0X49, 0X52, 0X69, 0X41, 0XE6, 0X41, 0XE6,
  0X41, 0XE6, 0X39, 0XE5, 0X31, 0X85, 0X08, 0X20, 0X00, 0X00, 0X08, 0X42,
  0X10, 0X83, 0X18, 0X83, 0X39, 0X66, 0X41, 0XC8, 0X41, 0XA8, 0X39, 0X87,
  0X39, 0X46, 0X31, 0X46, 0X49, 0XC9, 0X51, 0XA7, 0X51, 0XEA, 0X52, 0X2C,
  0X5A, 0X0A, 0X52, 0X0B, 0X5A, 0X08, 0X62, 0X09, 0X62, 0X09, 0X5A, 0X0A,
  0X52, 0X0B, 0X52, 0X0B, 0X51, 0XC9, 0X59, 0XEB, 0X5A, 0X4A, 0X52, 0X28,
  0X52, 0X29, 0X5A, 0X88, 0X5A, 0XA9, 0X4A, 0X89, 0X4A, 0X89, 0X4A, 0X69,
  0X52, 0XA9, 0X5B, 0X0A, 0X5A, 0XEA, 0X6A, 0XEA, 0X73, 0X2A, 0X7B, 0XAC,
  0X62, 0XEC, 0X52, 0XAC, 0X4A, 0XCB, 0X4B, 0X0C, 0X4A, 0XED, 0X5B, 0X0C,
  0X5B, 0X0C, 0X5B, 0X2D, 0X5B, 0X6D, 0X63, 0X6D, 0X5B, 0X8D, 0X5B, 0X0C,
  0X5A, 0XEC, 0X63, 0X6C, 0X5B, 0X4B, 0X52, 0XEA, 0X53, 0X0A, 0X53, 0X0B,
  0X5B, 0X0C, 0X5B, 0X0C, 0X5A, 0XEB, 0X4A, 0XC9, 0X42, 0XC9, 0X42, 0XC9,
  0X4A, 0XEA, 0X6B, 0X4E, 0X5A, 0XEB, 0X5A, 0XEC, 0X5A, 0XAA, 0X6A, 0XCA,
  0X73, 0X0B, 0X6A, 0XEB, 0X6A, 0XEB, 0X73, 0X2B, 0X7B, 0X2B, 0X5A, 0XCA,
  0X52, 0XC9, 0X5A, 0XEB, 0X62, 0XEC, 0X5A, 0X8A, 0X5A, 0X68, 0X5A, 0X68,
  0X5A, 0X68, 0X5A, 0X68, 0X5A, 0X68, 0X52, 0X8A, 0X42, 0X6A, 0X5A, 0XCA,
  0X73, 0X4C, 0X62, 0XCA, 0X5A, 0X69, 0X5A, 0X69, 0X42, 0X6A, 0X42, 0X8A,
  0X52, 0X69, 0X52, 0X88, 0X52, 0X89, 0X5A, 0X69, 0X52, 0XC8, 0X62, 0XEB,
  0X52, 0XAB, 0X4A, 0X8A, 0X52, 0XEC, 0X53, 0X90, 0X5B, 0X91, 0X5B, 0X91,
  0X53, 0X91, 0X6C, 0X55, 0X64, 0X35, 0X5C, 0X17, 0X5C, 0X18, 0X53, 0X73,
  0X5A, 0XEE, 0X73, 0XB3, 0X7B, 0XB3, 0X6B, 0X50, 0X7B, 0XB1, 0X7B, 0XF1,
  0X7B, 0X6F, 0X7C, 0X34, 0X74, 0XB6, 0X73, 0X6F, 0X62, 0XEF, 0X62, 0XAE,
  0X52, 0X90, 0X42, 0X92, 0X52, 0X90, 0X5A, 0X8F, 0X62, 0XCF, 0X52, 0X70,
  0X52, 0X71, 0X6A, 0XCD, 0X6A, 0X8D, 0X6A, 0X8C, 0X5A, 0X6F, 0X3A, 0X53,
  0X42, 0X71, 0X62, 0XAE, 0X62, 0X6D, 0X72, 0XAC, 0X6A, 0XCD, 0X42, 0X91,
  0X52, 0X8F, 0X62, 0X6D, 0X62, 0X8E, 0X52, 0X6F, 0X3A, 0X30, 0X4A, 0X0D,
  0X32, 0X51, 0X4A, 0XB1, 0X62, 0X8E, 0X4A, 0XAF, 0X32, 0X70, 0X6A, 0X2B,
  0X51, 0XEA, 0X2A, 0X0F, 0X3A, 0X4E, 0X5B, 0X2F, 0X62, 0XEE, 0X5A, 0XCD,
  0X5B, 0X0E, 0X4A, 0XCA, 0X3A, 0X27, 0X3A, 0X27, 0X4A, 0X48, 0X4A, 0X69,
  0X42, 0X89, 0X4A, 0X08, 0X52, 0X49, 0X62, 0XCC, 0X4A, 0X08, 0X73, 0X0D,
  0X83, 0X6F, 0X83, 0X4F, 0X7B, 0X2E, 0X83, 0X2E, 0X6A, 0XAB, 0X39, 0XE8,
  0X41, 0XC8, 0X5A, 0XAB, 0X73, 0X90, 0X73, 0X70, 0X73, 0X0F, 0X6B, 0X30,
  0X6B, 0X51, 0X52, 0X4E, 0X5A, 0X8E, 0X3A, 0X71, 0X32, 0X31, 0X63, 0XB5,
  0X6B, 0X94, 0X52, 0X0C, 0X52, 0X4C, 0X3A, 0X32, 0X42, 0X2F, 0X73, 0X73,
  0X52, 0XD3, 0X4A, 0X52, 0X73, 0X74, 0X6B, 0X55, 0X62, 0XF0, 0X73, 0X52,
  0X73, 0XD5, 0X52, 0X2C, 0X52, 0X2D, 0X5A, 0X2D, 0X62, 0X2E, 0X59, 0XED,
  0X5A, 0X2D, 0X4A, 0X50, 0X3A, 0XB5, 0X4A, 0XB2, 0X5A, 0XB1, 0X62, 0XB0,
  0X3A, 0X96, 0X4A, 0X92, 0X5A, 0X6D, 0X5A, 0X4D, 0X4A, 0X93, 0X32, 0XB7,
  0X4A, 0X72, 0X52, 0X0D, 0X39, 0XCB, 0X42, 0X4B, 0X4A, 0XCA, 0X52, 0XE9,
  0X63, 0X0C, 0X52, 0XAE, 0X5B, 0X32, 0X53, 0XB6, 0X6B, 0XB2, 0X73, 0XB0,
  0X73, 0XB0, 0X73, 0X8F, 0X6B, 0X4E, 0X63, 0X0D, 0X6B, 0X2C, 0X74, 0X32,
  0X74, 0X97, 0X6C, 0X78, 0X75, 0X1A, 0X7D, 0X7B, 0X7D, 0X1A, 0X6C, 0X35,
  0X63, 0X30, 0X7B, 0X0E, 0X7C, 0X13, 0X74, 0X15, 0X6B, 0X72, 0X73, 0X92,
  0X73, 0X92, 0X7B, 0XB0, 0X6B, 0X0F, 0X6A, 0X8C, 0X6A, 0X8F, 0X62, 0XB0,
  0X62, 0X6C, 0X8A, 0XAD, 0X72, 0X08, 0X6A, 0X90, 0X6A, 0X8E, 0X6A, 0X2A,
  0X7B, 0X0F, 0X7B, 0XB4, 0X73, 0X2F, 0X83, 0X6F, 0X7B, 0X0D, 0X6A, 0XEF,
  0X6B, 0X94, 0X84, 0X55, 0X73, 0XF5, 0X74, 0X39, 0X85, 0X1A, 0X7C, 0X53,
  0X73, 0XD2, 0X73, 0X90, 0X7B, 0X8F, 0X74, 0X13, 0X74, 0X37, 0X74, 0X79,
  0X74, 0X97, 0X7C, 0X76, 0X7C, 0X75, 0X7B, 0XF1, 0X63, 0X50, 0X6B, 0XF3,
  0X74, 0X96, 0X73, 0X30, 0X63, 0X12, 0X63, 0X31, 0X6A, 0XF0, 0X72, 0X8F,
  0X83, 0XB0, 0X8C, 0X30, 0X73, 0XF4, 0X7C, 0X37, 0X8C, 0X76, 0X84, 0XB9,
  0X7C, 0X78, 0X7C, 0X57, 0X6B, 0XD4, 0X53, 0X30, 0X63, 0X73, 0X5A, 0X72,
  0X5A, 0XB0, 0X5A, 0XB1, 0X5A, 0X70, 0X63, 0X10, 0X8C, 0XF6, 0X72, 0XEC,
  0X7B, 0X2E, 0X9C, 0X97, 0X84, 0X12, 0X84, 0X30, 0X7C, 0X11, 0X7C, 0X33,
  0X85, 0X19, 0X85, 0X3A, 0X6B, 0X6E, 0X7C, 0XFB, 0X7C, 0X54, 0X73, 0X4C,
  0X6A, 0XE9, 0X5A, 0XA8, 0X6B, 0X4D, 0X84, 0X12, 0X84, 0X31, 0X83, 0XF0,
  0X8B, 0XEF, 0X8B, 0XED, 0X7C, 0X52, 0X74, 0X32, 0X74, 0X31, 0X6A, 0XF0,
  0X5A, 0X0B, 0X62, 0X6D, 0X62, 0XD2, 0X5A, 0X91, 0X62, 0XB1, 0X6A, 0XEF,
  0X72, 0XEC, 0X7B, 0XB1, 0X83, 0XF4, 0X8B, 0X73, 0X7B, 0XD1, 0X7B, 0XB0,
  0X7B, 0XF4, 0X84, 0X55, 0X94, 0XD4, 0X94, 0X95, 0X7B, 0XB4, 0X83, 0XD5,
  0X94, 0X54, 0X8C, 0X33, 0X8C, 0X74, 0X7B, 0XB0, 0X6C, 0XD9, 0X8D, 0XDD,
  0X7C, 0X78, 0X6B, 0XD3, 0X7B, 0XF2, 0X6B, 0X52, 0X63, 0X11, 0X74, 0X54,
  0X63, 0XF5, 0X63, 0XD4, 0X7C, 0X54, 0X62, 0XF1, 0X7C, 0X12, 0X63, 0X32,
  0X4A, 0X8D, 0X52, 0X8D, 0X52, 0X0E, 0X62, 0XB1, 0X8A, 0XD0, 0X9B, 0X93,
  0X7B, 0XB4, 0X73, 0XD4, 0X7C, 0X56, 0X7B, 0XF3, 0X6B, 0X6F, 0X6B, 0X2F,
  0X72, 0XEC, 0X83, 0XCD, 0X73, 0X6C, 0X6B, 0X0D, 0X73, 0X32, 0X5A, 0X4F,
  0X63, 0X50, 0X73, 0XD2, 0X62, 0XAE, 0X6A, 0X6E, 0X72, 0X8D, 0X62, 0X73,
  0X62, 0XB3, 0X6A, 0XD3, 0X72, 0XD3, 0X6A, 0XD3, 0X62, 0XB3, 0X72, 0XD3,
  0X62, 0X93, 0X73, 0X50, 0X83, 0X52, 0X83, 0XD4, 0X8C, 0X11, 0X72, 0XD1,
  0X73, 0X15, 0X52, 0X72, 0X4A, 0X10, 0X5A, 0XF2, 0X7C, 0X55, 0X8C, 0X54,
  0X73, 0X10, 0X5A, 0X91, 0X5A, 0X72, 0X5A, 0X51, 0X7B, 0X12, 0X73, 0X73,
  0X53, 0X53, 0X7B, 0XF1, 0X5B, 0X91, 0X5B, 0X55, 0X6B, 0X57, 0X63, 0X56,
  0X52, 0X92, 0X62, 0X95, 0X5A, 0XB3, 0X5A, 0XB2, 0X52, 0X71, 0X63, 0X54,
  0X6B, 0XB4, 0X52, 0XF1, 0X52, 0X51, 0X4A, 0X71, 0X52, 0XD3, 0X5A, 0X92,
  0X62, 0XB1, 0X63, 0XB3, 0X74, 0X13, 0X83, 0X8E, 0X8C, 0X11, 0X73, 0X6D,
  0X73, 0X8D, 0X73, 0XF3, 0X63, 0X15, 0X62, 0XD3, 0X73, 0XB2, 0X5B, 0X13,
  0X6B, 0X91, 0X7B, 0XF1, 0X7B, 0X90, 0X6B, 0X2E, 0X73, 0X0E, 0X7B, 0X0D,
  0X73, 0X4F, 0X7C, 0X56, 0X73, 0XD4, 0X62, 0XCC, 0X73, 0X4C, 0X83, 0XAE,
  0X7B, 0XCE, 0X7B, 0XCF, 0X7B, 0X90, 0X7B, 0XB1, 0X7B, 0XF1, 0X8B, 0XCD,
  0X84, 0X76, 0X74, 0X7A, 0X7C, 0X59, 0X7C, 0X37, 0X53, 0X14, 0X52, 0X91,
  0X62, 0XD1, 0X8B, 0X4D, 0X73, 0X8F, 0X73, 0XB0, 0X93, 0XCE, 0X8B, 0XF0,
  0X8C, 0X0F, 0X93, 0X6A, 0X83, 0XAD, 0X94, 0X10, 0X8B, 0XD0, 0X73, 0X4E,
  0X73, 0X8E, 0X73, 0X6C, 0X6C, 0X30, 0X73, 0XAF, 0X7B, 0X8E, 0X73, 0XCF,
  0X73, 0X0B, 0X7B, 0XD0, 0X7C, 0X13, 0X7B, 0XF0, 0X84, 0X50, 0X7B, 0X6E,
  0X73, 0X4F, 0XA3, 0X6C, 0X8C, 0X11, 0X6B, 0X90, 0X64, 0X17, 0X6C, 0X59,
  0X5C, 0X17, 0X6B, 0X90, 0X5B, 0X94, 0X5C, 0X38, 0X64, 0X16, 0X5B, 0X70,
  0X52, 0XCB, 0X73, 0X8F, 0X63, 0X4D, 0X5B, 0X2F, 0X5B, 0X70, 0X5B, 0X8F,
  0X63, 0X2F, 0X63, 0XAE, 0X53, 0X0D, 0X6C, 0X14, 0X74, 0X97, 0X73, 0XF1,
  0X7C, 0X12, 0X7C, 0X34, 0X7C, 0X75, 0X7D, 0X3A, 0X74, 0X32, 0X52, 0XCB,
  0X29, 0XC6, 0X29, 0XE6, 0X3A, 0X48, 0X7B, 0XCE, 0X7B, 0XCF, 0X7B, 0X8E,
  0X7B, 0X8D, 0X6B, 0X6C, 0X6B, 0X6B, 0X7B, 0XF0, 0X84, 0X94, 0X7C, 0X32,
  0X7C, 0X53, 0X84, 0X94, 0X8D, 0X58, 0X95, 0X36, 0X9D, 0X74, 0X84, 0XD1,
  0X74, 0X4E, 0X74, 0X4E, 0X8C, 0XD1, 0X84, 0X90, 0X6C, 0X0E, 0X74, 0X91,
  0X7C, 0XF2, 0X63, 0XED, 0X5B, 0X2B, 0X74, 0X50, 0X7C, 0X70, 0X74, 0X4F,
  0X74, 0X4F, 0X74, 0X50, 0X7C, 0XB2, 0X74, 0X30, 0X6B, 0XCE, 0X74, 0X52,
  0X74, 0XB4, 0X74, 0XB4, 0X74, 0XB4, 0X7C, 0XB2, 0X7C, 0X92, 0X6B, 0X8D,
  0X7C, 0X31, 0X7C, 0X72, 0X74, 0X72, 0X6C, 0X72, 0X74, 0X30, 0X74, 0X0F,
  0X74, 0XB4, 0X73, 0XCE, 0X7C, 0X50, 0X84, 0X70, 0X7C, 0X71, 0X6C, 0X31,
  0X74, 0X31, 0X7C, 0X50, 0X7C, 0X10, 0X6B, 0XCE, 0X74, 0X0E, 0X74, 0X94,
  0X74, 0X31, 0X74, 0X10, 0X74, 0X30, 0X74, 0X30, 0X74, 0X32, 0X6C, 0X32,
  0X74, 0X72, 0X8C, 0XB1, 0X94, 0XB1, 0X8C, 0X92, 0X8C, 0X92, 0X94, 0XB1,
  0X94, 0X91, 0X95, 0X15, 0X7C, 0X71, 0X7C, 0X91, 0X94, 0XF3, 0X94, 0XF3,
  0X94, 0XF3, 0X9D, 0X14, 0X9D, 0X34, 0XA5, 0X55, 0XAD, 0X76, 0X9D, 0X35,
  0X94, 0XD3, 0X94, 0XF3, 0X94, 0XF4, 0X95, 0X14, 0X9D, 0X14, 0X94, 0XF3,
  0X7C, 0X92, 0X74, 0X2F, 0X74, 0X0F, 0X84, 0XF3, 0X8C, 0XD2, 0X8C, 0X6F,
  0X74, 0X50, 0X74, 0X50, 0X74, 0X0F, 0X7C, 0X51, 0X7C, 0X50, 0X8C, 0XD4,
  0X9D, 0X56, 0X84, 0XD4, 0X6C, 0X0F, 0X6C, 0X50, 0X74, 0X50, 0X74, 0X50,
  0X74, 0X2F, 0X6B, 0XEE, 0X74, 0X0E, 0X74, 0X2E, 0X7C, 0X92, 0X74, 0XB4,
  0X74, 0X72, 0X74, 0X71, 0X7C, 0X71, 0X84, 0XB2, 0X84, 0XF3, 0X84, 0XF3,
  0X84, 0XF4, 0X84, 0XB2, 0X7C, 0X6F, 0X7C, 0XD3, 0X7C, 0X92, 0X5A, 0X26,
  0X6B, 0X29, 0X85, 0X13, 0X84, 0XF2, 0X84, 0XF3, 0X8D, 0X13, 0X8D, 0X11,
  0X9D, 0X95, 0X9D, 0XB5, 0X9D, 0XB6, 0X9D, 0XB6, 0X9D, 0XB6, 0X9D, 0XB6,
  0X9D, 0XD6, 0X9D, 0XD6, 0X95, 0X95, 0XA6, 0X99, 0X85, 0X54, 0X3A, 0X06,
  0X63, 0XCE, 0X7C, 0XD1, 0X7C, 0XD1, 0X7C, 0XF2, 0X74, 0X90, 0X74, 0XB1,
  0X7C, 0X90, 0X7C, 0X70, 0X53, 0X0A, 0X42, 0X27, 0X74, 0XB2, 0X7C, 0XD1,
  0X74, 0X90, 0X74, 0X91, 0X74, 0XB2, 0X6C, 0XD3, 0X6C, 0XB2, 0X74, 0XF4,
  0X7D, 0XB8, 0X85, 0X95, 0X75, 0X13, 0X75, 0XD7, 0X64, 0XD2, 0X7D, 0X74,
  0X85, 0X95, 0X7D, 0X75, 0X6C, 0XB2, 0X64, 0X91, 0X64, 0X50, 0X5C, 0XF7,
  0X6D, 0X58, 0X7D, 0X36, 0X7D, 0X35, 0X65, 0X37, 0X64, 0XD2, 0X64, 0X92,
  0X64, 0X71, 0X5B, 0XAD, 0X5B, 0XAD, 0X6B, 0XEE, 0X6C, 0X2F, 0X64, 0X2F,
  0X5B, 0XEE, 0X42, 0XE9, 0X3A, 0X68, 0X3A, 0X68, 0X32, 0X68, 0X7C, 0X50,
  0X95, 0X34, 0X95, 0X34, 0X95, 0X14, 0X95, 0X14, 0X95, 0X14, 0X95, 0X34,
  0X8D, 0X13, 0X95, 0X13, 0X74, 0X2F, 0X3A, 0X06, 0X42, 0X27, 0X3A, 0X06,
  0X4A, 0XA9, 0X63, 0XAD, 0X63, 0XAC, 0X63, 0X8C, 0X63, 0XAC, 0X5B, 0X6B,
  0X31, 0XC6, 0X39, 0XE6, 0X5B, 0X2A, 0X5B, 0X4A, 0X4A, 0X68, 0X42, 0X27,
  0X42, 0X07, 0X42, 0X06, 0X3A, 0X06, 0X42, 0X06, 0X39, 0XE6, 0X31, 0XE6,
  0X3A, 0X06, 0X3A, 0X06, 0X3A, 0X26, 0X31, 0XE6, 0X31, 0XC5, 0X39, 0XE5,
  0X42, 0X06, 0X42, 0X06, 0X4A, 0X26, 0X4A, 0X25, 0X4A, 0X26, 0X52, 0X46,
  0X52, 0X66, 0X42, 0X47, 0X42, 0X06, 0X41, 0XC5, 0X4A, 0X46, 0X4A, 0X26,
  0X49, 0XE6, 0X7C, 0X10, 0X9D, 0X34, 0X9D, 0X34, 0X9D, 0X14, 0X8C, 0X71,
  0X7B, 0XEF, 0X7B, 0XEF, 0X6B, 0X4C, 0X5A, 0XA9, 0X42, 0X48, 0X42, 0X47,
  0X3A, 0X06, 0X3A, 0X06, 0X42, 0X47, 0X42, 0X27, 0X5B, 0X0A, 0X73, 0XCC,
  0X6B, 0X6B, 0X42, 0X06, 0X39, 0XE6, 0X4A, 0X67, 0X73, 0X8B, 0X73, 0X6B,
  0X52, 0XA8, 0X42, 0X47, 0X42, 0X27, 0X42, 0X68, 0X5B, 0X2B, 0X5B, 0X8D,
  0X73, 0XAC, 0X83, 0XCA, 0X8D, 0X55, 0X85, 0X97, 0X75, 0X16, 0X7C, 0X2E,
  0X7B, 0XED, 0X7C, 0X0E, 0X7C, 0X0E, 0X7B, 0XEE, 0X7C, 0X0E, 0X7C, 0X0D,
  0X8C, 0X2C, 0X8C, 0X4C, 0X8C, 0X4C, 0X8C, 0XB2, 0X8C, 0XF4, 0X8C, 0XD2,
  0X8C, 0X4D, 0X8C, 0XD2, 0X7C, 0XD2, 0X74, 0X50, 0X7C, 0X4F, 0X7C, 0X4E,
  0X7C, 0X4F, 0X84, 0X4F, 0X7C, 0X71, 0X74, 0X51, 0X74, 0X30, 0X74, 0X52,
  0X74, 0X0F, 0X6B, 0XED, 0X6C, 0X0F, 0X6C, 0X0F, 0X73, 0XCC, 0X7B, 0XAA,
  0X7B, 0XAB, 0X7B, 0XAA, 0X84, 0X70, 0X7C, 0X70, 0X73, 0X8B, 0X73, 0XAC,
  0X7B, 0XEE, 0X7C, 0X0E, 0X7C, 0X0E, 0X84, 0X0E, 0X83, 0XED, 0X84, 0X0D,
  0X84, 0XB2, 0X7C, 0XB2, 0X7C, 0X90, 0X84, 0XB1, 0X7C, 0X91, 0X6C, 0X51,
  0X6C, 0X30, 0X6C, 0X30, 0X73, 0XEF, 0X6B, 0XCF, 0X63, 0X8E, 0X74, 0X30,
  0X74, 0X51, 0X74, 0X4F, 0X7C, 0X91, 0X74, 0X2F, 0X74, 0X2E, 0X7C, 0X4F,
  0X7C, 0X4F, 0X7C, 0X70, 0X7C, 0X4E, 0X73, 0X89, 0X7C, 0X4E, 0X74, 0X2E,
  0X5B, 0X6B, 0X5B, 0X8C, 0X53, 0X4A, 0X63, 0X69, 0X6B, 0XCB, 0X73, 0XEC,
  0X83, 0XCB, 0X83, 0XEB, 0X7B, 0XEB, 0X74, 0X0C, 0X6B, 0X8B, 0X63, 0X4A,
  0X52, 0XEA, 0X42, 0X06, 0X4A, 0X47, 0X6B, 0X4A, 0X7B, 0XEC, 0X7B, 0XED,
  0X7B, 0XAB, 0X6B, 0X8D, 0X63, 0X8E, 0X4A, 0X88, 0X42, 0X47, 0X4A, 0X67,
  0X4A, 0X67, 0X4A, 0X67, 0X4A, 0X67, 0X42, 0XC9, 0X3A, 0X47, 0X3A, 0X27,
  0X42, 0X88, 0X4A, 0X87, 0X52, 0XE9, 0X5B, 0X09, 0X5B, 0X09, 0X63, 0X4A,
  0X63, 0X09, 0X42, 0X47, 0X42, 0X68, 0X4A, 0X68, 0X73, 0XAA, 0X83, 0XEB,
  0X73, 0X8A, 0X4A, 0X46, 0X4A, 0X46, 0X42, 0X26, 0X73, 0X8A, 0X5A, 0XE8,
  0X52, 0X87, 0X5A, 0XC7, 0X7C, 0X0C, 0X73, 0XED, 0X6B, 0X8C, 0X73, 0X8B,
  0X73, 0XCD, 0X73, 0X6B, 0X7B, 0X69, 0X63, 0X09, 0X39, 0XE6, 0X52, 0XA8,
  0X62, 0XE9, 0X42, 0X27, 0X4A, 0X48, 0X4A, 0X47, 0X4A, 0X67, 0X4A, 0X46,
  0X63, 0X0A, 0X7B, 0XCE, 0X73, 0X8D, 0X84, 0X30, 0X84, 0X0F, 0X8C, 0X71,
  0X9C, 0XF3, 0X84, 0X51, 0X63, 0X6E, 0X53, 0X2E, 0X2A, 0X2A, 0X21, 0XE9,
  0X22, 0X0A, 0X21, 0XC8, 0X31, 0X84, 0X21, 0X86, 0X09, 0X68, 0X09, 0X47,
  0X09, 0X27, 0X11, 0XA8, 0X22, 0X09, 0X22, 0X09, 0X22, 0X09, 0X22, 0X09,
  0X22, 0X0A, 0X2A, 0X0B, 0X21, 0XEA, 0X19, 0XA9, 0X19, 0XA8, 0X19, 0XA8,
  0X19, 0XA9, 0X22, 0X0A, 0X31, 0XE8, 0X21, 0XA7, 0X21, 0XE8, 0X29, 0X85,
  0X29, 0XA6, 0X2A, 0X09, 0X21, 0XE9, 0X19, 0XC9, 0X19, 0XA9, 0X11, 0X88,
  0X19, 0XA8, 0X29, 0XE7, 0X29, 0XE7, 0X19, 0XA7, 0X29, 0XE8, 0X19, 0XE9,
  0X19, 0XE9, 0X19, 0XC9, 0X21, 0XA8, 0X21, 0XA8, 0X19, 0XA8, 0X09, 0X47,
  0X00, 0X61, 0X00, 0X20, 0X21, 0X45, 0X29, 0X85, 0X29, 0X85, 0X29, 0X85,
  0X29, 0X85, 0X29, 0XA6, 0X5B, 0X2A, 0X63, 0X8B, 0X73, 0XAB, 0X6B, 0X6B,
  0X74, 0X51, 0X6B, 0XF0, 0X63, 0X8D, 0X64, 0X30, 0X64, 0X10, 0X64, 0X10,
  0X64, 0X10, 0X5B, 0XF0, 0X6B, 0X8C, 0X6B, 0X8B, 0X6B, 0XAD, 0X6B, 0XCE,
  0X5B, 0X8D, 0X63, 0X8D, 0X63, 0X8C, 0X63, 0XAD, 0X63, 0XED, 0X5B, 0XAC,
  0X6C, 0X2E, 0X6C, 0X0E, 0X63, 0XCD, 0X5B, 0XCB, 0X52, 0XE9, 0X5B, 0X4A,
  0X63, 0X09, 0X42, 0X26, 0X3A, 0X06, 0X63, 0XAC, 0X6C, 0X50, 0X74, 0X0E,
  0X84, 0XD2, 0X63, 0X8D, 0X7C, 0X2F, 0X6C, 0X92, 0X64, 0X52, 0X64, 0X11,
  0X74, 0X94, 0X5B, 0X0A, 0X49, 0XA1, 0X51, 0XE3, 0X5A, 0XA7, 0X6B, 0X6A,
  0X6B, 0X4A, 0X63, 0X09, 0X7C, 0X0D, 0X9D, 0X53, 0X8C, 0XB0, 0X84, 0X0C,
  0X84, 0X4E, 0X9D, 0X12, 0X95, 0X12, 0X7C, 0X51, 0X6B, 0XCE, 0X6B, 0XCE,
  0X7C, 0X50, 0XB5, 0XD6, 0X9D, 0X34, 0X8C, 0XB2, 0X84, 0X70, 0X7C, 0X30,
  0X84, 0X50, 0X73, 0XEE, 0X73, 0XEF, 0X84, 0X51, 0X84, 0X51, 0X94, 0XF4,
  0XA5, 0XB7, 0X95, 0X34, 0X95, 0X34, 0X9D, 0X75, 0X9D, 0XD7, 0X9D, 0XD7,
  0X9D, 0XB7, 0X9D, 0XB6, 0XA5, 0XF7, 0X8C, 0XF3, 0X7C, 0X71, 0X7C, 0X6F,
  0X6B, 0XAA, 0X6B, 0XCB, 0X6B, 0XCB, 0X6B, 0XCB, 0X63, 0X6A, 0X4A, 0XA8,
  0X42, 0X67, 0X42, 0X47, 0X5A, 0XE7, 0X5A, 0XA7, 0X63, 0X2A, 0X74, 0X4E,
  0X63, 0XAC, 0X52, 0X46, 0X63, 0XCC, 0X62, 0XC7, 0X63, 0X08, 0X74, 0X0E,
  0X73, 0XCD, 0X6B, 0X8B, 0X6B, 0XAC, 0X6B, 0XAC, 0X6C, 0X30, 0X74, 0X72,
  0X84, 0X50, 0X94, 0XB2, 0X94, 0XD2, 0X6B, 0XAC, 0X63, 0X4B, 0X63, 0X6B,
  0X5A, 0X87, 0X52, 0X66, 0X5A, 0X86, 0X62, 0XE8, 0X62, 0XE8, 0X62, 0XE8,
  0X63, 0X2A, 0X6C, 0X73, 0X63, 0XCF, 0X52, 0X88, 0X52, 0XC9, 0X52, 0XC8,
  0X52, 0XA7, 0X52, 0XA7, 0X42, 0X46, 0X52, 0XC8, 0X7C, 0X2F, 0X7C, 0X30,
  0X74, 0X0F, 0X74, 0X50, 0X74, 0X30, 0X84, 0X2E, 0X7C, 0X2E, 0X74, 0X2E,
  0X7C, 0X4F, 0X7C, 0X90, 0X7C, 0X90, 0X95, 0X33, 0X95, 0X34, 0X85, 0X56,
  0X8D, 0X55, 0X8D, 0X77, 0X9D, 0XF8, 0X84, 0X90, 0X73, 0X4B, 0X85, 0X34,
  0X7C, 0X4F, 0X7B, 0XCD, 0X7C, 0X92, 0X8D, 0XB8, 0X84, 0X90, 0X74, 0X0D,
  0X6B, 0X49, 0X84, 0X50, 0X85, 0X36, 0X6B, 0XAD, 0X74, 0XB2, 0X8E, 0X19,
  0X75, 0X14, 0X64, 0X51, 0X6C, 0X71, 0X74, 0X92, 0X73, 0XEE, 0X5A, 0XC7,
  0X6B, 0XAC, 0X84, 0XB1, 0X74, 0X0F, 0X6C, 0X2F, 0X74, 0X4F, 0X84, 0XF2,
  0X6C, 0XB4, 0X7D, 0X16, 0X8D, 0X34, 0X6C, 0X0F, 0X4A, 0XCA, 0X4A, 0X69,
  0X4A, 0X68, 0X5A, 0XC9, 0X5B, 0X0B, 0X5B, 0X0B, 0X63, 0X0A, 0X63, 0X4C,
  0X6B, 0X8D, 0X63, 0X0A, 0X62, 0XC9, 0X4A, 0X68, 0X4A, 0X68, 0X4A, 0XA9,
  0X4A, 0XA9, 0X4A, 0XA9, 0X4A, 0XCA, 0X4A, 0XA9, 0X4A, 0X89, 0X4A, 0XA9,
  0X52, 0XCA, 0X52, 0XEA, 0X4A, 0X89, 0X4A, 0XCA, 0X42, 0XA9, 0X52, 0X88,
  0X4A, 0X89, 0X4A, 0X68, 0X4A, 0X89, 0X4A, 0XAA, 0X4A, 0XA9, 0X4A, 0X89,
  0X3A, 0X29, 0X42, 0X8B, 0X3A, 0X49, 0X3A, 0X29, 0X32, 0X29, 0X3A, 0X8B,
  0X74, 0X2F, 0X64, 0X11, 0X4A, 0X8A, 0X3A, 0X28, 0X4A, 0X69, 0X4A, 0X89,
  0X3A, 0X49, 0X3A, 0X49, 0X42, 0X6A, 0X52, 0XEC, 0X5B, 0X8F, 0X6C, 0X75,
  0X5B, 0X6D, 0X52, 0XCA, 0X52, 0XC9, 0X4A, 0X27, 0X4A, 0X47, 0X52, 0X67,
  0X52, 0X47, 0X52, 0X67, 0X52, 0X06, 0X5A, 0XA9, 0X63, 0XCE, 0X63, 0X8D,
  0X5A, 0XEA, 0X52, 0XA8, 0X52, 0X67, 0X4A, 0X47, 0X52, 0XA8, 0X52, 0XA8,
  0X4A, 0X67, 0X4A, 0X68, 0X4A, 0X88, 0X4A, 0X47, 0X42, 0X27, 0X84, 0XB2,
  0X9D, 0X54, 0X9D, 0X55, 0XA5, 0X95, 0XA5, 0X75, 0X94, 0XF3, 0X94, 0XD2,
  0X94, 0XD2, 0X94, 0XD2, 0X94, 0XB2, 0X63, 0XAF, 0X32, 0X6A, 0X42, 0X48,
  0X52, 0XEA, 0X63, 0X4C, 0X6B, 0X6D, 0X6B, 0X4C, 0X5A, 0XEA, 0X63, 0X0A,
  0X4A, 0XAB, 0X42, 0X49, 0X42, 0X48, 0X42, 0X28, 0X42, 0X48, 0X42, 0X48,
  0X3A, 0X07, 0X3A, 0X08, 0X42, 0X29, 0X42, 0X28, 0X39, 0XE8, 0X39, 0XE7,
  0X39, 0XE7, 0X39, 0XC7, 0X39, 0XC7, 0X39, 0XC7, 0X39, 0XE8, 0X41, 0XE8,
  0X52, 0X6A, 0X4A, 0X48, 0X41, 0XE7, 0X42, 0X07, 0X42, 0X27, 0X42, 0X07,
  0X62, 0X69, 0X4A, 0X49, 0X42, 0X28, 0X4A, 0X06, 0X4A, 0X27, 0X52, 0X69,
  0X4A, 0X27, 0X4A, 0X28, 0X4A, 0X49, 0X52, 0X27, 0X4A, 0X28, 0X4A, 0X28,
  0X52, 0X49, 0X39, 0XC6, 0X41, 0XE7, 0X42, 0X28, 0X39, 0XE7, 0X3A, 0X08,
  0X41, 0XE7, 0X41, 0XC6, 0X41, 0XC6, 0X39, 0XC6, 0X41, 0XC6, 0X4A, 0X28,
  0X39, 0XC6, 0X41, 0XE7, 0X42, 0X28, 0X42, 0X28, 0X42, 0X07, 0X42, 0X29,
  0X42, 0X29, 0X42, 0X49, 0X4A, 0X6A, 0X4A, 0X6A, 0X4A, 0X6A, 0X4A, 0X28,
  0X52, 0X29, 0X5A, 0X69, 0X5A, 0X48, 0X5A, 0X48, 0X4A, 0X27, 0X4A, 0X29,
  0X42, 0X07, 0X3A, 0X08, 0X39, 0XC6, 0X41, 0XA5, 0X39, 0XA5, 0X39, 0XA5,
  0X42, 0X07, 0X4A, 0X28, 0X42, 0X07, 0X42, 0X07, 0X4A, 0X28, 0X4A, 0X27,
  0X42, 0X28, 0X42, 0X28, 0X42, 0X07, 0X42, 0X07, 0X42, 0X28, 0X4A, 0X07,
  0X52, 0X27, 0X4A, 0X48, 0X11, 0X46, 0X3A, 0X8B, 0X7C, 0XB3, 0X74, 0X72,
  0X7C, 0X93, 0X73, 0XCF, 0X4A, 0X48, 0X3A, 0X08, 0X42, 0X48, 0X42, 0X28,
  0X3A, 0X28, 0X42, 0X28, 0X3A, 0X28, 0X3A, 0X28, 0X3A, 0X07, 0X31, 0XE7,
  0X42, 0X48, 0X4A, 0X69, 0X42, 0X48, 0X42, 0X28, 0X4A, 0X48, 0X4A, 0X49,
  0X4A, 0X69, 0X42, 0X28, 0X39, 0XE7, 0X42, 0X28, 0X42, 0X48, 0X29, 0X86,
  0X21, 0X86, 0X21, 0XA8, 0X39, 0XE7, 0X42, 0X07, 0X39, 0XE7, 0X31, 0XE7,
  0X32, 0X08, 0X31, 0XE7, 0X31, 0XE8, 0X21, 0XCA, 0X19, 0XCA, 0X19, 0XCB,
  0X1A, 0X2C, 0X22, 0X6C, 0X2A, 0XAC, 0X2A, 0XCC, 0X2A, 0XAC, 0X2A, 0XAB,
  0X2A, 0XCC, 0X2A, 0XCC, 0X2A, 0XEC, 0X3B, 0X0B, 0X3A, 0XEC, 0X43, 0X2D,
  0X21, 0X86, 0X1B, 0X4A, 0X2C, 0X0D, 0X23, 0XAD, 0X2C, 0X10, 0X34, 0X51,
  0X3B, 0X8F, 0X43, 0X6E, 0X53, 0XEF, 0X5C, 0X2F, 0X63, 0XED, 0X73, 0X8A,
  0X52, 0XA7, 0X5A, 0XA9, 0X62, 0XC9, 0X52, 0XCA, 0X43, 0X6E, 0X54, 0X0F,
  0X4C, 0X30, 0X44, 0X10, 0X3C, 0X11, 0X3C, 0X10, 0X44, 0X10, 0X44, 0X0F,
  0X4B, 0XEF, 0X4B, 0XCF, 0X5B, 0XAE, 0X5B, 0X6D, 0X5B, 0X6D, 0X5B, 0X4B,
  0X53, 0X0A, 0X53, 0X2B, 0X5B, 0X48, 0X5B, 0X25, 0X83, 0XC6, 0X7B, 0X66,
  0X73, 0X67, 0X7B, 0XA7, 0X73, 0X87, 0X7B, 0XA7, 0X73, 0X86, 0X7B, 0XA7,
  0X73, 0X87, 0X6B, 0X2B, 0X6B, 0X0B, 0X7B, 0X6D, 0X83, 0X6F, 0X73, 0X4F,
  0X5B, 0X0E, 0X5B, 0X0E, 0X5B, 0X0E, 0X5B, 0X0E, 0X5B, 0X0D, 0X53, 0X0D,
  0X4A, 0XED, 0X4A, 0XCB, 0X52, 0XEB, 0X53, 0X0B, 0X42, 0XAA, 0X42, 0XAB,
  0X42, 0XCB, 0X53, 0X0B, 0X52, 0XCB, 0X4A, 0XAB, 0X42, 0X8C, 0X3A, 0X8C,
  0X22, 0XCC, 0X3B, 0X29, 0X3A, 0XCC, 0X4B, 0X0C, 0X42, 0XEC, 0X1A, 0XAB,
  0X22, 0XAD, 0X2A, 0XAE, 0X1A, 0XAE, 0X1A, 0XCD, 0X32, 0X2A, 0X31, 0XE8,
  0X3A, 0X29, 0X3A, 0X49, 0X42, 0X69, 0X4A, 0X8A, 0X4A, 0XAA, 0X6B, 0XAE,
  0X73, 0XF0, 0X6B, 0XAE, 0X5A, 0XEB, 0X52, 0XCB, 0X63, 0X8E, 0X74, 0X11,
  0X7C, 0X31, 0X6B, 0XCF, 0X53, 0X4D, 0X53, 0X6E, 0X4B, 0X0C, 0X42, 0X49,
  0X4A, 0XAA, 0X4B, 0X0B, 0X4A, 0XCA, 0X42, 0XA9, 0X52, 0XEB, 0X52, 0XEB,
  0X42, 0XCA, 0X42, 0XCA, 0X4A, 0XEA, 0X4A, 0XEA, 0X42, 0XEB, 0X3A, 0XCB,
  0X3A, 0XCB, 0X3A, 0XEC, 0X3A, 0XAA, 0X32, 0X69, 0X32, 0XEC, 0X32, 0XEC,
  0X32, 0XED, 0X3A, 0XCC, 0X4A, 0XEC, 0X43, 0X90, 0X43, 0X4E, 0X43, 0X2D,
  0X43, 0X90, 0X4B, 0XB0, 0X5B, 0X8E, 0X6B, 0XCF, 0X63, 0XAF, 0X8C, 0XB2,
  0X95, 0X14, 0X6B, 0XF0, 0X4B, 0X0B, 0X32, 0X69, 0X42, 0XCB, 0X3A, 0XEC,
  0X3A, 0XCC, 0X42, 0XEE, 0X3A, 0XEE, 0X42, 0XEE, 0X4B, 0X2E, 0X4B, 0X2E,
  0X4B, 0X4F, 0X43, 0X0D, 0X8C, 0X72, 0X7C, 0XB4, 0X85, 0X15, 0X7C, 0XF5,
  0X7D, 0X16, 0X93, 0X08, 0X92, 0X85, 0X6C, 0X32, 0X65, 0X17, 0X6C, 0X10,
  0X63, 0X8D, 0X73, 0XAD, 0X6C, 0X93, 0X62, 0X04, 0X7B, 0X8C, 0X84, 0X50,
  0X8B, 0X0A, 0X82, 0X87, 0X4A, 0X8A, 0X51, 0X43, 0X5B, 0XAC, 0X64, 0X50,
  0X64, 0X93, 0X6B, 0XF0, 0X6B, 0X28, 0X6B, 0X8A, 0X54, 0XD3, 0X6D, 0X13,
  0X7B, 0XAC, 0X73, 0X8B, 0X8B, 0X69, 0X63, 0XCE, 0X5C, 0XD3, 0X74, 0X50,
  0X73, 0XEF, 0X6A, 0X88, 0X62, 0X87, 0X73, 0XCF, 0X74, 0X31, 0X72, 0XE9,
  0X73, 0X2A, 0X6B, 0XEE, 0X63, 0X8D, 0X7B, 0XAD, 0X7C, 0X0F, 0X74, 0X51,
  0X84, 0XB2, 0X7C, 0X72, 0X8B, 0X8B, 0X83, 0XEE, 0X6B, 0X2A, 0X62, 0X66,
  0X6A, 0XC8, 0X83, 0XEE, 0X83, 0XEE, 0X84, 0X50, 0X74, 0XB4, 0X7C, 0X71,
  0X83, 0XEE, 0X83, 0XEE, 0X7C, 0X0F, 0X74, 0XB2, 0X7C, 0XF4, 0X74, 0X10,
  0X63, 0X2C, 0X7B, 0XCE, 0X7C, 0X0E, 0X7C, 0X50, 0X8D, 0X75, 0X85, 0X14,
  0X74, 0X51, 0X7B, 0XAD, 0X83, 0X8C, 0X73, 0X6C, 0X59, 0X83, 0X62, 0X67,
  0X6C, 0X72, 0X74, 0X71, 0X84, 0XD3, 0X74, 0X50, 0X74, 0XD4, 0X74, 0X51,
  0X72, 0X24, 0X83, 0XAD, 0X8B, 0X8C, 0X83, 0XCE, 0X83, 0XEF, 0X51, 0XE5,
  0X63, 0X2C, 0X63, 0X4C, 0X73, 0XCE, 0X41, 0X44, 0X30, 0XC1, 0X39, 0X64,
  0X6B, 0X6B, 0X82, 0XA4, 0X82, 0X65, 0X6A, 0X65, 0X6B, 0XCE, 0X63, 0XCF,
  0X6B, 0XAE, 0X73, 0XEE, 0X82, 0X44, 0X7B, 0X49, 0X7A, 0X86, 0X8A, 0X87,
  0X8B, 0X4B, 0X73, 0XCE, 0X73, 0XAD, 0X83, 0X4B, 0X73, 0X4B, 0X6B, 0X6B,
  0X6B, 0X6B, 0X73, 0X6A, 0X73, 0X8C, 0X5B, 0XCF, 0X5C, 0XB4, 0X73, 0XEE,
  0X73, 0X29, 0X73, 0XAD, 0X74, 0X0F, 0X74, 0X0F, 0X74, 0XB3, 0X6C, 0XD4,
  0X74, 0X92, 0X84, 0X0E, 0X7C, 0XD2, 0X4B, 0X4D, 0X3A, 0XEC, 0X5A, 0XE9,
  0X83, 0XCD, 0X43, 0X6F, 0X5B, 0X6E, 0X83, 0XEF, 0X73, 0XCE, 0X73, 0XCE,
  0X7B, 0XCE, 0X93, 0XCE, 0X93, 0XCD, 0X83, 0XAD, 0X63, 0XF0, 0X5C, 0XB5,
  0X54, 0XD6, 0X7B, 0X6A, 0X73, 0XEE, 0X5D, 0X17, 0X7B, 0X69, 0X8A, 0XA3,
  0X8A, 0XA3, 0X7B, 0X08, 0X4B, 0XF2, 0X5A, 0XC9, 0X5A, 0XA8, 0X62, 0XC9,
  0X7B, 0XAE, 0X6B, 0XCE, 0X8B, 0XCF, 0X83, 0XCF, 0X83, 0XCF, 0X83, 0X8A,
  0X73, 0XAA, 0X6A, 0X65, 0X69, 0XC4, 0X74, 0X10, 0X95, 0X58, 0XB4, 0XD4,
  0XB4, 0X70, 0X9B, 0X8C, 0X6B, 0X6C, 0X83, 0X8C, 0X9B, 0X8D, 0X8B, 0X4B,
  0X8B, 0X0B, 0X82, 0XCA, 0X83, 0X2C, 0X93, 0X6D, 0X93, 0X4C, 0X93, 0X4C,
  0X93, 0X6C, 0X8B, 0X4C, 0X8A, 0XEB, 0X9C, 0X31, 0X8B, 0X8E, 0X93, 0XD0,
  0XAC, 0XB4, 0X93, 0XAE, 0X8B, 0X2C, 0X8B, 0X8E, 0X9C, 0X31, 0X83, 0X0B,
  0X93, 0X8E, 0XAC, 0X51, 0X9B, 0X4C, 0X8A, 0XEA, 0XA4, 0X30, 0X9C, 0X74,
  0XAC, 0XAF, 0X82, 0XC5, 0X51, 0X42, 0X6A, 0X24, 0X7B, 0X07, 0X6A, 0X24,
  0X61, 0XE3, 0X72, 0XA6, 0X83, 0X48, 0X8B, 0XC9, 0X62, 0X66, 0X6A, 0X65,
  0X6A, 0X86, 0X72, 0XC7, 0X7B, 0X28, 0X83, 0X48, 0X82, 0XE7, 0X61, 0XA3,
  0X61, 0XA3, 0X82, 0XE8, 0X7A, 0XA7, 0X8B, 0X4A, 0X83, 0X4B, 0X6A, 0X47,
  0X72, 0X26, 0X69, 0XE4, 0X7A, 0X66, 0X9C, 0X2C, 0X9C, 0X0C, 0X9C, 0X2C,
  0XA4, 0X6D, 0XAC, 0X6E, 0XB4, 0X4C, 0XA3, 0XAA, 0X82, 0XE8, 0X72, 0X88,
  0X62, 0X05, 0X49, 0X44, 0X41, 0X24, 0X6A, 0X68, 0X82, 0XE9, 0X9B, 0X69,
  0X59, 0XC6, 0X40, 0XE4, 0X6A, 0X68, 0XB4, 0XAE, 0X72, 0X25, 0X8B, 0X29,
  0X8B, 0X4A, 0X93, 0X08, 0XA3, 0X89, 0XA4, 0X4D, 0X72, 0XC9, 0X49, 0X44,
  0X83, 0X09, 0X72, 0X88, 0X72, 0X87, 0X7A, 0XA8, 0X7B, 0X4B, 0X7A, 0XA8,
  0X72, 0X46, 0X72, 0X87, 0X7A, 0XA8, 0X82, 0XE8, 0X72, 0X88, 0X5A, 0X06,
  0X6A, 0X25, 0X59, 0XE4, 0X61, 0XE4, 0X49, 0X63, 0X28, 0XC1, 0X41, 0X23,
  0X4A, 0X27, 0X5A, 0XC9, 0X5A, 0XA8, 0X51, 0XC5, 0X51, 0X84, 0X6A, 0X67,
  0X69, 0XE5, 0X62, 0XE8, 0X49, 0X63, 0X38, 0XE2, 0X42, 0X68, 0X5A, 0X26,
  0X61, 0XA3, 0X40, 0XE2, 0X51, 0X23, 0X49, 0X22, 0X49, 0X02, 0X49, 0X02,
  0X41, 0X23, 0X49, 0XA5, 0X41, 0X02, 0X41, 0X43, 0X6A, 0X25, 0X59, 0XA4,
  0X41, 0X23, 0X49, 0X23, 0X49, 0X63, 0X7A, 0XA7, 0X72, 0XA7, 0X7A, 0XA8,
  0X72, 0X46, 0X72, 0X67, 0X7A, 0X86, 0X72, 0XA7, 0X72, 0X86, 0X7A, 0X86,
  0X92, 0XE7, 0X82, 0X86, 0X7A, 0XA6, 0X82, 0X86, 0X8A, 0XC7, 0XA4, 0X6D,
  0XA3, 0X89, 0X7A, 0XE8, 0X7A, 0X87, 0X8B, 0X09, 0X93, 0X6A, 0XA4, 0X8E,
  0XA4, 0X8D, 0XA3, 0X8A, 0XAB, 0X28, 0XAB, 0X29, 0XAB, 0X69, 0X93, 0XAA,
  0X8B, 0X49, 0X93, 0X49, 0X82, 0XC8, 0X6B, 0X09, 0X31, 0X23, 0X28, 0XE2,
  0X30, 0XC3, 0X49, 0XC5, 0X41, 0XC4, 0X31, 0X22, 0X28, 0XC1, 0X41, 0X62,
  0X59, 0XE3, 0X49, 0X83, 0X6B, 0X49, 0X93, 0XCA, 0X7B, 0X8A, 0X6B, 0X09,
  0X83, 0XEC, 0X83, 0X6A, 0X83, 0X29, 0X83, 0X29, 0X83, 0X4A, 0X7B, 0X4A,
  0X5A, 0X27, 0X6A, 0X67, 0X7B, 0X6B, 0X94, 0X6E, 0X94, 0X2D, 0X6B, 0X2A,
  0X61, 0XA6, 0X59, 0X64, 0X69, 0XC6, 0X62, 0XA8, 0X7A, 0X87, 0X92, 0XA7,
  0X82, 0XC7, 0X8A, 0XC8, 0X7A, 0X67, 0X82, 0X87, 0X7A, 0X87, 0X7A, 0XA8,
  0X93, 0XAB, 0X7B, 0X6B, 0X73, 0X4A, 0X62, 0X66, 0X6A, 0X86, 0X6A, 0X86,
  0X6A, 0XA7, 0X82, 0XE7, 0X7A, 0XE7, 0X7A, 0XC6, 0X8B, 0X8A, 0X9B, 0X69,
  0X8B, 0XCB, 0X93, 0XCB, 0X82, 0XC8, 0X93, 0X8A, 0X8B, 0XAB, 0X93, 0XEB,
  0X94, 0X2C, 0X92, 0X85, 0X8A, 0X44, 0X82, 0X25, 0X72, 0X46, 0X41, 0X86,
  0X62, 0X27, 0X8B, 0X29, 0X72, 0X45, 0X82, 0XE8, 0X93, 0X4A, 0X61, 0XE5,
  0X72, 0X66, 0X93, 0X49, 0X72, 0X67, 0X6A, 0X67, 0X93, 0X8A, 0X8B, 0X8B,
  0X83, 0X29, 0X82, 0X66, 0X62, 0X06, 0X7A, 0X67, 0X82, 0XA7, 0X82, 0X87,
  0X82, 0XA8, 0X7B, 0X08, 0X72, 0X66, 0X5A, 0X67, 0XA4, 0X8E, 0X93, 0X8B,
  0X72, 0X25, 0X59, 0XC4, 0X72, 0X87, 0X9C, 0X4D, 0X94, 0X4D, 0X94, 0X2C,
  0X93, 0XEC, 0X8B, 0X8B, 0X72, 0X25, 0X94, 0X0B, 0X5A, 0X25, 0X41, 0X63,
  0X51, 0XC4, 0X72, 0X86, 0X72, 0XA8, 0X83, 0X09, 0X59, 0XA4, 0X59, 0XE5,
  0X83, 0XAA, 0X83, 0X29, 0X8B, 0X09, 0X8B, 0X08, 0X93, 0X08, 0X5A, 0X29,
  0X7B, 0X2A, 0X7A, 0XE9, 0X62, 0X27, 0X8B, 0X29, 0X9B, 0X8A, 0X93, 0X09,
  0X92, 0XA7, 0X8A, 0X26, 0X89, 0XE5, 0X89, 0XE5, 0X81, 0XE4, 0X81, 0XE4,
  0X92, 0X05, 0X9A, 0X05, 0X79, 0X64, 0X61, 0X03, 0X61, 0X63, 0X69, 0X43,
  0X81, 0X44, 0X71, 0X44, 0X51, 0X45, 0X38, 0XC3, 0X38, 0XC3, 0X40, 0XE3,
  0X51, 0X65, 0X38, 0XE2, 0X38, 0XC1, 0X81, 0XC5, 0X48, 0XE4, 0X79, 0XA4,
  0X61, 0X43, 0X48, 0XE3, 0X50, 0XE3, 0X61, 0X24, 0X8A, 0XEE, 0X83, 0X0D,
  0XA1, 0XED, 0X81, 0X4B, 0X61, 0X23, 0X61, 0X45, 0X59, 0X25, 0X50, 0XE4,
  0X41, 0X03, 0X51, 0X03, 0X39, 0X04, 0X21, 0X26, 0X21, 0X06, 0X21, 0X06,
  0X21, 0X68, 0X29, 0X89, 0X29, 0X89, 0X20, 0XE5, 0X20, 0XE4, 0X21, 0X05,
  0X19, 0X05, 0X21, 0X26, 0X19, 0X26, 0X29, 0XCA, 0X29, 0X89, 0X21, 0X47,
  0X21, 0X47, 0X21, 0X26, 0X21, 0X68, 0X29, 0XA9, 0X21, 0X88, 0X21, 0X67,
  0X32, 0X4C, 0X29, 0XEA, 0X21, 0X88, 0X21, 0X89, 0X21, 0X88, 0X21, 0X88,
  0X21, 0X89, 0X21, 0X47, 0X29, 0X05, 0X21, 0X05, 0X21, 0X47, 0X19, 0X26,
  0X19, 0X47, 0X21, 0X88, 0X19, 0X46, 0X19, 0X46, 0X21, 0X67, 0X19, 0X05,
  0X19, 0X05, 0X19, 0X05, 0X19, 0X05, 0X19, 0X05, 0X19, 0X46, 0X19, 0X88,
  0X19, 0X68, 0X19, 0X26, 0X19, 0X47, 0X21, 0XC9, 0X19, 0XA9, 0X10, 0XC4,
  0X10, 0X41, 0X18, 0XC4, 0X19, 0X47, 0X21, 0XA9, 0X21, 0X25, 0X18, 0XE5,
  0X19, 0X05, 0X19, 0X26, 0X19, 0X26, 0X11, 0X26, 0X19, 0X47, 0X18, 0XE5,
  0X19, 0X47, 0X19, 0X47, 0X18, 0XC5, 0X19, 0X27, 0X19, 0X68, 0X21, 0X67,
  0X42, 0X4B, 0X59, 0XA8, 0XB9, 0XA7, 0X81, 0XA9, 0X71, 0XAA, 0X32, 0X6A,
  0X32, 0X07, 0X48, 0XE7, 0X39, 0X8B, 0X3A, 0XA9, 0X3A, 0X26, 0X81, 0X64,
  0X91, 0X05, 0X78, 0XEA, 0X59, 0X6D, 0X72, 0X2B, 0X91, 0X85, 0X89, 0X45,
  0X81, 0X65, 0X7A, 0XE7, 0X18, 0XE2, 0X08, 0X41, 0X30, 0XC2, 0X18, 0X61,
  0X10, 0XA1, 0X38, 0XC4, 0X68, 0XE4, 0X40, 0X83, 0X18, 0X61, 0X18, 0X61,
  0X59, 0X87, 0X31, 0X2B, 0X50, 0XA7, 0X61, 0X07, 0X9A, 0X45, 0X9A, 0X64,
  0X71, 0XE6, 0X6A, 0X85, 0X1A, 0X6B, 0X21, 0X26, 0X79, 0X28, 0X81, 0X49,
  0X61, 0X2A, 0X50, 0XE8, 0X69, 0X07, 0X60, 0X84, 0X51, 0X08, 0X39, 0XAA,
  0X61, 0XC2, 0X81, 0XC3, 0X29, 0XAB, 0X31, 0XAA, 0X20, 0XE3, 0X20, 0XC3,
  0X20, 0XC1, 0X18, 0X61, 0X10, 0X62, 0X29, 0X66, 0X31, 0X87, 0X31, 0XE9,
  0X20, 0XE3, 0X18, 0XA2, 0X18, 0XC2, 0X18, 0XC2, 0X18, 0XC2, 0X18, 0XC3,
  0X18, 0XA2, 0X10, 0X81, 0X18, 0XC2, 0X20, 0XC2, 0X18, 0XC2, 0X18, 0XC2,
  0X20, 0XE2, 0X28, 0XE3, 0X18, 0XA2, 0X18, 0XA2, 0X20, 0XA1, 0X29, 0X47,
  0X31, 0X88, 0X29, 0X47, 0X18, 0XA1, 0X21, 0X06, 0X21, 0X26, 0X18, 0XC2,
  0X18, 0XA2, 0X18, 0XA1, 0X18, 0X81, 0X18, 0X81, 0X18, 0XA1, 0X20, 0XC2,
  0X18, 0XC2, 0X20, 0XC1, 0X28, 0XC1, 0X29, 0X02, 0X39, 0X86, 0X39, 0X85,
  0X39, 0XA6, 0X39, 0X85, 0X20, 0XA1, 0X29, 0X24, 0X29, 0X03, 0X20, 0XC1,
  0X49, 0XC5, 0X39, 0X84, 0X29, 0X23, 0X31, 0X43, 0X29, 0X43, 0X31, 0X64,
  0X5A, 0XC9, 0X5A, 0XAA, 0X63, 0X2B, 0X52, 0X26, 0X52, 0X68, 0X49, 0XE4,
  0X4A, 0X26, 0X52, 0X88, 0X5A, 0XCA, 0X63, 0X2C, 0X63, 0X2B, 0X31, 0X64,
  0X20, 0XE2, 0X31, 0X64, 0X62, 0XE9, 0X5A, 0XA9, 0X52, 0X88, 0X5A, 0XC9,
  0X6B, 0X2B, 0X52, 0XA9, 0X52, 0X89, 0X5A, 0XA9, 0X5A, 0XA9, 0X5A, 0XAA,
  0X73, 0X8C, 0X84, 0X0D, 0X63, 0X0A, 0X52, 0XA9, 0X5A, 0XC9, 0X41, 0XC5,
  0X52, 0X89, 0X4A, 0X28, 0X41, 0X84, 0X41, 0X84, 0X31, 0X24, 0X83, 0X05,
  0XC4, 0XC8, 0X9B, 0XA7, 0X82, 0XA4, 0X49, 0X83, 0X62, 0X86, 0X52, 0X04,
  0X39, 0X63, 0X6A, 0XA6, 0X49, 0XC5, 0X52, 0X05, 0X6A, 0XC7, 0X41, 0XC4,
  0X72, 0XC5, 0X7A, 0XE5, 0X83, 0X06, 0X6A, 0X88, 0X29, 0X04, 0X10, 0X62,
  0X28, 0XA2, 0X20, 0X82, 0X38, 0XC2, 0X18, 0XA4, 0X20, 0XA2, 0X38, 0XE3,
  0X49, 0X03, 0X49, 0X03, 0X49, 0X23, 0X31, 0X04, 0X18, 0XA3, 0X28, 0XA2,
  0X28, 0XA2, 0X30, 0XE3, 0X39, 0X24, 0X39, 0X24, 0X31, 0X24, 0X18, 0XA3,
  0X18, 0XA4, 0X31, 0X05, 0X31, 0X05, 0X31, 0X25, 0X31, 0X25, 0X30, 0XC4,
  0X49, 0X44, 0X41, 0X43, 0X20, 0XA3, 0X39, 0X03, 0X61, 0XC5, 0X92, 0XA6,
  0X6A, 0X24, 0X51, 0X83, 0X39, 0X24, 0X39, 0X25, 0X39, 0X04, 0X51, 0X64,
  0X49, 0X44, 0X20, 0X83, 0X41, 0X24, 0X59, 0X04, 0X20, 0X83, 0X20, 0XA3,
  0X51, 0X24, 0X59, 0XA5, 0X39, 0X24, 0X51, 0X84, 0X51, 0X85, 0X31, 0X06,
  0X51, 0X65, 0X61, 0XC5, 0X49, 0X24, 0X69, 0XE4, 0X82, 0X84, 0X49, 0X44,
  0X59, 0X83, 0X72, 0X24, 0X61, 0XC5, 0X59, 0XC5, 0X59, 0XC4, 0X61, 0XC4,
  0X61, 0XE5, 0X61, 0XC4, 0X61, 0XA3, 0X59, 0XA3, 0X61, 0XA4, 0X49, 0X64,
  0X31, 0X04, 0X49, 0X23, 0X30, 0XE3, 0X28, 0XE3, 0X31, 0X04, 0X39, 0X25,
  0X39, 0X24, 0XAB, 0X6A, 0X62, 0X27, 0X49, 0X22, 0X6B, 0X4C, 0X6B, 0X4D,
  0X5A, 0XAA, 0X52, 0X89, 0X62, 0XEB, 0X62, 0X68, 0X6A, 0X89, 0X5A, 0X27,
  0X6A, 0X69, 0X6A, 0X89, 0X6A, 0XCB, 0X6A, 0XCB, 0X62, 0XAA, 0X62, 0XEA,
  0X62, 0XEB, 0X62, 0XAA, 0X6B, 0X2C, 0X6B, 0X6D, 0X62, 0XCA, 0X62, 0XA9,
  0X6A, 0XAA, 0X6A, 0X8A, 0X62, 0XCA, 0X7B, 0X6D, 0X7B, 0X4D, 0X6A, 0XA9,
  0X6A, 0XCB, 0X62, 0XCB, 0X5A, 0XAB, 0X6B, 0X2D, 0X6B, 0X2D, 0X5A, 0XCB,
  0X63, 0X2C, 0X63, 0X0B, 0X5A, 0X8A, 0X5A, 0X8A, 0X5A, 0X8A, 0X5A, 0X6A,
  0X5A, 0X89, 0X5A, 0X47, 0X62, 0X88, 0X6A, 0XCB, 0X62, 0XAA, 0X4A, 0X28,
  0X42, 0X28, 0X4A, 0X28, 0X5A, 0X8A, 0X6A, 0XEB, 0X5A, 0XCB, 0X62, 0XCB,
  0X6B, 0X2C, 0X5A, 0X49, 0X51, 0X65, 0X4A, 0X49, 0X4A, 0X8A, 0X83, 0X4B,
  0X93, 0X6A, 0X7B, 0X29, 0X7B, 0X29, 0X7B, 0X4A, 0X72, 0XE9, 0X7B, 0X09,
  0X7B, 0X4A, 0X8B, 0X4A, 0X83, 0X09, 0X7A, 0XC9, 0X8A, 0XC9, 0X8A, 0XE9,
  0X8B, 0X6B, 0X82, 0XE9, 0X6A, 0X26, 0X61, 0X64, 0X6A, 0X27, 0X72, 0X88,
  0X72, 0XA8, 0X62, 0X07, 0X51, 0X85, 0X41, 0X86, 0X31, 0X25, 0X29, 0X45,
  0X31, 0X65, 0X29, 0X25, 0X21, 0X04, 0X39, 0XA7, 0X5A, 0XAA, 0X29, 0X04,
  0X18, 0XC3, 0X18, 0XC3, 0X29, 0X25, 0X39, 0XA7, 0X29, 0X45, 0X18, 0XE3,
  0X18, 0XC3, 0X41, 0XE8, 0X21, 0X04, 0X08, 0X41, 0X21, 0X04, 0X29, 0X66,
  0X21, 0X04, 0X21, 0X04, 0X20, 0XE4, 0X21, 0X04, 0X29, 0X04, 0X10, 0X82,
  0X18, 0XC3, 0X21, 0X25, 0X20, 0XE4, 0X20, 0XE4, 0X29, 0X24, 0X29, 0X24,
  0X29, 0X24, 0X18, 0XA3, 0X21, 0X04, 0X29, 0X24, 0X10, 0X61, 0X21, 0X04,
  0X08, 0X41, 0X10, 0X61, 0X18, 0XC3, 0X18, 0X82, 0X18, 0XC2, 0X08, 0X41,
  0X18, 0XC2, 0X10, 0X81, 0X10, 0X61, 0X20, 0XC3, 0X18, 0X82, 0X08, 0X41,
  0X08, 0X61, 0X18, 0XA2, 0X20, 0XC3, 0X20, 0XE3, 0X20, 0XE3, 0X18, 0XC2,
  0X18, 0XA2, 0X18, 0XA2, 0X10, 0X61, 0X18, 0X81, 0X18, 0XA2, 0X18, 0XA2,
  0X20, 0XC2, 0X29, 0X24, 0X31, 0X66, 0X31, 0X86, 0X21, 0X04, 0X20, 0XE4,
  0X18, 0XC3, 0X10, 0XA3, 0X18, 0XA4, 0X18, 0XA4, 0X18, 0XC4, 0X20, 0XC4,
  0X18, 0XE5, 0X10, 0XA4, 0X10, 0X83, 0X18, 0XC4, 0X21, 0X06, 0X20, 0XE4,
  0X21, 0X05, 0X20, 0XE5, 0X10, 0XA3, 0X18, 0X82, 0X41, 0X64, 0X29, 0X03,
  0X18, 0XA2, 0X20, 0XC3, 0X18, 0XA2, 0X18, 0XC3, 0X28, 0XE3, 0X29, 0X04,
  0X21, 0X25, 0X29, 0X25, 0X29, 0X45, 0X29, 0X04, 0X18, 0XA2, 0X20, 0XE3,
  0X31, 0X45, 0X18, 0XC3, 0X21, 0X24, 0X29, 0X45, 0X18, 0XE4, 0X18, 0XC4,
  0X20, 0XE3, 0X18, 0XC2, 0X29, 0X44, 0X31, 0X45, 0X18, 0XC4, 0X21, 0X05,
  0X18, 0XC3, 0X18, 0XC3, 0X20, 0XE4, 0X20, 0XE4, 0X20, 0XE3, 0X31, 0X66,
  0X39, 0X86, 0X21, 0X04, 0X18, 0XC3, 0X18, 0XA2, 0X18, 0XA3, 0X21, 0X04,
  0X29, 0X66, 0X29, 0X66, 0X31, 0X87, 0X10, 0XA3, 0X00, 0X00, 0X20, 0X40,
  0X79, 0XE4, 0XAB, 0X68, 0X92, 0XE7, 0X39, 0X23, 0X41, 0X43, 0X41, 0X23,
  0X79, 0XC4, 0X8A, 0X25, 0X82, 0X25, 0X82, 0X25, 0X82, 0X05, 0X8A, 0X05,
  0X89, 0XE5, 0X79, 0X83, 0X89, 0XC4, 0X9A, 0X25, 0X79, 0X83, 0X81, 0XA3,
  0X79, 0X83, 0X81, 0XC4, 0X89, 0XE5, 0X89, 0XE5, 0X69, 0X85, 0X71, 0XC5,
  0X61, 0X84, 0X82, 0X6A, 0X82, 0XAC, 0X59, 0X44, 0X61, 0XA6, 0X59, 0X85,
  0X71, 0XE6, 0X79, 0XC6, 0X69, 0X85, 0X81, 0XE5, 0X82, 0X05, 0X82, 0X27,
  0X8A, 0X69, 0X92, 0X68, 0XA2, 0X86, 0X92, 0X68, 0X8A, 0X47, 0X81, 0XE5,
  0X9A, 0X66, 0X6B, 0X0A, 0X83, 0X08, 0X8B, 0X27, 0X82, 0XC7, 0X7A, 0XA7,
  0X82, 0XC8, 0X8B, 0X08, 0X8B, 0X08, 0X83, 0X08, 0X83, 0X08, 0X8B, 0X08,
  0X93, 0X28, 0X8B, 0X29, 0X7A, 0XE8, 0X8A, 0XE7, 0X93, 0X28, 0X9B, 0X89,
  0X8A, 0XC7, 0X8A, 0XE7, 0X8B, 0X28, 0X8B, 0X28, 0X8B, 0X08, 0X93, 0X07,
  0X82, 0XA6, 0X82, 0XC7, 0X82, 0XA6, 0X82, 0XA6, 0X8B, 0X28, 0X93, 0X69,
  0X93, 0X48, 0X8A, 0XE7, 0X7A, 0XE8, 0X7B, 0X08, 0X72, 0XA8, 0X7B, 0X09,
  0X7A, 0XE8, 0X82, 0XE8, 0X5A, 0X05, 0X6A, 0X67, 0X83, 0X09, 0X93, 0X49,
  0X7A, 0XC9, 0X7A, 0XE9, 0X8B, 0X29, 0X62, 0X47, 0X62, 0X67, 0X6A, 0X25,
  0X18, 0X62, 0X18, 0X62, 0X10, 0X62, 0X10, 0X42, 0X18, 0X62, 0X18, 0X62,
  0X18, 0X62, 0X20, 0X83, 0X28, 0XA3, 0X28, 0XA4, 0X18, 0X63, 0X18, 0X83,
  0X20, 0X84, 0X18, 0X83, 0X18, 0X83, 0X20, 0X83, 0X18, 0X83, 0X18, 0X62,
  0X39, 0X85, 0X72, 0XE8, 0X72, 0XA7, 0X6A, 0X45, 0X82, 0XE7, 0X8A, 0XE6,
  0X6A, 0X66, 0X8A, 0XC6, 0X7A, 0X66, 0X62, 0X25, 0X51, 0XE6, 0X5A, 0X67,
  0X49, 0XE5, 0X31, 0X44, 0X49, 0XE6, 0X49, 0XE5, 0X6A, 0X45, 0X59, 0XE5,
  0X6A, 0X86, 0X8B, 0X28, 0X8B, 0X28, 0X7A, 0XE7, 0X82, 0XE7, 0X83, 0X08,
  0X8B, 0X49, 0X93, 0X49, 0X93, 0X29, 0X93, 0X29, 0X93, 0X49, 0X93, 0X49,
  0XA3, 0X8A, 0X31, 0X44, 0X42, 0X26, 0XA3, 0X69, 0X8B, 0X29, 0X72, 0XEA,
  0X72, 0XCA, 0X29, 0XE7, 0X1A, 0X47, 0X5A, 0XA9, 0X21, 0XC6, 0X72, 0XA8,
  0X72, 0XA7, 0X8B, 0X09, 0X6A, 0X87, 0X6A, 0X66, 0X62, 0X66, 0X5A, 0X46,
  0X72, 0XC8, 0X5A, 0X45, 0X82, 0XE8, 0X6A, 0X87, 0X52, 0X05, 0X62, 0X46,
  0X5A, 0X46, 0X62, 0X66, 0X82, 0XC7, 0X62, 0X86, 0X5A, 0X26, 0X6A, 0X87,
  0X82, 0XE8, 0X8B, 0X07, 0X7B, 0X08, 0X6A, 0X46, 0X6A, 0XA7, 0X83, 0X09,
  0X5A, 0X67, 0X52, 0X07, 0X49, 0XC6, 0X49, 0XE6, 0X4A, 0X07, 0X52, 0X07,
  0X31, 0X85, 0X62, 0XCA, 0X6B, 0X0A, 0X52, 0X69, 0X52, 0X69, 0X6A, 0XEA,
  0X62, 0XA9, 0X4A, 0X07, 0X52, 0X48, 0X52, 0X69, 0X62, 0XEB, 0X52, 0X48,
  0X5A, 0X69, 0X52, 0X69, 0X4A, 0X48, 0X4A, 0X48, 0X42, 0X07, 0X41, 0XE7,
  0X42, 0X07, 0X4A, 0X27, 0X49, 0XE6, 0X49, 0XE6, 0X5A, 0X49, 0X72, 0XEA,
  0X5A, 0X47, 0X5A, 0X27, 0XB5, 0X34, 0X9C, 0X2F, 0XC5, 0X74, 0X8B, 0X8D,
  0XBD, 0X95, 0X9C, 0X0E, 0X8B, 0XCE, 0XBD, 0X75, 0X72, 0XEA, 0XA4, 0XB2,
  0X7B, 0X2B, 0X6A, 0XA9, 0X49, 0XE6, 0X6A, 0X89, 0X73, 0X0B, 0XA4, 0XD2,
  0X6A, 0X89, 0X6A, 0XA9, 0X6A, 0XA9, 0X6A, 0XA9, 0X6A, 0XA9, 0X6A, 0X88,
  0X6A, 0X88, 0X6A, 0XA9, 0X6A, 0XA8, 0X6A, 0XA9, 0X6A, 0XCA, 0X6A, 0XC9,
  0X7A, 0XEA, 0X7B, 0X2B, 0X7B, 0X2B, 0X7B, 0X2B, 0X7B, 0X2B, 0X7B, 0X0B,
  0X8B, 0XAC, 0X83, 0X6C, 0X72, 0XC9, 0X9C, 0X2D, 0X83, 0X6B, 0X72, 0XA9,
  0X72, 0XEA, 0X8B, 0X8D, 0X93, 0XCE, 0X93, 0XCD, 0X93, 0XCD, 0X93, 0XEE,
  0X9B, 0XEE, 0X9C, 0X0F, 0X9C, 0X2E, 0X8B, 0X6A, 0X8B, 0X4A, 0X8B, 0X4A,
  0X7B, 0X0A, 0X5A, 0X47, 0X73, 0XAF, 0X6B, 0X2D, 0X5A, 0X48, 0X62, 0X68,
  0X6A, 0XA9, 0X73, 0X4C, 0X83, 0XAE, 0X84, 0X11, 0XBD, 0XD8, 0XB6, 0X1B,
  0X43, 0X58, 0XAD, 0X7D, 0X3B, 0X3B, 0X64, 0X18, 0X32, 0X6C, 0X32, 0X8E,
  0X6C, 0X35, 0X7C, 0X53, 0X8C, 0XB2, 0X83, 0XEF, 0X7B, 0XCF, 0X73, 0XCD,
  0X6B, 0XF0, 0X6B, 0XD4, 0X73, 0XB1, 0X63, 0XD3, 0X52, 0XAE, 0X7C, 0X33,
  0X7B, 0XF1, 0X7B, 0XAE, 0X73, 0X8E, 0X73, 0X90, 0X7B, 0XCE, 0X7B, 0XAC,
  0X8C, 0X0E, 0X84, 0X12, 0X83, 0XD0, 0X83, 0X8E, 0X83, 0XAF, 0X7B, 0XD1,
  0X7B, 0XAF, 0X73, 0X8E, 0X8C, 0X30, 0X8C, 0X30, 0X73, 0XAF, 0X83, 0XCE,
  0X8C, 0X0F, 0X8C, 0X10, 0X83, 0XF0, 0X73, 0XF1, 0X7B, 0XF0, 0X84, 0X10,
  0X7B, 0XAD, 0X7B, 0X8F, 0X73, 0X6C, 0X73, 0X8E, 0X8C, 0X0F, 0X6B, 0X50,
  0X5A, 0XEF, 0X6C, 0X4E, 0X3B, 0X6C, 0X63, 0X4D, 0X8C, 0X2E, 0X84, 0X4F,
  0X73, 0X6C, 0X52, 0XEA, 0X43, 0X6A, 0X43, 0XEB, 0X43, 0XEB, 0X44, 0X0B,
  0X44, 0X0B, 0X63, 0XEE, 0X7C, 0X0F, 0X64, 0X2C, 0X73, 0XCD, 0X73, 0XCD,
  0X4B, 0X89, 0X43, 0X68, 0X4B, 0X69, 0X4B, 0X69, 0X43, 0X69, 0X4B, 0X69,
  0X6C, 0X73, 0X74, 0X94, 0X6C, 0XB5, 0X6C, 0X94, 0X64, 0X73, 0X6C, 0XB4,
  0X63, 0X4C, 0X73, 0X4A, 0X73, 0X8C, 0X52, 0X48, 0X62, 0XEA, 0X6B, 0X2B,
  0X6B, 0X2B, 0X6B, 0X2C, 0X5A, 0XA9, 0X63, 0X2B, 0X62, 0XEB, 0X73, 0X6D,
  0X7B, 0X8E, 0X7B, 0X8D, 0X52, 0X8A, 0X63, 0X0B, 0X7B, 0XCE, 0X73, 0X8D,
  0X7B, 0XCE, 0X7B, 0X8E, 0X7B, 0X8D, 0X7B, 0XAE, 0X6B, 0X2C, 0X73, 0X6D,
  0X7B, 0XAE, 0X83, 0XEF, 0X8C, 0X0F, 0X94, 0X50, 0X8C, 0X2F, 0X83, 0X8D,
  0X83, 0X6C, 0X83, 0X8D, 0X83, 0X4C, 0X83, 0X4C, 0X8B, 0XCE, 0X83, 0X6C,
  0X8B, 0XCE, 0X9C, 0X91, 0XA4, 0XF2, 0X9D, 0X14, 0X7B, 0XAE, 0X7B, 0X8D,
  0X94, 0X91, 0X8C, 0X50, 0X7B, 0X6C, 0X94, 0X2F, 0X83, 0XEE, 0X8C, 0X0F,
  0X7B, 0XAD, 0X9C, 0X0E, 0X93, 0XED, 0X73, 0X8D, 0X83, 0XCE, 0X83, 0XCE,
  0X83, 0XCE, 0X73, 0X8D, 0X39, 0XC7, 0X41, 0XE7, 0X41, 0XE6, 0X41, 0XE7,
  0X41, 0XC7, 0X4A, 0X08, 0X52, 0X48, 0X52, 0X28, 0X52, 0X28, 0X52, 0X28,
  0X4A, 0X07, 0X4A, 0X07, 0X4A, 0X07, 0X4A, 0X07, 0X52, 0X27, 0X52, 0X27,
  0X52, 0X27, 0X52, 0X27, 0X39, 0XA6, 0X31, 0X65, 0X31, 0X65, 0X31, 0X65,
  0X31, 0X65, 0X41, 0XE7, 0X41, 0XE7, 0X41, 0XC6, 0X4A, 0X48, 0X4A, 0X07,
  0X39, 0X65, 0X39, 0XA6, 0X31, 0X24, 0X5A, 0X69, 0X83, 0XAE, 0X73, 0X2C,
  0X4A, 0X07, 0X39, 0XA6, 0X42, 0X07, 0X4A, 0X28, 0X4A, 0X28, 0X41, 0XC7,
  0X8B, 0XEE, 0XA4, 0XB1, 0XA4, 0XD2, 0X54, 0X2F, 0X43, 0XED, 0X33, 0X27,
  0X4B, 0X89, 0X53, 0XEA, 0X3B, 0X4B, 0X19, 0XE9, 0X22, 0X08, 0X29, 0XE8,
  0X29, 0XC7, 0X42, 0XCA, 0X19, 0X05, 0X32, 0X29, 0X10, 0XC4, 0X21, 0XA6,
  0X21, 0XA6, 0X21, 0X45, 0X29, 0X65, 0X29, 0X65, 0X29, 0X65, 0X31, 0X65,
  0X21, 0X24, 0X29, 0X86, 0X39, 0XE7, 0X41, 0XE7, 0X39, 0XE7, 0X39, 0XE7,
  0X4A, 0X28, 0X42, 0X08, 0X42, 0X08, 0X41, 0XE7, 0X29, 0XA6, 0X21, 0X66,
  0X10, 0XE4, 0X18, 0XC4, 0X21, 0X66, 0X21, 0XC8, 0X22, 0X48, 0X4B, 0X6C,
  0X53, 0X6D, 0X7C, 0X0C, 0XA5, 0X0F, 0XA4, 0XEE, 0X9C, 0X2B, 0X8B, 0XEA,
  0XBD, 0X0D, 0XA4, 0X8D, 0X8B, 0XAA, 0X9B, 0X8C, 0X8B, 0XEC, 0X39, 0X63,
  0X39, 0XA4, 0X21, 0X03, 0X39, 0X84, 0X31, 0X23, 0X29, 0X24, 0X42, 0X86,
  0X42, 0XA6, 0X4A, 0X86, 0X41, 0XA4, 0X4A, 0X87, 0X42, 0X05, 0X39, 0X84,
  0X29, 0X04, 0X39, 0X84, 0X29, 0X04, 0X20, 0XC3, 0X20, 0XC3, 0X31, 0X64,
  0X29, 0X23, 0X29, 0X23, 0X31, 0X44, 0X29, 0X23, 0X29, 0X23, 0X31, 0X64,
  0X31, 0X44, 0X31, 0X44, 0X39, 0X84, 0X29, 0X44, 0X29, 0X04, 0X29, 0X03,
  0X20, 0XE3, 0X18, 0XA2, 0X29, 0XC3, 0X53, 0X4E, 0X4B, 0X12, 0X4B, 0X12,
  0X42, 0XF3, 0X42, 0XC7, 0X3A, 0XF0, 0X43, 0X14, 0X53, 0X4F, 0X4B, 0X12,
  0X5A, 0XEA, 0X53, 0X68, 0X53, 0X27, 0X5B, 0X6C, 0X63, 0XF7, 0X53, 0XB4,
  0X53, 0XF8, 0X73, 0XF0, 0X7B, 0XEC, 0X93, 0XAB, 0XAB, 0X69, 0XBC, 0X2B,
  0X93, 0X2A, 0X82, 0XEA, 0XAB, 0XCB, 0XD4, 0XAB, 0X93, 0X2A, 0X93, 0X28,
  0X8B, 0X2B, 0X93, 0X29, 0X8A, 0X66, 0X59, 0XE6, 0X19, 0X66, 0X21, 0X86,
  0X19, 0X45, 0X6C, 0X13, 0X74, 0X12, 0X52, 0XEA, 0X5B, 0XCF, 0X6B, 0X4C,
  0X7A, 0XC7, 0X7A, 0XC7, 0X42, 0XEF, 0X7B, 0X4D, 0X9C, 0X93, 0X63, 0XD1,
  0X53, 0XC7, 0X42, 0XE9, 0X5A, 0XCE, 0X94, 0XF1, 0X7C, 0X4E, 0X8B, 0XAA,
  0X7B, 0XEC, 0X83, 0X8A, 0X62, 0XE9, 0X83, 0X49, 0X93, 0XA9, 0X63, 0X4D,
  0X7C, 0X11, 0X8A, 0XC8, 0X52, 0X49, 0X53, 0XCE, 0X74, 0X4E, 0X8B, 0X49,
  0X7B, 0XAA, 0X84, 0X6C, 0X84, 0X6C, 0X83, 0X69, 0X84, 0X0A, 0X84, 0X0B,
  0X83, 0X2B, 0X61, 0XA9, 0X69, 0X69, 0XA2, 0X6D, 0XAA, 0XCF, 0X61, 0X89,
  0X38, 0X84, 0X38, 0XA4, 0X58, 0XC7, 0X71, 0X49, 0X69, 0XC8, 0XA2, 0X4D,
  0X89, 0XEB, 0X79, 0X69, 0X50, 0XE5, 0X61, 0X07, 0X48, 0XC5, 0X50, 0XE6,
  0X38, 0X83, 0X69, 0X67, 0X69, 0XA5, 0X40, 0XA4, 0X51, 0X46, 0X51, 0X66,
  0XA3, 0X89, 0XA3, 0X69, 0X7A, 0X67, 0X9B, 0X28, 0X39, 0X43, 0X20, 0XC2,
  0X72, 0X45, 0X31, 0X03, 0X7A, 0XAA, 0X41, 0X85, 0X39, 0X63, 0X51, 0XE6,
  0X10, 0X81, 0X18, 0X81, 0X6A, 0XAA, 0X39, 0X65, 0X42, 0X28, 0X29, 0X03,
  0X18, 0X81, 0X10, 0X61, 0X62, 0X26, 0X8B, 0X49, 0X8B, 0X48, 0X93, 0X88,
  0X8B, 0X07, 0X7A, 0X85, 0X72, 0X44, 0X61, 0XE3, 0X40, 0XE1, 0X61, 0XC3,
  0X61, 0XE3, 0X61, 0XC3, 0X61, 0XC3, 0X61, 0XC3, 0X61, 0XA3, 0X59, 0XC3,
  0X61, 0XC3, 0X51, 0XA3, 0X41, 0X82, 0X51, 0X82, 0X41, 0X63, 0X6A, 0X45,
  0X6A, 0X45, 0X41, 0X83, 0X41, 0X63, 0X51, 0X83, 0X51, 0X83, 0X49, 0X83,
  0X49, 0XC3, 0X49, 0XC3, 0X49, 0XC3, 0X49, 0XA3, 0X39, 0X63, 0X31, 0X02,
  0X31, 0X42, 0X41, 0X83, 0X49, 0X42, 0X49, 0X63, 0X8B, 0X6B, 0X83, 0X6A,
  0X8B, 0XCD, 0X8B, 0XCD, 0X93, 0XCD, 0X9B, 0XED, 0X9B, 0XED, 0X5A, 0X26,
  0X83, 0X29, 0X62, 0X68, 0X39, 0X64, 0X49, 0XA5, 0X39, 0X64, 0X83, 0X2A,
  0X7B, 0X2A, 0X4A, 0XCA, 0X7B, 0X2A, 0X9B, 0XEB, 0XCD, 0X71, 0XDD, 0XD3,
  0X62, 0XCA, 0X41, 0XC7, 0X5A, 0X28, 0X5A, 0X69, 0X52, 0X48, 0X52, 0X08,
  0X52, 0X28, 0X31, 0XA7, 0X19, 0X47, 0X19, 0X47, 0X21, 0X67, 0X31, 0XA8,
  0X63, 0XB0, 0X84, 0X2D, 0X84, 0X4C, 0X74, 0X0E, 0X83, 0XCC, 0XBB, 0XC8,
  0X29, 0X25, 0X29, 0X46, 0X21, 0X25, 0X21, 0X05, 0X29, 0X65, 0X3A, 0X08,
  0X52, 0XEB, 0X3A, 0X49, 0X42, 0X48, 0X39, 0XE8, 0X4A, 0X49, 0X4A, 0X49,
  0X3A, 0X49, 0X3A, 0X28, 0X31, 0XA6, 0X42, 0X69, 0X4B, 0XF0, 0X43, 0X8F,
  0X32, 0X87, 0X32, 0X65, 0X5A, 0X8A, 0X6B, 0X0D, 0X39, 0X87, 0X41, 0XE8,
  0X29, 0X67, 0X31, 0X66, 0X5A, 0X8A, 0X52, 0X69, 0X52, 0X89, 0X5A, 0X8A,
  0X52, 0X69, 0X3A, 0X07, 0X42, 0X28, 0X4A, 0X28, 0X41, 0XC7, 0X5A, 0X69,
  0X42, 0X49, 0X39, 0XC7, 0X4A, 0X68, 0X42, 0X28, 0X42, 0X27, 0X52, 0X89,
  0X4A, 0XA7, 0X43, 0X6D, 0X4B, 0X91, 0X53, 0XD2, 0X5C, 0X12, 0X43, 0X8D,
  0X3B, 0X2C, 0X2A, 0XAC, 0X32, 0XED, 0X5B, 0X89, 0X73, 0XEC, 0X5B, 0XAF,
  0X53, 0XAE, 0X43, 0X0A, 0X21, 0X86, 0X21, 0XA6, 0X29, 0XA6, 0X21, 0XA6,
  0X42, 0XCB, 0X4A, 0XEB, 0X29, 0XC6, 0X2A, 0X07, 0X2A, 0X07, 0X29, 0XE7,
  0X3A, 0X28, 0X42, 0X47, 0X31, 0XA5, 0X31, 0XA5, 0X29, 0X85, 0X31, 0X64,
  0X41, 0XC5, 0X5A, 0X8A, 0X5A, 0X8A, 0X4A, 0X07, 0X51, 0XE5, 0X6A, 0X64,
  0X6A, 0X44, 0X8B, 0X27, 0X8A, 0XE6, 0X82, 0XC5, 0X8B, 0X48, 0X83, 0X07,
  0X7A, 0X44, 0X72, 0X45, 0X7A, 0XA5, 0X7A, 0XA5, 0X31, 0X22, 0X28, 0XE1,
  0X29, 0X02, 0X29, 0X02, 0X28, 0XE1, 0X29, 0X02, 0X62, 0XE8, 0X6B, 0X29,
  0X6B, 0X27, 0X72, 0XC6, 0X6A, 0X45, 0X7A, 0XC6, 0X7A, 0XC6, 0X7A, 0XC6,
  0X7A, 0XC6, 0X7A, 0XC6, 0X82, 0XE6, 0X72, 0X85, 0X7A, 0X86, 0X7A, 0X65,
  0X73, 0X08, 0X5B, 0X49, 0X6A, 0XC6, 0X6B, 0X07, 0X5A, 0X66, 0X51, 0XE6,
  0X73, 0XEF, 0X73, 0XAD, 0X5A, 0X68, 0X52, 0X48, 0X73, 0XAD, 0X7B, 0XAC,
  0X83, 0X8D, 0X6B, 0X0A, 0X6A, 0XE9, 0X63, 0X4B, 0X73, 0XAD, 0X62, 0XC9,
  0X6A, 0XE9, 0X84, 0X0E, 0X83, 0XAC, 0X5A, 0X46, 0X6B, 0X29, 0X6B, 0X2B,
  0X42, 0X2A, 0X52, 0X49, 0X8A, 0XA4, 0X8A, 0X84, 0X72, 0X86, 0X6A, 0X86,
  0X7A, 0X85, 0X7A, 0XA5, 0X7A, 0XA6, 0X7A, 0X85, 0X5A, 0XA7, 0X7A, 0XC6,
  0X82, 0XA5, 0X8A, 0XA6, 0X8A, 0X85, 0X83, 0X08, 0X72, 0XC7, 0X7A, 0XC6,
  0X72, 0X45, 0X6A, 0X67, 0X11, 0X8F, 0X21, 0XEC, 0X2A, 0X49, 0X32, 0X69,
  0X42, 0XEB, 0X63, 0X69, 0X5B, 0X2A, 0X32, 0X26, 0X29, 0XC5, 0X29, 0XE6,
  0X3A, 0X8B, 0X2A, 0X4C, 0X22, 0X0B, 0X21, 0XEB, 0X42, 0XAA, 0X42, 0XAA,
  0X09, 0X49, 0X19, 0XA8, 0X32, 0X47, 0X32, 0X27, 0X32, 0X27, 0X32, 0X48,
  0X3A, 0X67, 0X32, 0X48, 0X3A, 0X88, 0X3A, 0X67, 0X72, 0X66, 0X51, 0XC4,
  0X39, 0XC4, 0X32, 0X47, 0X2A, 0X47, 0X21, 0X22, 0X29, 0XE6, 0X2A, 0X08,
  0X20, 0XE0, 0X20, 0XE1, 0X20, 0XE0, 0X21, 0X22, 0X29, 0X84, 0X29, 0X63,
  0X21, 0X86, 0X49, 0XC4, 0X8A, 0XA5, 0X92, 0XE7, 0X82, 0XE6, 0X8A, 0XA5,
  0X7A, 0XA5, 0X82, 0X85, 0X82, 0XC6, 0X5A, 0X45, 0X5A, 0X05, 0X7A, 0X45,
  0X8A, 0X85, 0X62, 0X04, 0X72, 0X23, 0X7A, 0X24, 0X72, 0X23, 0X62, 0X03,
  0X6A, 0X04, 0X38, 0XE2, 0X28, 0XC1, 0X31, 0X63, 0X39, 0X64, 0X5A, 0X04,
  0X4A, 0X46, 0X2A, 0XAA, 0X32, 0XAA, 0X2A, 0X8A, 0X32, 0XAA, 0X7B, 0XAC,
  0X53, 0X0B, 0X3A, 0X8A, 0X29, 0XE7, 0X21, 0X86, 0X21, 0XC7, 0X4A, 0XCB,
  0X61, 0XA3, 0X5A, 0X66, 0X72, 0XE8, 0X5A, 0XE8, 0X8A, 0XA6, 0X92, 0X85,
  0X7A, 0XC7, 0X4B, 0X0A, 0X6B, 0X09, 0X6A, 0XE9, 0X82, 0X86, 0X82, 0X45,
  0X49, 0X82, 0X29, 0X82, 0X6A, 0XC8, 0X63, 0X2A, 0X6A, 0X66, 0X6A, 0X24,
  0X72, 0X65, 0X7A, 0X45, 0X83, 0X49, 0X72, 0X46, 0X61, 0XE4, 0X42, 0X27,
  0X41, 0XC6, 0X39, 0X63, 0X62, 0X47, 0X6B, 0X4A, 0X52, 0X67, 0X6B, 0X09,
  0X39, 0X43, 0X7A, 0X45, 0X82, 0X85, 0XBB, 0XC9, 0X9A, 0XC7, 0XAA, 0XC6,
  0XA3, 0X07, 0XBB, 0X68, 0XB3, 0X47, 0XA2, 0X85, 0X8A, 0XC5, 0XAB, 0X66,
  0XA3, 0X46, 0XB3, 0X47, 0X9A, 0XE6, 0X7A, 0X64, 0X6A, 0X24, 0XA3, 0X27,
  0X9B, 0X06, 0XB3, 0X67, 0XB3, 0XA7, 0XBB, 0XE8, 0XB3, 0X27, 0XC3, 0XA9,
  0XA2, 0XE7, 0X7A, 0X26, 0X92, 0X65, 0X72, 0X43, 0X6B, 0X4C, 0X94, 0XB2,
  0X94, 0X91, 0X94, 0X91, 0X94, 0X92, 0X94, 0X91, 0X94, 0XB2, 0X42, 0X26,
  0X73, 0XAE, 0X94, 0XB2, 0X8C, 0X91, 0X8C, 0X91, 0X8C, 0X91, 0X8C, 0X91,
  0X42, 0X26, 0X31, 0XA4, 0X31, 0XA4, 0X42, 0X47, 0X94, 0X92, 0X94, 0X91,
  0X42, 0X07, 0X29, 0X64, 0X4A, 0X48, 0X5A, 0X88, 0X5A, 0X87, 0X5A, 0X87,
  0X41, 0XE6, 0X29, 0X64, 0X31, 0X85, 0X29, 0X64, 0X4A, 0XA9, 0X63, 0X6C,
  0X5B, 0X2B, 0X39, 0XE5, 0X39, 0XE5, 0X39, 0XE5, 0X39, 0XE5, 0X39, 0XC5,
  0X42, 0X67, 0X4A, 0X88, 0X39, 0XC5, 0X31, 0XC5, 0X42, 0X68, 0X52, 0XEA,
  0X52, 0XC9, 0X52, 0XC9, 0X52, 0XEA, 0X42, 0X47, 0X39, 0XC5, 0X3A, 0X05,
  0X4A, 0X88, 0X39, 0XE5, 0X3A, 0X06, 0X52, 0XC9, 0X4A, 0X68, 0X39, 0XE5,
  0X3A, 0X06, 0X52, 0XC9, 0X52, 0XC9, 0X52, 0XC9, 0X42, 0X47, 0X39, 0XC5,
  0X3A, 0X06, 0X4A, 0XA9, 0X52, 0XC9, 0X52, 0XC9, 0X52, 0XC9, 0X52, 0XC9,
  0X4A, 0XA9, 0X52, 0XC9, 0X4A, 0X88, 0X39, 0XE5, 0X39, 0XE5, 0X3A, 0X06,
  0X4A, 0XC9, 0X4A, 0X47, 0X41, 0XE5, 0X4A, 0XA8, 0X52, 0XC9, 0X4A, 0XA9,
  0X4A, 0XA9, 0X4A, 0XA9, 0X52, 0XC9, 0X52, 0XC9, 0X52, 0XC9, 0X52, 0XA9,
  0X4A, 0X06, 0X6B, 0X2A, 0X73, 0XAD, 0X52, 0XC9, 0X52, 0XCA, 0X7B, 0XAB,
  0XA4, 0XCE, 0X94, 0XD0, 0X8C, 0X6E, 0X8C, 0X8E, 0X8C, 0X8E, 0X42, 0X47,
  0X20, 0XE2, 0X31, 0X85, 0X42, 0X68, 0X73, 0XED, 0X9D, 0X33, 0X94, 0XF2,
  0X94, 0XF2, 0X95, 0X12, 0X9C, 0XF0, 0X9C, 0X8D, 0X9C, 0X6D, 0X9C, 0XAF,
  0XB4, 0XEE, 0X8B, 0XCA, 0X73, 0X8A, 0X7B, 0XAA, 0X73, 0X6A, 0X4A, 0X27,
  0X4A, 0X06, 0X4A, 0X07, 0X4A, 0X27, 0X4A, 0X27, 0X4A, 0X07, 0X4A, 0X27,
  0X4A, 0X68, 0X39, 0XE6, 0X21, 0X03, 0X5A, 0XA8, 0X7B, 0X8B, 0X73, 0X4A,
  0X7B, 0X8B, 0X8C, 0X4D, 0X8C, 0X2D, 0X8C, 0X4D, 0X8C, 0X2D, 0X8C, 0X2D,
  0X8C, 0X2D, 0X8C, 0X4D, 0X8C, 0X4D, 0X8C, 0X4E, 0X7B, 0X6B, 0X8C, 0X0E,
  0XAD, 0X74, 0XA5, 0X94, 0XA5, 0X12, 0X8C, 0X2D, 0X8C, 0X2D, 0X8C, 0X4D,
  0X7B, 0X8B, 0X73, 0X2A, 0X73, 0X4A, 0X73, 0X6A, 0X7B, 0X6A, 0X7B, 0X6A,
  0X7B, 0X6A, 0X73, 0X6A, 0X73, 0X6A, 0X8B, 0X8B, 0X83, 0X4A, 0X8B, 0X09,
  0X9A, 0XAA, 0X9A, 0XAA, 0X92, 0XEA, 0X7B, 0X6A, 0X7B, 0X6A, 0X7B, 0X8A,
  0X7B, 0X8A, 0X7B, 0X8A, 0X7B, 0X8B, 0X7B, 0X8B, 0X7B, 0X4A, 0X83, 0X09,
  0X83, 0X2A, 0X7B, 0X0A, 0X83, 0X4A, 0X52, 0X47, 0X4A, 0X47, 0X73, 0XAD,
  0X6B, 0X8D, 0X6B, 0X6C, 0X9D, 0X12, 0X8C, 0XD1, 0X7C, 0X2F, 0X7C, 0X4F,
  0X84, 0X0E, 0X84, 0X70, 0X52, 0X67, 0X7C, 0X2F, 0X7C, 0X2F, 0X5A, 0XC9,
  0X63, 0X2A, 0X63, 0X0A, 0X63, 0X0A, 0X5A, 0XE9, 0X7B, 0XCC, 0X8C, 0X6F,
  0X84, 0X4F, 0X7C, 0X4F, 0X7C, 0X4F, 0X7C, 0X4F, 0X8C, 0XF3, 0X95, 0X55,
  0X8D, 0X34, 0X8D, 0X34, 0XAD, 0X53, 0XAD, 0X32, 0XAC, 0XF1, 0XAD, 0X32,
  0XB5, 0X53, 0XA5, 0X12, 0X84, 0X70, 0X84, 0X70, 0X8C, 0XB1, 0X94, 0XD2,
  0X94, 0XB1, 0X94, 0XD2, 0X94, 0XF1, 0X6B, 0X6B, 0X39, 0XA5, 0X52, 0X88,
  0X94, 0XD1, 0X84, 0X2E, 0X84, 0X4E, 0X84, 0X2E, 0X84, 0X4E, 0X7B, 0XED,
  0X5B, 0X0B, 0X5B, 0X2B, 0X7C, 0X0E, 0X84, 0X0D, 0X7B, 0X89, 0X72, 0XE7,
  0X73, 0X08, 0X73, 0X08, 0X73, 0X08, 0X73, 0X08, 0X73, 0X08, 0X73, 0X08,
  0X73, 0X08, 0X73, 0X08, 0X73, 0X08, 0X73, 0X08, 0X7B, 0X07, 0X5A, 0XA8,
  0X73, 0XAB, 0X94, 0XAE, 0X9D, 0X31, 0XA5, 0X31, 0X94, 0X6D, 0X73, 0X68,
  0X73, 0X88, 0X63, 0X08, 0X8C, 0X0A, 0X9C, 0X8C, 0X94, 0X4C, 0X73, 0X8A,
  0X73, 0X8A, 0X94, 0X4D, 0X94, 0X2D, 0X9C, 0X6E, 0X8B, 0XEB, 0X93, 0X88,
  0X94, 0X6D, 0X94, 0XAF, 0X94, 0X8F, 0X94, 0X8E, 0X94, 0X8F, 0X8C, 0X4D,
  0X7B, 0XAA, 0X73, 0XAA, 0X73, 0XEB, 0X73, 0XCB, 0X84, 0X0C, 0X7B, 0XAB,
  0X84, 0X0D, 0X94, 0XD0, 0X6B, 0X2A, 0X4A, 0X06, 0X83, 0XEE, 0XB5, 0XB5,
  0XAD, 0X74, 0XB5, 0XF6, 0XB5, 0XB5, 0XB5, 0XD5, 0XB5, 0XD5, 0XB5, 0XB5,
  0XB5, 0XD5, 0X9D, 0X13, 0X9D, 0X33, 0XBD, 0XF6, 0XAD, 0X95, 0XB5, 0X94,
  0X9C, 0XCF, 0X8C, 0X6D, 0X84, 0X0C, 0X73, 0X8B, 0X7B, 0X8B, 0X8C, 0X2C,
  0X94, 0X4D, 0X94, 0X4D, 0X94, 0X4D, 0X94, 0X4D, 0X94, 0X4D, 0X94, 0X2C,
  0X94, 0X8F, 0X9C, 0XF2, 0X9C, 0XD1, 0X9D, 0X12, 0X7B, 0XCB, 0X73, 0X48,
  0X73, 0X69, 0X73, 0X49, 0X83, 0XCA, 0X8C, 0X0A, 0X8C, 0X2A, 0X63, 0X29,
  0X3A, 0X07, 0X52, 0XCA, 0X73, 0XCD, 0X9D, 0X12, 0XC6, 0X37, 0XD6, 0XD9,
  0XDE, 0XFA, 0XDE, 0XFA, 0XDE, 0XFA, 0XDE, 0XFA, 0XDE, 0XFA, 0XDE, 0XFA,
  0XDE, 0XFA, 0XDE, 0XFA, 0XDE, 0XFA, 0XDE, 0XFA, 0XDE, 0XFA, 0XDE, 0XFA,
  0XEF, 0X5C, 0XEF, 0X5C, 0XF7, 0X7C, 0XB5, 0XB5, 0X94, 0X91, 0X8C, 0X71,
  0X94, 0XB2, 0XD6, 0X57, 0X9C, 0XD2, 0XAD, 0X54, 0XE7, 0X1A, 0XDE, 0XDA,
  0XD6, 0XB9, 0XDE, 0XB9, 0XE6, 0XFB, 0XD6, 0XB9, 0X9D, 0X32, 0X9D, 0X32,
  0X9D, 0X32, 0X9D, 0X32, 0XBD, 0XB5, 0XBD, 0XB5, 0XBD, 0XB5, 0XAD, 0X53,
  0X52, 0X89, 0X4A, 0X68, 0X4A, 0X88, 0X4A, 0X68, 0X4A, 0X88, 0X4A, 0X88,
  0X4A, 0X88, 0X4A, 0X88, 0X4A, 0X88, 0X4A, 0X68, 0X4A, 0X88, 0X4A, 0X88,
  0X4A, 0X88, 0X4A, 0X88, 0X4A, 0X68, 0X4A, 0X88, 0X4A, 0X88, 0X4A, 0X68,
  0X4A, 0X88, 0X4A, 0X47, 0X4A, 0X05, 0X4A, 0X05, 0X4A, 0X05, 0X4A, 0X26,
  0X8C, 0X6F, 0X9C, 0XB0, 0X52, 0XA8, 0X42, 0X69, 0X4A, 0X48, 0X42, 0X27,
  0X42, 0X06, 0X41, 0XE5, 0X39, 0X85, 0X39, 0XA5, 0X39, 0XA4, 0X41, 0XC5,
  0X41, 0XC5, 0X52, 0X26, 0X5A, 0X86, 0X61, 0XE3, 0X61, 0XE3, 0X61, 0XE3,
  0X52, 0X25, 0X49, 0XE5, 0X49, 0XE5, 0X49, 0XE5, 0X52, 0X05, 0X4A, 0X05,
  0X41, 0XE6, 0X52, 0X26, 0X52, 0X26, 0X41, 0XE5, 0X31, 0XA4, 0X39, 0XC5,
  0X39, 0XC5, 0X31, 0XA4, 0X31, 0XA4, 0X39, 0XE5, 0X39, 0XE6, 0X4A, 0X47,
  0X4A, 0X47, 0X41, 0XE6, 0X39, 0XC5, 0X49, 0XE6, 0X41, 0XE6, 0X6B, 0X4B,
  0X52, 0XA9, 0X29, 0X84, 0X39, 0XC5, 0X41, 0X83, 0X5A, 0X67, 0X52, 0X47,
  0X39, 0XE6, 0X41, 0XE6, 0X41, 0XC5, 0X62, 0XA8, 0X52, 0X46, 0X49, 0XE5,
  0X4A, 0X06, 0X4A, 0X06, 0X4A, 0X06, 0X52, 0X26, 0X49, 0XE5, 0X39, 0X83,
  0X39, 0XA4, 0X29, 0X84, 0X39, 0XC5, 0X42, 0X05, 0X42, 0X05, 0X39, 0XE5,
  0X39, 0XC5, 0X52, 0X26, 0X49, 0XE6, 0X31, 0XA5, 0X4A, 0X89, 0X52, 0XCA,
  0X39, 0XA4, 0X4A, 0X05, 0X62, 0X26, 0X51, 0XE6, 0X29, 0X23, 0X00, 0X62,
  0X08, 0X62, 0X18, 0XC3, 0X10, 0XA2, 0X08, 0X82, 0X08, 0XA3, 0X10, 0XC3,
  0X08, 0XA3, 0X21, 0X03, 0X31, 0X65, 0X18, 0XE4, 0X18, 0XC3, 0X21, 0X24,
  0X29, 0X44, 0X21, 0X04, 0X29, 0X24, 0X19, 0X22, 0X10, 0XC2, 0X21, 0X23,
  0X18, 0XC2, 0X08, 0X62, 0X10, 0XC4, 0X10, 0XA3, 0X08, 0X82, 0X21, 0X25,
  0X31, 0X45, 0X20, 0XE3, 0X41, 0XE6, 0X4A, 0X48, 0X18, 0XE3, 0X21, 0X23,
  0X30, 0XE4, 0X20, 0XC4, 0X31, 0X86, 0X18, 0XC4, 0X20, 0XE3, 0X31, 0X43,
  0X10, 0X82, 0X18, 0XC2, 0X18, 0XC2, 0X18, 0XC2, 0X21, 0X05, 0X18, 0XE4,
  0X19, 0X05, 0X10, 0XA3, 0X18, 0XC3, 0X10, 0X62, 0X18, 0XE3, 0X31, 0X25,
  0X18, 0XE4, 0X18, 0XA2, 0X18, 0XA3, 0X31, 0X25, 0X31, 0X24, 0X29, 0X45,
  0X31, 0X44, 0X39, 0X87, 0X39, 0X87, 0X29, 0X24, 0X29, 0X23, 0X21, 0X43,
  0X10, 0XA2, 0X10, 0X82, 0X20, 0XE3, 0X10, 0XA2, 0X08, 0X61, 0X10, 0X82,
  0X10, 0X82, 0X08, 0X62, 0X31, 0X64, 0X52, 0X26, 0X39, 0XC5, 0X31, 0XC5,
  0X39, 0XA4, 0X41, 0XA4, 0X31, 0X84, 0X29, 0X84, 0X4A, 0X67, 0X5A, 0X67,
  0X63, 0X6D, 0X74, 0X72, 0X7C, 0X50, 0X84, 0X4E, 0X95, 0X32, 0XAD, 0XB5,
  0X8C, 0XF2, 0X9D, 0X53, 0X84, 0XB1, 0X9D, 0X53, 0X6B, 0XEF, 0X42, 0X28,
  0XBD, 0XD5, 0XB5, 0X94, 0XBD, 0XD5, 0XBD, 0XD5, 0XBD, 0XD5, 0XBD, 0XD5,
  0XBD, 0X94, 0XB5, 0X93, 0XA4, 0XF1, 0X6A, 0XE8, 0X7B, 0X8A, 0X83, 0XEC,
  0X7B, 0XCC, 0X7B, 0XAC, 0X7B, 0XAB, 0X7B, 0XAB, 0X7B, 0XCC, 0X6B, 0X29,
  0X5A, 0XA8, 0X8B, 0XCA, 0X8B, 0XCA, 0X7B, 0XAB, 0X7B, 0XCB, 0X7B, 0XAB,
  0X7B, 0XAB, 0X7B, 0XAB, 0X7B, 0XAB, 0X7B, 0XCB, 0X7B, 0XAB, 0X9C, 0XB0,
  0X9C, 0XD1, 0X6B, 0X09, 0X6A, 0XC7, 0X94, 0X2E, 0X9C, 0XB0, 0X9C, 0X90,
  0X9C, 0X90, 0X9C, 0X90, 0X9C, 0X6F, 0X9C, 0X6F, 0X94, 0X90, 0X83, 0XEC,
  0X7B, 0XCC, 0X7B, 0XCC, 0X7B, 0XCC, 0X7B, 0XCC, 0X83, 0XEC, 0X84, 0X0C,
  0X9C, 0XD1, 0X9C, 0XD2, 0X9C, 0XD2, 0XA5, 0X12, 0X6B, 0X2A, 0X29, 0X02,
  0X63, 0X2B, 0X5A, 0XCA, 0X52, 0X89, 0X4A, 0X67, 0X4A, 0X26, 0X52, 0X46,
  0X39, 0XC3, 0X39, 0XE4, 0X63, 0X6C, 0X9D, 0XB8, 0X73, 0XEF, 0X5A, 0XC8,
  0X5A, 0XE9, 0X7B, 0XEE, 0XAD, 0XB6, 0X84, 0X2F, 0X39, 0XA5, 0X73, 0X6B,
  0X84, 0XD3, 0X84, 0X70, 0X31, 0X85, 0X29, 0X44, 0X39, 0XE7, 0X95, 0X55,
  0X8D, 0X14, 0X6B, 0X8C, 0X6B, 0X8C, 0X6B, 0XAC, 0X52, 0X88, 0X5A, 0XE9,
  0X5B, 0X2A, 0X31, 0XA4, 0X31, 0XC4, 0X31, 0XC4, 0X31, 0XA4, 0X7B, 0XCD,
  0X8C, 0X6F, 0X84, 0X0E, 0X5A, 0XC9, 0X31, 0X63, 0X29, 0X22, 0X29, 0X22,
  0X21, 0X02, 0X31, 0X83, 0X4A, 0X24, 0X42, 0X04, 0X41, 0XE3, 0X52, 0X65,
  0X7B, 0XED, 0X73, 0X6A, 0X73, 0X49, 0X73, 0X4A, 0X73, 0X4A, 0X7B, 0X6A,
  0X7B, 0X6A, 0X7B, 0X6A, 0X7B, 0X6A, 0X7B, 0X6A, 0X7B, 0X6A, 0X7B, 0X8A,
  0X7B, 0X8B, 0X7B, 0XAC, 0X7B, 0XAC, 0X6B, 0X09, 0X6B, 0X29, 0X6B, 0X29,
  0X62, 0XE8, 0X63, 0X0A, 0X6B, 0X29, 0X5A, 0XE8, 0X63, 0X08, 0X63, 0X08,
  0X63, 0X08, 0X6B, 0X08, 0X63, 0X28, 0X63, 0X28, 0X63, 0X08, 0X63, 0X28,
  0X62, 0XE8, 0X62, 0XA7, 0X62, 0X86, 0X95, 0X55, 0XAE, 0XBD, 0XAE, 0X5B,
  0XAE, 0X5A, 0XA6, 0X18, 0X95, 0X34, 0X9D, 0X96, 0X95, 0X54, 0X95, 0X13,
  0X95, 0X34, 0X8C, 0XD2, 0X5A, 0XEA, 0X8C, 0XF3, 0X94, 0XF3, 0X9D, 0X54,
  0X73, 0X8D, 0X73, 0X8D, 0X9D, 0X75, 0X9D, 0X55, 0X95, 0X34, 0XA5, 0X95,
  0XC6, 0X79, 0XCE, 0X9A, 0X9D, 0X33, 0X84, 0X91, 0X8C, 0XB1, 0X73, 0XCB,
  0X63, 0X49, 0X6B, 0X29, 0X73, 0X48, 0X95, 0X54, 0X9D, 0XB7, 0X95, 0X55,
  0X8D, 0X35, 0X95, 0XD8, 0X9E, 0X19, 0XA6, 0X5A, 0X8C, 0XF3, 0X6B, 0XAD,
  0X8C, 0XB1, 0X9C, 0XF2, 0X94, 0XF2, 0X9D, 0X14, 0X9D, 0X34, 0X9D, 0X14,
  0X9D, 0X55, 0X94, 0X71, 0X84, 0X4F, 0X7B, 0X8B, 0X94, 0XB0, 0X94, 0XF2,
  0X8C, 0X2E, 0X7C, 0X70, 0X8C, 0XD2, 0X84, 0X0E, 0X8B, 0XAB, 0X83, 0X6A,
  0X7B, 0X6B, 0X8B, 0X29, 0X94, 0XF2, 0X7C, 0XB2, 0X83, 0XCE, 0X95, 0X34,
  0X6B, 0XAB, 0X52, 0X66, 0X52, 0X66, 0X4A, 0X66, 0X41, 0XC4, 0X39, 0X43,
  0X4A, 0X25, 0X5A, 0XE7, 0X5A, 0XA6, 0X84, 0X6F, 0X8C, 0X0D, 0X7B, 0X4A,
  0X63, 0X0A, 0X6B, 0X4A, 0X63, 0X49, 0X73, 0X87, 0X73, 0X67, 0X73, 0X67,
  0X52, 0X86, 0X73, 0X68, 0X7B, 0XC8, 0X7B, 0XA8, 0X52, 0X66, 0X31, 0X84,
  0X21, 0X64, 0X21, 0X44, 0X5A, 0XC8, 0X9C, 0X2B, 0X7B, 0XCB, 0X7B, 0XEC,
  0X7B, 0XEC, 0X7B, 0XEC, 0X95, 0X32, 0X9D, 0X32, 0X9C, 0XD0, 0X9C, 0XF0,
  0X9D, 0X32, 0XAE, 0X7A, 0XA5, 0XD6, 0X9D, 0X73, 0XA5, 0XB5, 0X9D, 0XB5,
  0X9D, 0X53, 0X9D, 0X73, 0XAE, 0X39, 0X9D, 0XB6, 0X8C, 0XF2, 0X8D, 0X12,
  0X8D, 0X12, 0X8C, 0XF2, 0X8D, 0X33, 0XA6, 0X59, 0X95, 0X95, 0X8D, 0X12,
  0X8D, 0X13, 0X8D, 0X13, 0XAE, 0X38, 0XB6, 0X79, 0XB6, 0X79, 0XB6, 0X58,
  0XB6, 0X58, 0XAE, 0X38, 0XA6, 0X39, 0XA6, 0X39, 0XA6, 0X39, 0X95, 0X74,
  0X8D, 0X12, 0X8D, 0X12, 0X8D, 0X12, 0X8D, 0X12, 0X8D, 0X12, 0X8D, 0X12,
  0X8C, 0XF2, 0X8C, 0XF2, 0X8D, 0X12, 0X84, 0XD1, 0X63, 0X4B, 0X63, 0X2A,
  0X8C, 0X90, 0X9D, 0X74, 0X7C, 0X0E, 0X4A, 0X25, 0X52, 0X26, 0X52, 0X46,
  0X4A, 0X06, 0X41, 0XE6, 0X42, 0X06, 0X42, 0X06, 0X42, 0X06, 0X39, 0XE6,
  0X42, 0X27, 0X4A, 0X69, 0X4A, 0X68, 0X4A, 0X68, 0X39, 0XA4, 0X41, 0XC4,
  0X4A, 0X05, 0X49, 0XE4, 0X39, 0XA4, 0X31, 0X83, 0X39, 0X83, 0X39, 0X83,
  0X31, 0X83, 0X39, 0X83, 0X31, 0X83, 0X39, 0X83, 0X41, 0XE5, 0X41, 0XE5,
  0X52, 0X26, 0X52, 0X26, 0X52, 0X46, 0X52, 0X05, 0X51, 0XE5, 0X4A, 0X05,
  0X52, 0X04, 0X4A, 0X05, 0X41, 0XE5, 0X39, 0XE5, 0X42, 0X26, 0X4A, 0X25,
  0X49, 0XA4, 0X41, 0XA5, 0X49, 0XC4, 0X52, 0X46, 0X5A, 0X45, 0X52, 0X25,
  0X4A, 0X26, 0X6B, 0X2A, 0XAD, 0X74, 0X94, 0X6F, 0X51, 0XE4, 0X52, 0X05,
  0X52, 0X45, 0X52, 0XC7, 0X52, 0XC7, 0X52, 0XC7, 0X5A, 0XC7, 0X62, 0X25,
  0X4A, 0X05, 0X59, 0XE3, 0X92, 0XC2, 0X5A, 0X05, 0X49, 0XE4, 0X8A, 0XC4,
  0XB3, 0X63, 0X9B, 0X23, 0XBB, 0X83, 0XE3, 0XA1, 0XD3, 0X42, 0XB3, 0X03,
  0XBB, 0X23, 0XBB, 0X43, 0XC3, 0X63, 0XC3, 0X62, 0XD4, 0X02, 0XD3, 0XE2,
  0XC3, 0X83, 0XC3, 0X64, 0XC3, 0X43, 0XB2, 0X82, 0X99, 0XE1, 0XD3, 0XC1,
  0XC3, 0X01, 0X89, 0X61, 0X69, 0X00, 0X81, 0X20, 0X81, 0X60, 0X71, 0XC1,
  0X89, 0XC1, 0X69, 0X20, 0X61, 0X20, 0X58, 0XC1, 0X60, 0XE1, 0X61, 0X00,
  0X50, 0XA0, 0X92, 0X02, 0XC3, 0X03, 0XBA, 0XE3, 0XD3, 0X82, 0XCB, 0X43,
  0XC3, 0X24, 0XBB, 0X86, 0XBB, 0X65, 0XBB, 0X65, 0XBB, 0X65, 0XBB, 0X65,
  0XA2, 0XC4, 0X8A, 0X22, 0XA2, 0X83, 0XC2, 0XE3, 0XE4, 0X87, 0XDC, 0XE8,
  0XC4, 0X46, 0XCC, 0X66, 0X7B, 0X07, 0X41, 0XE7, 0X52, 0XA9, 0X74, 0X4F,
  0X74, 0X2F, 0X73, 0XEE, 0X7B, 0XAC, 0XBD, 0XB6, 0XA5, 0X12, 0X41, 0XE5,
  0X10, 0X81, 0X18, 0XA2, 0X10, 0X61, 0X29, 0X43, 0X7C, 0X2C, 0X7C, 0X0C,
  0X7C, 0X0C, 0X7C, 0X0C, 0X7C, 0X0C, 0X7C, 0X0C, 0X7B, 0XA9, 0X7B, 0XEB,
  0X7B, 0XEB, 0X73, 0XCB, 0X7B, 0XEB, 0X93, 0X89, 0X8C, 0X0D, 0X8C, 0X90,
  0X8C, 0X8F, 0X8C, 0X6F, 0X84, 0X4F, 0X84, 0X6F, 0X8C, 0X6F, 0X8C, 0X90,
  0X94, 0X2C, 0X9C, 0X2C, 0X8C, 0X90, 0X84, 0X4F, 0X6B, 0X8C, 0X73, 0XCD,
  0X4A, 0X47, 0X18, 0XC2, 0X52, 0X88, 0X31, 0X85, 0X84, 0X70, 0X8C, 0X71,
  0X73, 0XCE, 0X20, 0XE2, 0X18, 0XA1, 0X29, 0X64, 0X7C, 0X50, 0X7C, 0X30,
  0X7C, 0X50, 0X7C, 0X4F, 0X7C, 0X4E, 0X74, 0X0D, 0X74, 0X0D, 0X7C, 0X2E,
  0X7C, 0X4E, 0X7C, 0X4E, 0X39, 0XC7, 0X5A, 0X68, 0X31, 0X44, 0X38, 0XC5,
  0X38, 0XA5, 0X49, 0X66, 0X5A, 0X27, 0X51, 0XA6, 0X61, 0XE6, 0X72, 0XEA,
  0X59, 0XE7, 0X83, 0X2B, 0X7A, 0XCA, 0X61, 0XE6, 0X5A, 0X06, 0X5A, 0X47,
  0X6A, 0XCA, 0X61, 0XE7, 0X72, 0XCA, 0X6A, 0XCA, 0X72, 0XA9, 0X72, 0X88,
  0X72, 0X27, 0X72, 0X47, 0X6A, 0XEA, 0X72, 0X68, 0X73, 0X0B, 0X5A, 0XA8,
  0X52, 0X26, 0X6A, 0X47, 0X4A, 0X26, 0X6A, 0X27, 0X52, 0X06, 0X59, 0XA5,
  0X6A, 0X06, 0X62, 0X26, 0X62, 0X26, 0X62, 0X05, 0X62, 0X06, 0X59, 0XC5,
  0X62, 0X06, 0X7A, 0X67, 0X93, 0X0A, 0X82, 0X87, 0X72, 0X26, 0X72, 0X06,
  0X69, 0XC5, 0X69, 0XC5, 0X71, 0XE6, 0X6A, 0X06, 0X82, 0X06, 0X71, 0XE6,
  0X8A, 0XA8, 0X7A, 0X67, 0X7A, 0X06, 0X82, 0X47, 0X8A, 0XE9, 0X6A, 0X27,
  0X6A, 0X47, 0X71, 0XE5, 0X7A, 0X67, 0X82, 0XE9, 0X8A, 0XE9, 0X7A, 0X26,
  0X82, 0XA8, 0X8A, 0X47, 0X72, 0X06, 0X62, 0X06, 0X83, 0X09, 0X82, 0X87,
  0X72, 0X47, 0X82, 0X26, 0X71, 0X85, 0X69, 0XE6, 0X71, 0XE6, 0X69, 0XA5,
  0X61, 0XA5, 0X51, 0X85, 0X52, 0X06, 0X52, 0X26, 0X4A, 0X26, 0X62, 0X07,
  0X7A, 0X07, 0X69, 0XA6, 0X51, 0X03, 0X59, 0X23, 0X50, 0XE2, 0X48, 0XA1,
  0X48, 0XE3, 0X40, 0XE3, 0X30, 0XE4, 0X29, 0X04, 0X38, 0XE3, 0X50, 0XE3,
  0X51, 0X03, 0X48, 0XE2, 0X40, 0XC2, 0X49, 0X23, 0X51, 0X64, 0X51, 0X64,
  0X51, 0X44, 0X51, 0X44, 0X49, 0X43, 0X49, 0X44, 0X30, 0XC2, 0X30, 0XC2,
  0X30, 0XC2, 0X38, 0XC3, 0X30, 0XC2, 0X28, 0XC2, 0X38, 0XE3, 0X50, 0XE3,
  0X38, 0XC3, 0X30, 0XA2, 0X28, 0XA2, 0X28, 0XA2, 0X38, 0XA2, 0X40, 0XC2,
  0X38, 0XC3, 0X38, 0XC3, 0X38, 0XA2, 0X38, 0XC2, 0X30, 0XA2, 0X30, 0XA2,
  0X40, 0XE2, 0X41, 0X03, 0X41, 0X43, 0X10, 0X41, 0X41, 0XA5, 0X83, 0X49,
  0X6A, 0XE9, 0X7B, 0X8B, 0X8A, 0X87, 0XBB, 0X68, 0X7A, 0XE9, 0X6B, 0X2A,
  0X6A, 0XE9, 0X83, 0X6A, 0X5A, 0X47, 0X62, 0X87, 0X7B, 0X29, 0X8B, 0X89,
  0XBD, 0X96, 0X8C, 0X51, 0X49, 0XE8, 0X41, 0XC7, 0X21, 0X24, 0X21, 0X44,
  0X29, 0X44, 0X31, 0X44, 0X41, 0X24, 0X20, 0XE2, 0X18, 0XE3, 0X29, 0X24,
  0X48, 0XC3, 0X50, 0XE2, 0X40, 0XC2, 0X40, 0XA2, 0X48, 0XA2, 0X40, 0X81,
  0X48, 0XC2, 0X40, 0XC2, 0X48, 0XE3, 0X30, 0X81, 0X28, 0X61, 0X38, 0XE3,
  0X30, 0XC3, 0X51, 0X23, 0X41, 0X63, 0X32, 0X06, 0X31, 0X84, 0X21, 0X45,
  0X2A, 0X88, 0X2A, 0X07, 0X3B, 0X29, 0X33, 0XAD, 0X3B, 0X6D, 0X2A, 0X68,
  0X22, 0X27, 0X2A, 0X88, 0X3B, 0X0B, 0X2A, 0XC9, 0X32, 0X27, 0X32, 0XAA,
  0X3A, 0XAA, 0X32, 0X69, 0X22, 0X08, 0X32, 0XA9, 0X4C, 0X6C, 0X52, 0XA8,
  0X39, 0X24, 0X21, 0XC6, 0X31, 0X44, 0X28, 0XC2, 0X19, 0X44, 0X22, 0X27,
  0X22, 0X47, 0X2A, 0X47, 0X2A, 0XC9, 0X2A, 0X89, 0X19, 0X65, 0X19, 0XE7,
  0X21, 0X85, 0X21, 0X44, 0X21, 0X85, 0X21, 0X44, 0X21, 0X64, 0X21, 0X64,
  0X31, 0XC6, 0X31, 0XE6, 0X2A, 0X07, 0X21, 0X65, 0X21, 0X65, 0X21, 0X85,
  0X19, 0X24, 0X11, 0X24, 0X2A, 0X47, 0X39, 0X86, 0X5A, 0X28, 0X39, 0X46,
  0X51, 0X87, 0X62, 0X29, 0X39, 0X65, 0X29, 0X03, 0X52, 0X47, 0X4A, 0X26,
  0X31, 0X65, 0X49, 0XE7, 0XAB, 0X6C, 0X6A, 0X27, 0X39, 0XC6, 0X4A, 0X47,
  0X4A, 0X69, 0X52, 0X08, 0X49, 0XC6, 0X52, 0X06, 0X7A, 0X89, 0XA3, 0X4C,
  0X41, 0X86, 0X4A, 0X06, 0X31, 0X44, 0X10, 0XA2, 0X10, 0X82, 0X20, 0XA3,
  0X41, 0XA5, 0X52, 0X47, 0X49, 0XC6, 0X31, 0X24, 0X20, 0XC3, 0X28, 0XE3,
  0X20, 0XA3, 0X52, 0X07, 0X89, 0XA5, 0X59, 0X63, 0X59, 0XC5, 0X61, 0X23,
  0X69, 0X23, 0X58, 0XE3, 0X50, 0XE2, 0X69, 0X63, 0X69, 0X63, 0X61, 0X43,
  0X31, 0X23, 0X39, 0X44, 0X51, 0XC6, 0X51, 0X84, 0X51, 0X64, 0X61, 0XC6,
  0X51, 0XC6, 0X59, 0XA5, 0X62, 0X26, 0X59, 0XC6, 0X4A, 0X06, 0X11, 0X65,
  0X11, 0X24, 0X11, 0X24, 0X21, 0XE7, 0X19, 0X86, 0X22, 0X07, 0X22, 0X07,
  0X21, 0XC6, 0X21, 0XC6, 0X29, 0XC6, 0X31, 0X45, 0X49, 0X85, 0X62, 0X27,
  0X31, 0X65, 0X19, 0X85, 0X19, 0X03, 0X19, 0X03, 0X21, 0X65, 0X29, 0XE6,
  0X21, 0X44, 0X21, 0XE6, 0X19, 0X65, 0X11, 0X03, 0X11, 0X24, 0X19, 0X85,
  0X21, 0XC6, 0X19, 0XC6, 0X11, 0X03, 0X10, 0XE3, 0X21, 0XC7, 0X19, 0X85,
  0X19, 0XA6, 0X19, 0XA6, 0X21, 0XC6, 0X22, 0X28, 0X19, 0X65, 0X19, 0X65,
  0X2A, 0X89, 0X4B, 0X8C, 0X6A, 0XE9, 0X7A, 0X68, 0X39, 0X25, 0X3A, 0X68,
  0X3A, 0X68, 0X21, 0X85, 0X19, 0XC6, 0X19, 0X85, 0X1A, 0X07, 0X22, 0X48,
  0X2A, 0X27, 0X3A, 0X69, 0X29, 0XC6, 0X11, 0X23, 0X21, 0XC6, 0X22, 0X28,
  0X19, 0X44, 0X08, 0XA2, 0X19, 0X24, 0X31, 0XA5, 0X31, 0XC5, 0X19, 0XC6,
  0X21, 0X85, 0X21, 0XC6, 0X31, 0X03, 0X52, 0X06, 0X73, 0X09, 0X72, 0XC8,
  0X41, 0X64, 0X39, 0XA6, 0XA5, 0X32, 0XAC, 0XF2, 0X94, 0X0E, 0XBD, 0X12,
  0XBC, 0XD1, 0XBC, 0X91, 0XBB, 0XB1, 0XAB, 0XCF, 0X7A, 0XA9, 0XA3, 0X8D,
  0XCB, 0XF4, 0XCA, 0XF7, 0XCB, 0X32, 0XC3, 0X0C, 0XCB, 0XCF, 0XCC, 0X91,
  0XC4, 0X30, 0XB4, 0X2F, 0X9C, 0X0E, 0XAA, 0XCB, 0XCB, 0XCF, 0XC3, 0XEF,
  0XC4, 0X0F, 0XCC, 0XD2, 0XD6, 0X97, 0XDC, 0XB3, 0XD3, 0X8F, 0XCC, 0X0F,
  0XB5, 0XF4, 0XD3, 0XEF, 0XC3, 0XEE, 0XC3, 0XAE, 0XC4, 0X0F, 0XC4, 0XF0,
  0XBC, 0X8F, 0XAB, 0X4C, 0XAA, 0XCB, 0XBC, 0X2F, 0XC5, 0X12, 0XAB, 0X0C,
  0XBC, 0X70, 0XBB, 0XAE, 0XD4, 0XF3, 0XD4, 0XD3, 0XCC, 0XD2, 0XC3, 0XF0,
  0XD3, 0X8F, 0XAB, 0X0C, 0X91, 0XE8, 0XAB, 0XAD, 0XB4, 0X2F, 0XA3, 0X2C,
  0XAB, 0X8D, 0X9A, 0X29, 0X92, 0X49, 0XB3, 0X2D, 0XDB, 0X0D, 0XCC, 0XD1,
  0XC5, 0X54, 0XC4, 0X91, 0XC2, 0XEC, 0XB5, 0X32, 0XAE, 0X15, 0XCC, 0X50,
  0XCD, 0X13, 0XBE, 0XB7, 0XAF, 0X79, 0XC6, 0X16, 0XC5, 0XD4, 0XD5, 0X95,
  0XD3, 0XCF, 0XD3, 0XAF, 0XDB, 0X0E, 0XCB, 0X0E, 0XA3, 0X2D, 0XCB, 0X2E,
  0XD3, 0X4F, 0XD3, 0X0E, 0XDB, 0X2F, 0XD3, 0X6F, 0XDB, 0XCF, 0XCB, 0X8E,
  0X7B, 0X09, 0X72, 0XE8, 0X62, 0XA8, 0X5A, 0X67, 0X5A, 0X67, 0X52, 0X06,
  0X52, 0X05, 0X5A, 0X66, 0X5A, 0X66, 0X41, 0XC5, 0X49, 0XC5, 0X41, 0XA5,
  0X62, 0XA8, 0X62, 0XA8, 0X52, 0X06, 0X7B, 0X29, 0X7B, 0X6A, 0X6A, 0XE8,
  0X52, 0X05, 0X6A, 0XC8, 0X6A, 0XC8, 0X5A, 0X67, 0X39, 0X43, 0X61, 0XE3,
  0X93, 0X07, 0X93, 0X26, 0X93, 0X67, 0X9B, 0XCA, 0X31, 0XA5, 0X31, 0XE6,
  0X31, 0XA5, 0X31, 0XA5, 0X31, 0X85, 0X31, 0X85, 0X31, 0XC5, 0X31, 0XE5,
  0X31, 0XE6, 0X29, 0XC6, 0X21, 0XC5, 0X62, 0X46, 0XCC, 0X8F, 0XBC, 0X70,
  0XB4, 0XB0, 0XBC, 0X4E, 0XC4, 0X2D, 0XB3, 0XEE, 0XAB, 0X6B, 0XCC, 0XAB,
  0XCC, 0X6C, 0XBB, 0XCE, 0XCC, 0X91, 0XB4, 0X50, 0XAB, 0XEE, 0XA3, 0XCE,
  0XAB, 0XEC, 0XAB, 0XCC, 0X9B, 0XCE, 0XB4, 0X0E, 0XBC, 0X4C, 0XA3, 0XCE,
  0XB4, 0X2F, 0X62, 0X08, 0X73, 0X4D, 0XAC, 0X70, 0XBB, 0XAB, 0XAB, 0X8C,
  0XA3, 0XEF, 0XB4, 0X0D, 0XA4, 0X2A, 0XA3, 0XEE, 0XC4, 0X4D, 0XB5, 0X31,
  0XAC, 0X2E, 0XC4, 0X2E, 0X8B, 0X0A, 0X84, 0X2E, 0XC4, 0XD0, 0XC4, 0X0D,
  0X81, 0XE8, 0X71, 0X87, 0XB3, 0X8B, 0XBC, 0X6F, 0XB4, 0X2E, 0XBC, 0X4D,
  0XBC, 0X6E, 0XC4, 0X0D, 0XBC, 0X4D, 0XA3, 0XAC, 0X52, 0XAA, 0XAD, 0X12,
  0XA4, 0X0E, 0XBB, 0XCA, 0XAC, 0X0D, 0X9C, 0X70, 0X9B, 0XAC, 0XAC, 0X50,
  0XBC, 0X0E, 0X92, 0X46, 0XB3, 0X69, 0X93, 0X8C, 0XB4, 0X4E, 0XA4, 0X2C,
  0XAC, 0X0C, 0X71, 0XA5, 0X93, 0X4B, 0XC4, 0X2E, 0X72, 0X69, 0X93, 0X4B,
  0XA5, 0X10, 0X9C, 0X91, 0XAC, 0X6F, 0XB3, 0XCD, 0XAB, 0XED, 0XAC, 0X8F,
  0XAB, 0X6C, 0XAB, 0X6B, 0XA3, 0X6B, 0XA3, 0X8C, 0XC4, 0XCD, 0X92, 0XE8,
  0X9B, 0X2A, 0XB3, 0XEC, 0XA3, 0X8C, 0XA3, 0X4C, 0XA3, 0X8C, 0X9C, 0X0C,
  0XA5, 0X51, 0X73, 0X6B, 0X62, 0X88, 0X71, 0XE8, 0X7A, 0X08, 0X92, 0X08,
  0XB2, 0X8A, 0XB2, 0X6A, 0X82, 0X08, 0X82, 0X68, 0XAA, 0XAA, 0X92, 0X69,
  0XAA, 0XCA, 0X8A, 0X29, 0X8A, 0X48, 0X82, 0X28, 0X82, 0X28, 0X82, 0X48,
  0X92, 0X48, 0X82, 0X48, 0XA2, 0XAA, 0XA2, 0XA9, 0X92, 0X48, 0XA2, 0X48,
  0X92, 0X69, 0X8A, 0X89, 0XAA, 0XEA, 0XB3, 0X0A, 0X9A, 0X89, 0X92, 0X89,
  0XAA, 0XEA, 0X92, 0XA9, 0XAA, 0XEB, 0X9A, 0XCA, 0X9A, 0XA9, 0X9A, 0XA9,
  0XA2, 0XCA, 0X9A, 0XCA, 0X92, 0X69, 0XAA, 0XCA, 0XAA, 0XC9, 0XA2, 0XA9,
  0X9A, 0XA9, 0XA2, 0XCA, 0XAA, 0XEA, 0X9A, 0X69, 0X9A, 0X08, 0X81, 0XE8,
  0X82, 0X28, 0X82, 0X48, 0X82, 0X28, 0X92, 0X29, 0XAB, 0XAB, 0X92, 0XC8,
  0X7A, 0X07, 0X72, 0X8A, 0X9A, 0X69, 0XAA, 0X89, 0X9A, 0X48, 0X9A, 0XAA,
  0X82, 0X47, 0X7A, 0X06, 0X82, 0X26, 0X82, 0X06, 0X82, 0X48, 0X8A, 0X07,
  0XAB, 0X6C, 0XA3, 0X6C, 0X93, 0X0B, 0X9B, 0X8C, 0X9A, 0XC9, 0X92, 0X88,
  0X93, 0X4B, 0X92, 0XC9, 0X92, 0X68, 0X92, 0X68, 0XA2, 0XC9, 0XAB, 0X0A,
  0XAB, 0X0A, 0X92, 0X88, 0X8A, 0X68, 0X9A, 0XA9, 0X9A, 0XA9, 0X92, 0X88,
  0X92, 0XEA, 0X82, 0X69, 0X82, 0X68, 0X82, 0X68, 0X82, 0X68, 0X8A, 0XA9,
  0X8A, 0X89, 0X9B, 0XAD, 0X92, 0XE9, 0X72, 0X27, 0X7A, 0X48, 0X92, 0XA9,
  0X92, 0XEA, 0X9B, 0XAD, 0X92, 0XE9, 0X9A, 0XA9, 0X9A, 0XEA, 0XAC, 0X4F,
  0XAC, 0X90, 0XA4, 0X0E, 0XAA, 0X67, 0XA3, 0X8C, 0XA3, 0X4A, 0XB2, 0XC7,
  0X9B, 0X8C, 0XA2, 0XE9, 0XB2, 0XE8, 0X93, 0XAD, 0XAB, 0X8B, 0XAA, 0XE9,
  0X9A, 0XC9, 0XBB, 0X69, 0XB3, 0X49, 0XB4, 0X4F, 0XB3, 0XAB, 0XB3, 0X49,
  0XB3, 0XCC, 0XB3, 0XAB, 0X92, 0X8A, 0X92, 0X6A, 0XA2, 0XA8, 0X9A, 0X89,
  0X92, 0XEC, 0XA2, 0X8A, 0X9A, 0XA9, 0X93, 0X4C, 0X92, 0XA9, 0X92, 0X68,
  0X92, 0X8A, 0X9A, 0XAA, 0X81, 0XE6, 0X79, 0XC6, 0X79, 0XC6, 0X79, 0XC6,
  0X71, 0X86, 0X81, 0XC5, 0X79, 0XA6, 0X79, 0XA6, 0X79, 0XA5, 0X79, 0XC6,
  0XA2, 0XC8, 0X82, 0X07, 0X9A, 0X88, 0X79, 0X86, 0X9A, 0XA9, 0XAA, 0XCB,
  0XAA, 0X2A, 0XCB, 0X6F, 0XB3, 0X4E, 0X7C, 0XB0, 0X6C, 0X0E, 0X39, 0XA5,
  0X74, 0XD0, 0XBC, 0X71, 0XCA, 0XCD, 0XA2, 0X6D, 0XB2, 0X8F, 0X92, 0X2B,
  0X92, 0X2B, 0XB2, 0X6F, 0XB2, 0X90, 0XC2, 0XD0, 0X41, 0X25, 0X20, 0XE3,
  0X52, 0X4C, 0X52, 0X4C, 0X30, 0XE3, 0X29, 0X06, 0X31, 0X48, 0X20, 0X83,
  0X31, 0XA6, 0X4A, 0XE9, 0X3A, 0X27, 0X41, 0XC7, 0X39, 0X87, 0X30, 0XE4,
  0X39, 0X03, 0X51, 0X65, 0X79, 0XA6, 0X9A, 0X49, 0XCB, 0X4C, 0XB2, 0XEB,
  0X59, 0X03, 0X51, 0X23, 0X49, 0X24, 0X41, 0X24, 0X49, 0X03, 0X50, 0XE3,
  0X51, 0X03, 0X61, 0X23, 0X59, 0X03, 0X61, 0XA6, 0XDC, 0XF0, 0X71, 0XA5,
  0X71, 0XA5, 0XAB, 0XAC, 0X59, 0X03, 0X61, 0X44, 0X61, 0X23, 0X9B, 0X0A,
  0X51, 0X44, 0X61, 0X44, 0X69, 0X84, 0X61, 0X23, 0X92, 0XE9, 0X93, 0X0A,
  0X69, 0X84, 0XAB, 0XEE, 0X50, 0XE3, 0X7A, 0X26, 0X8A, 0XC9, 0X8A, 0X68,
  0XCB, 0XEF, 0XCB, 0XEF, 0XC3, 0XCF, 0XCB, 0XF0, 0XDC, 0XB1, 0XDC, 0X90,
  0XBB, 0X4D, 0XBB, 0X4D, 0XB2, 0XEC, 0XBB, 0X0C, 0X39, 0X04, 0X08, 0X40,
  0X18, 0X82, 0X7B, 0XCB, 0X94, 0XAD, 0X8C, 0X6C, 0X9C, 0X6C, 0X63, 0X89,
  0X33, 0X87, 0X43, 0XC8, 0X3B, 0XA7, 0X3B, 0XA7, 0X10, 0XC2, 0X18, 0X41,
  0X28, 0XA3, 0X30, 0XC3, 0X30, 0XA3, 0X28, 0X83, 0X20, 0XC2, 0X5A, 0X27,
  0X51, 0XC6, 0X5A, 0X07, 0X6A, 0X47, 0X9B, 0XCC, 0X61, 0XC6, 0X9B, 0X0A,
  0XA3, 0X6B, 0X8B, 0X6B, 0X82, 0XC8, 0X6A, 0X25, 0X72, 0X67, 0X72, 0X46,
  0X72, 0X26, 0X62, 0X07, 0X62, 0X46, 0X8A, 0X07, 0X8A, 0X07, 0X82, 0X27,
  0X72, 0X67, 0X92, 0XCA, 0X62, 0X48, 0X51, 0XC6, 0X9B, 0X0B, 0X72, 0X28,
  0X8A, 0X69, 0X6A, 0X89, 0X41, 0XA6, 0X41, 0XE7, 0X59, 0XE7, 0X82, 0X28,
  0X59, 0XA6, 0X59, 0XA6, 0X69, 0XA7, 0X71, 0XC7, 0X7A, 0X08, 0X82, 0X09,
  0X69, 0XC7, 0X81, 0XC7, 0X69, 0X86, 0X41, 0X24, 0X51, 0X85, 0X8B, 0X2A,
  0X8B, 0XAB, 0X9B, 0XEC, 0X9B, 0X6A, 0X8A, 0X88, 0X60, 0XE4, 0X61, 0X44,
  0X59, 0X85, 0X61, 0X65, 0X62, 0X07, 0X72, 0XCB, 0X7A, 0XAA, 0X72, 0X49,
  0X69, 0X86, 0X69, 0X65, 0X61, 0XE7, 0X72, 0XCB, 0X7A, 0XCB, 0X94, 0X2E,
  0X8C, 0X2B, 0X83, 0X8B, 0X7A, 0X89, 0X71, 0XC7, 0X7A, 0X4B, 0XA3, 0X0E,
  0X7A, 0X2A, 0X7A, 0X6A, 0X9B, 0X4C, 0X71, 0XE7, 0X72, 0X49, 0X18, 0X41,
  0X61, 0XE6, 0XA3, 0X4B, 0X61, 0X26, 0X59, 0X65, 0X59, 0X65, 0X61, 0X86,
  0X51, 0X04, 0X28, 0X60, 0X30, 0XA2, 0X30, 0XA1, 0X28, 0X81, 0X30, 0XA2,
  0X30, 0XA1, 0X30, 0XA1, 0X28, 0XA1, 0X28, 0X61, 0X30, 0X61, 0X30, 0X81,
  0X38, 0X81, 0X30, 0X61, 0X30, 0X61, 0X30, 0X81, 0X30, 0XA1, 0X38, 0X61,
  0X28, 0XA1, 0X30, 0XA1, 0X38, 0XC1, 0X49, 0X02, 0X38, 0XA1, 0X30, 0X81,
  0X30, 0XC2, 0X20, 0XE4, 0X40, 0XE3, 0X61, 0X24, 0X59, 0X23, 0X29, 0X04,
  0X59, 0X23, 0X51, 0X03, 0X79, 0X63, 0X49, 0X02, 0X28, 0XC3, 0X20, 0XA3,
  0X59, 0X22, 0X59, 0X22, 0X40, 0XE2, 0X48, 0XE1, 0X49, 0X01, 0X40, 0XE3,
  0X40, 0XE3, 0X49, 0X01, 0X49, 0X01, 0X49, 0X02, 0X40, 0XC3, 0X40, 0XC3,
  0X49, 0X02, 0X49, 0X01, 0X49, 0X21, 0X48, 0XE2, 0X40, 0XC3, 0X51, 0X03,
  0X59, 0X22, 0X59, 0X63, 0X51, 0X23, 0X51, 0X23, 0X49, 0X24, 0X50, 0XE1,
  0X49, 0X02, 0X49, 0X03, 0X40, 0XE3, 0X49, 0X04, 0X51, 0X23, 0X50, 0XE4,
  0X51, 0X03, 0X51, 0X23, 0X59, 0X24, 0X71, 0X44, 0X69, 0X04, 0X69, 0X04,
  0X59, 0X23, 0X49, 0X03, 0X51, 0X44, 0X51, 0X64, 0X51, 0X44, 0X59, 0X65,
  0X51, 0X24, 0X59, 0X85, 0X51, 0X43, 0X59, 0X43, 0X49, 0X02, 0X30, 0XA2,
  0X51, 0X03, 0X49, 0X02, 0X48, 0XE2, 0X51, 0X03, 0X41, 0X03, 0X39, 0X02,
  0X38, 0XC2, 0X30, 0XA2, 0X30, 0XE2, 0X28, 0XA2, 0X30, 0XA2, 0X38, 0XE4,
  0X30, 0XC4, 0X28, 0XC3, 0X20, 0XC4, 0X7A, 0X26, 0X8A, 0X45, 0X28, 0X82,
  0X59, 0X23, 0X39, 0X03, 0X39, 0X04, 0X49, 0X04, 0X49, 0X23, 0X49, 0X24,
  0X49, 0X04, 0X49, 0X03, 0X41, 0X03, 0XBD, 0X91, 0XA4, 0XF0, 0X59, 0X06,
  0X49, 0X05, 0X59, 0X45, 0X81, 0XE8, 0X79, 0XA8, 0X61, 0X26, 0X69, 0X47,
  0X69, 0X67, 0X82, 0X09, 0X61, 0X06, 0X3A, 0XCD, 0X34, 0X32, 0X3B, 0X0C,
  0X20, 0X82, 0X31, 0X44, 0X31, 0X84, 0X10, 0X81, 0X10, 0X81, 0X28, 0XE3,
  0X20, 0XE3, 0X3A, 0X2A, 0X31, 0X25, 0X18, 0X82, 0X20, 0XA2, 0X20, 0XA3,
  0X20, 0X82, 0X31, 0XA7, 0X31, 0XE9, 0X18, 0X82, 0X18, 0XA3, 0X18, 0X82,
  0X18, 0X82, 0X20, 0XC3, 0X18, 0XA2, 0X20, 0XE3, 0X29, 0X04, 0X29, 0X24,
  0X29, 0X24, 0X29, 0X03, 0X20, 0XA2, 0X18, 0X61, 0X18, 0X82, 0X18, 0X61,
  0X21, 0X86, 0X21, 0XE9, 0X20, 0X81, 0X20, 0XC2, 0X18, 0XA2, 0X31, 0X23,
  0X39, 0X24, 0X31, 0X23, 0X28, 0XA2, 0X31, 0X03, 0X29, 0X03, 0X31, 0X04,
  0X41, 0X87, 0X31, 0X45, 0X29, 0X04, 0X39, 0XA6, 0X39, 0XA6, 0X39, 0XA6,
  0X39, 0X85, 0X39, 0X65, 0X39, 0XA6, 0X39, 0X86, 0X39, 0XA6, 0X39, 0XA6,
  0X39, 0XA6, 0X29, 0X03, 0X28, 0XE3, 0X31, 0X24, 0X41, 0X66, 0X39, 0XA6,
  0X31, 0X45, 0X31, 0X65, 0X39, 0X85, 0X31, 0XA6, 0X29, 0X04, 0X41, 0X85,
  0X62, 0X88, 0X21, 0X04, 0X39, 0X86, 0X49, 0XC6, 0X41, 0XA6, 0X41, 0XA6,
  0X39, 0X86, 0X41, 0XA6, 0X31, 0X85, 0X08, 0XE3, 0X18, 0XE4, 0X18, 0XE5,
  0X19, 0X05, 0X19, 0X05, 0X18, 0XE5, 0X10, 0XE4, 0X10, 0XE4, 0X18, 0XE4,
  0X21, 0X05, 0X21, 0X05, 0X21, 0X04, 0X18, 0XE4, 0X10, 0XE4, 0X10, 0XE3,
  0X39, 0X23, 0X28, 0XE3, 0X31, 0X03, 0X20, 0XC3, 0X49, 0X84, 0X61, 0XE5,
  0X61, 0XE5, 0X51, 0X84, 0X59, 0XC5, 0X39, 0X23, 0X39, 0X44, 0XFF, 0XBD,
  0X94, 0X52, 0X10, 0X41, 0X8A, 0XE9, 0X9B, 0X4A, 0X61, 0X84, 0XAB, 0XEE,
  0X9C, 0X91, 0X00, 0X00, 0X31, 0X23, 0X39, 0X43, 0X18, 0XE3, 0X08, 0XA2,
  0X42, 0XEC, 0X53, 0X6E, 0X32, 0X4A, 0X08, 0XA2, 0X19, 0X04, 0X21, 0X45,
  0X29, 0X66, 0X31, 0XA6, 0X29, 0X86, 0X29, 0XA6, 0X32, 0X08, 0X2A, 0X29,
  0X22, 0X08, 0X22, 0X29, 0X19, 0XC7, 0X08, 0X62, 0X63, 0X4F, 0X5B, 0X4E,
  0X29, 0XA6, 0X29, 0XA7, 0X31, 0XA6, 0X31, 0XA6, 0X29, 0X45, 0X29, 0X65,
  0X29, 0X65, 0X3A, 0X49, 0X31, 0XA6, 0X39, 0XE7, 0X3A, 0X08, 0X3A, 0X28,
  0X4A, 0X49, 0X42, 0X28, 0X39, 0XE7, 0X63, 0X2C, 0X4A, 0X49, 0X18, 0XC3,
  0X21, 0X25, 0X29, 0X45, 0X29, 0X45, 0X29, 0X45, 0X29, 0X66, 0X21, 0X24,
  0X29, 0X45, 0X31, 0XA5, 0X39, 0XC6, 0X31, 0X86, 0X29, 0X65, 0X42, 0X08,
  0X5A, 0X8A, 0X92, 0X07, 0X7B, 0XCB, 0X5A, 0X87, 0X49, 0XE5, 0X52, 0X26,
  0X52, 0X26, 0X6B, 0X2C, 0X62, 0XA9, 0X52, 0X26, 0X52, 0X26, 0X52, 0X26,
  0X62, 0XA9, 0X62, 0XA9, 0X6B, 0X4C, 0X73, 0XAF, 0X73, 0X2A, 0X73, 0X6D,
  0X73, 0X6C, 0X73, 0X4A, 0X6B, 0X4C, 0X5A, 0XCA, 0X5A, 0X89, 0X4A, 0X48,
  0X39, 0XE7, 0X52, 0X48, 0X52, 0X89, 0X62, 0XEA, 0X5A, 0XA9, 0X63, 0X0B,
  0X4A, 0X69, 0X5A, 0XAA, 0X73, 0X8D, 0X6B, 0X2B, 0X5B, 0X0B, 0X1A, 0X8B,
  0X1A, 0X8B, 0X3A, 0XAA, 0X6A, 0XEA, 0X62, 0XC9, 0X6B, 0X4B, 0X6B, 0X4B,
  0X62, 0XA8, 0X83, 0X4A, 0X52, 0X67, 0X52, 0X67, 0X21, 0X03, 0X10, 0X81,
  0X29, 0X24, 0X39, 0X85, 0X39, 0XA6, 0X62, 0XEB, 0X63, 0X0B, 0X63, 0X0B,
  0X52, 0X48, 0X52, 0X48, 0X62, 0XCA, 0X52, 0XCA, 0X1A, 0XCC, 0X4A, 0XEA,
  0X5A, 0X89, 0X5A, 0XA9, 0X62, 0XCA, 0X41, 0XE7, 0X39, 0XE7, 0X41, 0XE7,
  0X41, 0XE7, 0X41, 0XC6, 0X52, 0X6A, 0X6B, 0X2D, 0X7B, 0XF1, 0X39, 0XC6,
  0X52, 0X67, 0X52, 0X88, 0X52, 0X47, 0X62, 0XC9, 0X6B, 0X6D, 0X62, 0XEA,
  0X31, 0X44, 0X41, 0XA5, 0X6B, 0X2B, 0X6B, 0X4D, 0X6B, 0X0B, 0X6B, 0X4D,
  0X52, 0X8A, 0X73, 0X4D, 0X6B, 0X2C, 0X6B, 0X4E, 0X73, 0XD0, 0X5A, 0XCB,
  0X39, 0XE8, 0X4A, 0X6A, 0X6B, 0X2C, 0X52, 0X6A, 0X49, 0XE7, 0X4A, 0X07,
  0X49, 0XE6, 0X6B, 0X0B, 0X73, 0X6E, 0X4A, 0X49, 0X39, 0XE7, 0X42, 0X08,
  0X42, 0X08, 0X31, 0X86, 0X62, 0XEB, 0X52, 0X8A, 0X39, 0X86, 0X6B, 0X2D,
  0X62, 0XEC, 0X5A, 0X88, 0X62, 0XC9, 0X6B, 0X2A, 0X29, 0X44, 0X20, 0XE3,
  0X18, 0XA2, 0X18, 0XE3, 0X42, 0X06, 0X5A, 0XA8, 0X5A, 0X88, 0X4A, 0X27,
  0X39, 0X85, 0X52, 0X67, 0X62, 0XC8, 0X31, 0X85, 0X39, 0XA5, 0X21, 0X03,
  0X00, 0X41, 0X08, 0X62, 0X10, 0XA2, 0X10, 0XA3, 0X08, 0X82, 0X10, 0XA2,
  0X08, 0X82, 0X10, 0XE4, 0X19, 0X65, 0X2A, 0X48, 0X32, 0X8A, 0X11, 0X04,
  0X11, 0X04, 0X19, 0X24, 0X08, 0XA2, 0X18, 0XE3, 0X10, 0XC3, 0X10, 0XA3,
  0X11, 0X24, 0X19, 0X04, 0X21, 0XA7, 0X21, 0XC8, 0X19, 0X87, 0X19, 0X25,
  0X19, 0X24, 0X11, 0X04, 0X10, 0XC3, 0X19, 0X04, 0X19, 0X25, 0X10, 0XC3,
  0X18, 0XC3, 0X10, 0XC3, 0X10, 0XA3, 0X18, 0XE3, 0X19, 0X04, 0X18, 0XC3,
  0X18, 0XE4, 0X10, 0XC4, 0X10, 0XC3, 0X10, 0XE4, 0X21, 0X25, 0X18, 0XE4,
  0X21, 0X45, 0X10, 0XC4, 0X18, 0XE4, 0X52, 0X68, 0X49, 0XE5, 0X6B, 0X2B,
  0X6B, 0X2D, 0X83, 0XF0, 0X83, 0XCF, 0X73, 0X4C, 0X7B, 0X8D, 0X73, 0X8E,
  0X7C, 0X10, 0X6B, 0X2A, 0X6B, 0X69, 0X73, 0X8A, 0X73, 0X8A, 0X73, 0X2A,
  0X73, 0X6A, 0X6B, 0X8A, 0X73, 0X8A, 0X7B, 0XAC, 0X7B, 0X8B, 0X7B, 0XAC,
  0X7B, 0X8B, 0X7B, 0XCC, 0X7B, 0X8B, 0X73, 0X6B, 0X7B, 0X8C, 0X73, 0XAB,
  0X73, 0XAA, 0X73, 0XAA, 0X73, 0X6A, 0X73, 0X2A, 0X62, 0XC8, 0X62, 0XC7,
  0X52, 0X26, 0X5A, 0XA7, 0X73, 0XCC, 0X33, 0X0C, 0X6B, 0X29, 0X6B, 0X69,
  0X7B, 0XEB, 0X7C, 0X2C, 0X6B, 0X8A, 0X6B, 0X2A, 0X8C, 0X50, 0X84, 0X0F,
  0X73, 0XCE, 0X59, 0XE6, 0X39, 0X23, 0X29, 0X03, 0X5A, 0X05, 0X62, 0X25,
  0X62, 0X25, 0X62, 0X25, 0X62, 0X25, 0X62, 0X46, 0X49, 0XA4, 0X39, 0X64,
  0X49, 0X63, 0X51, 0XC5, 0X6A, 0X66, 0X6A, 0XE8, 0X62, 0X67, 0X5A, 0X27,
  0X52, 0X07, 0X62, 0XA8, 0X5A, 0X47, 0X52, 0X27, 0X5A, 0X27, 0X5A, 0X06,
  0X51, 0XC6, 0X51, 0XE6, 0X62, 0X05, 0X62, 0X26, 0X62, 0X26, 0X62, 0X05,
  0X51, 0XA5, 0X51, 0XC6, 0X49, 0XC6, 0X49, 0XA5, 0X41, 0X84, 0X49, 0XA4,
  0X41, 0X84, 0X59, 0XE5, 0X82, 0X67, 0X51, 0XA4, 0X39, 0X44, 0X31, 0X03,
  0X31, 0X03, 0X31, 0X03, 0X31, 0X03, 0X41, 0XA5, 0X29, 0X24, 0X29, 0X24,
  0X31, 0X65, 0X31, 0X45, 0X29, 0X04, 0X31, 0X03, 0X52, 0X06, 0X52, 0X06,
  0X49, 0X83, 0X49, 0X83, 0X51, 0XA4, 0X52, 0X06, 0X39, 0X23, 0X39, 0X23,
  0X31, 0X02, 0X49, 0XE6, 0X49, 0XC5, 0X49, 0XA4, 0X49, 0XA5, 0X31, 0X02,
  0X39, 0X64, 0X62, 0X88, 0X5A, 0X27, 0X29, 0XE8, 0X52, 0X68, 0X52, 0X06,
  0X62, 0X27, 0X52, 0X06, 0X49, 0XC5, 0X52, 0X06, 0X72, 0XA8, 0X52, 0X26,
  0X31, 0X03, 0X41, 0X64, 0X52, 0X06, 0X51, 0XE6, 0X51, 0XC5, 0X51, 0XC5,
  0X51, 0XC5, 0X51, 0XC5, 0X49, 0X84, 0X39, 0X44, 0X19, 0X04, 0X18, 0XE4,
  0X29, 0X66, 0X62, 0XCA, 0X62, 0XA9, 0X62, 0XA8, 0X31, 0X88, 0X29, 0XCA,
  0X29, 0X46, 0X29, 0X66, 0X3A, 0X2B, 0X4A, 0X6B, 0X39, 0XA7, 0X31, 0X05,
  0X31, 0X86, 0X2A, 0X48, 0X2A, 0X47, 0X2A, 0X28, 0X31, 0XE6, 0X29, 0X04,
  0X29, 0X26, 0X21, 0X25, 0X52, 0X48, 0X8B, 0XCD, 0X4A, 0X27, 0X52, 0X68,
  0X31, 0X85, 0X62, 0XA9, 0X62, 0XA9, 0X29, 0X03, 0X21, 0X46, 0X29, 0X68,
  0X29, 0X26, 0X42, 0X06, 0X6B, 0X8B, 0X52, 0XA8, 0X21, 0X04, 0X2B, 0X2C,
  0X29, 0X65, 0X22, 0X29, 0X1A, 0XCB, 0X41, 0X85, 0X5A, 0X67, 0X31, 0X23,
  0X39, 0XC7, 0X21, 0X46, 0X10, 0X82, 0X39, 0X44, 0X41, 0XA5, 0X49, 0XC5,
  0X5A, 0X27, 0X7A, 0XC9, 0X49, 0XE6, 0X5A, 0X47, 0X83, 0X0A, 0X7A, 0XE9,
  0X7A, 0XE9, 0X7A, 0XE9, 0X7A, 0XC9, 0X83, 0X0A, 0X62, 0X47, 0X21, 0X03,
  0X29, 0X03, 0X20, 0XE4, 0X21, 0X04, 0X20, 0XE4, 0X18, 0XC3, 0X21, 0X46,
  0X21, 0X25, 0X20, 0XE2, 0X10, 0X61, 0X10, 0XA2, 0X10, 0X82, 0X20, 0XE3,
  0X31, 0X44, 0X21, 0X23, 0X18, 0XE3, 0X18, 0XC3, 0X21, 0X04, 0X21, 0X24,
  0X19, 0X04, 0X08, 0XC3, 0X10, 0XC3, 0X10, 0XC3, 0X18, 0XC3, 0X10, 0X82,
  0X08, 0XA3, 0X08, 0X62, 0X10, 0XA2, 0X10, 0XE3, 0X10, 0XE3, 0X10, 0XE3,
  0X10, 0XC3, 0X10, 0X82, 0X10, 0X61, 0X21, 0X25, 0X52, 0XAC, 0X39, 0XC7,
  0X08, 0X61, 0X18, 0XC3, 0X18, 0XC3, 0X18, 0XC3, 0X18, 0XC3, 0X18, 0XC3,
  0X18, 0XE3, 0X21, 0X03, 0X18, 0XC3, 0X18, 0XC3, 0X18, 0XC3, 0X18, 0XA2,
  0X10, 0XC3, 0X18, 0XE3, 0X10, 0XA2, 0X08, 0X61, 0X10, 0X81, 0X10, 0XC2,
  0X10, 0XA2, 0X18, 0XE4, 0X18, 0XE3, 0X21, 0X04, 0X18, 0XE3, 0X19, 0X24,
  0X19, 0X05, 0X10, 0XA3, 0X10, 0XA2, 0X21, 0X04, 0X18, 0XC2, 0X21, 0X66,
  0X39, 0XA6, 0X31, 0X03, 0X10, 0X81, 0X19, 0X03, 0X31, 0X23, 0X49, 0XC5,
  0X49, 0XC6, 0X39, 0X84, 0X20, 0XE3, 0X28, 0XC3, 0X31, 0X24, 0X29, 0X44,
  0X39, 0XC7, 0X20, 0XE4, 0X18, 0XC3, 0X29, 0X03, 0X39, 0X44, 0X51, 0XE6,
  0X21, 0X03, 0X18, 0XC2, 0X29, 0X24, 0X29, 0X03, 0X31, 0XC7, 0X20, 0XE3,
  0X20, 0XC3, 0X39, 0X85, 0X39, 0X44, 0X20, 0XE3, 0X39, 0XE8, 0X29, 0X65,
  0X20, 0XA2, 0X28, 0XE2, 0X18, 0XC3, 0X31, 0X44, 0X39, 0X85, 0X31, 0X65,
  0X21, 0X03, 0X29, 0XA5, 0X21, 0XE5, 0X11, 0X23, 0X19, 0X65, 0X18, 0XC3,
  0X19, 0X25, 0X11, 0X05, 0X08, 0X62, 0X08, 0XA3, 0X19, 0X25, 0X21, 0X45,
  0X19, 0XA7, 0X09, 0X04, 0X09, 0X24, 0X08, 0XC3, 0X08, 0XC3, 0X11, 0X66,
  0X09, 0X04, 0X08, 0XE3, 0X09, 0X86, 0X09, 0X66, 0X19, 0X66, 0X19, 0X25,
  0X18, 0XE4, 0X18, 0XE4, 0X18, 0XE3, 0X18, 0XC2, 0X29, 0X03, 0X41, 0XA5,
  0X52, 0X06, 0X5A, 0X67, 0X5A, 0X67, 0X62, 0X46, 0X62, 0X26, 0X62, 0X26,
  0X62, 0X47, 0X62, 0X47, 0X62, 0X46, 0X7B, 0X4A, 0X82, 0XA6, 0X79, 0X81,
  0X69, 0XE4, 0X62, 0X65, 0X51, 0XE4, 0X51, 0XC5, 0X59, 0XE6, 0X39, 0XE6,
  0X31, 0XC6, 0X59, 0XC6, 0X51, 0XA6, 0X41, 0XA6, 0X51, 0XC6, 0X5A, 0X69,
  0X49, 0XC6, 0X62, 0X28, 0X9B, 0X2B, 0X6A, 0X48, 0X39, 0X65, 0X93, 0X4C,
  0X7A, 0XCA, 0X7A, 0XEA, 0X8B, 0X2C, 0X52, 0X49, 0X52, 0X69, 0X52, 0X29,
  0X49, 0X86, 0X49, 0XA5, 0X49, 0XC6, 0X52, 0X06, 0X5A, 0X07, 0X51, 0XC7,
  0X49, 0XA7, 0X49, 0XA6, 0X49, 0XA6, 0X51, 0XC6, 0X7B, 0X8F, 0X62, 0XAA,
  0X49, 0X64, 0X51, 0XA5, 0X5A, 0X48, 0X5A, 0X89, 0X5A, 0X89, 0X52, 0X08,
  0X49, 0X85, 0X41, 0X44, 0X31, 0X23, 0X49, 0XA5, 0X49, 0XA5, 0X4A, 0X47,
  0X41, 0XC5, 0X51, 0XE6, 0X6A, 0XC9, 0X62, 0XA9, 0X62, 0X88, 0X62, 0XA9,
  0X49, 0XE6, 0X31, 0X03, 0X39, 0X24, 0X39, 0X24, 0X41, 0X65, 0X51, 0XC6,
  0X62, 0X28, 0X2B, 0X4B, 0X1A, 0X26, 0X29, 0X65, 0X41, 0XC7, 0X5A, 0XCA,
  0X2A, 0X27, 0X3A, 0XEA, 0X6A, 0X88, 0X59, 0XA5, 0X42, 0X05, 0X63, 0X2A,
  0X7B, 0X8B, 0X5A, 0X88, 0X39, 0XE6, 0X63, 0X29, 0X6B, 0X8B, 0X6B, 0X8B,
  0X6B, 0XCF, 0X6B, 0X6B, 0X63, 0X4A, 0X6B, 0X8C, 0X4A, 0X47, 0X32, 0XEA,
  0X41, 0X65, 0X52, 0X28, 0X59, 0XE7, 0X59, 0X65, 0X49, 0X45, 0X32, 0X28,
  0X52, 0X28, 0X52, 0X29, 0X83, 0X0A, 0X6A, 0X88, 0X3A, 0X48, 0X63, 0X2A,
  0X63, 0X2B, 0X6B, 0X6C, 0X6B, 0X6C, 0X73, 0X6C, 0X5B, 0X2B, 0X5B, 0X09,
  0X52, 0X4A, 0X5A, 0X6A, 0X7B, 0X8B, 0X6A, 0XA7, 0X72, 0X27, 0X72, 0X8C,
  0X72, 0XEA, 0X83, 0X4A, 0X7B, 0X4B, 0X83, 0X6C, 0X83, 0X8C, 0X83, 0X6C,
  0X83, 0X4C, 0X83, 0X4C, 0X83, 0X4C, 0X8B, 0X8D, 0X7B, 0X29, 0X62, 0XA7,
  0X83, 0X6C, 0X83, 0X6C, 0X7B, 0X2A, 0X61, 0XA4, 0X79, 0X87, 0X9B, 0X2D,
  0XAB, 0X0E, 0XB2, 0XAE, 0X93, 0X8F, 0X83, 0X8C, 0X83, 0XD0, 0X83, 0XB0,
  0X69, 0XE8, 0X6A, 0X29, 0X8B, 0X4D, 0X83, 0X6F, 0XA3, 0XCE, 0XC5, 0X34,
  0X6A, 0XED, 0X7A, 0XEB, 0X62, 0X48, 0X93, 0X2E, 0XB3, 0XEF, 0XB3, 0XCE,
  0X9B, 0X2C, 0X3B, 0XD2, 0X73, 0X0D, 0XA2, 0XAA, 0X9A, 0XEB, 0X72, 0X49,
  0X41, 0X66, 0X83, 0X90, 0X93, 0X4C, 0X9B, 0XCF, 0X6A, 0XED, 0X6A, 0XCC,
  0X72, 0XEC, 0X7B, 0X0C, 0X7B, 0X2E, 0X62, 0XCC, 0X83, 0X4D, 0X6A, 0XA9,
  0X5A, 0X2B, 0X6A, 0XF0, 0X62, 0XAC, 0X62, 0XEE, 0X52, 0X4A, 0X39, 0X87,
  0X52, 0X2B, 0X4A, 0X09, 0X7A, 0XCC, 0X6A, 0XAE, 0X83, 0X92, 0X8B, 0XB0,
  0X6A, 0X8B, 0X83, 0XD3, 0X9C, 0X95, 0X7A, 0XAC, 0X7A, 0XCD, 0X9B, 0X6C,
  0X8B, 0X2D, 0X7A, 0X8B, 0X72, 0XAC, 0XAB, 0XEE, 0XBC, 0X4E, 0X69, 0XA8,
  0X61, 0XA9, 0X72, 0X2C, 0X6A, 0X0C, 0X9A, 0XCD, 0XC3, 0X8F, 0X7A, 0X4C,
  0X72, 0X0B, 0X72, 0X0B, 0X69, 0XEB, 0X82, 0X8D, 0XED, 0X14, 0XA3, 0X2F,
  0X69, 0XCB, 0X8A, 0X2B, 0X89, 0XCA, 0X61, 0X06, 0XA2, 0X8B, 0XBB, 0X2D,
  0XA2, 0XAD, 0X72, 0X0A, 0X3A, 0X09, 0X5A, 0X6B, 0X9A, 0XEE, 0X62, 0X8B,
  0X71, 0X69, 0X61, 0XB0, 0X51, 0X8D, 0X59, 0XF1, 0X52, 0X34, 0X59, 0XCF,
  0X69, 0XF1, 0X41, 0XAB, 0X41, 0X88, 0X41, 0XCB, 0X49, 0XCC, 0X4A, 0X2E,
  0X39, 0XAC, 0X31, 0X6A, 0X31, 0X69, 0X29, 0X27, 0X59, 0XED, 0X5A, 0X4C,
  0X73, 0XCF, 0X41, 0X8A, 0X39, 0X68, 0X41, 0XAA, 0X49, 0XCB, 0X29, 0X26,
  0X10, 0XC3, 0X19, 0X04, 0X10, 0XA2, 0X31, 0X25, 0X41, 0X67, 0X08, 0X61,
  0X08, 0X41, 0X18, 0XA2, 0X20, 0XE3, 0X20, 0XE3, 0X21, 0X04, 0X21, 0X04,
  0X20, 0XE3, 0X28, 0XE3, 0X18, 0XC3, 0X18, 0XE3, 0X18, 0XC2, 0X10, 0XC3,
  0X21, 0X44, 0X29, 0XC7, 0X32, 0X28, 0X29, 0XC7, 0X19, 0X04, 0X29, 0X86,
  0X29, 0X86, 0X10, 0XA2, 0X18, 0XA3, 0X18, 0XE4, 0X18, 0X83, 0X18, 0XE4,
  0X31, 0X25, 0X48, 0X63, 0X21, 0X25, 0X19, 0X46, 0X21, 0X25, 0X3A, 0X6C,
  0X53, 0X90, 0X29, 0XE9, 0X42, 0XCD, 0X42, 0XAD, 0X64, 0X13, 0X5B, 0XB1,
  0X42, 0XAD, 0X5C, 0X15, 0X5B, 0XD3, 0X3A, 0X6B, 0X53, 0XB2, 0X5B, 0XD4,
  0X5B, 0XB3, 0X5B, 0XF5, 0X3A, 0X8C, 0X31, 0XE8, 0X42, 0XAC, 0X3A, 0X6B,
  0X43, 0X2E, 0X43, 0X0E, 0X29, 0XE9, 0X74, 0XB6, 0X4B, 0X2E, 0X3A, 0X4A,
  0X3A, 0X6A, 0X3A, 0X8B, 0X32, 0X29, 0X21, 0X86, 0X4B, 0X30, 0X64, 0X14,
  0X52, 0XED, 0X73, 0X2A, 0X52, 0XED, 0X53, 0XB1, 0X4B, 0X0E, 0X52, 0XAB,
  0X52, 0XCC, 0X53, 0X2E, 0X29, 0XA7, 0X29, 0XC7, 0X29, 0XA6, 0X19, 0X05,
  0X18, 0XE4, 0X10, 0XA2, 0X10, 0XA2, 0X21, 0XA7, 0X21, 0X66, 0X29, 0X66,
  0X21, 0X86, 0X19, 0X04, 0X29, 0X66, 0X29, 0X45, 0X19, 0X25, 0X21, 0X25,
  0X20, 0XE4, 0X21, 0XA6, 0X29, 0XE7, 0X32, 0X28
};

//== Variables for fakeTV == End ============================

// == running text == start ===========================================================================================================================

void pxlBlckUtils_prepare_runing_text(String text, uint32_t txtColor, uint32_t bgColor, uint16_t delayTime, uint8_t startPosition)
{
  //This function initates a running text with the given parameters

  PXLBLCK_RNG_TXT_STRUCT.runtxtColor = txtColor;
  PXLBLCK_RNG_TXT_STRUCT.runtxtBgColor = bgColor;
  PXLBLCK_RNG_TXT_STRUCT.runtxtDelayTime = delayTime;
  PXLBLCK_RNG_TXT_STRUCT.runtxtPassedDelayTime = 0;
  PXLBLCK_RNG_TXT_STRUCT.runtxtText = text;

  if (startPosition >= PXLBLCK_MATRIX_WIDTH || startPosition < 0) //if the startPosition is greater than the width of the led matrix we shift the startPosition over to the start of te left side of the led matrix. This ensures that the running text is directly visible.
    PXLBLCK_RNG_TXT_STRUCT.runtxtPosition = PXLBLCK_MATRIX_WIDTH;
  else
    PXLBLCK_RNG_TXT_STRUCT.runtxtPosition = startPosition;

  String log = F(PXLBLCK_DEVICE_NAME);
  addLog(LOG_LEVEL_DEBUG, log);
  log = F("   -Running-Text started! Text: \"");
  log += text;
  log += F("\"");
  addLog(LOG_LEVEL_DEBUG, log);
  log = F("   -PXLBLCK_RNG_TXT_STRUCT.runtxtPosition: ");
  log += PXLBLCK_RNG_TXT_STRUCT.runtxtPosition;
  addLog(LOG_LEVEL_DEBUG, log);
  log = F("   -PXLBLCK_RNG_TXT_STRUCT.runtxtDelayTime: ");
  log += PXLBLCK_RNG_TXT_STRUCT.runtxtDelayTime;
  addLog(LOG_LEVEL_DEBUG, log);
  log = F("   -PXLBLCK_RNG_TXT_STRUCT.runtxtBgColor: ");
  log += PXLBLCK_RNG_TXT_STRUCT.runtxtBgColor;
  addLog(LOG_LEVEL_DEBUG, log);
  log = F("   -PXLBLCK_RNG_TXT_STRUCT.runtxtColor: ");
  log += PXLBLCK_RNG_TXT_STRUCT.runtxtColor;
  addLog(LOG_LEVEL_DEBUG, log);

}

void pxlBlckUtils_check_running_text()
{
  //This function updates the display with the actual running text if there is one initiated

  if (PXLBLCK_RNG_TXT_STRUCT.runtxtDelayTime > 0) //running text will only updated if there is a delay time set(>0)
  {
    if (pxlBlckUtils_execute_if_interval_passed(&PXLBLCK_RNG_TXT_STRUCT.runtxtPassedDelayTime, PXLBLCK_RNG_TXT_STRUCT.runtxtDelayTime)) //interval/delay time is passed so move the running text one pixel to the left
    {

      String log = F(PXLBLCK_DEVICE_NAME);
      addLog(LOG_LEVEL_DEBUG, log);
      log = F("   -Running-Text: ");
      log += PXLBLCK_RNG_TXT_STRUCT.runtxtText;
      addLog(LOG_LEVEL_DEBUG, log);
      log = F("   -PXLBLCK_RNG_TXT_STRUCT.runtxtPosition: ");
      log += PXLBLCK_RNG_TXT_STRUCT.runtxtPosition;
      addLog(LOG_LEVEL_DEBUG, log);
      log = F("   -PXLBLCK_RNG_TXT_STRUCT.runtxtDelayTime: ");
      log += PXLBLCK_RNG_TXT_STRUCT.runtxtDelayTime;
      addLog(LOG_LEVEL_DEBUG, log);
      log = F("   -PXLBLCK_RNG_TXT_STRUCT.runtxtBgColor: ");
      log += PXLBLCK_RNG_TXT_STRUCT.runtxtBgColor;
      addLog(LOG_LEVEL_DEBUG, log);
      log = F("   -PXLBLCK_RNG_TXT_STRUCT.runtxtColor: ");
      log += PXLBLCK_RNG_TXT_STRUCT.runtxtColor;
      addLog(LOG_LEVEL_DEBUG, log);


      //int border = (0 - (PXLBLCK_RNG_TXT_STRUCT.runtxtText.length() * 6)); //6 because each charcter has a width of five pixels and a "space"

      if (PXLBLCK_RNG_TXT_STRUCT.runtxtPosition > (int16_t)(0 - (int16_t)(PXLBLCK_RNG_TXT_STRUCT.runtxtText.length() * 6))) //checks if right border of the running text has passed the left side of the led matrix. Multiplied 6 because each charcter has a width of five pixels and a "space"-pixel.
      {
        pxlBlckUtils_fill_matrix(PXLBLCK_RNG_TXT_STRUCT.runtxtBgColor);

        if (PXLBLCK_HIGHER_COLOR_RESOLUTION_ENABLED)
          PXLBLCK_INSTANCE->setPassThruColor(PXLBLCK_RNG_TXT_STRUCT.runtxtColor);
        else
          PXLBLCK_INSTANCE->setTextColor(PXLBLCK_RNG_TXT_STRUCT.runtxtColor);

        PXLBLCK_INSTANCE->setTextSize(1);
        PXLBLCK_INSTANCE->setCursor(PXLBLCK_RNG_TXT_STRUCT.runtxtPosition, 0);
        PXLBLCK_INSTANCE->print(PXLBLCK_RNG_TXT_STRUCT.runtxtText);
        PXLBLCK_RNG_TXT_STRUCT.runtxtPosition--;
        pxlBlckUtils_update_matrix();
      } else //Scrolling is finished(running text has passed the left side of the led matrix) so reset variables to end execution of running text routine
      {
        PXLBLCK_RNG_TXT_STRUCT.runtxtDelayTime = 0;
        pxlBlckUtils_clear_matrix();
        pxlBlckUtils_update_matrix();
      }
    }
  }
}

// == running text == end ===========================================================================================================================

// == icon handling == start ===========================================================================================================================

void pxlBlckUtils_prepare_multi_colored_icon(struct EventStruct * event, uint8_t inAnimation, uint8_t outAnimation, uint16_t inDelay, uint16_t showDelay, uint16_t outDelay, uint8_t brightness, String textThatFollows, String spiffsIcon, uint8_t repetition)
{
  if (pxlBlckUtils_execute_if_interval_passed(&PXLBLCK_ICON_SHOWED_TIMESTAMP, PXLBLCK_ICON_COOLDOWN_TIME))
  {
    PXLBLCK_ICON_STRUCT.iconPending = true;
    PXLBLCK_ICON_STRUCT.iconState = PXLBLCK_ICON_STATE_START;
    PXLBLCK_ICON_STRUCT.inAnimation = inAnimation;
    PXLBLCK_ICON_STRUCT.outAnimation = outAnimation;
    PXLBLCK_ICON_STRUCT.inDelay = inDelay;
    PXLBLCK_ICON_STRUCT.outDelay = outDelay;
    PXLBLCK_ICON_STRUCT.showDelay = showDelay;
    PXLBLCK_ICON_STRUCT.brightness = brightness;
    PXLBLCK_ICON_STRUCT.showDelayTimestamp = 0;
    PXLBLCK_ICON_STRUCT.textThatFollows = textThatFollows;
    PXLBLCK_ICON_STRUCT.repetition = repetition;
    PXLBLCK_ICON_STRUCT.spiffsIcon = spiffsIcon;

    //First check if file exists and then try to read/load it, If icon is available its data will be copied to PXLBLCK_ICON_STRUCT.logo
    if (!pxlBlckUtils_check_if_icon_file_exists(PXLBLCK_ICON_STRUCT.spiffsIcon) || !pxlBlckUtils_load_ppm_file_to_dynamic_array(PXLBLCK_ICON_STRUCT.spiffsIcon))
    {
      //deactivate icon because spiffs-file was not found or could not read
      PXLBLCK_ICON_STRUCT.iconPending = false;
      PXLBLCK_ICON_STRUCT.textThatFollows = "";

      String log = F("pxlBlck: Icon not shown. Icon file not found.");
      SendStatus(event->Source, log);

      log = F(PXLBLCK_DEVICE_NAME);
      addLog(LOG_LEVEL_INFO, log);
      log = F("   -Error: Icon-file \"");
      log += PXLBLCK_ICON_STRUCT.spiffsIcon;
      log += F("\" does not exist");
      addLog(LOG_LEVEL_INFO, log);

      //let matrix blink five times to show that icon was not found
      for (uint8_t i = 0; i < 5; i++)
      {
        pxlBlckUtils_fill_matrix(PXLBLCK_INSTANCE->Color(255, 0, 0));
        pxlBlckUtils_update_matrix();
        delay(250);
        pxlBlckUtils_clear_matrix();
        pxlBlckUtils_update_matrix();
        delay(250);
      }
    } else
    {

      String log = F("pxlBlck: Icon found and loaded.");
      SendStatus(event->Source, log);

      log = F(PXLBLCK_DEVICE_NAME);
      addLog(LOG_LEVEL_DEBUG, log);
      log = F("   -Icon found and loaded: ");
      addLog(LOG_LEVEL_DEBUG, log);
      log = F("   -spiffsIcon: ");
      log += PXLBLCK_ICON_STRUCT.spiffsIcon;
      addLog(LOG_LEVEL_DEBUG, log);
      log = F("   -inAnimation: ");
      log += PXLBLCK_ICON_STRUCT.inAnimation;
      addLog(LOG_LEVEL_DEBUG, log);
      log = F("   -outAnimation: ");
      log += PXLBLCK_ICON_STRUCT.outAnimation;
      addLog(LOG_LEVEL_DEBUG, log);
      log = F("   -inDelay: ");
      log += PXLBLCK_ICON_STRUCT.inDelay;
      addLog(LOG_LEVEL_DEBUG, log);
      log = F("   -outDelay: ");
      log += PXLBLCK_ICON_STRUCT.outDelay;
      addLog(LOG_LEVEL_DEBUG, log);
      log = F("   -showDelay: ");
      log += PXLBLCK_ICON_STRUCT.showDelay;
      addLog(LOG_LEVEL_DEBUG, log);
      log = F("   -brightness: ");
      log += PXLBLCK_ICON_STRUCT.brightness;
      addLog(LOG_LEVEL_DEBUG, log);
      log = F("   -textThatFollows: ");
      log += PXLBLCK_ICON_STRUCT.textThatFollows;
      addLog(LOG_LEVEL_DEBUG, log);
      log = F("   -repetition: ");
      log += PXLBLCK_ICON_STRUCT.repetition;
      addLog(LOG_LEVEL_DEBUG, log);
      log = F("   -spiffsIcon: ");
      log += PXLBLCK_ICON_STRUCT.spiffsIcon;
      addLog(LOG_LEVEL_DEBUG, log);
    }
  } else
  {
    String log = F("pxlBlck: Icon not shown. Cool-down-time not passed.");
    SendStatus(event->Source, log);

    log = F(PXLBLCK_DEVICE_NAME);
    addLog(LOG_LEVEL_DEBUG, log);
    log = F("   -Icon not shown because cooldown-time is not passed yet.");
    addLog(LOG_LEVEL_DEBUG, log);


  }
}

void pxlBlckUtils_check_multi_colored_icon()
{
  if (PXLBLCK_ICON_STRUCT.iconPending)
  {
    switch (PXLBLCK_ICON_STRUCT.iconState)
    {
      case PXLBLCK_ICON_STATE_START:
        {
          float brightness = PXLBLCK_ICON_STRUCT.brightness / 100.0;

          String log = F(PXLBLCK_DEVICE_NAME);
          addLog(LOG_LEVEL_DEBUG, log);
          log = F("   -Icon state: start");
          addLog(LOG_LEVEL_DEBUG, log);
          log = F("   -brightness: ");
          log += brightness;
          addLog(LOG_LEVEL_DEBUG, log);

          switch (PXLBLCK_ICON_STRUCT.inAnimation)
          {
            case PXLBLCK_ICON_ANIM_INSTANTLY_ON:
              {

                pxlBlckUtils_clear_matrix();
                for (int row = 0; row < PXLBLCK_ICON_HEIGHT; row++)
                {
                  for (int column = 0; column < PXLBLCK_ICON_WIDTH; column++)
                  {
                    uint8_t r = PXLBLCK_ICON_STRUCT.logo[0][row][column];
                    uint8_t g = PXLBLCK_ICON_STRUCT.logo[1][row][column];
                    uint8_t b = PXLBLCK_ICON_STRUCT.logo[2][row][column];

                    pxlBlckUtils_exchange_if_led_type_is_rgb(&r, &g);

                    pxlBlckUtils_draw_pixel(column, row, pxlBlckUtils_convert_color_values_to_32bit(brightness * r, brightness * g, brightness * b));
                  }
                }

                if (PXLBLCK_ICON_STRUCT.textThatFollows.length() > 1)
                {
                  if (PXLBLCK_HIGHER_COLOR_RESOLUTION_ENABLED)
                    PXLBLCK_INSTANCE->setPassThruColor(pxlBlckUtils_convert_color_values_to_32bit(brightness * 255, brightness * 255, brightness * 255));
                  else
                    PXLBLCK_INSTANCE->setTextColor(pxlBlckUtils_convert_color_values_to_32bit(brightness * 255, brightness * 255, brightness * 255));

                  PXLBLCK_INSTANCE->setCursor(PXLBLCK_ICON_WIDTH + 2, 0);
                  PXLBLCK_INSTANCE->print(PXLBLCK_ICON_STRUCT.textThatFollows);
                }

                pxlBlckUtils_update_matrix();

                PXLBLCK_ICON_STRUCT.iconState = PXLBLCK_ICON_STATE_SHOWING;
              }
              break;
            case PXLBLCK_ICON_ANIM_FADE_IN:
              {
                uint8_t inDelay = PXLBLCK_ICON_STRUCT.inDelay / (brightness / PXLBLCK_ICON_FADE_STEP_SIZE) ;

                log = F("   -inDelay: ");
                log += inDelay;
                addLog(LOG_LEVEL_DEBUG, log);

                pxlBlckUtils_clear_matrix();
                for (float i = 0.0; i < brightness; i += PXLBLCK_ICON_FADE_STEP_SIZE)
                {
                  for (int row = 0; row < PXLBLCK_ICON_HEIGHT; row++)
                  {
                    for (int column = 0; column < PXLBLCK_ICON_WIDTH; column++)
                    {

                      uint8_t r = PXLBLCK_ICON_STRUCT.logo[0][row][column];
                      uint8_t g = PXLBLCK_ICON_STRUCT.logo[1][row][column];
                      uint8_t b = PXLBLCK_ICON_STRUCT.logo[2][row][column];

                      pxlBlckUtils_exchange_if_led_type_is_rgb(&r, &g);

                      pxlBlckUtils_draw_pixel(column, row, pxlBlckUtils_convert_color_values_to_32bit(i * r, i * g, i * b));
                    }
                  }


                  if (PXLBLCK_ICON_STRUCT.textThatFollows.length() > 1)
                  {
                    if (PXLBLCK_HIGHER_COLOR_RESOLUTION_ENABLED)
                      PXLBLCK_INSTANCE->setPassThruColor(pxlBlckUtils_convert_color_values_to_32bit(i * 255, i * 255, i * 255));
                    else
                      PXLBLCK_INSTANCE->setTextColor(pxlBlckUtils_convert_color_values_to_32bit(i * 255, i * 255, i * 255));

                    PXLBLCK_INSTANCE->setCursor(PXLBLCK_ICON_WIDTH + 2, 0);
                    PXLBLCK_INSTANCE->print(PXLBLCK_ICON_STRUCT.textThatFollows);
                  }

                  pxlBlckUtils_update_matrix();
                  delay(inDelay);
                }

                PXLBLCK_ICON_STRUCT.iconState = PXLBLCK_ICON_STATE_SHOWING;
              }
              break;
            case PXLBLCK_ICON_ANIM_FLY_IN_FROM_RIGHT:
              {
                uint8_t inDelay = PXLBLCK_ICON_STRUCT.inDelay / PXLBLCK_MATRIX_WIDTH ;

                log = F("   -inDelay: ");
                log += inDelay;
                addLog(LOG_LEVEL_DEBUG, log);

                int8_t x = PXLBLCK_MATRIX_WIDTH;

                while (x > -1)
                {
                  pxlBlckUtils_clear_matrix();
                  for (int row = 0; row < PXLBLCK_ICON_HEIGHT ; row++)
                  {
                    for (int column = 0; column < PXLBLCK_ICON_WIDTH; column++)
                    {

                      uint8_t r = PXLBLCK_ICON_STRUCT.logo[0][row][column];
                      uint8_t g = PXLBLCK_ICON_STRUCT.logo[1][row][column];
                      uint8_t b = PXLBLCK_ICON_STRUCT.logo[2][row][column];

                      pxlBlckUtils_exchange_if_led_type_is_rgb(&r, &g);

                      pxlBlckUtils_draw_pixel(column + x, row, pxlBlckUtils_convert_color_values_to_32bit(brightness * r, brightness * g, brightness * b));
                    }
                  }

                  if (PXLBLCK_ICON_STRUCT.textThatFollows.length() > 1)
                  {
                    if (PXLBLCK_HIGHER_COLOR_RESOLUTION_ENABLED)
                      PXLBLCK_INSTANCE->setPassThruColor(pxlBlckUtils_convert_color_values_to_32bit(brightness * 255, brightness * 255, brightness * 255));
                    else
                      PXLBLCK_INSTANCE->setTextColor(pxlBlckUtils_convert_color_values_to_32bit(brightness * 255, brightness * 255, brightness * 255));

                    PXLBLCK_INSTANCE->setCursor(x + PXLBLCK_ICON_WIDTH + 2, 0);
                    PXLBLCK_INSTANCE->print(PXLBLCK_ICON_STRUCT.textThatFollows);
                  }

                  pxlBlckUtils_update_matrix();
                  delay(inDelay);
                  x--;
                }

                PXLBLCK_ICON_STRUCT.iconState = PXLBLCK_ICON_STATE_SHOWING;
              }
              break;
            default:
              {
                String log = F(PXLBLCK_DEVICE_NAME);
                addLog(LOG_LEVEL_INFO, log);
                log = F("   -icon-routine called with unknown inAnimation: ");
                log += PXLBLCK_ICON_STRUCT.inAnimation;
                addLog(LOG_LEVEL_INFO, log);
              }
              break;
          }
          PXLBLCK_ICON_STRUCT.showDelayTimestamp = millis(); //save timestamp of end of in-animation so the duration of the showing-state starts now
          break;
        }
        break;

      case PXLBLCK_ICON_STATE_SHOWING:
        {
          if (pxlBlckUtils_execute_if_interval_passed(&PXLBLCK_ICON_STRUCT.showDelayTimestamp, PXLBLCK_ICON_STRUCT.showDelay)) //interval/delay time is passed so move to the next icon state
          {
            String log = F(PXLBLCK_DEVICE_NAME);
            addLog(LOG_LEVEL_DEBUG, log);
            log = F("   -Icon state: showing (finished)");
            addLog(LOG_LEVEL_DEBUG, log);

            PXLBLCK_ICON_STRUCT.iconState = PXLBLCK_ICON_STATE_END;
          }
        }
        break;

      case PXLBLCK_ICON_STATE_END:
        {
          float brightness = PXLBLCK_ICON_STRUCT.brightness / 100.0;

          String log = F(PXLBLCK_DEVICE_NAME);
          addLog(LOG_LEVEL_DEBUG, log);
          log = F("   -Icon state: start");
          addLog(LOG_LEVEL_DEBUG, log);
          log = F("   -brightness: ");
          log += brightness;
          addLog(LOG_LEVEL_DEBUG, log);

          switch (PXLBLCK_ICON_STRUCT.outAnimation)
          {
            case PXLBLCK_ICON_ANIM_INSTANTLY_OFF:
              {
                pxlBlckUtils_clear_matrix();
                pxlBlckUtils_update_matrix();
              }
              break;
            case PXLBLCK_ICON_ANIM_FADE_OUT:
              {
                uint8_t outDelay = PXLBLCK_ICON_STRUCT.outDelay / (brightness / PXLBLCK_ICON_FADE_STEP_SIZE) ;

                log = F("   -outDelay: ");
                log += outDelay;
                addLog(LOG_LEVEL_DEBUG, log);

                pxlBlckUtils_clear_matrix();

                for (float i = brightness; i > 0; i -= 0.01)
                {
                  for (int row = 0; row < PXLBLCK_ICON_HEIGHT; row++)
                  {
                    for (int column = 0; column < PXLBLCK_ICON_WIDTH; column++)
                    {

                      uint8_t r = PXLBLCK_ICON_STRUCT.logo[0][row][column];
                      uint8_t g = PXLBLCK_ICON_STRUCT.logo[1][row][column];
                      uint8_t b = PXLBLCK_ICON_STRUCT.logo[2][row][column];

                      pxlBlckUtils_exchange_if_led_type_is_rgb(&r, &g);

                      pxlBlckUtils_draw_pixel(column, row, pxlBlckUtils_convert_color_values_to_32bit(i * r, i * g, i * b));
                    }
                  }

                  if (PXLBLCK_ICON_STRUCT.textThatFollows.length() > 1)
                  {
                    if (PXLBLCK_HIGHER_COLOR_RESOLUTION_ENABLED)
                      PXLBLCK_INSTANCE->setPassThruColor(pxlBlckUtils_convert_color_values_to_32bit(i * 255, i * 255, i * 255));
                    else
                      PXLBLCK_INSTANCE->setTextColor(pxlBlckUtils_convert_color_values_to_32bit(i * 255, i * 255, i * 255));

                    PXLBLCK_INSTANCE->setCursor(PXLBLCK_ICON_WIDTH + 2, 0);
                    PXLBLCK_INSTANCE->print(PXLBLCK_ICON_STRUCT.textThatFollows);
                  }

                  pxlBlckUtils_update_matrix();
                  delay(outDelay);
                }
              }
              break;
            case PXLBLCK_ICON_ANIM_FLY_OUT_TO_LEFT:
              {
                uint8_t outDelay = PXLBLCK_ICON_STRUCT.outDelay / PXLBLCK_MATRIX_WIDTH ;

                log = F("   -outDelay: ");
                log += outDelay;
                addLog(LOG_LEVEL_DEBUG, log);

                int8_t x = 0;
                //if there is also a text displayed the number of steps for moving out the display-content needs to be increased depending on the number of characters of the displayed text
                int16_t limit = (PXLBLCK_ICON_STRUCT.textThatFollows.length() > 1) ? (0 - (PXLBLCK_ICON_WIDTH + (PXLBLCK_ICON_STRUCT.textThatFollows.length() * 6) + 1)) : (0 - PXLBLCK_ICON_WIDTH);
                while (x >= limit)
                {
                  pxlBlckUtils_clear_matrix();
                  for (int row = 0; row <= PXLBLCK_ICON_HEIGHT; row++)
                  {
                    for (int column = 0; column < PXLBLCK_ICON_WIDTH; column++)
                    {

                      uint8_t r = PXLBLCK_ICON_STRUCT.logo[0][row][column];
                      uint8_t g = PXLBLCK_ICON_STRUCT.logo[1][row][column];
                      uint8_t b = PXLBLCK_ICON_STRUCT.logo[2][row][column];

                      pxlBlckUtils_exchange_if_led_type_is_rgb(&r, &g);

                      pxlBlckUtils_draw_pixel(column + x, row, pxlBlckUtils_convert_color_values_to_32bit(brightness * r, brightness * g, brightness * b));
                    }
                  }

                  if (PXLBLCK_ICON_STRUCT.textThatFollows.length() > 1)
                  {
                    if (PXLBLCK_HIGHER_COLOR_RESOLUTION_ENABLED)
                      PXLBLCK_INSTANCE->setPassThruColor(pxlBlckUtils_convert_color_values_to_32bit(brightness * 255, brightness * 255, brightness * 255));
                    else
                      PXLBLCK_INSTANCE->setTextColor(pxlBlckUtils_convert_color_values_to_32bit(brightness * 255, brightness * 255, brightness * 255));

                    PXLBLCK_INSTANCE->setCursor(x + PXLBLCK_ICON_WIDTH + 2, 0);
                    PXLBLCK_INSTANCE->print(PXLBLCK_ICON_STRUCT.textThatFollows);
                  }

                  pxlBlckUtils_update_matrix();
                  delay(outDelay);
                  x--;
                }
              }
              break;
            default:
              {
                String log = F(PXLBLCK_DEVICE_NAME);
                addLog(LOG_LEVEL_INFO, log);
                log = F("   -icon-routine called with unknown outAnimation: ");
                log += PXLBLCK_ICON_STRUCT.outAnimation;
                addLog(LOG_LEVEL_INFO, log);
              }
              break;
          }

          //in case this icon should be repeated reset the icon state and decrease the repition value
          if (PXLBLCK_ICON_STRUCT.repetition > 0 && PXLBLCK_ICON_STRUCT.textThatFollows.length() <= 1)
          {
            PXLBLCK_ICON_STRUCT.iconState = PXLBLCK_ICON_STATE_START;
            PXLBLCK_ICON_STRUCT.repetition -= 1;
          } else
          {
            PXLBLCK_ICON_STRUCT.iconPending = false;
          }
        }
        break;

      default:
        {
          PXLBLCK_ICON_STRUCT.iconPending = false;
          String log = F(PXLBLCK_DEVICE_NAME);
          addLog(LOG_LEVEL_INFO, log);
          log = F("   -icon-routine called with unknown iconState: ");
          log += PXLBLCK_ICON_STRUCT.iconState;
          addLog(LOG_LEVEL_INFO, log);
        }
        break;
    }
  }
}

// == icon handling == end ===========================================================================================================================

// == Matrix helper functions == start ===============================================================================================================



void pxlBlckUtils_draw_rectangle(uint8_t xPosStart, uint8_t yPosStart, uint8_t width, uint8_t height, uint32_t color)
{
  if (PXLBLCK_HIGHER_COLOR_RESOLUTION_ENABLED)
    PXLBLCK_INSTANCE->setPassThruColor(color);
  PXLBLCK_INSTANCE->fillRect(xPosStart, yPosStart, width, height, color);
}

void pxlBlckUtils_update_matrix()
{
  PXLBLCK_INSTANCE->show();

  String log = F(PXLBLCK_DEVICE_NAME);
  addLog(LOG_LEVEL_DEBUG, log);
  log = F("   -pxlBlckUtils_update_matrix executed");
  addLog(LOG_LEVEL_DEBUG, log);
}

void pxlBlckUtils_fill_matrix(uint8_t red, uint8_t green, uint8_t blue)
{
  pxlBlckUtils_fill_matrix(pxlBlckUtils_convert_color_values_to_32bit(red, green, blue));
}

void pxlBlckUtils_fill_matrix(uint32_t color)
{
  String log = F(PXLBLCK_DEVICE_NAME);
  addLog(LOG_LEVEL_DEBUG, log);
  log = F("   -Fill matrix: Color: ");
  log += color;
  addLog(LOG_LEVEL_DEBUG, log);

  if (PXLBLCK_HIGHER_COLOR_RESOLUTION_ENABLED)
    PXLBLCK_INSTANCE->setPassThruColor(color);
  PXLBLCK_INSTANCE->fillScreen(color);
}

void pxlBlckUtils_clear_matrix()
{
  pxlBlckUtils_fill_matrix(0);
}

void pxlBlckUtils_draw_pixel(uint8_t x, uint8_t y, uint32_t color)
{
  if (PXLBLCK_HIGHER_COLOR_RESOLUTION_ENABLED)
    PXLBLCK_INSTANCE->setPassThruColor(color);
  PXLBLCK_INSTANCE->drawPixel(x, y, color);
}

void pxlBlckUtils_draw_horizontal_bar(uint8_t y, uint32_t color)
{
  pxlBlckUtils_draw_horizontal_bar_no_update(y, color);
  pxlBlckUtils_update_matrix();
}

void pxlBlckUtils_draw_horizontal_bar_no_update(uint8_t y, uint32_t color)
{
  if (PXLBLCK_HIGHER_COLOR_RESOLUTION_ENABLED)
    PXLBLCK_INSTANCE->setPassThruColor(color);

  for (uint16_t i = 0; i < PXLBLCK_MATRIX_WIDTH; i++)
  {
    //pxlBlckUtils_draw_pixel(0, i, pxlBlckUtils_convert_color_values_to_32bit(anim_red_on, anim_green_on, anim_blue_on));
    PXLBLCK_INSTANCE->drawPixel(i, y, color);
    //delay(10);
  }
}

// == Matrix helper functions == end ===============================================================================================================

/*
  void pxlBlckUtils_save_color_values_to_runtime_variables(uint8_t r, uint8_t g, uint8_t b)
  {
  //this function is used by the wordclock matrix plugin
  float brightness = (float)Plugin_205_displayBrightness / PXLBLCK_MAX_SETABLE_BRIGHTNESS;
  if (!PXLBLCK_HIGHER_COLOR_RESOLUTION_ENABLED)
  {
    //runtime color-variable gets the 16bit value
    PXLBLCK_DISPLAY_COLOR = PXLBLCK_INSTANCE->Color(brightness * r, brightness * g, brightness * b);

              Plugin_205_colorOne = PXLBLCK_INSTANCE->Color(brightness * r, brightness * g, brightness * b);
  } else
  {
    PXLBLCK_DISPLAY_COLOR = pxlBlckUtils_convert_color_values_to_32bit(brightness * r, brightness * g, brightness * b);
  }

  String log = F(PXLBLCK_DEVICE_NAME);
  addLog(LOG_LEVEL_INFO, log);
  log = F("   -PXLBLCK_DISPLAY_COLOR: ");
  log += PXLBLCK_DISPLAY_COLOR;
  addLog(LOG_LEVEL_INFO, log);
  }
*/

/*
  void pxlBlckUtils_save_color_values_to_runtime_variables(
  uint8_t hr_r, uint8_t hr_g, uint8_t hr_b,
  uint8_t mn_r, uint8_t mn_g, uint8_t mn_b,
  uint8_t bg_r, uint8_t bg_g, uint8_t bg_b)
  {
  pxlBlckUtils_save_color_values_to_runtime_variables(
    hr_r,  hr_g,  hr_b,
    mn_r,  mn_g,  mn_b,
    bg_r,  bg_g,  bg_b,
    0,  0,  0);
  }
*/

void pxlBlckUtils_save_color_values_to_runtime_variables(
  uint8_t colorOneR, uint8_t colorOneG, uint8_t colorOneB,
  uint8_t colorTwoR, uint8_t colorTwoG, uint8_t colorTwoB,
  uint8_t colorThreeR, uint8_t colorThreeG, uint8_t colorThreeB,
  uint8_t colorFourR, uint8_t colorFourG, uint8_t colorFourB)
{

  float brightness = 0;

  if (Plugin_205_displayBrightness > 0)
  {
    brightness = (float)Plugin_205_displayBrightness / PXLBLCK_MAX_SETABLE_BRIGHTNESS;

  } else //pxlBlckUtils_displayBrightness is set to zero so we will use pxlBlckUtils_minimalBrightness-value instead
  {
    brightness = (float)PXLBLCK_MINIMAL_BRIGHTNESS / 255.0;
  }

  if (!PXLBLCK_HIGHER_COLOR_RESOLUTION_ENABLED)
  {
    Plugin_205_colorOne = PXLBLCK_INSTANCE->Color(brightness * colorOneR, brightness * colorOneG, brightness * colorOneB);
    Plugin_205_colorTwo = PXLBLCK_INSTANCE->Color(brightness * colorTwoR, brightness * colorTwoG, brightness * colorTwoB);
    Plugin_205_colorThree = PXLBLCK_INSTANCE->Color(brightness * colorThreeR, brightness * colorThreeG, brightness * colorThreeB);
    Plugin_205_colorFour = PXLBLCK_INSTANCE->Color(brightness * colorFourR, brightness * colorFourG, brightness * colorFourB);

  } else
  {
    Plugin_205_colorOne = pxlBlckUtils_convert_color_values_to_32bit(brightness * colorOneR, brightness * colorOneG, brightness * colorOneB);
    Plugin_205_colorTwo = pxlBlckUtils_convert_color_values_to_32bit(brightness * colorTwoR, brightness * colorTwoG, brightness * colorTwoB);
    Plugin_205_colorThree = pxlBlckUtils_convert_color_values_to_32bit(brightness * colorThreeR, brightness * colorThreeG, brightness * colorThreeB);
    Plugin_205_colorFour = pxlBlckUtils_convert_color_values_to_32bit(brightness * colorFourR, brightness * colorFourG, brightness * colorFourB);
  }

  /*
    if (!PXLBLCK_HIGHER_COLOR_RESOLUTION_ENABLED)
    {
      //runtime color-variable gets the 16bit value
      Plugin_205_colorOne = PXLBLCK_INSTANCE->Color(brightness * hr_r, brightness * hr_g, brightness * hr_b);
      Plugin_205_colorTwo = PXLBLCK_INSTANCE->Color(brightness * mn_r, brightness * mn_g, brightness * mn_b);

    #ifdef Plugin_205_colorThree
      Plugin_205_colorThree = PXLBLCK_INSTANCE->Color(brightness * hr_bg_r, brightness * hr_bg_g, brightness * hr_bg_b);
    #endif

    #ifdef PXLBLCK_HOUR_BG_COLOR
      PXLBLCK_HOUR_BG_COLOR = PXLBLCK_INSTANCE->Color(brightness * hr_bg_r, brightness * hr_bg_g, brightness * hr_bg_b);
    #endif

    #ifdef PXLBLCK_MINUTE_BG_COLOR
      PXLBLCK_MINUTE_BG_COLOR = PXLBLCK_INSTANCE->Color(brightness * mn_bg_r, brightness * mn_bg_g, brightness * mn_bg_b);
    #endif

    } else
    {
      Plugin_205_colorOne = pxlBlckUtils_convert_color_values_to_32bit(brightness * hr_r, brightness * hr_g, brightness * hr_b);
      Plugin_205_colorTwo = pxlBlckUtils_convert_color_values_to_32bit(brightness * mn_r, brightness * mn_g, brightness * mn_b);

    #ifdef Plugin_205_colorThree
      Plugin_205_colorThree = pxlBlckUtils_convert_color_values_to_32bit(brightness * hr_bg_r, brightness * hr_bg_g, brightness * hr_bg_b);
    #endif

    #ifdef PXLBLCK_HOUR_BG_COLOR
      PXLBLCK_HOUR_BG_COLOR = pxlBlckUtils_convert_color_values_to_32bit(brightness * hr_bg_r, brightness * hr_bg_g, brightness * hr_bg_b);
    #endif

    #ifdef PXLBLCK_MINUTE_BG_COLOR
      PXLBLCK_MINUTE_BG_COLOR = pxlBlckUtils_convert_color_values_to_32bit(brightness * mn_bg_r, brightness * mn_bg_g, brightness * mn_bg_b);
    #endif
    }
  */
}




/*
  //this function is used by the 10x10 matrix plugin
  //runtime color-variable gets the 16bit value
  Plugin_205_colorOne = PXLBLCK_INSTANCE->Color(((float)Plugin_205_displayBrightness / PXLBLCK_MAX_SETABLE_BRIGHTNESS) * hr_r, ((float)Plugin_205_displayBrightness / PXLBLCK_MAX_SETABLE_BRIGHTNESS) * hr_g, ((float)Plugin_205_displayBrightness / PXLBLCK_MAX_SETABLE_BRIGHTNESS) * hr_b);
  Plugin_205_colorTwo = PXLBLCK_INSTANCE->Color(((float)Plugin_205_displayBrightness / PXLBLCK_MAX_SETABLE_BRIGHTNESS) * mn_r, ((float)Plugin_205_displayBrightness / PXLBLCK_MAX_SETABLE_BRIGHTNESS) * mn_g, ((float)Plugin_205_displayBrightness / PXLBLCK_MAX_SETABLE_BRIGHTNESS) * mn_b);
  Plugin_205_colorThree = PXLBLCK_INSTANCE->Color(((float)Plugin_205_displayBrightness / PXLBLCK_MAX_SETABLE_BRIGHTNESS) * hr_bg_r, ((float)Plugin_205_displayBrightness / PXLBLCK_MAX_SETABLE_BRIGHTNESS) * hr_bg_g, ((float)Plugin_205_displayBrightness / PXLBLCK_MAX_SETABLE_BRIGHTNESS) * hr_bg_b);

  #ifdef PXLBLCK_DEBUG_OUTPUT
  String log = F(PXLBLCK_DEVICE_NAME);
  addLog(LOG_LEVEL_INFO, log);
  log = F("   -Plugin_205_colorOne: ");
  log += Plugin_205_colorOne;
  addLog(LOG_LEVEL_INFO, log);
  log = F("   -Plugin_205_colorTwo: ");
  log += Plugin_205_colorTwo;
  addLog(LOG_LEVEL_INFO, log);
  log = F("   -Plugin_205_colorThree: ");
  log += Plugin_205_colorThree;
  addLog(LOG_LEVEL_INFO, log);
  #endif
*/

/*
  void pxlBlckUtils_save_color_values_to_runtime_variables(
  uint8_t hr_r, uint8_t hr_g, uint8_t hr_b,
  uint8_t mn_r, uint8_t mn_g, uint8_t mn_b,
  uint8_t hr_bg_r, uint8_t hr_bg_g, uint8_t hr_bg_b,
  uint8_t mn_bg_r, uint8_t mn_bg_g, uint8_t mn_bg_b)
  {
  float brightness = 0;
  if (pxlBlckUtils_displayBrightness > 0)
  {
    brightness = (float)pxlBlckUtils_displayBrightness / PLUGIN_203_MAX_SETABLE_BRIGHTNESS;

  } else //pxlBlckUtils_displayBrightness is set to zero so we will use pxlBlckUtils_minimalBrightness-value instead
  {
    brightness = (float)pxlBlckUtils_minimalBrightness / 255.0;
  }
  if (!pxlBlckUtils_higherColorResultionEnabled)
  {
    //runtime color-variable gets the 16bit value
    pxlBlckUtils_hourColor = pxlBlckUtils_matrix->Color(brightness * hr_r, brightness * hr_g, brightness * hr_b);
    pxlBlckUtils_minuteColor = pxlBlckUtils_matrix->Color(brightness * mn_r, brightness * mn_g, brightness * mn_b);
    pxlBlckUtils_hourBgColor = pxlBlckUtils_matrix->Color(brightness * hr_bg_r, brightness * hr_bg_g, brightness * hr_bg_b);
    pxlBlckUtils_minuteBgColor = pxlBlckUtils_matrix->Color(brightness * mn_bg_r, brightness * mn_bg_g, brightness * mn_bg_b);
  } else
  {
    pxlBlckUtils_hourColor = pxlBlckUtils_return_high_res_color(brightness * hr_r, brightness * hr_g, brightness * hr_b);
    pxlBlckUtils_minuteColor = pxlBlckUtils_return_high_res_color(brightness * mn_r, brightness * mn_g, brightness * mn_b);
    pxlBlckUtils_hourBgColor = pxlBlckUtils_return_high_res_color(brightness * hr_bg_r, brightness * hr_bg_g, brightness * hr_bg_b);
    pxlBlckUtils_minuteBgColor = pxlBlckUtils_return_high_res_color(brightness * mn_bg_r, brightness * mn_bg_g, brightness * mn_bg_b);
  }
  }

*/



void pxlBlckUtils_update_user_vars(struct EventStruct * event, boolean enabled, uint32_t color, uint8_t brightness)
{
  UserVar[event->BaseVarIndex] = enabled;
  UserVar[event->BaseVarIndex + 1] = color;
  UserVar[event->BaseVarIndex + 2] = brightness;
}

// == Spiffs-Icon Stuff == start ===============================================================================================================

boolean pxlBlckUtils_check_if_icon_file_exists(String desiredFile)
{
  boolean iconFound = false;

#if defined(ESP8266)
  fs::Dir dir = SPIFFS.openDir("");
  while (dir.next())
  {
    String fileName = dir.fileName();
    // String filetype = fileName.substring(fileName.indexOf("."));
    // filetype.toLowerCase();

    if (fileName.equals(desiredFile))
    {
      fs::File f = dir.openFile("r");

      String log = F(PXLBLCK_DEVICE_NAME);
      addLog(LOG_LEVEL_INFO, log);
      log = F("   -Icon-file was found: ");
      log += fileName;
      log += F("(");
      log += f.size();
      log += F(" bytes)");
      addLog(LOG_LEVEL_INFO, log);

      iconFound = true;
      break;
    }
  }
#endif
#if defined(ESP32)
  File root = SPIFFS.open("/");
  File file = root.openNextFile();
  while (file)
  {
    String fileName = file.name();
    //  String filetype = fileName.substring(fileName.indexOf("."));
    // filetype.toLowerCase();

    if (fileName.equals(desiredFile))
    {
      //fs::File f = dir.openFile("r");

      String log = F(PXLBLCK_DEVICE_NAME);
      addLog(LOG_LEVEL_INFO, log);
      log = F("   -Icon-file was found: ");
      log += fileName;
      log += F("(");
      log += file.size();
      log += F(" bytes)");
      addLog(LOG_LEVEL_INFO, log);

      iconFound = true;
      break;
    }
    file = root.openNextFile();
  }
#endif

  return iconFound;
}

boolean pxlBlckUtils_load_ppm_file_to_dynamic_array(String fileName)
{
  String fileContent = pxlBlckUtils_read_file(fileName);
  if (fileContent == "%ERROR%")
  {
    String log = F(PXLBLCK_DEVICE_NAME);
    addLog(LOG_LEVEL_INFO, log);
    log += F("   -Error: Failed to load icon-file");
    addLog(LOG_LEVEL_INFO, log);

    return false;
  }

  //Count lines(linebreaks) in file
  uint16_t lineCount = 0;
  for (uint16_t i = 0; fileContent[i]; i++)
  {
    if (fileContent[i] == '\n')
      lineCount++;
  }

  //Read header and parse rest of file
  boolean fileTypeApproved = false;
  boolean widthAndHeightFound = false;
  boolean maxBrightnessFound = false;
  boolean dataValidated = false;
  uint8_t width, height, maxBrightness = 0;

  uint16_t lastFound = 0;
  uint16_t dataPointer = 0;
  uint8_t pixelPointer = 0;
  String actualLine = "";

  for (uint16_t i = 0; i < lineCount; i++)
  {
    //itterate thorugh lines of file to analyze contents
    yield();
    uint16_t actualFound = fileContent.indexOf("\n", lastFound);
    actualLine = fileContent.substring(lastFound, actualFound);
    lastFound = actualFound + 1;

    if (actualLine[0] == '#' )
    {
      //ignore comments in file
      continue;
    }

    if (!fileTypeApproved)
    {
      //First thing we have to check for is the correct Filetype: This should be "P3" or "p3"
      actualLine.trim();
      actualLine.toLowerCase();
      fileTypeApproved = (actualLine == "p3");
      if (fileTypeApproved)
      {
        continue; //we finish the actual iteration and jump to the next line
      } else
      {
        String log = F(PXLBLCK_DEVICE_NAME);
        addLog(LOG_LEVEL_INFO, log);
        log += F("   -Error: Icon-file content is not p3");
        addLog(LOG_LEVEL_INFO, log);
        break;
      }
    }

    if (fileTypeApproved && !widthAndHeightFound && !maxBrightnessFound)
    {
      //Second thing we have to check after we proved the correct fileType: Search for Width and Height which should be in the next line (after the fileType) in a .ppm file
      uint8_t blankPosition = actualLine.indexOf(" "); //The line that contains width and height is also the only line that contains a space because this is used to seperate width and height from each other
      if (blankPosition > 0)
      {
        //yeeeha we found a space so we found the line that contains width and height
        width = actualLine.substring(0, blankPosition).toInt();
        height = actualLine.substring(blankPosition + 1).toInt();

        widthAndHeightFound = width > 0 && width <= PXLBLCK_MATRIX_WIDTH && height > 0 && height <= PXLBLCK_MATRIX_HEIGHT; //integer conversion succeded and image fits in matrix

        if (widthAndHeightFound)
        {
          continue;
        } else
        {
          String log = F(PXLBLCK_DEVICE_NAME);
          addLog(LOG_LEVEL_INFO, log);
          log += F("   -Error: Icon-file content doesn't fit in display");
          addLog(LOG_LEVEL_INFO, log);
          break;
        }
      }
    }

    if (fileTypeApproved && widthAndHeightFound && !maxBrightnessFound)
    {
      //Third thing to check(after fileType and dimensions are approved): Search for maxBrightness. This value is stored right after the line of width and height
      actualLine.trim();
      maxBrightnessFound = actualLine.toInt() > 0 && actualLine.toInt() <= 255;
      if (maxBrightnessFound)
      {
        maxBrightness = actualLine.toInt();
        continue;
      } else
      {
        String log = F(PXLBLCK_DEVICE_NAME);
        addLog(LOG_LEVEL_INFO, log);
        log += F("   -Error: Icon-file max-brightness-value is missing");
        addLog(LOG_LEVEL_INFO, log);
        break;
      }
    }

    if (fileTypeApproved && widthAndHeightFound && maxBrightnessFound)
    {
      //We checked the whole header and are ready to go to read the color values
      actualLine.trim();
      if (actualLine.toInt() >= 0 && actualLine.toInt() <= maxBrightness)
      {
        //pixelValuesGlobal[iconCounter][dataPointer / 3][pixelPointer] = actualLine.toInt();
        //        PXLBLCK_ICON_STRUCT.logo[pixelPointer][dataPointer / PXLBLCK_MATRIX_WIDTH][dataPointer % PXLBLCK_MATRIX_WIDTH] = actualLine.toInt(); //(dataPointer / PXLBLCK_MATRIX_WIDTH)==ROW; (dataPointer % PXLBLCK_MATRIX_WIDTH)==COLUMN
        PXLBLCK_ICON_STRUCT.logo[pixelPointer][dataPointer / width][dataPointer % width] = actualLine.toInt(); //(dataPointer / PXLBLCK_MATRIX_WIDTH)==ROW; (dataPointer % PXLBLCK_MATRIX_WIDTH)==COLUMN

        if (pixelPointer < 2)
        {
          pixelPointer++;
        } else
        {
          pixelPointer = 0;
          dataPointer++; //we filled all color-values of the actual pixel so lets go to the next pixel
        }
      } else
      {
        //each .ppm file contains a value(maxBrightness)that represents the max possible brightness. Each pixel-Value must be lower/equal to this value.
        String log = F(PXLBLCK_DEVICE_NAME);
        addLog(LOG_LEVEL_INFO, log);
        log += F("   -Error: Icon-file color value doesn't fit to max-brightness-value");
        addLog(LOG_LEVEL_INFO, log);
        break;
      }
    }
  }
  dataValidated = true;

  return (fileTypeApproved && widthAndHeightFound && maxBrightnessFound && dataValidated);
}

String pxlBlckUtils_read_file(String name)
{
  //read file from SPIFFS and store it as a String variable
  String contents;
  fs::File file = SPIFFS.open(name.c_str(), "r");
  if (!file)
  {
    String errorMessage = "Can't open '" + name + "' !\r\n";
    Serial.println(errorMessage);
    return "%ERROR%";
  }
  else
  {
    // this is going to get the number of bytes in the file and give us the value in an integer
    uint16_t fileSize = file.size();
    uint16_t chunkSize = 128;
    //This is a character array to store a chunk of the file.
    //We'll store 1024 characters at a time
    char buf[chunkSize];
    uint16_t numberOfChunks = (fileSize / chunkSize) + 1;

    uint16_t remainingBytes = fileSize;
    for (int i = 1; i <= numberOfChunks; i++)
    {
      memset(buf, 0, chunkSize); //oder chunkSize anstatt sizeof buf
      if (remainingBytes - chunkSize < 0)
      {
        chunkSize = remainingBytes + chunkSize; //"+chunksize" to compensate the "-1" in "file.read((uint8_t *)buf, chunkSize - 1);"
      }
      file.read((uint8_t *)buf, chunkSize - 1);
      remainingBytes -= chunkSize;
      contents += String(buf);
    }
    file.close();
    return contents;
  }
}

// == Spiffs-Icon Stuff == end ===============================================================================================================

// == Start-animation == start ===============================================================================================================

void pxlBlckUtils_show_start_animation(uint16_t delay_time)
{
  /*
    pxlBlckUtils_clear_matrix();
    for (int xAndY = 0; xAndY < (PXLBLCK_MATRIX_HEIGHT / 2); xAndY++)
    {
    uint8_t wheelPosition = ((float)xAndY / (float)(PXLBLCK_MATRIX_HEIGHT / 2)) * 255;
    PXLBLCK_INSTANCE->drawRect(xAndY, xAndY, (PXLBLCK_MATRIX_HEIGHT - (xAndY * 2)), (PXLBLCK_MATRIX_HEIGHT - (xAndY * 2)), pxlBlckUtils_color_wheel(wheelPosition));
    pxlBlckUtils_update_matrix();
    delay(1000);
    }
  */

  /*
    pxlBlckUtils_clear_matrix();
    uint8_t animationProgress = 0;
    for (int xAndY = 0; xAndY < PXLBLCK_MATRIX_HEIGHT; xAndY++)
    {
      animationProgress++;
      uint8_t wheelPosition = ((float)animationProgress / 32.0) * 255; //32.0 because this is 4*PXLBLCK_MATRIX_HEIGHT
      PXLBLCK_INSTANCE->drawLine(xAndY, 0, xAndY, PXLBLCK_MATRIX_HEIGHT, pxlBlckUtils_color_wheel(wheelPosition));
      pxlBlckUtils_update_matrix();
      delay(40);
    }
    for (int xAndY = 0; xAndY < PXLBLCK_MATRIX_HEIGHT; xAndY++)
    {
      animationProgress++;
      uint8_t wheelPosition = ((float)animationProgress / 32.0) * 255; //32.0 because this is 4*PXLBLCK_MATRIX_HEIGHT
      PXLBLCK_INSTANCE->drawLine(0, xAndY, PXLBLCK_MATRIX_HEIGHT, xAndY, pxlBlckUtils_color_wheel(wheelPosition));
      pxlBlckUtils_update_matrix();
      delay(40);
    }
    for (int xAndY = (PXLBLCK_MATRIX_HEIGHT-1); xAndY >= 0; xAndY--)
    {
      animationProgress++;
      uint8_t wheelPosition = ((float)animationProgress / 32.0) * 255; //32.0 because this is 4*PXLBLCK_MATRIX_HEIGHT
      PXLBLCK_INSTANCE->drawLine(xAndY, 0, xAndY, PXLBLCK_MATRIX_HEIGHT, pxlBlckUtils_color_wheel(wheelPosition));
      pxlBlckUtils_update_matrix();
      delay(40);
    }
    for (int xAndY = (PXLBLCK_MATRIX_HEIGHT-1); xAndY >= 0; xAndY--)
    {
      animationProgress++;
      uint8_t wheelPosition = ((float)animationProgress / 32.0) * 255; //32.0 because this is 4*PXLBLCK_MATRIX_HEIGHT
      PXLBLCK_INSTANCE->drawLine(0, xAndY, PXLBLCK_MATRIX_HEIGHT, xAndY, pxlBlckUtils_color_wheel(wheelPosition));
      pxlBlckUtils_update_matrix();
      delay(40);
    }*/

  /*

    pxlBlckUtils_clear_matrix();
    uint8_t animationProgress = 0;
    uint8_t lineCounter = 0;
    for (int xAndY = 0; xAndY < PXLBLCK_MATRIX_HEIGHT; xAndY++)
    {
      uint8_t wheelPosition = ((float)animationProgress / 64.0) * 255; //32.0 because this is 4*PXLBLCK_MATRIX_HEIGHT
      //PXLBLCK_INSTANCE->drawLine(xAndY, 0, xAndY, PXLBLCK_MATRIX_HEIGHT, pxlBlckUtils_color_wheel(wheelPosition));
      while (true)
      {

        pxlBlckUtils_draw_pixel(xAndY, lineCounter, pxlBlckUtils_color_wheel(wheelPosition));
        pxlBlckUtils_update_matrix();

        if ((xAndY % 2) == 0)
        {
          if (lineCounter < (PXLBLCK_MATRIX_HEIGHT - 1))
            lineCounter++;
          else
            break;
        } else
        {
          if (lineCounter > 0)
            lineCounter--;
          else
            break;
        }


        animationProgress++;
      delay(20);

      }
    }
      delay(2000);

  */
  /*
          pxlBlckUtils_clear_matrix();
          pxlBlckUtils_update_matrix();
    uint8_t animationProgress = 0;
    uint8_t coordinates[16][2] = {{0, 0}, {6, 6}, {0, 6}, {6, 0}, {2, 2},  {4, 2},    {2, 0}, {6, 4}, {2, 6}, {4, 0},  {4, 4}, {6, 2},   {0, 4},   {2, 4},    {4, 6}, {0, 2}  };

    for (int nr = 0; nr < 16; nr++)
    {
    uint8_t wheelPosition = ((float)animationProgress / 16.0) * 255;
    PXLBLCK_INSTANCE->drawRect(coordinates[nr][0], coordinates[nr][1], 2, 2, pxlBlckUtils_color_wheel(wheelPosition));
    pxlBlckUtils_update_matrix();
    animationProgress++;
    delay(delay_time);
    }
  */
  /*
    pxlBlckUtils_clear_matrix();
    pxlBlckUtils_update_matrix();
    uint8_t animationProgress = 0;

    for (int nr = 0; nr < 100; nr++)
    {
      uint8_t wheelPosition = ((float)animationProgress / 100) * 255;
      pxlBlckUtils_draw_pixel(random(0, 8), random(0, 8), pxlBlckUtils_color_wheel(random(0, 255)));
      pxlBlckUtils_update_matrix();
      animationProgress++;
      delay(delay_time);
    }

    pxlBlckUtils_clear_matrix();
    for (int xAndY = 0; xAndY < PXLBLCK_MATRIX_HEIGHT; xAndY++)
    {
    uint8_t wheelPosition = ((float)xAndY / (float)PXLBLCK_MATRIX_HEIGHT) * 255;
    for (float brghtns = 0.0; brghtns < 1.0; brghtns += 0.1)
    {
      PXLBLCK_INSTANCE->drawLine(0, xAndY, PXLBLCK_MATRIX_WIDTH - 1, xAndY, pxlBlckUtils_color_wheel(wheelPosition, brghtns));
      pxlBlckUtils_update_matrix();
      delay(delay_time);
    }
    }
    for (int xAndY = (PXLBLCK_MATRIX_HEIGHT - 1); xAndY >= -1; xAndY--)
    {
    uint8_t wheelPosition = ((float)xAndY / (float)PXLBLCK_MATRIX_HEIGHT) * 255;
    for (float brghtns = 1.0; brghtns > 0.0; brghtns -= 0.1)
    {
      PXLBLCK_INSTANCE->drawLine(0, xAndY, PXLBLCK_MATRIX_WIDTH - 1, xAndY, pxlBlckUtils_color_wheel(wheelPosition, brghtns));
      pxlBlckUtils_update_matrix();
      delay(delay_time);
    }
    PXLBLCK_INSTANCE->drawLine(0, xAndY, PXLBLCK_MATRIX_WIDTH - 1, xAndY, 0);
    }
  */


  pxlBlckUtils_clear_matrix();
  for (int xAndY = 0; xAndY < PXLBLCK_MATRIX_HEIGHT; xAndY++)
  {
    uint8_t wheelPosition = ((float)xAndY / (float)PXLBLCK_MATRIX_HEIGHT) * 255;
    for (float brghtns = 0.0; brghtns < 1.0000; brghtns += 0.1)
    {
      if (PXLBLCK_HIGHER_COLOR_RESOLUTION_ENABLED)
        PXLBLCK_INSTANCE->setPassThruColor(pxlBlckUtils_color_wheel(wheelPosition, brghtns));

      PXLBLCK_INSTANCE->drawLine(0, xAndY, PXLBLCK_MATRIX_WIDTH - 1, xAndY, pxlBlckUtils_color_wheel(wheelPosition, brghtns));
      pxlBlckUtils_update_matrix();
      delay(PXLBLCK_MATRIX_WIDTH * delay_time);
    }
  }
  for (int xAndY = (PXLBLCK_MATRIX_HEIGHT - 1); xAndY >= -1; xAndY--)
  {
    uint8_t wheelPosition = ((float)xAndY / (float)PXLBLCK_MATRIX_HEIGHT) * 255;
    for (float brghtns = 1.0; brghtns >= -0.0001; brghtns -= 0.1)
    {
      if (PXLBLCK_HIGHER_COLOR_RESOLUTION_ENABLED)
        PXLBLCK_INSTANCE->setPassThruColor(pxlBlckUtils_color_wheel(wheelPosition, brghtns));

      PXLBLCK_INSTANCE->drawLine(0, xAndY, PXLBLCK_MATRIX_WIDTH - 1, xAndY, pxlBlckUtils_color_wheel(wheelPosition, brghtns));
      pxlBlckUtils_update_matrix();
      delay(delay_time);
    }
  }
}


// == Start-animation == end ===============================================================================================================

// == fakeTV == Start ===============================================================================================================
//fakeTV-color-data used from: https://learn.adafruit.com/fake-tv-light-for-engineers?view=all

void pxlBlckUtils_switch_fakeTV_onOff(boolean state)
{
  PXLBLCK_FAKE_TV_STRUCT.running = state;
  PXLBLCK_FAKE_TV_STRUCT.frameNumber = (sizeof(pxlBlckUtils_fakeTVcolors) / sizeof(pxlBlckUtils_fakeTVcolors[0]));
  PXLBLCK_FAKE_TV_STRUCT.framePosition = random(PXLBLCK_FAKE_TV_STRUCT.frameNumber);
}

void pxlBlckUtils_check_fakeTV()
{
  //the whole fakeTV functionality is mainly based on the code you can find here: https://learn.adafruit.com/fake-tv-light-for-engineers?view=all
  /*
    struct PXLBLCK_FAKE_TV_STRUCTStruct
    {
    pxlBlckUtils_iconStruct() :
    running(false), executionInterval(0), lastExecution(0), r(0), g(0), b(100), frameNumber(0), framePosition(0) {}
    boolean running = false;
    uint32_t executionInterval = 0;
    uint32_t lastExecution = 0;
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    uint32_t frameNumber = 0;
    uint32_t framePosition = 0;
    } PXLBLCK_FAKE_TV_STRUCT;
  */
  //if (PXLBLCK_FAKE_TV_STRUCT.running)
  if (Plugin_205_selectedDial == PXLBLCK_DIAL_NAME_TV_SIMULATOR_ID_INT)
  {
    if (pxlBlckUtils_execute_if_interval_passed(&PXLBLCK_FAKE_TV_STRUCT.lastExecution, PXLBLCK_FAKE_TV_STRUCT.executionInterval))
    {
      if (++PXLBLCK_FAKE_TV_STRUCT.framePosition >= PXLBLCK_FAKE_TV_STRUCT.frameNumber)
        PXLBLCK_FAKE_TV_STRUCT.framePosition = 0;

      PXLBLCK_FAKE_TV_STRUCT.executionInterval = random(100, 2500);

      uint8_t highColorByte = pgm_read_byte(&pxlBlckUtils_fakeTVcolors[PXLBLCK_FAKE_TV_STRUCT.framePosition * 2]);
      uint8_t lowColorByte = pgm_read_byte(&pxlBlckUtils_fakeTVcolors[PXLBLCK_FAKE_TV_STRUCT.framePosition * 2 + 1]);

      // Expand to 24-bit (8/8/8)
      uint8_t r8 = (highColorByte & 0xF8) | (highColorByte >> 5);
      uint8_t g8 = (highColorByte << 5) | ((lowColorByte & 0xE0) >> 3) | ((highColorByte & 0x06) >> 1);
      uint8_t b8 = (lowColorByte << 3) | ((lowColorByte & 0x1F) >> 2);

      pxlBlckUtils_fill_matrix(r8, g8, b8);
      pxlBlckUtils_update_matrix();

      String log = F(PXLBLCK_DEVICE_NAME);
      addLog(LOG_LEVEL_DEBUG, log);
      log = F("   -PXLBLCK_FAKE_TV_STRUCT.framePosition: ");
      log += PXLBLCK_FAKE_TV_STRUCT.framePosition;
      addLog(LOG_LEVEL_DEBUG, log);
      log = F("   -PXLBLCK_FAKE_TV_STRUCT.frameNumber: ");
      log += PXLBLCK_FAKE_TV_STRUCT.frameNumber;
      addLog(LOG_LEVEL_DEBUG, log);
      log = F("   -PXLBLCK_FAKE_TV_STRUCT.executionInterval: ");
      log += PXLBLCK_FAKE_TV_STRUCT.executionInterval;
      addLog(LOG_LEVEL_DEBUG, log);
      log = F("   -r8: ");
      log += r8;
      addLog(LOG_LEVEL_DEBUG, log);
      log = F("   -g8: ");
      log += g8;
      addLog(LOG_LEVEL_DEBUG, log);
      log = F("   -b8: ");
      log += b8;
      addLog(LOG_LEVEL_DEBUG, log);

    }
  }
}

// == fakeTV == End ===============================================================================================================

uint32_t pxlBlckUtils_add_brightness_to_color(uint8_t brightness, uint8_t minimalBrightness, uint32_t color)
{
  float brightnessFactor = 0;
  if (brightness >= 1)
    brightnessFactor = (float)brightness / PXLBLCK_MAX_SETABLE_BRIGHTNESS;
  else //Plugin_203_displayBrightness is set to zero so we will use Plugin_203_minimalBrightness-value instead
    brightnessFactor = (float)minimalBrightness / 255.0;

  uint8_t red = pxlBlckUtils_return_red_from_config(color) * brightnessFactor;
  uint8_t green = pxlBlckUtils_return_green_from_config(color) * brightnessFactor;
  uint8_t blue = pxlBlckUtils_return_blue_from_config(color) * brightnessFactor;

  uint8_t warmWhite = 0;
  if (PXLBLCK_LED_COLOR_ORDER == NEO_RGBW)
  {
    warmWhite = pxlBlckUtils_return_warmwhite_from_config(color) * brightnessFactor;
  }

  return pxlBlckUtils_return_correct_color_value(red, green, blue, warmWhite);

}

uint32_t pxlBlckUtils_return_correct_color_value(uint8_t red, uint8_t green, uint8_t blue)
{
  return pxlBlckUtils_return_correct_color_value(red, green, blue, 0);
}

uint32_t pxlBlckUtils_return_correct_color_value(uint8_t red, uint8_t green, uint8_t blue, uint8_t warmWhite)
{
  if (!PXLBLCK_HIGHER_COLOR_RESOLUTION_ENABLED)
  {
    if (PXLBLCK_LED_COLOR_ORDER == NEO_RGBW)
    {
      return pxlBlckUtils_convert_color_values_to_32bit(red, green, blue, warmWhite);
    } else
    {
      return PXLBLCK_INSTANCE->Color(red, green, blue);
    }
  } else
  {
    if (PXLBLCK_LED_COLOR_ORDER == NEO_RGBW)
    {
      return pxlBlckUtils_convert_color_values_to_32bit(red, green, blue, warmWhite);
    } else
    {
      return pxlBlckUtils_convert_color_values_to_32bit(red, green, blue);
    }
  }
}

uint32_t pxlBlckUtils_convert_color_values_to_32bit(uint8_t r, uint8_t g, uint8_t b)
{
  //Here we convert the single rgb values to one 24bit value like it is done with the neopixel color information. In fact the neomatrix color information only needs 16 bit.
  //Problem: It is not possible to convert back from the 16bit value to every single rgb value because the conversion 24->16bit brings an accuracy loss.
  //Solution: color value is stored in the confguration as a 24bit value and only converted to a 16bit value for the runtime variable

  //Magic conversion of single rgb values to 24bit color value
  return (uint32_t)(((uint32_t)r << 16) | ((uint32_t)g << 8) | b);
}

uint32_t pxlBlckUtils_convert_color_values_to_32bit(uint8_t r, uint8_t g, uint8_t b, uint8_t ww)
{
  //Here we convert the single rgb values to one 24bit value like it is done with the neopixel color information. In fact the neomatrix color information only needs 16 bit.
  //Problem: It is not possible to convert back from the 16bit value to every single rgb value because the conversion 24->16bit brings an accuracy loss.
  //Solution: color value is stored in the confguration as a 24bit value and only converted to a 16bit value for the runtime variable

  //Magic conversion of single rgb values to 24bit color value
  return (((uint32_t)ww << 24) | ((uint32_t)r << 16) | ((uint32_t)g << 8) | b);
}

uint8_t pxlBlckUtils_return_red_from_config(uint32_t encodedColor)
{
  return (uint8_t)(encodedColor >> 16);
}

uint8_t pxlBlckUtils_return_green_from_config(uint32_t encodedColor)
{
  return (uint8_t)(encodedColor >> 8);
}

uint8_t pxlBlckUtils_return_blue_from_config(uint32_t encodedColor)
{
  return (uint8_t)(encodedColor >> 0);
}

uint8_t pxlBlckUtils_return_warmwhite_from_config(uint32_t encodedColor)
{
  return (uint8_t)(encodedColor >> 24);
}

// == Misc functions == start ===============================================================================================================

void pxlBlckUtils_addFormSubHeaderCaution(const String & header)
{
  String str = F("<TR><TD bgcolor='#FF0000' colspan='2'><h4>Note: ");
  str += header;
  str += F("</h4>");
  addHtml(str);
}

void pxlBlckUtils_addHelpButton(const String & label, const String & url)
{
  String completeUrl = "http://www.nerdiy.de/" + url;
  addRowLabel(label);
  addHtmlLink(F("button help"), completeUrl, F("&#10068;"));
}

void pxlBlckUtils_addColorPicker(const String & label, const String & id, String selectedColor)
{
  addRowLabel(label);
  pxlBlckUtils_addColorPicker(id, selectedColor);
}

void pxlBlckUtils_addColorPicker(const String & id, String selectedColor)
{
  String str = F("<input type=\"color\" name=\"");
  str += id;
  str += F("\" value=\"#");
  str += selectedColor;
  str += F("\">");
  addHtml(str);
}

String pxlBlckUtils_convert_32bit_to_hex_string(uint32_t inputNumber)
{
  String actualColorInHex = String(inputNumber, HEX);

  if (PXLBLCK_LED_COLOR_ORDER == NEO_RGBW || PXLBLCK_LED_COLOR_ORDER == NEO_RGB)
  {
    uint8_t r = pxlBlckUtils_return_red_from_config(inputNumber);
    uint8_t g = pxlBlckUtils_return_green_from_config(inputNumber);
    uint8_t b = pxlBlckUtils_return_blue_from_config(inputNumber);

    actualColorInHex = String(pxlBlckUtils_convert_color_values_to_32bit(g, r, b), HEX);
  }

  uint8_t numberOfMissingZeros = 6 - actualColorInHex.length();
  for (uint8_t i = 0; i < numberOfMissingZeros; i++)
  {
    actualColorInHex = "0" + actualColorInHex;
  }
  return actualColorInHex;
}


String pxlBlckUtils_getFormItemString(const String & id)
{
  return WebServer.arg(id);
}

void pxlBlckUtils_convert_hex_to_rgb(String hexString, uint8_t *r, uint8_t *g, uint8_t *b)
{
  //This code was found on https://stackoverflow.com/questions/23576827/arduino-convert-a-string-hex-ffffff-into-3-int
  // Get rid of '#' and convert it to integer
  int number = (int) strtol( &hexString[1], NULL, 16);

  // Split them up into r, g, b values
  *r = number >> 16;
  *g = number >> 8 & 0xFF;
  *b = number & 0xFF;
}

void pxlBlckUtils_select_color_values_from_color_picker_or_number_input(String hexString, uint32_t actualColorValue, uint8_t *newR, uint8_t *newG, uint8_t *newB, uint8_t inputR, uint8_t inputG, uint8_t inputB)
{
  //this function checks if the color values of the color picker was changed. If so it returns the color-picker-values as new color values. otherwise it returns the directly entered rgb values

  uint8_t colorPickerR, colorPickerG, colorPickerB = 0;
  pxlBlckUtils_convert_hex_to_rgb(hexString, &colorPickerR, &colorPickerG, &colorPickerB);

  //uint8_t colorPickerR, colorPickerG, colorPickerB = 0;
  uint8_t oldR = pxlBlckUtils_return_red_from_config(actualColorValue);
  uint8_t oldG = pxlBlckUtils_return_green_from_config(actualColorValue);
  uint8_t oldB = pxlBlckUtils_return_blue_from_config(actualColorValue);

  if (PXLBLCK_LED_COLOR_ORDER == NEO_RGBW || PXLBLCK_LED_COLOR_ORDER == NEO_RGB)
  {
    uint8_t tempColorValue = oldR;
    oldR = oldG;
    oldG = tempColorValue;
  }

  *newR = inputR;
  *newG = inputG;
  *newB = inputB;

  if (colorPickerR != oldR || colorPickerG != oldG || colorPickerB != oldB)
  {
    *newR = colorPickerR;
    *newG = colorPickerG;
    *newB = colorPickerB;
  }
}

String pxlBlckUtils_parseString(String & string, byte indexFind)
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

boolean pxlBlckUtils_execute_if_interval_passed(unsigned long * lastExecution, unsigned long interval)
{
  //This function checks if a given interval-length is passed since the last execution. The last last-execution-time is hold in *lastExecution.

  if ((millis() - *lastExecution) >= interval)
  {
    *lastExecution = millis();
    return true;
  }
  return false;
}

uint32_t pxlBlckUtils_color_wheel(uint8_t wheelPos)
{
  return pxlBlckUtils_color_wheel(wheelPos, 1.0);
}

uint32_t pxlBlckUtils_color_wheel(uint8_t wheelPos, float brightness)
{
  //Takes the input value whelPos and uses it as the degree on a colorWheel. Then uses the associated colors and returns them.
  if (wheelPos < 85)
  {
    if (PXLBLCK_HIGHER_COLOR_RESOLUTION_ENABLED)
      return pxlBlckUtils_convert_color_values_to_32bit(brightness * (float)(wheelPos * 3), brightness * (float)(255 - wheelPos * 3), 0);
    else
      return PXLBLCK_INSTANCE->Color(brightness * (float)(wheelPos * 3), brightness * (float)(255 - wheelPos * 3), 0);
  }
  else if (wheelPos < 170)
  {
    wheelPos -= 85;
    if (PXLBLCK_HIGHER_COLOR_RESOLUTION_ENABLED)
      return pxlBlckUtils_convert_color_values_to_32bit(brightness * (float)(255 - wheelPos * 3), 0, brightness * (float)(wheelPos * 3));
    else
      return PXLBLCK_INSTANCE->Color(brightness * (float)(255 - wheelPos * 3), 0, brightness * (float)(wheelPos * 3));
  }
  else
  {
    wheelPos -= 170;
    if (PXLBLCK_HIGHER_COLOR_RESOLUTION_ENABLED)
      return pxlBlckUtils_convert_color_values_to_32bit(0, brightness * (float)(wheelPos * 3), brightness * (float)(255 - wheelPos * 3));
    else
      return PXLBLCK_INSTANCE->Color(0, brightness * (float)(wheelPos * 3), brightness * (float)(255 - wheelPos * 3));
  }
}

uint32_t pxlBlckUtils_color_value_update(uint32_t actualStoredColorValue, int16_t red, int16_t green, int16_t blue, int16_t warmWhite)
{
  uint8_t valueRed = pxlBlckUtils_return_red_from_config(actualStoredColorValue);
  uint8_t valueGreen = pxlBlckUtils_return_green_from_config(actualStoredColorValue);
  uint8_t valueBlue = pxlBlckUtils_return_blue_from_config(actualStoredColorValue);
  uint8_t valueWarmWhite = pxlBlckUtils_return_warmwhite_from_config(actualStoredColorValue);

  if (red >= 0)
  {
    valueRed = red;
  }
  if (green >= 0)
  {
    valueGreen = green;
  }
  if (blue >= 0)
  {
    valueBlue = blue;
  }
  if (warmWhite >= 0)
  {
    valueWarmWhite = warmWhite;
  }

  pxlBlckUtils_exchange_if_led_type_is_rgb(&valueRed, &valueGreen);

  return pxlBlckUtils_return_correct_color_value(valueRed, valueGreen, valueBlue, valueWarmWhite);
}

void pxlBlckUtils_exchange_if_led_type_is_rgb(uint8_t *firstValue, uint8_t *secondValue)
{
  //This function helps to exchange the values of "firstValue" and "secondValue". This is needed because in case of an "RGB"-LedType an exchange of the red and green values is needed
  //because the .Color function which is used to convert the single RGB values to one color-value expects an GRB-sequence.
  if (PXLBLCK_LED_COLOR_ORDER == NEO_RGBW || PXLBLCK_LED_COLOR_ORDER == NEO_RGB)
  {
    uint8_t tempColorValue = *firstValue;
    *firstValue = *secondValue;
    *secondValue = tempColorValue;
  }
}

void pxlBlckUtils_test_matrix()
{
  String log = F(PXLBLCK_DEVICE_NAME);
  addLog(LOG_LEVEL_INFO, log);
  log = F("   -Matrix test running... ");
  addLog(LOG_LEVEL_INFO, log);

  //quick and dirty "blocking" implementation of a test mode because this will(should really) only used for testing the led-matrix
  if (PXLBLCK_LED_COLOR_ORDER == NEO_RGBW)
  {
    log += F("  ...(rgb)white.");
    addLog(LOG_LEVEL_INFO, log);
    pxlBlckUtils_fill_matrix(pxlBlckUtils_convert_color_values_to_32bit(100, 100, 100, 0));
    pxlBlckUtils_update_matrix();
    delay(1000);

    log += F("  ...red.");
    addLog(LOG_LEVEL_INFO, log);
    pxlBlckUtils_fill_matrix(pxlBlckUtils_convert_color_values_to_32bit(100, 0, 0, 0));
    pxlBlckUtils_update_matrix();
    delay(1000);

    log += F("  ...green.");
    addLog(LOG_LEVEL_INFO, log);
    pxlBlckUtils_fill_matrix(pxlBlckUtils_convert_color_values_to_32bit(0, 100, 0, 0));
    pxlBlckUtils_update_matrix();
    delay(1000);

    log += F("  ...blue.");
    addLog(LOG_LEVEL_INFO, log);
    pxlBlckUtils_fill_matrix(pxlBlckUtils_convert_color_values_to_32bit(0, 0, 100, 0));
    pxlBlckUtils_update_matrix();
    delay(1000);

    log += F("  ...(warm)white.");
    addLog(LOG_LEVEL_INFO, log);
    pxlBlckUtils_fill_matrix(pxlBlckUtils_convert_color_values_to_32bit(0, 0, 0, 100));
    pxlBlckUtils_update_matrix();
    delay(1000);
  } else
  {

    log += F("  ...white.");
    addLog(LOG_LEVEL_INFO, log);
    pxlBlckUtils_fill_matrix(pxlBlckUtils_convert_color_values_to_32bit(100, 100, 100));
    pxlBlckUtils_update_matrix();
    delay(1000);

    log += F("  ...red.");
    addLog(LOG_LEVEL_INFO, log);
    pxlBlckUtils_fill_matrix(pxlBlckUtils_convert_color_values_to_32bit(100, 0, 0));
    pxlBlckUtils_update_matrix();
    delay(1000);

    log += F("  ...green.");
    addLog(LOG_LEVEL_INFO, log);
    pxlBlckUtils_fill_matrix(pxlBlckUtils_convert_color_values_to_32bit(0, 100, 0));
    pxlBlckUtils_update_matrix();
    delay(1000);

    log += F("  ...blue.");
    pxlBlckUtils_fill_matrix(pxlBlckUtils_convert_color_values_to_32bit(0, 0, 100));
    pxlBlckUtils_update_matrix();
    delay(1000);
  }
}

void pxlBlckUtils_start_demo_mode()
{
  //show animations
  //show icons
  //show clocks?
  //show arrows?
}

void pxlBlckUtils_save_bool_values_in_byte(uint8_t *byteVariable, uint8_t boolArray[8])
{
  *byteVariable = 0;

  String log = F(PXLBLCK_DEVICE_NAME);
  log = F("-boolToByteConverter");
  addLog(LOG_LEVEL_DEBUG, log);

  for (uint8_t i = 0; i < 8; i++)
  {
    if (boolArray[i] == 0 || boolArray[i] == 1)
    {
      *byteVariable = *byteVariable | boolArray[i] << i;

      // return (uint32_t)(((uint32_t)r << 16) | ((uint32_t)g << 8) | b);
    }
    log = F("   parameter \"");
    log += String(i);
    log += F("\" value =");
    log += String(boolArray[i] == 1);
    addLog(LOG_LEVEL_DEBUG, log);
  }

  log = F("   saved bool values in byte as bin=");
  log += String(*byteVariable, BIN);
  log += F("   saved bool values in byte as dec=");
  log += String(*byteVariable);
  addLog(LOG_LEVEL_DEBUG, log);
}

void pxlBlckUtils_return_bool_values_from_byte(uint8_t *byteVariable, uint8_t boolArray[8])
{
  String log = F(PXLBLCK_DEVICE_NAME);
  log = F("-byteToBoolConverter");
  addLog(LOG_LEVEL_DEBUG, log);

  for (uint8_t i = 0; i < 8; i++)
  {
    //*byteVariable = boolArray[i] << i;
    uint8_t tempByte = 0;
    tempByte = 1 << i;
    boolArray[i] = (*byteVariable & tempByte) >> i;
    //boolArray[i] = *byteVariable >> i;
    log = F("   boolArray[");
    log += String(i);
    log += F("]=");
    log += String(boolArray[i]);
    addLog(LOG_LEVEL_DEBUG, log);

  }

}

void pxlBlckUtils_addColorFormParts(String prefix, uint32_t formerColorValue, String name)
{
  if (!name.equals(PXLBLCK_WEBSERVER_FORM_COLOR_NOT_USED_VALUE))
  {
    uint8_t formerColorR = pxlBlckUtils_return_red_from_config(formerColorValue);
    uint8_t formerColorG = pxlBlckUtils_return_green_from_config(formerColorValue);
    uint8_t formerColorB = pxlBlckUtils_return_blue_from_config(formerColorValue);
    uint8_t formerColorWW = pxlBlckUtils_return_warmwhite_from_config(formerColorValue);

    pxlBlckUtils_exchange_if_led_type_is_rgb(&formerColorR, &formerColorG);

    addFormNumericBox(name, prefix + "R", formerColorR, 0, 255);
    addNumericBox(prefix + "G", formerColorG, 0, 255);
    addNumericBox(prefix + "B", formerColorB, 0, 255);
    if (PXLBLCK_LED_COLOR_ORDER == NEO_RGBW)
    {
      addNumericBox(prefix + "Ww", formerColorWW, 0, 255);
    }
    pxlBlckUtils_addColorPicker(prefix + "CP", pxlBlckUtils_convert_32bit_to_hex_string(formerColorValue));
    addFormNote("Set " + name + "-color directly (range:0-255) or by color picker");
  }
}

void pxlBlckUtils_add_color_values_to_debug_log(String colorName)
{
  String log = "   " + colorName + "CP=";
  log += pxlBlckUtils_getFormItemString(String(colorName + "CP"));
  addLog(LOG_LEVEL_DEBUG, log);
  log = "   " + colorName + "R=";
  log += String(getFormItemInt(String(colorName + "R")));
  addLog(LOG_LEVEL_DEBUG, log);
  log = "   " + colorName + "G=";
  log += String(getFormItemInt(String(colorName + "G")));
  addLog(LOG_LEVEL_DEBUG, log);
  log = "   " + colorName + "B=";
  log += String(getFormItemInt(String(colorName + "B")));
  addLog(LOG_LEVEL_DEBUG, log);
}

void pxlBlckUtils_handle_and_save_new_color_values(String hexString, uint32_t *actualColorValue, uint8_t *newR, uint8_t *newG, uint8_t *newB, uint8_t inputR, uint8_t inputG, uint8_t inputB)
{
  pxlBlckUtils_exchange_if_led_type_is_rgb(newR, newG);

  pxlBlckUtils_select_color_values_from_color_picker_or_number_input(
    hexString,
    *actualColorValue,
    newR, newG, newB,
    inputR,
    inputG,
    inputB);

  *actualColorValue = pxlBlckUtils_convert_color_values_to_32bit(*newR, *newG, *newB);
}

void pxlBlckUtils_save_two_bytes_in_uint16(uint16_t *intVariable, uint8_t firstByte, uint8_t secondByte)
{
  *intVariable = (((uint16_t)firstByte << 8) | secondByte);

  String log = F(PXLBLCK_DEVICE_NAME);
  log += F("-byteToIntConverter");
  addLog(LOG_LEVEL_DEBUG, log);
  log = F("   saved bytes ");
  log += String(firstByte, BIN);
  log += F(" and ");
  log += String(secondByte, BIN);
  log += F(" to uint16 as bin=");
  log += String(*intVariable, BIN);
  addLog(LOG_LEVEL_DEBUG, log);
  log = F("   saved bytes ");
  log += String(firstByte);
  log += F(" and ");
  log += String(secondByte);
  log += F(" to uint16 as dec=");
  log += String(*intVariable);
  addLog(LOG_LEVEL_DEBUG, log);
}

void pxlBlckUtils_return_byte_values_from_uint16(uint16_t intVariable, uint8_t *firstByte, uint8_t *secondByte)
{
  *secondByte = intVariable;
  *firstByte = (intVariable >> 8);

  String log = F(PXLBLCK_DEVICE_NAME);
  log = F("-intToByteConverter");
  addLog(LOG_LEVEL_DEBUG, log);
  log = F("   returned byte values from uint16 as bin: firstByte=");
  log += String(*firstByte, BIN);
  log += F(" ,secondByte=");
  log += String(*secondByte, BIN);
  addLog(LOG_LEVEL_DEBUG, log);
  log = F("   returned byte values from uint16 as dec: firstByte=");
  log += String(*firstByte);
  log += F(" ,secondByte=");
  log += String(*secondByte);
  addLog(LOG_LEVEL_DEBUG, log);
}

uint16_t pxlBlckUtils_save_boolean_runtime_variables_to_permanent_storage()
{
  //The 10 is used as a placeholder for unused "places"
  uint8_t boolArray[16] = {
    Plugin_205_displayEnabled,
    Plugin_205_wordclockShowItIsEnabled,
    Plugin_205_wordclockShowOClockEnabled,
    Plugin_205_ringclockThick12markEnabled,
    Plugin_205_ringclockClockDirInversed,
    Plugin_205_diallLeadingZerosEnabled,
    10,
    10
  };
  uint8_t byteVariable = 0;

  pxlBlckUtils_save_bool_values_in_byte(&byteVariable, boolArray);
  return byteVariable;
}


#endif
