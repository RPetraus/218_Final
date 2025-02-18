//=====[#include guards - begin]===============================================

#ifndef _WINDSHIELD_WIPER_SUBSYSTEM_H_
#define _WINDSHIELD_WIPER_SUBSYSTEM_H_

//=====[Declaration of public defines]=========================================

//=====[Declaration of public data types]======================================


typedef enum {
   WIPER_HI,
   WIPER_LO,
   WIPER_INT,
   WIPER_OFF
} WiperMode_t;


//=====[Declarations (prototypes) of public functions]=========================

//void ignitionSubsystemInit();
//bool ignitionSubsystemUpdate();
void windshieldWiperInit();
void windshieldWiperModeUpdate();
WiperMode_t windshieldWiperMode();


//=====[#include guards - end]=================================================

#endif // _WINDSHIELD_WIPER_SUBSYSTEM_H_