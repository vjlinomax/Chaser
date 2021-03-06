/*
  ==============================================================================

    SliceManager.h
    Created: 20 Feb 2016 6:11:01pm
    Author:  Joris de Jong

  ==============================================================================
*/

#ifndef SLICEMANAGER_H_INCLUDED
#define SLICEMANAGER_H_INCLUDED

#include "../../HybridApi/Source/HybridApi.h"
#include "../xml/ChaserXmlManager.h"
#include "../gui logic/UpdateableComponent.h"

using namespace hybrid;


//takes care of keeping all the slice data in one place and accesible by other classes
class SliceManager
{
public:
	SliceManager( ChaserXmlManager* xmlManager );
	~SliceManager();
	
	OwnedArray<Slice>& getSlices();
	Point<int>& getResolution();
	File& getAssFile();
    
    Array<Screen>& getScreens();

	void writeToXml();

	void addUpdateable( UpdateableComponent* newUpdateable );

	bool isSliceShowing( int64 uid );
	
private:
	OwnedArray<Slice> slices;
	Point<int> resolution;
	File assFile;

	XmlElement* getSlicesAsXml();
	XmlElement* getScreensAsXml();
	ChaserXmlManager* xmlManager;
    
    Array<Screen> screens;

	void updateComponents();
	Array<UpdateableComponent*> updateables;
};

#endif  // SLICEMANAGER_H_INCLUDED
