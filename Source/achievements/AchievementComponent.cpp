/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.2.0

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "AchievementComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
AchievementComponent::AchievementComponent ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (nameLabel = new Label ("new label",
                                              TRANS("Name")));
    nameLabel->setFont (Font (Font::getDefaultSansSerifFontName(), 15.00f, Font::plain).withTypefaceStyle ("Bold"));
    nameLabel->setJustificationType (Justification::centredLeft);
    nameLabel->setEditable (false, false, false);
    nameLabel->setColour (TextEditor::textColourId, Colours::black);
    nameLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (descLabel = new Label ("new label",
                                              TRANS("Description")));
    descLabel->setFont (Font (Font::getDefaultSansSerifFontName(), 15.00f, Font::plain).withTypefaceStyle ("Regular"));
    descLabel->setJustificationType (Justification::topLeft);
    descLabel->setEditable (false, false, false);
    descLabel->setColour (TextEditor::textColourId, Colours::black);
    descLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));


    //[UserPreSize]
    //[/UserPreSize]

    setSize (500, 100);


    //[Constructor] You can add your own custom stuff here..
	addToDesktop( ComponentPeer::StyleFlags::windowIsTemporary );
	Rectangle<int> area = Desktop::getInstance().getDisplays().getMainDisplay().totalArea;
	setCentrePosition ( area.getWidth() / 2, (area.getHeight() / 4) * 3);
	setVisible( true );
	toFront( false );
	Desktop::getInstance().getAnimator().animateComponent( this, getBounds(), 0.0f, 8000, false, 1.0, 100.0 );
	count = 0;
	startTimer( 1000 );
    //[/Constructor]
}

AchievementComponent::~AchievementComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    nameLabel = nullptr;
    descLabel = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void AchievementComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff19abff));

    //[UserPaint] Add your own custom painting code here..
	g.setColour( Colour::fromRGBA( 255, 146, 0, 255 ));
	g.drawRect( getLocalBounds(), 2 );
    //[/UserPaint]
}

void AchievementComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    nameLabel->setBounds (proportionOfWidth (0.2492f), proportionOfHeight (0.0000f), proportionOfWidth (0.7508f), proportionOfHeight (0.3309f));
    descLabel->setBounds (proportionOfWidth (0.2492f), proportionOfHeight (0.3309f), proportionOfWidth (0.7508f), proportionOfHeight (0.6595f));
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void AchievementComponent::setText( String name, String description )
{
	nameLabel->setText( name, dontSendNotification );
	descLabel->setText( description, dontSendNotification );
}

void AchievementComponent::timerCallback()
{
	count++;
	if ( count > 10 )
		delete this;

}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="AchievementComponent" componentName=""
                 parentClasses="public Component, public Timer" constructorParams=""
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="0" initialWidth="500" initialHeight="100">
  <BACKGROUND backgroundColour="ff19abff"/>
  <LABEL name="new label" id="4f46e3685df3c34c" memberName="nameLabel"
         virtualName="" explicitFocusOrder="0" pos="24.927% 0% 75.073% 33.142%"
         edTextCol="ff000000" edBkgCol="0" labelText="Name" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default sans-serif font"
         fontsize="15" kerning="0" bold="1" italic="0" justification="33"
         typefaceStyle="Bold"/>
  <LABEL name="new label" id="49293c01478a59ee" memberName="descLabel"
         virtualName="" explicitFocusOrder="0" pos="24.927% 33.142% 75.073% 65.94%"
         edTextCol="ff000000" edBkgCol="0" labelText="Description" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default sans-serif font"
         fontsize="15" kerning="0" bold="0" italic="0" justification="9"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
