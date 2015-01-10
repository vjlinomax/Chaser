/*
  ==============================================================================

    SliceButton.h
    Created: 28 Dec 2014 11:15:33am
    Author:  Joris de Jong

  ==============================================================================
*/

#ifndef SLICEBUTTON_H_INCLUDED
#define SLICEBUTTON_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "Slice.h"

//==============================================================================
/*
*/
class SliceButton    : public TextButton
{
public:
    SliceButton ( String n, bool enable, double l, double t, double r, double b );
    SliceButton ( Slice* s );
    ~SliceButton();
    
    double proportionalX;
    double proportionalY;
    double proportionalW;
    double proportionalH;
    bool enabled;
    String name;
    
    void update();

private:
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SliceButton)
};


#endif  // SLICEBUTTON_H_INCLUDED
