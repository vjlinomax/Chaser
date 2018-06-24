/*
  ==============================================================================

	PixelMapCreator.cpp
	Created: 30 Apr 2018 9:04:17pm
	Author:  Joris

  ==============================================================================
*/

#include "PixelMapCreator.h"
#include "../gui/PixelMapButton.h"

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

	//create a sub array to get a better distribution of colors when lots of slices are disabled
	Array<Slice*> visibleSlices;
	for ( auto slice : slices )
		if ( !slice->screenIsCollapsed && slice->enabled )
			visibleSlices.add( slice );
	
	for ( auto visibleSlice : visibleSlices )
	{
		PixelMapButton b( *visibleSlice );
		b.updatePath( resolution );
		b.paint( g, visibleSlices.indexOf( visibleSlice ) / (float) visibleSlices.size() );
	}

	File tempFile = File( File::getSpecialLocation( File::SpecialLocationType::userDesktopDirectory ).getFullPathName() + "/" + name + ".png" );
	File pngFile;
	FileChooser chooser("Export Pixelmap as...", tempFile);
	if ( chooser.browseForFileToSave( true ) )
	{
		pngFile = chooser.getResult();
		if ( pngFile.exists() )
			pngFile.deleteFile();
		FileOutputStream stream( pngFile );
		PNGImageFormat pngWriter;
		pngWriter.writeImageToStream( pixelMap, stream );
	}
}
