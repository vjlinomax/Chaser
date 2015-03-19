/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "Preview.h"
#include "Slice.h"
#include "SliceList.h"
#include "Sequencer.h"
#include "XmlSequence.h"
#include "Copier.h"
#include "ColourLookAndFeel.h"



//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public Component, public Button::Listener, public ChangeListener, public Sequencer::Listener, public Preview::Listener, public Copier::Listener
{
public:
    //==============================================================================
    MainContentComponent();
    ~MainContentComponent();

    void resized();
    void buttonClicked( Button* b );
    
    virtual void changeListenerCallback (ChangeBroadcaster* source);
	
	//sequencer listener methods
	virtual void stepSelected ( int step );
	virtual void sequenceNameChanged ( String newName );
	virtual void sequenceSelected ( int sequence );
	virtual void sequenceLengthChanged ( int newSequenceLength );
	
	//preview listener methods
	virtual void sliceClicked ( Array<int> activeSlices );
	
	//copier listener methods
	virtual void copierClicked ( int m );
	
    void parseXml ( File f );
    
    

private:
	
	int currentStep;
	int currentSequence;
	int currentSequenceLength;
	Array<int> activeSlices;
	
    ScopedPointer<ColourLookAndFeel> laf;
    
    ScopedPointer<Button> openOutput;
    File activeFile;
    
    OwnedArray<Slice> slices;
    
    ScopedPointer<Preview> previewWindow;
    ScopedPointer<SliceList> sliceList;
    ScopedPointer<Sequencer> sequencer;
    ScopedPointer<XmlSequence> xmlSequence;
	ScopedPointer<Copier> copier;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED
