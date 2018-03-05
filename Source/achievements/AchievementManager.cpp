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
	//fetch achievements that have been written before
	ScopedPointer<XmlElement> mainElement = XmlDocument::parse( Achievement::getAchievementsFile() );
	if ( mainElement )
		forEachXmlChildElement( *mainElement, achievementXml )
		achievements.add( new Achievement( achievementXml ) );
}

AchievementManager::~AchievementManager()
{
}

Achievement* AchievementManager::getAchievement( Achievement::Types type )
{
	for ( Achievement* achievement : achievements )
		if ( achievement->getType() == type )
			return achievement;

	Achievement* newAchievement = new Achievement( type );
	achievements.add( newAchievement );
	return newAchievement;
}
