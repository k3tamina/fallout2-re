#include "cycle.h"

#include "color.h"
#include "core.h"
#include "game_config.h"
#include "palette.h"

// 0x51843C
int gColorCycleSpeedFactor = 1;

// TODO: Convert colors to RGB.
// clang-format off

// Green.
//
// 0x518440
unsigned char _slime[12] = {
    0, 108, 0,
    11, 115, 7,
    27, 123, 15,
    43, 131, 27,
};

// Light gray?
//
// 0x51844C
unsigned char _shoreline[18] = {
    83, 63, 43,
    75, 59, 43,
    67, 55, 39,
    63, 51, 39,
    55, 47, 35,
    51, 43, 35,
};

// Orange.
//
// 0x51845E
unsigned char _fire_slow[15] = {
    255, 0, 0,
    215, 0, 0,
    147, 43, 11,
    255, 119, 0,
    255, 59, 0,
};

// Red.
//
// 0x51846D
unsigned char _fire_fast[15] = {
    71, 0, 0,
    123, 0, 0,
    179, 0, 0,
    123, 0, 0,
    71, 0, 0,
};

// Light blue.
//
// 0x51847C
unsigned char _monitors[15] = {
    107, 107, 111,
    99, 103, 127,
    87, 107, 143,
    0, 147, 163,
    107, 187, 255,
};

// clang-format on

// 0x51848C
bool gColorCycleInitialized = false;

// 0x518490
bool gColorCycleEnabled = false;

// 0x518494
int _slime_start = 0;

// 0x518498
int _shoreline_start = 0;

// 0x51849C
int _fire_slow_start = 0;

// 0x5184A0
int _fire_fast_start = 0;

// 0x5184A4
int _monitors_start = 0;

// 0x5184A8
unsigned char _bobber_red = 0;

// 0x5184A9
signed char _bobber_diff = -4;

// 0x56D7D0
unsigned int gColorCycleTimestamp3;

// 0x56D7D4
unsigned int gColorCycleTimestamp1;

// 0x56D7D8
unsigned int gColorCycleTimestamp2;

// 0x56D7DC
unsigned int gColorCycleTimestamp4;

// 0x42E780
void colorCycleInit()
{
    if (gColorCycleInitialized) {
        return;
    }

    bool colorCycling;
    if (!configGetBool(&gGameConfig, GAME_CONFIG_SYSTEM_KEY, GAME_CONFIG_COLOR_CYCLING_KEY, &colorCycling)) {
        colorCycling = true;
    }

    if (!colorCycling) {
        return;
    }

    for (int index = 0; index < 12; index++) {
        _slime[index] >>= 2;
    }

    for (int index = 0; index < 18; index++) {
        _shoreline[index] >>= 2;
    }

    for (int index = 0; index < 15; index++) {
        _fire_slow[index] >>= 2;
    }

    for (int index = 0; index < 15; index++) {
        _fire_fast[index] >>= 2;
    }

    for (int index = 0; index < 15; index++) {
        _monitors[index] >>= 2;
    }

    tickersAdd(colorCycleTicker);

    gColorCycleInitialized = true;
    gColorCycleEnabled = true;

    int cycleSpeedFactor;
    if (!configGetInt(&gGameConfig, GAME_CONFIG_SYSTEM_KEY, GAME_CONFIG_CYCLE_SPEED_FACTOR_KEY, &cycleSpeedFactor)) {
        cycleSpeedFactor = 1;
    }

    change_cycle_speed(cycleSpeedFactor);
}

// 0x42E8CC
void colorCycleReset()
{
    if (gColorCycleInitialized) {
        gColorCycleTimestamp1 = 0;
        gColorCycleTimestamp2 = 0;
        gColorCycleTimestamp3 = 0;
        gColorCycleTimestamp4 = 0;
        tickersAdd(colorCycleTicker);
        gColorCycleEnabled = true;
    }
}

// 0x42E90C
void colorCycleFree()
{
    if (gColorCycleInitialized) {
        tickersRemove(colorCycleTicker);
        gColorCycleInitialized = false;
        gColorCycleEnabled = false;
    }
}

// 0x42E930
void colorCycleDisable()
{
    gColorCycleEnabled = false;
}

