/*
  ==============================================================================

  SliceList.cpp
  Created: 10 Jan 2015 7:14:14pm
  Author:  Joris de Jong

  ==============================================================================
  */

#include "../JuceLibraryCode/JuceHeader.h"
#include "SliceList.h"

MyPropertyPanel::MyPropertyPanel( SliceList& parent ) : parent( parent )
{
	setMessageWhenEmpty( "(no screensetup loaded...)" );
}

MyPropertyPanel::~MyPropertyPanel()
{

}

void MyPropertyPanel::resized()
{
	//first call the regular resized method
	PropertyPanel::resized();

	//now check the states of all the sections
	for ( int i = 0; i < getSectionNames().size(); i++ )
	{
		Array<Slice*> slices = parent.getSlicesFromSection( i );
		for ( int j = 0; j < slices.size(); j++ )
		{
			slices[ j ]->screenIsCollapsed = !isSectionOpen( i );
		}

		parent.screenVisibilityChanged( i );
	}
}

SlicePropertyButton::SlicePropertyButton( SliceList& parent, Slice& slice ) : BooleanPropertyComponent( slice.sliceId.first, slice.sliceId.first, slice.sliceId.first ), parent( parent ), slice( slice )
{
	state = slice.enabled;
}

SlicePropertyButton::~SlicePropertyButton(){}


void SlicePropertyButton::setState( bool newState )
{
	state = newState;

	slice.enabled = getState();
	refresh();

	parent.sliceVisibilityChanged();
}

bool SlicePropertyButton::getState() const
{
	return state;
}

Slice& SlicePropertyButton::getSlice()
{
	return slice;
}

void SlicePropertyButton::paint( Graphics& g )
{
	//I need this resized here to make sure the button stay left aligned after switching tabs
	resized();
	PropertyComponent::paint( g );
}





//==============================================================================
SliceList::SliceList( ChaseManager* chaseManager, SliceManager* sliceManager ) :
sliceManager( sliceManager ),
chaseManager ( chaseManager ),
panel( new MyPropertyPanel( *this ) )
{
	addAndMakeVisible( panel );
}

SliceList::~SliceList()
{

}

Array<Slice*> SliceList::getSlicesFromSection( int section )
{
	Array<Slice*> returnArray;

	SectionMap::iterator s = sections.begin();
	for ( int i = 0; i < section; i++ )
		s++;
	for ( int j = 0; j < s->second.size(); j++ )
	{
		returnArray.add( &static_cast<SlicePropertyButton*>(s->second[ j ])->getSlice() );
	}

	return returnArray;
}

void SliceList::paint( Graphics& g )
{
	g.fillAll( claf.backgroundColour );   // clear the background

	g.setColour( claf.outlineColour );
	g.drawRect( getLocalBounds(), 1 );   // draw an outline around the component
}

void SliceList::setSlices()
{
	clear();

	//first get the screennames
	Array<Screen> screens = sliceManager->getScreens();

	OwnedArray<Slice>& slices = sliceManager->getSlices();

	for ( int i = 0; i < slices.size(); i++ )
	{
		int64 screenId = slices[ i ]->screenId;

		//get the folded state for this screen
		for ( Screen screen : screens )
			if ( screen.uid == screenId )
				slices[i]->screenIsCollapsed = screen.folded;

		//create a new SlicePropertyButton
		SlicePropertyButton* newComponent = new SlicePropertyButton( *this, *slices[ i ] );
		newComponent->setColour( BooleanPropertyComponent::backgroundColourId, claf.backgroundColour );
		newComponent->setState( slices[ i ]->enabled );

		sections[ screenId ].add( newComponent );
	}

	//now go through all the arrays again and create sections out of them
	for ( const auto& s : sections )
	{
		//get the name for this screen's uid
		int64 uid = s.first;
		for ( Screen screen : screens )
		{
			if ( screen.uid == uid )
				panel->addSection( screen.name, s.second, !screen.folded );
		}
	}
}

void SliceList::screenVisibilityChanged( int foldedSectionIndex )
{
	//wrtie the status of this section to xml
	for ( Screen& screen : sliceManager->getScreens() )
	{
		if ( screen.name == panel->getSectionNames()[ foldedSectionIndex ] )
			screen.folded = !panel->isSectionOpen( foldedSectionIndex );
	}
	sliceManager->writeToXml();
}
void SliceList::sliceVisibilityChanged()
{
	//save the new state
	sliceManager->writeToXml();
	//we also need to write the chases, because we need to remove the invisible slice
	chaseManager->writeToXml();
}

void SliceList::update()
{
	resized();
}

void SliceList::clear()
{
	sections.clear();
	panel->clear();
}

void SliceList::resized()
{
	BorderSize<int> b = { 5, 5, 5, 5 };
	panel->setBoundsInset( b );
}
