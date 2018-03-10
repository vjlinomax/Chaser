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
	{
		if ( Achievement* newAchievement = Achievements::getAchievement( achievementXml->getStringAttribute( "id" ).getIntValue() ) )
		{
			newAchievement->setValuesFromXml( achievementXml );
			achievements.add( newAchievement );
		}
	}
}

AchievementManager::~AchievementManager()
{
}

Achievement* AchievementManager::getAchievement( Achievement achievement )
{
	//check if this achievement is already in the list
	for ( Achievement* existing : achievements )
		if ( achievement.id == existing->id )
			return existing;

	Achievement* newAchievement = new Achievement( achievement );
	achievements.add( newAchievement );
	return newAchievement;
}

