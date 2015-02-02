#ifndef JPRDEFINES_H
#define JPRDEFINES_H

#define MAX_SPEED 255
#define ZERO_SPEED 50

#define PI 3.1415926535897932384626433832795

#define JPR_MIN_PWM 55

#define JPR_GYRO_ANALOG 6

#define JPR_DEBUG true
#define SAVE_VIDEO (true && JPR_DEBUG)
#define SAVE_LOG (true && JPR_DEBUG)
#define JPR_SHOW_IMAGE false

#define JPR_VASAK_IR analog[1]
#define JPR_PAREM_IR analog[0]
#define JPR_YLEMINE_IR analog[4]

#define JPR_ANDUR_SEIN 150
#define JPR_V_ANDUR ((analog[0]) > 200)
#define JPR_P_ANDUR ((analog[1]) > 200)
#define JPR_SEIN_EES (JPR_V_ANDUR && JPR_P_ANDUR)
#define JPR_SEIN_VASAKUL (JPR_V_ANDUR && !JPR_P_ANDUR)
#define JPR_SEIN_PAREMAL (JPR_P_ANDUR && !JPR_V_ANDUR)
#define JPR_SEIN (JPR_SEIN_EES || JPR_SEIN_VASAKUL || JPR_SEIN_PAREMAL)
#define JPR_PALL_EES1 ((analog[2]) > 500)
#define JPR_PALL_EES2 ((analog[3]) > 500)
#define JPR_PALL_EES (JPR_PALL_EES1 || JPR_PALL_EES2)
#define JPR_NURK ((analog[0]) > 490 && (analog[1]) > 490 && (analog[5] < 200) )
#define JPR_PORAND (analog[4]<160)

#define JPR_IR_V_ANDUR ((analog[7]) < 100)
#define JPR_IR_P_ANDUR ((analog[6]) < 100)
#define JPR_VARAV_EES (JPR_IR_V_ANDUR && JPR_IR_P_ANDUR)
#define JPR_VARAV_VASAKUL (JPR_IR_V_ANDUR && !JPR_IR_P_ANDUR)
#define JPR_VARAV_PAREMAL (JPR_IR_P_ANDUR && !JPR_IR_V_ANDUR)
#define JPR_VARAV (JPR_VARAV_EES || JPR_VARAV_VASAKUL || JPR_VARAV_PAREMAL)
//temp
#define GOAL GOAL_BLUE
#define JPR_NAEB_VARAVAT (getGoal()!=NULL)
#define JPR_NAEB_SUVALIST_VARAVAT (getAnyGoal()!=NULL)

#define JPR_SV_ANDUR ((analog[0]) > 80)
#define JPR_SP_ANDUR ((analog[1]) > 80)

#define JPR_NAEB_PALLI (pall != NULL)

#endif // JPRDEFINES_H
