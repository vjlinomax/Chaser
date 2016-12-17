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

/*responsible for loading in chaser data from xml 
and populating the various managers*/
class ChaserCreator
{
public:
	ChaserCreator( SliceManager* sliceManager, ChaseManager* chaseManager, 
					Preview* preview, SliceList* sliceList );
	~ChaserCreator();

	void createChaserFromAssFile( File assFile, bool createNew );
	
	//will return true if succesful
	bool createChaserFromChaserFile();
	bool createChaserFromChaserFile( File chaserToLoad );

private:
	SliceManager* sliceManager;
	ChaseManager* chaseManager;

	/**creator needs pointers to previewWindow and sliceList
	because it needs to update those with new data when a new file is loaded
	this is not something that can be done with an updateable callback
	unless I create a new updateable class that only updates when a new file is loaded
	this seems like too much work for now*/
	Preview* previewWindow;
	SliceList* sliceList;
};



#endif  // CHASERCREATOR_H_INCLUDED
