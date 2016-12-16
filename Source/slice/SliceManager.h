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

//takes care of keeping all the slice data in one place and accesible by other classes
class SliceManager
{
public:
	SliceManager( ChaserXmlManager* xmlManager );
	~SliceManager();
	
	OwnedArray<Slice>& getSlices();
	Point<int>& getResolution();
	File& getAssFile();
    
    Array<NamedUniqueId>& getScreens();

	void writeToXml();
	
private:
	OwnedArray<Slice> slices;
	Point<int> resolution;
	File assFile;

	XmlElement* getSlicesAsXml();
	ChaserXmlManager* xmlManager;
    
    Array<NamedUniqueId> screens;
};

#endif  // SLICEMANAGER_H_INCLUDED
