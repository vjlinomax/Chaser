/*
  ==============================================================================

    Achievement.h
    Created: 5 Mar 2018 6:53:39pm
    Author:  Joris

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "AchievementComponent.h"

class Achievement
{
public:
	Achievement( String name, String description, int limit );
	~Achievement();

	void setValuesFromXml( XmlElement* xml );

	/* keeps score and shows its popup when necessarry */
	void trigger();
	void reset();

	static File getAchievementsFile();
	int id;

private:
	
	bool hasTriggered;
	int count;
	void writeToXml();

	int limit;
	String name;
	String description;
};
