/*
  ==============================================================================

    AutoUpdateTimer.cpp
    Created: 12 Oct 2016 8:36:04pm
    Author:  Joris

  ==============================================================================
*/

#include "AutoUpdateTimer.h"


AutoUpdateTimer::AutoUpdateTimer( ChaserCreator* creator ) :
creator( creator )
{
	lastModifiedTime = ChaserXmlParser::getLastUpdateTimeForActiveAssFile();
}

AutoUpdateTimer::~AutoUpdateTimer()
{

}

void AutoUpdateTimer::start()
{
	startTimerHz( 1 );

	//jtodo write to xml
}

void AutoUpdateTimer::stop()
{
	stopTimer();

	//jtodo write to xml
}

void AutoUpdateTimer::timerCallback()
{
	//check if the file has been modified
	Time checkTime = ChaserXmlParser::getLastUpdateTimeForActiveAssFile();

	//if we didn't get a valid time, 
	//maybe because the file doesn't exist
	//stop the timer
	if ( checkTime.toMilliseconds() == 0 )
	{
		stop();
		return;
	}

	if ( checkTime > lastModifiedTime )
	{
		lastModifiedTime = checkTime;
		File assFile;
		ChaserXmlParser::parseAssFile( FileHelper::getLastUsedChaserFile(), assFile );
		creator->createChaserFromAssFile( assFile, false );
	}
}
