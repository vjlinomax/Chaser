/*
  ==============================================================================

	PixelMapButton.cpp
	Created: 15 May 2018 7:39:18am
	Author:  Joris

  ==============================================================================
*/

#include "PixelMapButton.h"
#include "JuceHeader.h"

PixelMapButton::PixelMapButton( hybrid::Slice slice ) :
	PathButton( slice.maskPoints.size() > 0 ? slice.maskPoints : slice.inputRectPoints )
{
	name = slice.sliceId.first;
	rotation = slice.maskPoints.size() > 0 ? slice.maskRectOrientation : slice.inputRectOrientation;
}

PixelMapButton::~PixelMapButton()
{
}

void PixelMapButton::paint( Graphics & g, float index )
{
	Point<float> center = path.getBounds().getCentre();
	Path upright = path;
	upright.applyTransform( AffineTransform::rotation( -rotation, center.x, center.y ) );
	Rectangle<float> bounds = upright.getBounds();

	g.setColour( Colours::white );

	g.saveState();
	g.reduceClipRegion( path );

	AffineTransform trans = AffineTransform::rotation( rotation, center.x, center.y );
	g.addTransform( trans );

    float xTiles = jmax( 1.0f, float( bounds.getWidth() / round( bounds.getWidth() / 64.0f ) ) );
	float yTiles = jmax( 1.0f, float( bounds.getHeight() / round( bounds.getHeight() / 64.0f ) ) );
	Colour col = Colour::fromHSV( 1.0f - index, 1.0f, 1.0f, 1.0f );
	g.fillCheckerBoard( bounds, xTiles, yTiles, col, col.darker() );

	if ( pathPoints.size() > 3 )
	{
		float radius = jmin( bounds.getWidth(), bounds.getHeight() );
		g.drawEllipse( center.x - radius * 0.5f, center.y - radius * 0.5f, radius, radius, 1.0f );
	}

	g.drawLine( bounds.getX(), bounds.getY(), bounds.getRight(), bounds.getBottom() );
	g.drawLine( bounds.getX(), bounds.getBottom(), bounds.getRight(), bounds.getY() );

	int logoOffset = 0;
	int width = int( upright.getBounds().getWidth() );
	int height = int( upright.getBounds().getHeight() );
	if ( height > 400 && width > 400 )
	{
		//scan the logo folder for files
		File logoDir = File( File::getSpecialLocation( File::SpecialLocationType::userDocumentsDirectory ).getFullPathName() + "/Chaser/logo" );
		if ( logoDir.isDirectory() )
		{
			Array<File> files = logoDir.findChildFiles( File::findFiles, false, "*.jpg" );
			files.addArray( logoDir.findChildFiles( File::findFiles, false, "*.png" ) );
			files.addArray( logoDir.findChildFiles( File::findFiles, false, "*.jpeg" ) );
			if ( files.size() > 0 )
			{
				Image logo = ImageFileFormat::loadFrom( files[ 0 ] );
				if ( logo.isValid() )
				{
					g.drawImage( logo, upright.getBounds().withSizeKeepingCentre( jmin( 300.0f, width / 2.0f ), jmin( 300.0f, height / 2.0f ) ), RectanglePlacement::centred );
					logoOffset = jmin( 300, height / 2 ) / 2;
				}
			}
		}
	}

	if ( height > 48 && width > 128 ) //show the name
	{
		Font f;
		f.setTypefaceName( "Impact" );
		float h = 24;
		logoOffset = logoOffset + int( h );
		f.setHeight( h );
		g.setFont( f );
		int textWidth = f.getStringWidth( name );
		g.setColour( Colours::black );
		g.fillRect( bounds.withSizeKeepingCentre( float( textWidth + 16 ), h * 1.5f ).translated( 0, h * -0.75f + logoOffset ) );
		g.setColour( Colours::white );
		g.drawText( name, upright.getBounds().toNearestInt().translated( 0, int( h * -0.75f ) + logoOffset ), Justification::centred, 1 );
		
		if ( height > 64 ) //show the coordinates too
		{
			h *= 0.75f;
			f.setHeight( h );
			String data = String( bounds.toNearestInt().getX() ) + ", " + String( bounds.toNearestInt().getY() ) + " // " + String( bounds.toNearestInt().getWidth() ) + " x " + String( bounds.toNearestInt().getHeight() );
			textWidth = f.getStringWidth( data );
			g.fillRect( bounds.withSizeKeepingCentre( float( textWidth + 16 ), h *  1.5f ).translated( 0.0f, h * 0.75f + logoOffset ) );
			g.setColour( Colours::black );
			g.setFont( f );
			g.drawText( data, upright.getBounds().toNearestInt().translated( 0, int( h * 0.75f ) + logoOffset ), Justification::centred, 1 );
		}
	}

	g.restoreState();

	g.saveState();
	g.reduceClipRegion( path );
	g.strokePath( path, PathStrokeType( 2.0f ) );
	g.setColour( Colours::red );
	Path::Iterator it( path );
	float r = 10;
	while ( it.next() )
		g.fillEllipse( it.x1 - r * 0.5f, it.y1 - r * 0.5f, r, r );
	g.restoreState();
}

