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
#include "../gui logic/UpdateableComponent.h"

//==============================================================================
/*
*/
class SequenceList    : 
	public Component,
	public ListBoxModel,
	public UpdateableComponent

{
public:
    SequenceList( ChaseManager* chMan );
    ~SequenceList();

    void paint (Graphics&) override;
    void resized() override;

	void update() override;

	int getNumRows() override;
	void paintListBoxItem( int rowNumber,
		Graphics& g,
		int width, int height,
		bool rowIsSelected ) override;
	void listBoxItemClicked( int row, const MouseEvent& ) override;

private:
	ListBox sequenceListBox;
	StringArray sequenceNames;

	ChaseManager* chaseManager;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SequenceList)
};


#endif  // SEQUENCELIST_H_INCLUDED
