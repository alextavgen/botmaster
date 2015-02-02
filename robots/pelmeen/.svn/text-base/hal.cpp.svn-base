//
// C++ Implementation: tolmurull
//
// Description: 
//
//
// Author: Valdur Kaldvee, Margus Ernits <margus.ernits@itcollege.ee>, Katrin Kibbal (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "hal.h"
#include <QTime>
#define CAN  1
#define SOCK 2
#define CAN_BEACON 3
#define SOCK_BEACON 4




Hal::Hal()
{
}


Hal::~Hal()
{
}
void Hal::viivitus(long t)
{
	usleep(t * 1000);
}

void Hal::go()
{
	Config  & conf  = Config::getConfig();
	
        qDebug("Hal Start...\n");
	conf.setSendCmdEnabled(1);
	
	Image::Object * obj;
	Image::Object * suurimObj;
	Image::Object * canBeacon;
	Image::Object * sockBeacon;
	Image::Object * suurimSokk;

while(1){
_img = image->getFrame();
image->process(1,1,1,1);

if (image->found_objects != NULL) {
   for (int i = 0; i < image->found_objects->total; i++) {

     obj =  (Image::Object* ) cvGetSeqElem( (image->found_objects), i );
      
      if(obj!=0){
        std::cout<<((obj->type))<<std::endl;
//        std::cout<<((obj->distanceH))<<std::endl;
        std::cout<<((obj->area))<<std::endl;
        
      }
      
   }
 }
view->show(_img);

}
	
/*
Dokumentatsioon!
Kõik määrangud on tagant ja pealtvaates!

Analog(0) - ees parem andur
Analog(1) - ees vasak andur
Analog(2) - küljel parem andur 360-380
Analog(3) - küljel vasak andur 370-390
Analog(4) - sokiandur
Analog(5) - purgiandur
Servo 130 - parem sõiduservo
Servo 131 - vasak sõiduservo
Servo 129 - käpa servo
Servo 132 - lahtilükkaja servo
Servo 133 - kastiservo 80 kast all, 147 kõrval


*/
	


	int aeg;
	QTime t; //taimeri muutuja
	t.start(); // Paneme taimeri käima, minut on 60000, 3 minutit 180000

	int kinni = 0, i = 0, suund = 0, purk = 5, arv = 0, loog, kala, muna = 0, kinni2 = 0, kinni3 = 0, toru = 0, sokk = 0, otsing = 0, juust = 0, tumba = 0;
	int kontinue = 0, pindala = 0, korgus = 0, kaugus = 0, PBpindala = 0, PBkorgus = 0, PBkaugus = 0, Skaugus = 0, Spindala = 0, majakasuund = 0;
	/*int parem_ees=-1,vasak_ees=-1,parem_kylg=-1,vasak_kylg=-1;*/				//suunad: 1 on paremale, -1 vasakule
	while(0)
	{
// 		if (getAnalog(5) > 950)
// 		{
// 			k2pp();
// 		}
		if (getAnalog(4) > 70)
		{
			sokkyles();
		}
// 		std::cout<<getAnalog(4)<<std::endl;
// 		std::cout<<getAnalog(0)<<" parem ees"<<std::endl;
// 		std::cout<<getAnalog(1)<<" vasak ees"<<std::endl;
// 		std::cout<<getAnalog(2)<<" parem külg"<<std::endl;
// 		std::cout<<getAnalog(3)<<" vasak külg"<<std::endl;
// 		viivitus(2000);
// 		aeglaselt_vasakule();
// 		viivitus(3000);
// 		aeglaselt_paremale();
// 		viivitus(3000);
// 		kiiremini_vasakule();
// 		viivitus(3000);
// 		kiiremini_paremale();
// 		viivitus(3000);
	}
	while(0)
	{
		aeg = t.elapsed();  // stardist möödunud aeg millisekundites
		setServo(5,125);
// 		std::cout<<"vaikimisi on kataloog ees"<<std::endl;
// 		getAnalogs();			
// 		parem_ees=getAnalog(0);
// 		vasak_ees=getAnalog(1);
// 		parem_kylg=getAnalog(2);
// 		vasak_kylg=getAnalog(3);
		_img = image->getFrame();
		_img = image->getFrame();
		_img = image->getFrame();
		image->process(1,1,1,1);
		canBeacon = 0;
		suurimObj = 0;
		suurimSokk = 0; 
		sockBeacon = 0;
		//kui leidis midagi vaateväljalt
		if (image->found_objects != NULL)
		{
			// käime üle kõik leitud objektid ja valime neist suurima purgi
			for (i = 0; i < image->found_objects->total; i++)
			{
				obj = (Image::Object *) cvGetSeqElem( (image->found_objects), i);
				if (obj != 0)
				{
					if (obj->type == CAN) 
					{
						//leidsime purgi, vaatame kas on suurim
						if (!suurimObj || (obj->area > suurimObj->area))
						{	
// 							std::cout<<"vahetasin objekti"<<std::endl;
							suurimObj = obj;
// 							std::cout<<((suurimObj->rect.width))<<std::endl;
// 							std::cout<<"distanceH "<<((suurimObj->distanceH))<<std::endl;
// 							std::cout<<((suurimObj->area))<<std::endl;
							kaugus = suurimObj->distanceH;
							pindala = suurimObj->area;
							korgus = suurimObj->rect.height;
						}
					}
					else if (obj->type == SOCK) 
					{
						//leidsime purgi, vaatame kas on suurim
						if (!suurimSokk || (obj->area > suurimSokk->area))
						{	
// 							std::cout<<"vahetasin objekti"<<std::endl;
							suurimSokk = obj;
							Skaugus = suurimSokk->distanceH;
							Spindala = suurimSokk->area;
// 							std::cout<<((suurimObj->area))<<std::endl;
						}
					}
					else if ((obj->type == SOCK_BEACON) && (purk >= 6))			//purgi piikon
					{
						if (!canBeacon || (obj->area > canBeacon->area))
						{
							canBeacon = obj;
							PBkaugus = canBeacon->distanceH;
							PBpindala = canBeacon->area;
							PBkorgus = canBeacon->rect.height;
							std::cout<<((canBeacon->rect.width))<<std::endl;
						}
					}
					else if ((obj->type == CAN_BEACON))					//soki piikon
					{
						if (!sockBeacon || (obj->area > sockBeacon->area))
						{
// 							std::cout<<"leidsin piikoni"<<std::endl;
							sockBeacon = obj;
// 							std::cout<<((canBeacon->area))<<std::endl;
						}
					}
				}
			}
		}
		view->show(_img);
		if (getAnalog(5) > 950 && purk < 6)
		{
			k2pp();
			purk++;
			continue;
		}
		if (((suurimObj != 0 && suurimSokk != 0 && ((pindala*1.5) > suurimSokk->area)) || (suurimObj != 0 && suurimSokk == 0)) /*&& purk < 6*/)	//kui ta leidis PURGI ja purk on lähemal kui sokk või sokki pole
		{	
// 			std::cout<<((suurimObj->type))<<std::endl;
// 			std::cout<<((kaugus))<<std::endl;
// 			std::cout<<((pindala))<<std::endl;
// 			if (getAnalog(5) > 950 && purk < 6)
// 			{
// 				k2pp();
// 				purk++;
// 				_img = image->getFrame();
// 				_img = image->getFrame();
// 				_img = image->getFrame();
// 			}
			int vasak = getAnalog(3);
			int parem = getAnalog(2);
			while (((getAnalog(0) > 330) || getAnalog(1) > 330) && (parem < 200 && vasak < 200))         //kataloog on purgi ja pelmeeni vahel
			{
				kinni = 0;
				kinni2 = 0;
				kinni3 = 0;
				if (getAnalog(0) < getAnalog(1))			                   //kui takistus on rohkem vasakul analog(1) on vasak
				{
					kohapeal_paremale(6);
// 					std::cout<<"kohapeal paremale "<<std::endl;
					arv++;
					suund = 0;
					loog = 1;
				}
				else if (getAnalog(0) > getAnalog(1))			                //kui takistus on rohkem paremal
				{
					kohapeal_vasakule(6);
// 					std::cout<<"kohapeal vasakule"<<std::endl;
					arv++;
					suund = 1;
					loog = 1;
				}
				else if (arv > 50)
				{
					paremale_tagasi(8);
					viivitus(1000);
				}
				kontinue = 1;
			}
			if (kontinue == 1)
			{
				kontinue = 0;
				continue;
			}
			if (getAnalog(0) > 310 && getAnalog(1) > 310)               //robot on kuskil pees... tagurdame
			{
				kinni = 0;
				kinni2 = 0;
				kinni3 = 0;
				kontinue = 1;
				paremale_tagasi(15);
				viivitus(400);
				paremale_edasi(15);
				viivitus(400);
			}
			if (kontinue == 1)
			{
				kontinue = 0;
				continue;
			}
			if (loog == 1)
			{
				if (suund == 0)
				{
					while (getAnalog(3) > 200)
					{
						edasi(10);
						muna++;
// 						std::cout<<"kala"<<std::endl;
						if (muna > 80)
						{
							muna = 0;
							break;
						}
					}
					aeglaselt_vasakule();
					viivitus(400);
					kala = 0;
					loog = 0;
					continue;
				}
				else if (suund == 1)
				{
					while (getAnalog(2) > 200)
					{
						edasi(10);
						muna++;
// 						std::cout<<"kala2"<<std::endl;
						if (muna > 80)
						{
							muna = 0;
							break;
						}
					}
					aeglaselt_paremale();				//veel jupp maad edasi et oleks parem pöörata
					viivitus(400);
					kala = 0;
					loog = 0;
					continue;
				}
			}
			else if (kaugus < -40)
			{
// 				std::cout<<"vasakule"<<std::endl;
// 				std::cout<<kaugus<<std::endl;
				kinni = 0;
				kinni2 = 0;
				kinni3 = 0;
				if (pindala < 10000)
				{
					aeglaselt_vasakule();
					std::cout<<"aeglaselt vasakule"<<std::endl;
				}
				else if ((pindala < 40000) && (pindala > 10000))  //l�hemale j�udes aeglustame, suurendame nurkkiirust
				{
					kiiremini_vasakule();
					std::cout<<"kiiremini vasakule"<<std::endl;
				}
				else 
				{
					kohapeal_vasakule(3);
					std::cout<<"kohapeal vasakule"<<std::endl;
				}
			}
			else if (kaugus > 40)
			{
// 				std::cout<<"paremale"<<std::endl;
// 				std::cout<<kaugus<<std::endl;
				kinni = 0;
				kinni2 = 0;
				kinni3 = 0;
				if (pindala < 10000)
				{
					aeglaselt_paremale();
					std::cout<<"aeglaselt paremale"<<std::endl;
				}
				else if ((pindala < 40000) && (pindala > 10000))
				{
					kiiremini_paremale();
					std::cout<<"kiiremini paremale"<<std::endl;
				}
				else 
				{
					kohapeal_paremale(3);
					std::cout<<"kohapeal paremale"<<std::endl;
				}
			}
			else if ((pindala < 10000) && (getAnalog(1) > 150 || getAnalog(0) > 150))
			{
				kinni = 0;
				edasi(10);
// 				std::cout<<"edasi, purke kogutud "<<std::endl;
// 				std::cout<<kaugus<<std::endl;
			}
			else if ((pindala < 10000))
			{
				kinni = 0;
				edasi(15);
				kinni2++;
				if (kinni2 > 50)
				{
					paremale_tagasi(5);
					viivitus(1000);
					continue;
				}
// 				std::cout<<"edasi, purke kogutud "<<purk<<std::endl;
// 				std::cout<<kaugus<<std::endl;
			}
			else if ((pindala > 10000) && (pindala < 45000))
			{
				kinni = 0;
				edasi(10);
				kinni3++;
				if (kinni3 > 50)
				{
					paremale_tagasi(5);
					viivitus(1000);
					continue;
				}
// 				std::cout<<"edasi, purke kogutud "<<purk<<std::endl;
// 				std::cout<<kaugus<<std::endl;
			}
			else 
			{
				kinni = 0;
				edasi(3);
// 				std::cout<<"edasi, purke kogutud "<<purk<<std::endl;
// 				std::cout<<kaugus<<std::endl;
			}
// 			std::cout<<"uuesti purgi peale"<<std::endl;
			continue;
		}
		else if (suurimSokk != 0 && sokk < 2)
		{
			if (getAnalog(4) > 950)
			{
				setServo(5,255);
				edasi(7);
				juust++;
				if (juust > 20)
				{
					tagasi(3);
					viivitus(1000);
					juust = 0;
				}
				tumba = 1;
				std::cout<<"imeb sokki"<<std::endl;
			}
			if ((tumba == 1) && (getAnalog(4) < 970))
			{
				sokk++;
				std::cout<<"sokke korjatud "<<sokk<<std::endl;
				tumba = 0;
			}
			while ((getAnalog(0) > 330) && getAnalog(1) > 330)               //sein on ees
			{
				kinni = 0;
				kinni2 = 0;
				kinni3 = 0;
				kontinue = 1;
				paremale_tagasi(15);
				viivitus(400);
				paremale_edasi(15);
				viivitus(400);
			}
			if (kontinue == 1)
			{
				kontinue = 0;
				continue;
			}
			if (suurimSokk->area < 3000)
			{
				if (suurimSokk->distanceH < -40)
				{
					kinni = 0;
					kinni2 = 0;
					kinni3 = 0;
					aeglaselt_vasakule();
					std::cout<<" vasakule soki peale"<<std::endl;
				}
				else if (suurimSokk->distanceH > 40)
				{
					kinni = 0;
					kinni2 = 0;
					kinni3 = 0;
					aeglaselt_paremale();
					std::cout<<" paremale soki peale"<<std::endl;
				}
			}
			else if (suurimSokk->distanceH < -40)
			{
				kinni = 0;
				kinni2 = 0;
				kinni3 = 0;
				vasakule_edasi(2);
			}
			else if (suurimSokk->distanceH > 40)
			{
				kinni = 0;
				kinni2 = 0;
				kinni3 = 0;
				paremale_edasi(2);
			}
			else 
			{
				kinni = 0;
				edasi(10);
				std::cout<<"OTSE soki peale"<<std::endl;
			}
		}
                else if (purk >= 6 && sokk >= 2)
		{
			if (canBeacon != 0)
			{
				int vasak = getAnalog(3);
				int parem = getAnalog(2);
				while (((getAnalog(0) > 310) || getAnalog(1) > 310) && (parem < 200 && vasak < 200) && canBeacon->rect.height < 120)    //kataloog on purgi ja pelmeeni vahel
				{
					if (getAnalog(0) < getAnalog(1))			                   //kui takistus on rohkem vasakul analog(1) on vasak
					{
						kohapeal_paremale(6);
	// 					std::cout<<"kohapeal paremale "<<std::endl;
						arv++;
						suund = 0;
						otsing = 0;
						loog = 1;
					}
					else if (getAnalog(0) > getAnalog(1))					                //kui takistus on rohkem paremal
					{
						kohapeal_vasakule(6);
	// 					std::cout<<"kohapeal vasakule"<<std::endl;
						arv++;
						suund = 1;
						otsing = 1;
						loog = 1;
					}
					else if (arv > 50)
					{
						paremale_tagasi(10);
						viivitus(1000);
						arv = 0;
					}
					kontinue = 1;
				}
				if (kontinue == 1)
				{
					kontinue = 0;
					continue;
				}
				while ((getAnalog(0) > 310) && getAnalog(1) > 310)               //sein on ees
				{
					paremale_tagasi(5);
					viivitus(1000);
					kontinue = 1;
				}
				if (kontinue == 1)
				{
					kontinue = 0;
					continue;
				}
				if (loog == 1)
				{
					if (suund == 0)
					{
						while (getAnalog(3) > 200)
						{
							edasi(10);
							muna++;
							if (muna > 80)
							{
								muna = 0;
								break;
							}
						}
						edasi(10);
						viivitus(300);
						kala = 0;
						loog = 0;
						continue;
					}
					else if (suund == 1)
					{
						while (getAnalog(2) > 200)
						{
							edasi(10);
							muna++;
							if (muna > 80)
							{
								muna = 0;
								break;
							}
						}
						edasi(10);				//veel jupp maad edasi et oleks parem pöörata
						viivitus(300);
						kala = 0;
						loog = 0;
						continue;
					}
				}
				if (PBpindala > 6000)
				{
					purgidmaha();
					purk = 5;
					continue;
				}
				else if (PBpindala < 6000 && PBpindala > 1500)
				{
					if (PBkaugus < -40)
					{
						kiiremini_vasakule();
						toru = 0;
		// 							std::cout<<"aeglaselt_vasakule"<<std::endl
					}
					else if (PBkaugus > 40)
					{
						kiiremini_paremale();
						toru = 0;
		// 							std::cout<<"aeglaselt_vasakule"<<std::endl
					}
				}
				else if (PBpindala < 1500)
				{
					if (PBkaugus < -40)
					{
						aeglaselt_vasakule();
						toru = 0;
		// 							std::cout<<"aeglaselt_vasakule"<<std::endl
					}
					else if (PBkaugus > 40)
					{
						aeglaselt_paremale();
						toru = 0;
		// 							std::cout<<"aeglaselt_vasakule"<<std::endl
					}
				}
				else if ((PBpindala < 1000) && (getAnalog(1) > 200 || getAnalog(0) > 200))
				{
					edasi(10);
					toru = 0;
	// 				std::cout<<"edasi, purke kogutud "<<purk<<std::endl;
				}
				else if (PBpindala < 1000)
				{
					edasi(20);
					toru = 0;
				}
				else 
				{
					edasi(5);
					toru = 0;
	// 							std::cout<<"edasi"<<std::endl;
				}
			}
			else
			{	
// 				std::cout<<"otsin majukat"<<std::endl;
				otsi_majakat();
				toru++;
				if (toru > 40)
				{
					edasi(5);
					viivitus(1000);
					toru = 0;
					continue;
				}
			}
		}
		else if (sokk >= 2 )
		{
			if (sockBeacon != 0)
			{
				int vasak = getAnalog(3);
				int parem = getAnalog(2);
				while (((getAnalog(0) > 320) || getAnalog(1) > 320) && (parem < 200 && vasak < 200) && canBeacon->rect.height < 120)    //kataloog on purgi ja pelmeeni vahel
				{
					if (getAnalog(0) < getAnalog(1))			                   //kui takistus on rohkem vasakul analog(1) on vasak
					{
						kohapeal_paremale(4);
	// 					std::cout<<"kohapeal paremale "<<std::endl;
						arv++;
						suund = 0;
						otsing = 0;
						loog = 1;
					}
					else if (getAnalog(0) > getAnalog(1))					                //kui takistus on rohkem paremal
					{
						kohapeal_vasakule(4);
	// 					std::cout<<"kohapeal vasakule"<<std::endl;
						arv++;
						suund = 1;
						otsing = 1;
						loog = 1;
					}
					else if (arv > 50)
					{
						paremale_tagasi(10);
						viivitus(1000);
					}
					kontinue = 1;
				}
				if (kontinue == 1)
				{
					kontinue = 0;
					continue;
				}
				while ((getAnalog(0) > 310) && getAnalog(1) > 310)               //sein on ees
				{
					paremale_tagasi(5);
					viivitus(1000);
					kontinue = 1;
				}
				if (kontinue == 1)
				{
					kontinue = 0;
					continue;
				}
				if (loog == 1)
				{
					if (suund == 0)
					{
						while (getAnalog(3) > 200)
						{
							edasi(10);
							muna++;
							if (muna > 80)
							{
								muna = 0;
								break;
							}
						}
						edasi(10);
						viivitus(300);
						kala = 0;
						loog = 0;
						continue;
					}
					else if (suund == 1)
					{
						while (getAnalog(2) > 200)
						{
							edasi(10);
							muna++;
							if (muna > 80)
							{
								muna = 0;
								break;
							}
						}
						edasi(10);				//veel jupp maad edasi et oleks parem pöörata
						viivitus(300);
						kala = 0;
						loog = 0;
						continue;
					}
				}
				if (sockBeacon->area > 4900)
				{
					stop();
					viivitus(5000);
					continue;
				}
				else if (sockBeacon->area < 4900 && sockBeacon->area > 1500)
				{
					if (sockBeacon->distanceH < -40)
					{
						kiiremini_vasakule();
						toru = 0;
		// 							std::cout<<"aeglaselt_vasakule"<<std::endl
					}
					else if (sockBeacon->distanceH > 40)
					{
						kiiremini_paremale();
						toru = 0;
		// 							std::cout<<"aeglaselt_vasakule"<<std::endl
					}
				}
				else if (sockBeacon->area < 1500)
				{
					if (sockBeacon->distanceH < -40)
					{
						aeglaselt_vasakule();
						toru = 0;
		// 							std::cout<<"aeglaselt_vasakule"<<std::endl
					}
					else if (sockBeacon->distanceH > 40)
					{
						aeglaselt_paremale();
						toru = 0;
		// 							std::cout<<"aeglaselt_vasakule"<<std::endl
					}
				}
				else if ((sockBeacon->area < 1000) && (getAnalog(1) > 200 || getAnalog(0) > 200))
				{
					edasi(5);
					toru = 0;
	// 				std::cout<<"edasi, purke kogutud "<<purk<<std::endl;
				}
				else if (sockBeacon->area < 1000)
				{
					edasi(10);
					toru = 0;
				}
				else 
				{
					edasi(5);
					toru = 0;
	// 							std::cout<<"edasi"<<std::endl;
				}
			}
			else
			{	
// 				std::cout<<"otsin majukat"<<std::endl;
				otsi_majakat();
				toru++;
				if (toru > 40)
				{
					edasi(5);
					viivitus(1000);
					toru = 0;
					continue;
				}
			}
		}
		else if (purk < 6)
		{
// 			std::cout<<"otsin bürke"<<std::endl;
			if (suund == 1)
			{
				otsi_paremale();
				kinni++;
				if (kinni > 50)
				{
					edasi(5);
					viivitus(1000);
					kinni = 0;
					continue;
				}
			}
			else if (suund == 0)
			{
				otsi_vasakule();
				kinni++;
				if (kinni > 50)
				{
					edasi(5);
					viivitus(1000);
					kinni = 0;
					continue;
				}
			}
		}
		else
		{	
// 			std::cout<<"otsin majukat"<<std::endl;
			otsi_majakat();
			toru++;
			if (toru > 40)
			{
				edasi(5);
				viivitus(1000);
				toru = 0;
				continue;
			}
		}
	}


	while(1)
	{
		aeg = t.elapsed();  // stardist möödunud aeg millisekundites
// 		edasi(10);
// 		otsi_paremale();
// 		std::cout<<getAnalog(0)<<"  parem ees"<<std::endl;
// 		std::cout<<getAnalog(1)<<"  vasak ees"<<std::endl;
// 		std::cout<<getAnalog(2)<<"  parem kõrval"<<std::endl;
// 		std::cout<<getAnalog(3)<<"  vasak kõrval"<<std::endl;
  
		_img = image->getFrame();
		_img = image->getFrame();
		image->process(1,1,1,1);
		canBeacon = 0;
		suurimObj = 0;
		sockBeacon = 0;
		suurimSokk = 0;
		//kui leidis midagi vaateväljalt
		if (image->found_objects != NULL)
		{
			// käime üle kõik leitud objektid ja valime neist suurima purgi
			for (i = 0; i < image->found_objects->total; i++)
			{
				obj = (Image::Object *) cvGetSeqElem( (image->found_objects), i);
				if (obj != 0)
				{
					/*if (obj->type == CAN && purk < 6) 
					{
						//leidsime purgi, vaatame kas on suurim
						if (!suurimObj || (obj->area > suurimObj->area))
						{	
// 							std::cout<<"vahetasin objekti"<<std::endl;
							suurimObj = obj;
							std::cout<<((suurimObj->rect.width))<<std::endl;
							std::cout<<((suurimObj->distanceH))<<std::endl;
							std::cout<<((suurimObj->area))<<std::endl;
						}
					}
					else*/ if (obj->type == SOCK) 
					{
						//leidsime purgi, vaatame kas on suurim
						if (!suurimSokk || (obj->area > suurimSokk->area))
						{	
							std::cout<<"mingi värdjas juust"<<std::endl;
							suurimSokk = obj;
							Skaugus = suurimSokk->distanceH;
							Spindala = suurimSokk->area;
							if (Skaugus < 0)
							{
								majakasuund = 0;
							}
							else
							{
								majakasuund = 1;
							}
// 							std::cout<<((suurimObj->area))<<std::endl;
						}
					}
// 					else if ((obj->type == SOCK_BEACON) && (purk >= 6))			//purgi piikon
// 					{
// 						if (!canBeacon || (obj->area > canBeacon->area))
// 						{
// // 							std::cout<<"leidsin piikoni"<<std::endl;
// 							canBeacon = obj;
// // 							std::cout<<((canBeacon->area))<<std::endl;
// 						}
// 					}
					else if ((obj->type == SOCK_BEACON) && sokk >= 2)					//soki piikon
					{
						if (!sockBeacon || (obj->area > sockBeacon->area))
						{
// 							std::cout<<"leidsin piikoni"<<std::endl;
							sockBeacon = obj;
// 							std::cout<<((canBeacon->area))<<std::endl;
						}
					}
				}
			}
		}
// 		if ((suurimObj != 0 && suurimSokk != 0 && ((suurimObj->area*1.5) > suurimSokk->area)) || (suurimObj != 0 && suurimSokk == 0))
// 		{
// 			std::cout<<"sõidan purgi peale"<<std::endl;
// // 			std::cout<<"sokipindala"<<(suurimSokk->area)<<std::endl;
// 			std::cout<<"purgipindala"<<((suurimObj->area)*1.5)<<std::endl;
// 		}
// 		else 
		if (suurimSokk != 0)
		{
			if (getAnalog(4) > 70)
			{
				sokkyles();
				tumba = 1;
				std::cout<<"imeb sokki"<<std::endl;
				continue;
			}
			if ((tumba == 1) && (getAnalog(4) < 70))
			{
				sokk++;
				std::cout<<"sokke korjatud "<<sokk<<std::endl;
				tumba = 0;
			}
			while ((getAnalog(0) > 330) && getAnalog(1) > 330)               //sein on ees
			{
				kinni = 0;
				kinni2 = 0;
				kinni3 = 0;
				kontinue = 1;
				paremale_tagasi(15);
				viivitus(400);
				paremale_edasi(15);
				viivitus(400);
			}
			if (kontinue == 1)
			{
				kontinue = 0;
				continue;
			}
			if (suurimSokk->area < 3000/* && suurimSokk->rect.width < 400*/)
			{
				if (suurimSokk->distanceH < -40)
				{
					kinni = 0;
					kinni2 = 0;
					kinni3 = 0;
					aeglaselt_vasakule();
					std::cout<<" vasakule soki peale"<<std::endl;
				}
				else if (suurimSokk->distanceH > 40)
				{
					kinni = 0;
					kinni2 = 0;
					kinni3 = 0;
					aeglaselt_paremale();
					std::cout<<" paremale soki peale"<<std::endl;
				}
			}
// 			else if (suurimSokk->area < 3000 && suurimSokk->rect.width > 400)
// 			{
// 				if (suurimSokk->distanceH < -130)
// 				{
// 					kinni = 0;
// 					kinni2 = 0;
// 					kinni3 = 0;
// 					aeglaselt_vasakule();
// 					std::cout<<" vasakule soki peale"<<std::endl;
// 				}
// 				else if (suurimSokk->distanceH > -90)
// 				{
// 					kinni = 0;
// 					kinni2 = 0;
// 					kinni3 = 0;
// 					aeglaselt_paremale();
// 					std::cout<<" paremale soki peale"<<std::endl;
// 				}
// 			}
			else if (suurimSokk->distanceH < -40 /*&& suurimSokk->rect.width < 400*/)
			{
				kinni = 0;
				kinni2 = 0;
				kinni3 = 0;
				kiiremini_vasakule();
				std::cout<<"kiiremini vasakule sokk"<<std::endl;
			}
			else if (suurimSokk->distanceH > 40/* && suurimSokk->rect.width < 400*/)
			{
				kinni = 0;
				kinni2 = 0;
				kinni3 = 0;
				kiiremini_paremale();
				std::cout<<"kiiremini paremale sokk"<<std::endl;
			}
// 			else if (suurimSokk->distanceH < -130 && suurimSokk->rect.width > 400)
// 			{
// 				kinni = 0;
// 				kinni2 = 0;
// 				kinni3 = 0;
// 				kiiremini_vasakule();
// 			}
// 			else if (suurimSokk->distanceH > -90 && suurimSokk->rect.width > 400)
// 			{
// 				kinni = 0;
// 				kinni2 = 0;
// 				kinni3 = 0;
// 				kiiremini_paremale();
// 			}
			else 
			{
				kinni = 0;
				edasi(10);
				std::cout<<"OTSE soki peale"<<std::endl;
			}
// 			std::cout<<"sõidan soki peale"<<std::endl;
		}
		else if (sockBeacon != 0)
		{
// 			int vasak = getAnalog(3);
// 			int parem = getAnalog(2);
// 			while (((getAnalog(0) > 320) || getAnalog(1) > 320) && (parem < 200 && vasak < 200) && canBeacon->rect.height < 120)    //kataloog on purgi ja pelmeeni vahel
// 			{
// 				if (getAnalog(0) < getAnalog(1))			                   //kui takistus on rohkem vasakul analog(1) on vasak
// 				{
// 					kohapeal_paremale(4);
// // 					std::cout<<"kohapeal paremale "<<std::endl;
// 					arv++;
// 					suund = 0;
// 					otsing = 0;
// 					loog = 1;
// 				}
// 				else if (getAnalog(0) > getAnalog(1))					                //kui takistus on rohkem paremal
// 				{
// 					kohapeal_vasakule(4);
// // 					std::cout<<"kohapeal vasakule"<<std::endl;
// 					arv++;
// 					suund = 1;
// 					otsing = 1;
// 					loog = 1;
// 				}
// 				else if (arv > 50)
// 				{
// 					paremale_tagasi(10);
// 					viivitus(1000);
// 				}
// 				kontinue = 1;
// 			}
// 			if (kontinue == 1)
// 			{
// 				kontinue = 0;
// 				continue;
// 			}
// 			while ((getAnalog(0) > 310) && getAnalog(1) > 310)               //sein on ees
// 			{
// 				paremale_tagasi(5);
// 				viivitus(1000);
// 				kontinue = 1;
// 			}
// 			if (kontinue == 1)
// 			{
// 				kontinue = 0;
// 				continue;
// 			}
// 			if (loog == 1)
// 			{
// 				if (suund == 0)
// 				{
// 					while (getAnalog(3) > 200)
// 					{
// 						edasi(10);
// 						muna++;
// 						if (muna > 80)
// 						{
// 							muna = 0;
// 							break;
// 						}
// 					}
// 					edasi(10);
// 					viivitus(300);
// 					kala = 0;
// 					loog = 0;
// 					continue;
// 				}
// 				else if (suund == 1)
// 				{
// 					while (getAnalog(2) > 200)
// 					{
// 						edasi(10);
// 						muna++;
// 						if (muna > 80)
// 						{
// 							muna = 0;
// 							break;
// 						}
// 					}
// 					edasi(10);				//veel jupp maad edasi et oleks parem pöörata
// 					viivitus(300);
// 					kala = 0;
// 					loog = 0;
// 					continue;
// 				}
// 			}
			if (sockBeacon->area > 3000)
			{
				if (sockBeacon->distanceH < -5)
				{
					kohapeal_vasakule(1);
					toru = 0;
					continue;
			// 					std::cout<<"aeglaselt_vasakule"<<std::endl
				}
				else if (sockBeacon->distanceH > 5)
				{
					kohapeal_paremale(1);
					toru = 0;
					continue;
			// 					std::cout<<"aeglaselt_vasakule"<<std::endl
				}
				sokidmaha();
				stop();
				break;
			}
			else if (sockBeacon->area < 3000 && sockBeacon->area > 1500)
			{
				if (sockBeacon->distanceH < -40)
				{
					kiiremini_vasakule();
					std::cout<<"kiiremini vasakule majakas"<<std::endl;
					toru = 0;
	// 							std::cout<<"aeglaselt_vasakule"<<std::endl
				}
				else if (sockBeacon->distanceH > 40)
				{
					kiiremini_paremale();
					std::cout<<"kiiremini paremale majakas"<<std::endl;
					toru = 0;
	// 							std::cout<<"aeglaselt_vasakule"<<std::endl
				}
			}
			else if (sockBeacon->area < 1500)
			{
				if (sockBeacon->distanceH < -40)
				{
					aeglaselt_vasakule();
					std::cout<<"aeglaselt vasakule majakas"<<std::endl;
					toru = 0;
	// 							std::cout<<"aeglaselt_vasakule"<<std::endl
				}
				else if (sockBeacon->distanceH > 40)
				{
					aeglaselt_paremale();
					std::cout<<"aeglaselt paremale majakas"<<std::endl;
					toru = 0;
	// 							std::cout<<"aeglaselt_vasakule"<<std::endl
				}
			}
// 			else if ((sockBeacon->area < 1500) && (getAnalog(1) > 200 || getAnalog(0) > 200))
// 			{
// 				edasi(5);
// 				std::cout<<"edasi majaka peale"<<std::endl;
// 				toru = 0;
// // 				std::cout<<"edasi, purke kogutud "<<purk<<std::endl;
// 			}
// 			else if (sockBeacon->area < 1500)
// 			{
// 				edasi(10);
// 				std::cout<<"edasi majaka peale"<<std::endl;
// 				toru = 0;
// 			}
			else 
			{
				edasi_kiiresti();
				toru = 0;
				std::cout<<"edasi majaka peale"<<std::endl;
			}
		}
		else
		{	
			std::cout<<"otsin midagi"<<std::endl;
			if (majakasuund == 1)
			{
				otsi_paremale();
				kinni++;
				if (kinni > 50)
				{
					edasi(5);
					viivitus(1000);
					kinni = 0;
					continue;
				}
			}
			else if (majakasuund == 0)
			{
				otsi_vasakule();
				kinni++;
				if (kinni > 50)
				{
					edasi(5);
					viivitus(1000);
					kinni = 0;
					continue;
				}
			}
			toru++;
			if (toru > 60)
			{
				edasi(5);
				viivitus(1000);
				toru = 0;
				continue;
			}
		}
		view->show(_img);
	}
	
}

