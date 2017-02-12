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
#include "JuceBased\Components\DeletableItemListComponent.h"

//==============================================================================
/*
*/
class SequenceList    : 
	public Component,
	public UpdateableComponent,
	public DeletableItemListComponent::Listener

{
public:
    SequenceList( ChaseManager* chMan );
    ~SequenceList();

    void paint (Graphics&) override;
    void resized() override;

	void itemRemoved( int index ) override;

	void update() override;

private:
	StringArray sequenceNames;

	ScopedPointer<TextButton> addSeqButton;
	ScopedPointer<DeletableItemListComponent> sequenceListComponent;

	ChaseManager* chaseManager;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SequenceList)
};


#endif  // SEQUENCELIST_H_INCLUDED
