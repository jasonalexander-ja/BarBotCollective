
// The I2C address of the unit, can be between 8 and 77 
#define UNIT_ADDRESS 18

// Code to run once on setup 
void unitSetup();

// Code to run when an action is requested 
int performAction(int option);

typedef enum {
    DistSensorInitError = 1
} CaddyErrors;
