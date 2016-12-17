/*
  ==============================================================================

    SequenceList.h
    Created: 17 Dec 2016 3:01:46pm
    Author:  Joris

  ==============================================================================
*/

#ifndef SEQUENCELIST_H_INCLUDED
#define SEQUENCELIST_H_INCLUDED

#include "../../JuceLibraryCode/JuceHeader.h"

#include "../chase/ChaseManager.h"
#include "Sequencer.h"

//==============================================================================
/*
*/
class SequenceList    : 
	public Component,
	public ListBoxModel
{
public:
    SequenceList( ChaseManager* sliceManager, Sequencer* s );
    ~SequenceList();

    void paint (Graphics&) override;
    void resized() override;

	int getNumRows() override;
	void paintListBoxItem( int rowNumber,
		Graphics& g,
		int width, int height,
		bool rowIsSelected ) override;
	void listBoxItemClicked( int row, const MouseEvent& ) override;

	void selectItem( int i );


private:
	ListBox sequenceListBox;
	StringArray sequenceNames;

	ChaseManager* chaseManager;
	Sequencer* sequencer;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SequenceList)
};


#endif  // SEQUENCELIST_H_INCLUDED
