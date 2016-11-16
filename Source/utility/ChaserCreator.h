/*
  ==============================================================================

    ChaserCreator.h
    Created: 12 Oct 2016 8:40:03pm
    Author:  Joris

  ==============================================================================
*/

#ifndef CHASERCREATOR_H_INCLUDED
#define CHASERCREATOR_H_INCLUDED

#include "JuceHeader.h"
#include "../slice/SliceManager.h"
#include "../chase/ChaseManager.h"
#include "../xml/ChaserXmlManager.h"
#include "../gui/Preview.h"
#include "../gui/SliceList.h"
#include "../gui/Sequencer.h"

/*responsible for loading in chaser data from xml 
and populating the various managers*/
class ChaserCreator
{
public:
	ChaserCreator( SliceManager* sliceManager, ChaseManager* chaseManager, 
					Preview* preview, SliceList* sliceList, Sequencer* sequencer );
	~ChaserCreator();

	void createChaserFromAssFile( File assFile, bool createNew );
	
	//will return true if succesful
	bool createChaserFromChaserFile();
	bool createChaserFromChaserFile( File chaserToLoad );

private:
	SliceManager* sliceManager;
	ChaseManager* chaseManager;
	Preview* previewWindow;
	SliceList* sliceList;
	Sequencer* sequencer;
	
};



#endif  // CHASERCREATOR_H_INCLUDED
