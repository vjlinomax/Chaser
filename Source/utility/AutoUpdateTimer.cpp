/*
  ==============================================================================

    AutoUpdateTimer.cpp
    Created: 12 Oct 2016 8:36:04pm
    Author:  Joris

  ==============================================================================
*/

#include "AutoUpdateTimer.h"
#include "../../HybridApi/Source/JuceBased/Fileless/FileLess.h"


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
}

void AutoUpdateTimer::stop()
{
	stopTimer();
}

void AutoUpdateTimer::timerCallback()
{
	//check if the file has been modified
	Time checkTime = ChaserXmlParser::getLastUpdateTimeForActiveAssFile();

	//if we didn't get a valid time, 
	//maybe because the file doesn't exist
	if ( checkTime.toMilliseconds() == 0 )
	{
		return;
	}

	if ( checkTime > lastModifiedTime )
	{
		lastModifiedTime = checkTime;
		File assFile;
		ChaserXmlParser::parseAssFile( FileLess::getLastUsedFileName( FileLess::Chaser ), assFile );
		creator->createChaserFromAssFile( assFile, false );
	}
}
