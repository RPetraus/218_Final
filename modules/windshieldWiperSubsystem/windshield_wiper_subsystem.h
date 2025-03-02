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

typedef enum {
    INT_SHORT,
    INT_MED,
    INT_LONG
} IntDelayTime_t;


//=====[Declarations (prototypes) of public functions]=========================

void windshieldWiperInit();
void windshieldWiperUpdate();
WiperMode_t windshieldWiperMode();
IntDelayTime_t intDelayType();
void windshieldWiperStop();


//=====[#include guards - end]=================================================

#endif // _WINDSHIELD_WIPER_SUBSYSTEM_H_
