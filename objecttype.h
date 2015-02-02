//
// C++ Interface: objecttype
//
// Description: 
//
//
// Author: Valdur Kaldvee, Margus Ernits <margus.ernits@itcollege.ee>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef OBJECTTYPE_H
#define OBJECTTYPE_H

/**
	@author Valdur Kaldvee, Margus Ernits <margus.ernits@itcollege.ee>
*/
class ObjectType{
public:
    ObjectType();

    ~ObjectType();
    int hMin;
    int hMax;
    int sMin;
	int sMax;
    int lMin;
    int lMax;
};

#endif
