#define kp .25
#define kd .29
#define ekp 35
#define ekd 50
void catapultPID(void* ignore);
void fire();
void elevPID(void *ignore);
void setTarg(int a);