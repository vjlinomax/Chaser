/*
  ==============================================================================

	Achievement.cpp
	Created: 5 Mar 2018 6:53:39pm
	Author:  Joris

  ==============================================================================
*/

#include "Achievement.h"

Achievement::Achievement( Types type ) : type( type )
{
	hasTriggered = false;
	count = 0;
}

Achievement::Achievement( XmlElement * xml )
{
	type = Achievement::Types( xml->getIntAttribute( "type" ) );
	hasTriggered = xml->getBoolAttribute( "triggered", false );
	count = xml->getIntAttribute( "count", 0 );
}

Achievement::~Achievement()
{
}


void Achievement::add()
{
	count++;
	writeToXml();
}

void Achievement::trigger()
{
	if ( !hasTriggered )
	{
		switch ( type )
		{
		case Types::Darklord:
			AlertWindow::showMessageBox( AlertWindow::AlertIconType::WarningIcon,
				"Greetings, Hellchild!",
				"You have set your sequence to exactly 666 steps. You are truly the spawn of Satan.",
				"All Hail!", nullptr );
		}
		hasTriggered = true;
	}

	writeToXml();
}

Achievement::Types Achievement::getType()
{
	return type;
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
		if ( Types( achievementXml->getIntAttribute( "type" ) ) == type )
			elementToWrite = achievementXml;

	if ( !elementToWrite )
	{
		elementToWrite = new XmlElement( "achievement" );
		elementToWrite->setAttribute( "type", int( type ) );
		mainElement->addChildElement( elementToWrite );
	}

	elementToWrite->setAttribute( "count", count );
	elementToWrite->setAttribute( "triggered", hasTriggered );

	if ( !mainElement->writeToFile( getAchievementsFile(), "" ) )
		DBG( "Error writing achievement!" ); //don't bother the user with this
}
