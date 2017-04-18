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
	Preview* preview, SliceList* sliceList ) :
	sliceManager( sliceManager ),
	chaseManager( chaseManager ),
	previewWindow( preview ),
	sliceList( sliceList )
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

	//at this point, all the slices have their position and screens assigned
	//so we can save this to xml
	sliceManager->writeToXml();

	//see if we need to reload an existing chaser or create a fresh one
	if ( createNew )
		chaseManager->clearAll();

	//if we don't create a new slicelist, we can cause a crash in the plugin at this point
	//if we removed a slice in the assfile
	//the chasemanager can have that slice stored in a chase, while the slice does not exist anymore in the slicelist
	chaseManager->removeDeletedSlices( sliceManager->getSlices() );

	//this needs to be here, in order for the window to be properly resized
	//when the dimension change
	previewWindow->getParentComponent()->resized();
}

bool ChaserCreator::createChaserFromChaserFile( bool giveFeedback )
{
	//check if we have a Chaser file previously saved
	//if so, load that bad boy
	File lastUsedChaser = FileHelper::getLastUsedChaserFile();

	return createChaserFromChaserFile( lastUsedChaser, giveFeedback );

}

bool ChaserCreator::createChaserFromChaserFile( File chaserToLoad, bool giveFeedback )
{
	//first check if the file we want to load is valid
	//this will throw an error message
	if ( !FileHelper::isFileValid( chaserToLoad, giveFeedback ) )
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
		if ( giveFeedback )
			FileHelper::throwLoadError();
		return false;
	}

	//now populate the previewwindow with buttons for these slices
	previewWindow->createSliceButtons();

	//now populate the slicelist with entries for these slices
	sliceList->setSlices();

	//this will try its best to get useful info from the chaserfile
	chaseManager->createSequencesFromXml( ChaserXmlParser::parseSequences( chaserToLoad ) );

	//this needs to be here, in order for the window to be properly resized
	//when the dimension change
	previewWindow->getParentComponent()->resized();

	return true;
}
