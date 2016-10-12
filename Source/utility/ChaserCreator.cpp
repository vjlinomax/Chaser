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
	ResXmlParser::parseAssFile( assFile, sliceManager->getSlices(), sliceManager->getResolution() );
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
	{
		//this will return 1920x1080 if no resolution was saved
		ChaserXmlParser::parseResolution( lastUsedChaser, sliceManager->getResolution() );
		ChaserXmlParser::parseAssFile( lastUsedChaser, sliceManager->getAssFile() );
		ChaserXmlParser::parseSlices( lastUsedChaser, sliceManager->getSlices() );

		//now populate the previewwindow with buttons for these slices
		previewWindow->createSliceButtons();

		//now populate the slicelist with entries for these slices
		sliceList->setSlices();
		
		//now redraw maincomponent, so the previewwindow is fitted and slicelist updated
		previewWindow->getParentComponent()->resized();

		//this will try its best to get useful info from the chaserfile
		chaseManager->createSequencesFromXml( ChaserXmlParser::parseSequences( lastUsedChaser ) );

		previewWindow->setActiveSlices();

		//make the first step active
		sequencer->selectStep( 0 );

		return true;
	}

	return false;
}
