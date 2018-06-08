/*
  ==============================================================================

    PixelMapButton.h
    Created: 15 May 2018 7:39:18am
    Author:  Joris

	PixelMapButton is a path button that just draws itself nicely
  ==============================================================================
*/

#pragma once

#include "../../HybridApi/Source/JuceBased/Components/PathButton.h"
#include "../../HybridApi/Source/JuceBased/ArenaHelpers/Slice/Slice.h"

class PixelMapButton :
	public PathButton
{
public:
	PixelMapButton( hybrid::Slice s );
	~PixelMapButton();

	void paint( Graphics& g, float index );

private:
	String name;
	float rotation;

	struct Scheme
	{
		Scheme() {};
		Scheme( Colour a, Colour b ) { firstCol = a; secondCol = b; }
		Colour firstCol;
		Colour secondCol;
	};
	Array<Scheme> schemes;
};
