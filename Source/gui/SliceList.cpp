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
	}

	parent.screenVisibilityChanged();
}

SlicePropertyButton::SlicePropertyButton( SliceList& parent, Slice& slice ) : BooleanPropertyComponent( slice.sliceId.second, slice.sliceId.second, slice.sliceId.second ), parent( parent ), slice( slice )
{
	state = slice.enabled;
}
SlicePropertyButton::~SlicePropertyButton(){}

void SlicePropertyButton::buttonClicked( Button* b )
{
	setState( !getState() );
	b->setToggleState( getState(), dontSendNotification );
	slice.enabled = getState();

	parent.sliceVisibilityChanged();
}

void SlicePropertyButton::setState( bool newState )
{
	state = newState;
}

bool SlicePropertyButton::getState() const
{
	return state;
}

Slice& SlicePropertyButton::getSlice()
{
	return slice;
}





//==============================================================================
SliceList::SliceList( SliceManager* sliceManager, Preview* preview ) :
panel( new MyPropertyPanel( *this ) ),
sliceManager( sliceManager ),
preview( preview )
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

	OwnedArray<Slice>& slices = sliceManager->getSlices();

	for ( int i = 0; i < slices.size(); i++ )
	{
		NamedUniqueId screen = slices[ i ]->screenId;

		//create a new SlicePropertyButton
		SlicePropertyButton* newComponent = new SlicePropertyButton( *this, *slices[ i ] );
		newComponent->setColour( BooleanPropertyComponent::backgroundColourId, claf.backgroundColour );
		newComponent->setState( slices[ i ]->enabled );

		sections[ screen ].add( newComponent );
	}

	//now go through all the arrays again and create sections out of them
	for ( const auto& s : sections )
		panel->addSection( s.first.second, s.second );
}

void SliceList::screenVisibilityChanged()
{
	//redraw the preview window
	preview->resized();
}
void SliceList::sliceVisibilityChanged()
{
	//save the new state
	sliceManager->writeToXml();

	//redraw the preview window
	preview->resized();
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
