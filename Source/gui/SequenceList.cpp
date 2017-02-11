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
	/*
	sequenceListBox.setModel( this );
	sequenceListBox.updateContent();
	sequenceListBox.setRowHeight( 30 );
	sequenceListBox.setColour( ListBox::ColourIds::backgroundColourId, Colours::transparentBlack );
	addAndMakeVisible( sequenceListBox );
	*/

	sequenceListComponent = new DeletableItemListComponent();
	for ( String seqName : chaseManager->getSequenceNames() )
		sequenceListComponent->addItem( seqName );
	addAndMakeVisible( sequenceListComponent );
	sequenceListComponent->addListener( this );

	////set the selected row
	int selectedRow = chaseManager->getCurrentSequenceIndex();
	//sequenceListBox.selectRow( selectedRow );

	addSeqButton = new TextButton( "+" );
	addAndMakeVisible( addSeqButton );
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

	if ( rowNumber < chaseManager->getSequenceNames().size() )
		g.drawText( chaseManager->getSequenceNames()[ rowNumber ], 15, 0, width - 15, height, Justification::centredLeft );
}

void SequenceList::update()
{
	sequenceListBox.deselectAllRows();
	sequenceListBox.updateContent();
	sequenceListBox.selectRow( chaseManager->getCurrentSequenceIndex() );
}

void SequenceList::listBoxItemClicked( int row, const MouseEvent& )
{
	sequenceListBox.deselectAllRows();
	sequenceListBox.selectRow( row );
	chaseManager->skipToSequence( row );
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
	//sequenceListBox.setBoundsInset( BorderSize < int > {1,1,40,1} );
	sequenceListComponent->setBoundsInset( BorderSize<int> {1, 1, 40, 1} );
	addSeqButton->setBounds( 1, sequenceListBox.getBottom() + 1, getWidth() - 1, 38 );
}

void SequenceList::itemRemoved( int index )
{
	chaseManager->removeSequence( index );
}
