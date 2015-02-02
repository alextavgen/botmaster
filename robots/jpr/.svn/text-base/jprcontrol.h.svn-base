#ifndef JPRCONTROL_H
#define JPRCONTROL_H
#include "../../abstractrobot.h"
#include "jpromnicontrol.h"

#include "jprdefines.h"

class JprControl: public QObject, public JprOmniControl {
	Q_OBJECT
public:
	JprControl(AbstractRobot*);

	~JprControl();
	QTime solTime;
        void omni(float a, float b, int c, float d);
	void omniBreak();
        void solenoid(bool s = false); // s = true - sleepiga versioon
        void checkSolenoid();
	void headLight(bool on);
	void reel(bool on);
        int pwm[3]; //logimiseks
private:
	AbstractRobot* abr;
        void sendWheelSpeeds();

};

#endif
