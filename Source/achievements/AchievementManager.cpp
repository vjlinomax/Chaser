/*
  ==============================================================================

	AchievementManager.cpp
	Created: 5 Mar 2018 6:51:17pm
	Author:  Joris

  ==============================================================================
*/

#include "AchievementManager.h"

AchievementManager::AchievementManager()
{
}

AchievementManager::~AchievementManager()
{
}

Achievement* AchievementManager::getAchievement( Achievement::Types type )
{
	for ( Achievement* achievement : achievements )
		if ( achievement->getType() == type )
			return achievement;
	
	return new Achievement( type );
}
