//=====[#include guards - begin]===============================================

#ifndef _IGNITION_SUBSYSTEM_H_
#define _IGNITION_SUBSYSTEM_H_

//=====[Declaration of public defines]=========================================

//=====[Declaration of public data types]======================================

typedef enum {
    ENGINE_OFF,
    ENGINE_ON
} engineState_t;

//=====[Declaration and initialization of public global variables]=============

extern engineState_t engineState;


//=====[Declarations (prototypes) of public functions]=========================

void inputsInit();
void outputsInitIgnition();
void engineInit();
void ignitionUpdate();

//=====[#include guards - end]=================================================

#endif // _IGNITION_SUBSYSTEM_H_