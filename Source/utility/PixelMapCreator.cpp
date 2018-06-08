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

	for ( auto slice : slices )
	{
		if ( !slice->screenIsCollapsed && slice->enabled )
		{
			PixelMapButton b( *slice );
			b.updatePath( resolution );
			b.paint( g, slices.indexOf( slice ) / (float) slices.size() );
		}
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
