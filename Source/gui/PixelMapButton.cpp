/*
  ==============================================================================

	PixelMapButton.cpp
	Created: 15 May 2018 7:39:18am
	Author:  Joris

  ==============================================================================
*/

#include "PixelMapButton.h"

PixelMapButton::PixelMapButton( hybrid::Slice slice ) :
	PathButton( slice.maskPoints.size() > 0 ? slice.maskPoints : slice.inputRectPoints )
{
	name = slice.sliceId.first;
	rotation = slice.maskPoints.size() > 0 ? slice.maskRectOrientation : slice.inputRectOrientation;

	schemes.add( Scheme( Colours::darkgrey, Colours::darkorchid ) );
	schemes.add( Scheme( Colours::blanchedalmond, Colours::darkgoldenrod ) );
	schemes.add( Scheme( Colours::blue, Colours::darkkhaki ) );
	schemes.add( Scheme( Colours::grey, Colours::maroon ) );
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

	float xTiles = jmax( 1.0f, bounds.getWidth() / round( bounds.getWidth() / 64.0f ) );
	float yTiles = jmax( 1.0f, bounds.getHeight() / round( bounds.getHeight() / 64.0f ) );
	Colour col = Colour::fromHSV( 1.0f - index, 1.0f, 1.0f, 1.0f );
	g.fillCheckerBoard( bounds, xTiles, yTiles, col, col.darker() );
	
	if ( pathPoints.size() > 3 )
	{
		float radius = jmin( bounds.getWidth(), bounds.getHeight() );
		g.drawEllipse( center.x - radius * 0.5f, center.y - radius * 0.5f, radius, radius, 1.0f );
	}

	g.drawLine( bounds.getX(), bounds.getY(), bounds.getRight(), bounds.getBottom() );
	g.drawLine( bounds.getX(), bounds.getBottom(), bounds.getRight(), bounds.getY() );

	
	if ( upright.getBounds().getHeight() > 48 && upright.getBounds().getWidth() > 128 )
	{
		Font f;
		f.setTypefaceName( "Impact" );
		int h = 24;
		f.setHeight( h );
		g.setFont( f );
		int textWidth = f.getStringWidth( name );
		g.setColour( Colours::black );
		g.fillRect( bounds.withSizeKeepingCentre( textWidth + 16, h * 1.5 ).translated( 0, h * -0.75 ) );
		g.setColour( Colours::white );
		g.drawText( name, upright.getBounds().toNearestInt().translated( 0, h * -0.75 ), Justification::centred, 1 );
		h *= 0.75;
		f.setHeight( h );
		String data = String( bounds.toNearestInt().getX() ) + ", " + String( bounds.toNearestInt().getY() ) + " // " + String( bounds.toNearestInt().getWidth() ) + ", " + String( bounds.toNearestInt().getHeight() );
		textWidth = f.getStringWidth( data );
		g.fillRect( bounds.withSizeKeepingCentre( textWidth + 16, h *  1.5 ).translated( 0, h * 0.75 ) );
		g.setColour( Colours::black );
		g.setFont( f );
		g.drawText( data, upright.getBounds().toNearestInt().translated( 0, h * 0.75 ), Justification::centred, 1 );
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

