//
// C++ Interface: testrobot
//
// Description: 
//
//
// Author: 
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef TONT_H
#define TONT_H
//#include "log.h"
#include "abstractrobot.h"
//#include "objectfinder.h"
#include "image.h"
#include "view.h"
#include <iostream>


/**
	@author  Margus Ernits <margus.ernits@itcollege.ee>
*/
class Tont: public AbstractRobot{
public:
  
    Tont(); 

    ~Tont();
    
    void go();
	 void msleep(long t) {usleep(t * 1000);}

	void kickBall();
	void p88ra(int, int);
	void otse();
	void pidur();
	void liigu(int);
	void tagurda();

	void KPp88reP(); //kohapeal p88re paremale

	void KPkiirendP(); //kohapeal p88re paremale kiirendusega
	void KPkiirendV(); //kohapeal p88re vasakule kiirendusega

	void p88reKaugPallileV(); //p88re kaugemal vasakul asuvale pallile
	void p88reKaugPallileP(); //p88re kaugemal paremal asuvale pallile
	void p88reL2hiPallileV(); //p88re l2hedal paremal asuvale pallile
	void p88reL2hiPallileP(); //p88re l2hedal vasakul asuvale pallile
	void otseL2hiPallile(); //otses6it pallile, mis j22b kohe kaamerapildist v2lja, aga on otse

	void p88reL2hiV2ravaleV(); //p88ramine v2rava poole, mis on l2hedal, vasakul
	void p88reL2hiV2ravaleP(); //p88ramine v2rava poole, mis on l2hedal, paremal
	void p88reKaugV2ravaleV(); //p88ramine v2rava poole, mis on kaugel, vasakul
	void p88reKaugV2ravaleP(); //p88ramine v2rava poole, mis on kaugel, paremal
	void p88reKeskV2ravaleV(); //p88ramine v2rava poole, mis on keskmisel kaugusel, vasakul
	void p88reKeskV2ravaleP(); //p88ramine v2rava poole, mis on keskmisel kaugusel, paremal

	void otsiV2rav();
	void otsiPall();
	void eemaldu();


	void clearBuffer();
	void driveTurn(int);
	void whereToMove();
  private:
   	Image::Object v2rav;
	Image::Object maxObj;
	QTime otsimine;
	QTime l88giTimer;
	QTime pildiReopenTimer;
	QTime palliPesaTimer;
	QTime pallL2hedalTimer;
	QTime kiiruseT6stmineTimer;
	QTime palliPyydmisTimer;
	QTime seinaTimer;
	QTime sihibKeskeleTimer;
	
	bool v2ravaTimerT88tab;
	bool palliTimerT88tab;
	bool palliOtsimisTimerT88tab;
	bool seinaTimerT88tab;
	bool sihibKeskeleTimerT88tab;
	bool pallK2es; //pallianduri asendamiseks 
	bool pallL2hedal;
	bool pallN2ha;
	bool v2ravN2ha;
	bool seinL2hedal;

	int palliAsukoht;
	int v2ravaAsukoht;
	int v2ravaSignaal;
	int v2ravaLaius;
	int v2ravaKaugus;
	int seinaKaugus;
	int v2ravWidth;	

	int otseKiirus;
	int tahaKiirus;
	int otseKiirusL2hedal;

	int vMootorVaheEdasi; //mootori kiiruste vahe - vasak v6imsam, vasakust tuleb lahutada
	int pMootorVaheTagasi; //mootori kiiruste vahe - parem tagurdades v6imsam - paremast tuleb lahutada


	int kiirusKP; //kohapeal p88ramise kiirus
	int algkiirusKP; //algkiirus kohapeal p88ramisel kiirendades
	int v2ravaOtsimKiirusKP;
	

	int kiirusKaugPallileSuur;
	int kiirusKaugPallileV2ike;
	int kiirusL2hiPallileSuur;
	int kiirusL2hiPallileV2ike;

	int kiirusL2hiV2ravale; //kiirus, millega p88ratakse l2hedal asuva v2rava poole
	int kiirusKaugV2ravale; //kiirus, millega p88ratakse kaugel asuva v2rava poole
	int kiirusKeskV2ravale; //kiirus millega p88ratakse keskmiselt kaugel asuva v2rava poole
	

	int pildiKaunter;

	
};	

#endif
