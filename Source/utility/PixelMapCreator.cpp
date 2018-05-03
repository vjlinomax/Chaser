/*
  ==============================================================================

	PixelMapCreator.cpp
	Created: 30 Apr 2018 9:04:17pm
	Author:  Joris

  ==============================================================================
*/

#include "PixelMapCreator.h"
#include "../../HybridApi/Source/JuceBased/Components/SliceButton.h"

PixelMapCreator::PixelMapCreator()
{
}

PixelMapCreator::~PixelMapCreator()
{
}


void PixelMapCreator::saveAsPng( String name, OwnedArray<Slice>& slices, Point<int> resolution )
{
	Image pixelMap = Image( Image::PixelFormat::ARGB, resolution.x, resolution.y, true );
	Graphics g( pixelMap );

	Component c;
	c.setSize( resolution.x, resolution.y );
	for ( auto slice : slices )
	{
		SliceButton b( *slice );
		c.addChildComponent( b );
		b.resized();
		b.paintButton( g, false, false );
	}

	FileOutputStream stream( File( File::getSpecialLocation( File::SpecialLocationType::userDesktopDirectory ).getFullPathName() + "/" + name + ".png" ) );
	stream.flush();
	PNGImageFormat pngWriter;
	pngWriter.writeImageToStream( pixelMap, stream );
}
