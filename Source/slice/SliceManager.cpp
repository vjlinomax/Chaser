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
		slicesXml->addChildElement( slice->toXml() );

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

bool SliceManager::isSliceShowing( int64 uid )
{
	for ( Slice* slice : slices )
		if ( slice->sliceId.second == uid )
			return slice->enabled;
	return false;
}

void SliceManager::updateComponents()
{
	for ( auto component : updateables )
		component->update();
}
