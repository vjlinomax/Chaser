/*
  ==============================================================================

    AchievementManager.h
    Created: 5 Mar 2018 6:51:17pm
    Author:  Joris

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "Achievements.h"

class AchievementManager
{
public:
	AchievementManager();
	~AchievementManager();

	//Achievement* getAchievement(  Achievements::Achievement achievement );
	Achievement* getAchievement( Achievement achievement );

	
private:
	OwnedArray<Achievement> achievements;
};