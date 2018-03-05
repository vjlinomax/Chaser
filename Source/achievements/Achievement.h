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
		Empty,
		Darklord
	};

	Achievement( Types type );
	Achievement ( XmlElement* xml );
	~Achievement();

	void add();
	void trigger();

	Types getType();

	static File getAchievementsFile();
	
private:
	Types type;
	bool hasTriggered;
	int count;
	void writeToXml();

};
