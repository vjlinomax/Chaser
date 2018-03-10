/*
  ==============================================================================

	Achievement.cpp
	Created: 5 Mar 2018 6:53:39pm
	Author:  Joris

  ==============================================================================
*/

#include "Achievement.h"


void Achievement::setValuesFromXml( XmlElement * xml )
{
	hasTriggered = xml->getBoolAttribute( "triggered", false );
	count = xml->getIntAttribute( "count", 0 );
}

Achievement::Achievement( String name, String description, int limit ) 
	: name( name ), description( description ), limit( limit )
{
	id = name.hashCode();
}

Achievement::~Achievement()
{

}


void Achievement::trigger()
{
#ifdef DEBUG
	if ( /*!hasTriggered &&*/ count >= limit )
	{
		AchievementComponent* window;
		window = new AchievementComponent();
		window->setText( name, description );

		hasTriggered = true;
	}

	count++;
	writeToXml();
#endif
}

void Achievement::reset()
{
	count = 0;
}

File Achievement::getAchievementsFile()
{
	File achievementsFile = File( File::getSpecialLocation( File::SpecialLocationType::userApplicationDataDirectory ).getFullPathName() + "/Chaser/achievements.xml" );
	if ( !achievementsFile.exists() )
		achievementsFile.create();

	return achievementsFile;
}

void Achievement::writeToXml()
{
	ScopedPointer<XmlElement> mainElement = XmlDocument::parse( getAchievementsFile() );
	if ( !mainElement )
		mainElement = new XmlElement( "achievements" );

	XmlElement* elementToWrite = nullptr;
	forEachXmlChildElement( *mainElement, achievementXml )
		if ( achievementXml->getIntAttribute( "id" ) == id )
			elementToWrite = achievementXml;

	if ( !elementToWrite )
	{
		elementToWrite = new XmlElement( "achievement" );
		elementToWrite->setAttribute( "id", id );
		mainElement->addChildElement( elementToWrite );
	}

	elementToWrite->setAttribute( "count", count );
	elementToWrite->setAttribute( "triggered", hasTriggered );

	if ( !mainElement->writeToFile( getAchievementsFile(), "" ) )
		DBG( "Error writing achievement!" ); //don't bother the user with this
}
