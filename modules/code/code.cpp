//=====[Libraries]=============================================================

#include "code.h"

#include "pc_serial_com.h"

//=====[Declaration of private defines]========================================

#define CODE_LENGTH 4

//=====[Declaration of private data types]=====================================

//=====[Declaration and initialization of public global objects]===============

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============

int codeSequence[CODE_LENGTH]   = { 1, 1, 0, 0 }; //idk if this should be private or public (depends if used elsewhere, prob like unlocking subsytem)

//=====[Declaration and initialization of private global variables]============

static int codeLetterNum;

//=====[Declarations (prototypes) of private functions]========================

//=====[Implementations of public functions]===================================

void resetCode() {
    pcSerialComStringWrite("Please enter a new code sequence.\r\n");
    pcSerialComStringWrite("The allowable length of the sequence is 4.\r\n");
    pcSerialComStringWrite("The sequence may include any of:\r\n");
    pcSerialComStringWrite("'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'\r\n\r\n");

    int tempCode[CODE_LENGTH];  // Temporary array to store the first entry
    char receivedChar;

    while (true) {  // Keep asking until the user enters the same code twice

        // First Code Entry
        pcSerialComStringWrite("Enter the new code: ");
        for (int i = 0; i < CODE_LENGTH; i++) {
            do {
                pcSerialComCharRead(&receivedChar);

                if (receivedChar >= '0' && receivedChar <= '9') {
                    tempCode[i] = receivedChar - '0';
                    pcSerialComStringWrite("*");  // Print * for valid input
                    break;
                } else {
                    pcSerialComStringWrite("\r\nInvalid input! Please enter a digit (0-9).\r\n");
                    pcSerialComStringWrite("Current sequence: ");
                    for (int j = 0; j < i; j++) {
                        pcSerialComStringWrite("*");
                    };
                }
            } while (true);
        }

        pcSerialComStringWrite("\r\nRe-enter the code to confirm: ");

        // Second Code Entry (Confirmation)
        bool match = true;
        for (int i = 0; i < CODE_LENGTH; i++) {
            do {
                pcSerialComCharRead(&receivedChar);

                if (receivedChar >= '0' && receivedChar <= '9') {
                    if (tempCode[i] != receivedChar - '0') {
                        match = false;  // If any digit is different, set match to false
                    }
                    pcSerialComStringWrite("*");  // Print * for valid input
                    break;
                } else {
                    pcSerialComStringWrite("\r\nInvalid input! Please enter a digit (0-9).\r\n");
                    pcSerialComStringWrite("Current sequence: ");
                    for (int j = 0; j < i; j++) {
                        pcSerialComStringWrite("*");
                    };
                }
            } while (true);
        }

        if (match) {
            // If both entries match, update the actual code
            for (int i = 0; i < CODE_LENGTH; i++) {
                codeSequence[i] = tempCode[i];
            }
            pcSerialComStringWrite("\r\nNew code set successfully!\r\n\r\n");
            break;  // Exit the loop once successful
        } else {
            // If mismatch, ask again
            pcSerialComStringWrite("\r\nError: The codes did not match! Please try again.\r\n\r\n");
        }
    }
}

//=====[Implementations of private functions]==================================