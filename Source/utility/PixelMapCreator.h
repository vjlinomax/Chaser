/*
  ==============================================================================

    PixelMapCreator.h
    Created: 30 Apr 2018 9:04:17pm
    Author:  Joris

	let's make purdy pictures too
  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "../slice/SliceManager.h"

class PixelMapCreator
{
public:
	PixelMapCreator();
	~PixelMapCreator();

	void saveAsPng( String name, OwnedArray<Slice>& slices, Point<int> resolution );
private:



};
