#pragma once

#include <wut.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct KPADVec2f {
    float x;
    float y;
} KPADVec2f;

typedef struct KPADVec3f {
    float x;
    float y;
    float z;
} KPADVec3f;

typedef struct KPADStatus {
    uint32_t hold;
    uint32_t trigger;
    uint32_t release;
    KPADVec3f acc;
    KPADVec3f acc_diff;
    KPADVec3f dir;
    KPADVec2f stick;
    KPADVec2f substick;
    uint16_t cl_button_hold;
    uint16_t cl_trigger;
    uint16_t cl_release;
    uint16_t error;
    uint8_t dpd_valid_fg;
    uint8_t wave_index;
} KPADStatus;

void KPADInit(void);
int KPADRead(int chan, KPADStatus *status, int count, int *outCount);

#ifdef __cplusplus
}
#endif
