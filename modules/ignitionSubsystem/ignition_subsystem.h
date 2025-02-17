//=====[#include guards - begin]===============================================

#ifndef _IGNITION_SUBSYSTEM_H_
#define _IGNITION_SUBSYSTEM_H_

//=====[Declaration of public defines]=========================================

//=====[Declaration of public data types]======================================

//=====[Declaration and initialization of public global variables]=============

extern bool engineRunning;

//=====[Declarations (prototypes) of public functions]=========================

void ignitionSubsystemInit();
bool ignitionSubsystemUpdate();


//=====[#include guards - end]=================================================

#endif // _IGNITION_SUBSYSTEM_H_