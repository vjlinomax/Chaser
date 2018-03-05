/*
  ==============================================================================

    Achievement.h
    Created: 5 Mar 2018 6:53:39pm
    Author:  Joris

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"



class Achievement
{
public:
	enum Types
	{
		Darklord
	};

	Achievement( Types type );
	~Achievement();

	void add();
	void trigger();

	Types getType();
	
private:
	Types type;
	int count;
	void writeToXml();

};
