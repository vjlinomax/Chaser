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
SequenceList::SequenceList( ChaseManager* cm, Sequencer* s )
{
	chaseManager = cm;
	sequencer = s;
	sequenceListBox.setModel( this );
	sequenceListBox.updateContent();
	sequenceListBox.setRowHeight( 30 );
	sequenceListBox.setColour( ListBox::ColourIds::backgroundColourId, Colours::transparentBlack );
	addAndMakeVisible( sequenceListBox );

	////set the selected row
	int selectedRow = chaseManager->getCurrentSequenceIndex();
	sequenceListBox.selectRow( selectedRow );
}

SequenceList::~SequenceList()
{

}

int SequenceList::getNumRows()
{
	return chaseManager->getSequenceNames().size();
}

void SequenceList::paintListBoxItem( int rowNumber, Graphics& g, int width, int height, bool rowIsSelected )
{
	ColourLookAndFeel claf;
	g.fillAll( claf.backgroundColour );   // clear the background
	g.setColour( claf.textColour );

	if ( rowIsSelected )
		g.fillAll( claf.primaryColour );

	DBG( "Names " << chaseManager->getSequenceNames().size() );
	if ( rowNumber < chaseManager->getSequenceNames().size() )
	{
		g.drawText( chaseManager->getSequenceNames()[ rowNumber ], 15, 0, width - 15, height, Justification::centredLeft );
	}
}

void SequenceList::selectItem( int i )
{
	sequenceListBox.deselectAllRows();
	sequenceListBox.updateContent();
	sequenceListBox.selectRow( i );
}

void SequenceList::listBoxItemClicked( int row, const MouseEvent& )
{
	sequenceListBox.deselectAllRows();
	sequenceListBox.selectRow( row );
	sequencer->selectSequence( row );
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
	sequenceListBox.setBoundsInset( BorderSize < int > {1} );
}
