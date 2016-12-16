/*
  ==============================================================================

  ChaserCreator.cpp
  Created: 12 Oct 2016 8:40:03pm
  Author:  Joris

  ==============================================================================
  */

#include "ChaserCreator.h"
#include "../../HybridApi/Source/HybridApi.h"

ChaserCreator::ChaserCreator( SliceManager* sliceManager, ChaseManager* chaseManager, 
								Preview* preview, SliceList* sliceList, Sequencer* sequencer ) :
sliceManager( sliceManager ),
chaseManager( chaseManager ),
previewWindow( preview ),
sliceList( sliceList ),
sequencer( sequencer )
{

}

ChaserCreator::~ChaserCreator()
{

}

void ChaserCreator::createChaserFromAssFile( File assFile, bool createNew )
{
	ResXmlParser::parseAssFile( assFile, sliceManager->getSlices(), sliceManager->getScreens(), sliceManager->getResolution() );
	sliceManager->getAssFile() = assFile;

	//now populate the previewwindow with buttons for these slices
	previewWindow->createSliceButtons();

	//now populate the slicelist with entries for these slices
	//because sliceList has a pointer to sliceManager, 
	//I don't need to pass anything in
	sliceList->setSlices();

	//now redraw maincomponent, so the previewwindow is fitted and slicelist updated
	previewWindow->getParentComponent()->resized();

	//at this point, all the slices have their position and screens assigned
	//so we can save this to xml
	sliceManager->writeToXml();

	//see if we need to reload an existing chaser or create a fresh one
	if ( createNew )
		chaseManager->clearAll();

	//make the first step active
	sequencer->selectStep( 0 );
}

bool ChaserCreator::createChaserFromChaserFile()
{
	//check if we have a Chaser file previously saved
	//if so, load that bad boy
	File lastUsedChaser = FileHelper::getLastUsedChaserFile();

	if ( FileHelper::isFileValid( lastUsedChaser ) )
		return createChaserFromChaserFile( lastUsedChaser );

	DBG( "Not a valid file!" );
	return false;
}

bool ChaserCreator::createChaserFromChaserFile( File chaserToLoad )
{
	//first check if the file we want to load is valid
	//this will throw an error message
	if ( !FileHelper::isFileValid( chaserToLoad ) )
		return false;

	//now check if we can load this version
	//this will throw an error message
	if ( !ChaserXmlParser::canThisAppVersionLoadThisChaser( chaserToLoad, ProjectInfo::versionString ) )
		return false;

	if ( !ChaserXmlParser::parseResolution( chaserToLoad, sliceManager->getResolution() )
		|| !ChaserXmlParser::parseAssFile( chaserToLoad, sliceManager->getAssFile() )
		|| !ChaserXmlParser::parseScreens( chaserToLoad, sliceManager->getScreens() )
		|| !ChaserXmlParser::parseSlices( chaserToLoad, sliceManager->getSlices() ) )
	{
		FileHelper::throwLoadError();
		return false;
	}

	//now populate the previewwindow with buttons for these slices
	previewWindow->createSliceButtons();

	//now populate the slicelist with entries for these slices
	sliceList->setSlices();

	//now redraw maincomponent, so the previewwindow is fitted and slicelist updated
	previewWindow->getParentComponent()->resized();

	//this will try its best to get useful info from the chaserfile
	chaseManager->createSequencesFromXml( ChaserXmlParser::parseSequences( chaserToLoad ) );

	previewWindow->setActiveSlices();

	//make the first step active
	sequencer->selectStep( 0 );

	return true;
}
