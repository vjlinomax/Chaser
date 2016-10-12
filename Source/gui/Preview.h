/*
  ==============================================================================

    Preview.h
    Created: 28 Dec 2014 11:00:00am
    Author:  Joris de Jong

  ==============================================================================
*/

#ifndef PREVIEW_H_INCLUDED
#define PREVIEW_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "SliceButton.h"
#include "../chase/ChaseManager.h"
#include "../slice/SliceManager.h"
#include "ColourLookAndFeel.h"


//==============================================================================
/*
*/

class SliceLookAndFeel : public ColourLookAndFeel
{
public:
    
    SliceLookAndFeel(){}
    ~SliceLookAndFeel(){}
	
    //override the button drawing function
    void drawButtonBackground (Graphics& g, Button& button, const Colour& backColour,
                                               bool isMouseOverButton, bool isButtonDown)
    {
        const int width = button.getWidth();
        const int height = button.getHeight();
        
        const float indent = 2.0f;
        
        Path p;
        p.addRectangle (indent, indent,
                        width - indent * 2.0f,
                        height - indent * 2.0f);
        
        Colour bc (backColour);
        
        if (isMouseOverButton)
        {
            if (isButtonDown)
                bc = bc.brighter();
            else if (bc.getBrightness() > 0.5f)
                bc = bc.darker (0.1f);
            else
                bc = bc.brighter (0.1f);
        }
        
        g.setColour (bc);
        g.fillPath (p);
        
		g.setColour ( primaryColour.withAlpha ((isMouseOverButton) ? 1.0f : 0.8f));
        g.strokePath (p, PathStrokeType ((isMouseOverButton) ? 2.0f : 1.4f));
    }
    
};


class Preview    : public Component, public Button::Listener
{
public:
    Preview( ChaseManager* chaseManager, SliceManager* sliceManager );
    ~Preview();

    void paint (Graphics&);
    void resized();
    
    virtual void buttonClicked (Button*);
	
	/* this will clear the list of sliceButtons and create new ones
	based on the slices passed in as argument*/
	void createSliceButtons ();
    
	/* this check the list of slicebuttons to see if there are buttons
	with a matching uniqueid, if so, it toggles them on*/
	void setActiveSlices();


    

private:
	void clearSlices();
	void addSliceButton( Slice& slice);
	
	ChaseManager* chaseManager;
	SliceManager* sliceManager;

    OwnedArray<SliceButton> sliceButtons;
    ScopedPointer<SliceLookAndFeel> sliceLaf;
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Preview)
};


#endif  // PREVIEW_H_INCLUDED
