/*
  ==============================================================================

    SequenceList.cpp
    Created: 17 Dec 2016 3:01:46pm
    Author:  Joris

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"
#include "SequenceList.h"
#include "ColourLookAndFeel.h"



//==============================================================================
SequenceList::SequenceList( ChaseManager* cm )
{
	chaseManager = cm;

	sequenceListComponent = new DeletableItemListComponent();
	for ( String seqName : chaseManager->getSequenceNames() )
		sequenceListComponent->addItem( seqName );
	addAndMakeVisible( sequenceListComponent );
	sequenceListComponent->addListener( this );

	addSeqButton = new TextButton( "+" );
	addSeqButton->addListener( this );
	addAndMakeVisible( addSeqButton );
}

SequenceList::~SequenceList()
{

}

void SequenceList::update()
{
	//I don't know how the sequences have changed here
	//so I just clear the whole list and build it again
	sequenceListComponent->removeAllItems();
	for ( auto newItem : chaseManager->getSequenceNames() )
		sequenceListComponent->addItem( newItem );
}



void SequenceList::paint (Graphics& g)
{
	ColourLookAndFeel claf;
	g.fillAll( claf.backgroundColour );   // clear the background

	g.setColour( claf.outlineColour );
	g.drawRect( getLocalBounds(), 1 );   // draw an outline around the component
}

void SequenceList::resized()
{
	sequenceListComponent->setBoundsInset( BorderSize<int> {1, 1, 40, 1} );
	addSeqButton->setBounds( 1, sequenceListComponent->getBottom() + 1, getWidth() - 1, 38 );
}

void SequenceList::itemRemoved( int index )
{
	chaseManager->removeSequence( index );
}

void SequenceList::buttonClicked( Button* )
{
	//there's only button
	chaseManager->addSequence();
}
