/*
  ==============================================================================

  SliceManager.cpp
  Created: 20 Feb 2016 6:11:01pm
  Author:  Joris de Jong

  ==============================================================================
  */

#include "SliceManager.h"

SliceManager::SliceManager( ChaserXmlManager* xmlManager ) : xmlManager( xmlManager )
{
	resolution = Point<int>( 1920, 1080 );
}

SliceManager::~SliceManager()
{
	slices.clear();
}

OwnedArray<Slice>& SliceManager::getSlices()
{
	return slices;
}

Point<int>& SliceManager::getResolution()
{
	return resolution;
}

File& SliceManager::getAssFile()
{
	return assFile;
}

XmlElement* SliceManager::getSlicesAsXml()
{
	XmlElement* slicesXml = new XmlElement( "slices" );
	slicesXml->setAttribute( "assFile", assFile.getFullPathName() );
	slicesXml->setAttribute( "width", resolution.x );
	slicesXml->setAttribute( "height", resolution.y );

	//loop through all the slices
	for ( Slice* slice : slices )
	{
		//for every slice, create an xmlelement and its id, and which screen it belongs to
		XmlElement* sliceXml = new XmlElement( "slice" );
		sliceXml->setAttribute( "name", slice->sliceId.first );
		sliceXml->setAttribute( "uniqueId", String( slice->sliceId.second ) );
		sliceXml->setAttribute( "enable", slice->enabled );
		sliceXml->setAttribute( "screenId", String( slice->screenId ) );
		slicesXml->addChildElement( sliceXml );

		//then create an xmlelement to store its rects and points
		XmlElement* inputRectXml = new XmlElement( "inputRect" );
		inputRectXml->setAttribute( "orientation", slice->inputRectOrientation );
		sliceXml->addChildElement( inputRectXml );

		//loop through this rect's points
		for ( Point<float> point : slice->inputRectPoints )
		{
			//for every step, create an xmlelement and store the step nr
			XmlElement* pointXml = new XmlElement( "point" );
			pointXml->setAttribute( "x", point.getX() );
			pointXml->setAttribute( "y", point.getY() );
			inputRectXml->addChildElement( pointXml );
		}

		//then create an xmlelement to store its rects and points
		XmlElement* maskXml = new XmlElement( "mask" );
		sliceXml->addChildElement( maskXml );

		//loop through this mask's points
		for ( Point<float> point : slice->maskPoints )
		{
			//for every step, create an xmlelement and store the step nr
			XmlElement* pointXml = new XmlElement( "point" );
			pointXml->setAttribute( "x", point.getX() );
			pointXml->setAttribute( "y", point.getY() );
			maskXml->addChildElement( pointXml );
		}
	}

	return slicesXml;
}

Array<Screen>& SliceManager::getScreens()
{
	return screens;
}

XmlElement* SliceManager::getScreensAsXml()
{
	XmlElement* screensXml = new XmlElement( "screens" );
	for ( auto screen : getScreens() )
	{
		XmlElement* screenXml = new XmlElement( "screen" );
		screenXml->setAttribute( "name", screen.name );
		screenXml->setAttribute( "uniqueId", String( screen.uid ) );
		screenXml->setAttribute( "folded", screen.folded );
		screensXml->addChildElement( screenXml );
	}
	return screensXml;
}

void SliceManager::writeToXml()
{
	updateComponents();

	if ( xmlManager )
	{
		xmlManager->saveXmlElement( getSlicesAsXml() );
		xmlManager->saveXmlElement( getScreensAsXml() );
	}
}

void SliceManager::addUpdateable( UpdateableComponent* newUpdateable )
{
	updateables.add( newUpdateable );
}

void SliceManager::updateComponents()
{
	for ( auto component : updateables )
		component->update();
}
