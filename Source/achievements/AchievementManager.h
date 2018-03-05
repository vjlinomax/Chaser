/*
  ==============================================================================

    AchievementManager.h
    Created: 5 Mar 2018 6:51:17pm
    Author:  Joris

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "Achievement.h"

class AchievementManager
{
public:
	AchievementManager();
	~AchievementManager();

	Achievement* getAchievement( Achievement::Types achievement );

	
private:
	OwnedArray<Achievement> achievements;
};