// 0x42E93C
void colorCycleEnable()
{
    gColorCycleEnabled = true;
}

// 0x42E948
bool colorCycleEnabled()
{
    return gColorCycleEnabled;
}

// 0x42E97C
void colorCycleTicker()
{
    if (!gColorCycleEnabled) {
        return;
    }

    bool changed = false;

    unsigned char* palette = _getSystemPalette();
    unsigned int time = _get_time();

    if (getTicksBetween(time, gColorCycleTimestamp1) >= COLOR_CYCLE_PERIOD_1 * gColorCycleSpeedFactor) {
        changed = true;
        gColorCycleTimestamp1 = time;

        int paletteIndex = 229 * 3;

        for (int index = _slime_start; index < 12; index++) {
            palette[paletteIndex++] = _slime[index];
        }

        for (int index = 0; index < _slime_start; index++) {
            palette[paletteIndex++] = _slime[index];
        }

        _slime_start -= 3;
        if (_slime_start < 0) {
            _slime_start = 9;
        }

        paletteIndex = 248 * 3;

        for (int index = _shoreline_start; index < 18; index++) {
            palette[paletteIndex++] = _shoreline[index];
        }

        for (int index = 0; index < _shoreline_start; index++) {
            palette[paletteIndex++] = _shoreline[index];
        }

        _shoreline_start -= 3;
        if (_shoreline_start < 0) {
            _shoreline_start = 15;
        }

        paletteIndex = 238 * 3;

        for (int index = _fire_slow_start; index < 15; index++) {
            palette[paletteIndex++] = _fire_slow[index];
        }

        for (int index = 0; index < _fire_slow_start; index++) {
            palette[paletteIndex++] = _fire_slow[index];
        }

        _fire_slow_start -= 3;
        if (_fire_slow_start < 0) {
            _fire_slow_start = 12;
        }
    }

    if (getTicksBetween(time, gColorCycleTimestamp2) >= COLOR_CYCLE_PERIOD_2 * gColorCycleSpeedFactor) {
        changed = true;
        gColorCycleTimestamp2 = time;

        int paletteIndex = 243 * 3;

        for (int index = _fire_fast_start; index < 15; index++) {
            palette[paletteIndex++] = _fire_fast[index];
        }

        for (int index = 0; index < _fire_fast_start; index++) {
            palette[paletteIndex++] = _fire_fast[index];
        }

        _fire_fast_start -= 3;
        if (_fire_fast_start < 0) {
            _fire_fast_start = 12;
        }
    }

    if (getTicksBetween(time, gColorCycleTimestamp3) >= COLOR_CYCLE_PERIOD_3 * gColorCycleSpeedFactor) {
        changed = true;
        gColorCycleTimestamp3 = time;

        int paletteIndex = 233 * 3;

        for (int index = _monitors_start; index < 15; index++) {
            palette[paletteIndex++] = _monitors[index];
        }

        for (int index = 0; index < _monitors_start; index++) {
            palette[paletteIndex++] = _monitors[index];
        }

        _monitors_start -= 3;

        if (_monitors_start < 0) {
            _monitors_start = 12;
        }
    }

    if (getTicksBetween(time, gColorCycleTimestamp4) >= COLOR_CYCLE_PERIOD_4 * gColorCycleSpeedFactor) {
        changed = true;
        gColorCycleTimestamp4 = time;

        if (_bobber_red == 0 || _bobber_red == 60) {
            _bobber_diff = -_bobber_diff;
        }

        _bobber_red += _bobber_diff;

        int paletteIndex = 254 * 3;
        palette[paletteIndex++] = _bobber_red;
        palette[paletteIndex++] = 0;
        palette[paletteIndex++] = 0;
    }

    if (changed) {
        paletteSetEntriesInRange(palette + 229 * 3, 229, 255);
    }
}

// 0x42E950
void change_cycle_speed(int value)
{
    gColorCycleSpeedFactor = value;
    configSetInt(&gGameConfig, GAME_CONFIG_SYSTEM_KEY, GAME_CONFIG_CYCLE_SPEED_FACTOR_KEY, value);
}

// NOTE: Unused.
//
// 0x42E974
int get_cycle_speed()
{
    return gColorCycleSpeedFactor;
}
