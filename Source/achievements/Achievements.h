/*
  ==============================================================================

	Achievements.h
	Created: 7 Mar 2018 6:29:49pm
	Author:  Joris

  ==============================================================================
*/

#pragma once

#include "Achievement.h"

class Achievements
{
public:
	static const Achievement Darklord;
	static const Achievement Skidoo;
	static const Achievement FirstBorn;
	static const Achievement MakeUpYourMind;

	static Achievement* getAchievement( int hash );
private:
	Achievements();
};
