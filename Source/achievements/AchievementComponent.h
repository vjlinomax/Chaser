/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.2.1

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "../../JuceLibraryCode/JuceHeader.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class AchievementComponent  : public Component,
                              public Timer
{
public:
    //==============================================================================
    AchievementComponent ();
    ~AchievementComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void setText( String name, String description );
	void timerCallback() override;
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	int count;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Label> nameLabel;
    ScopedPointer<Label> descLabel;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AchievementComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
