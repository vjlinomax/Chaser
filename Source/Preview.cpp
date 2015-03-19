/*
  ==============================================================================

    Preview.cpp
    Created: 28 Dec 2014 11:00:00am
    Author:  Joris de Jong

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Preview.h"

//==============================================================================
Preview::Preview()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    sliceLaf = new SliceLookAndFeel();
}

Preview::~Preview()
{
    clearSlices();
}

void Preview::buttonClicked(Button* b)
{
	//create an int array of all the slices that are active
	Array<int> activeSlices;
	for ( int i = 0; i < sliceButtons.size(); i++ )
	{
		if ( sliceButtons[i]->getToggleState() )
		{
			activeSlices.add(i);
		}
	}
	//let the listeners know
	Component::BailOutChecker checker (this);
	if (! checker.shouldBailOut())
		previewListeners.callChecked ( checker, &Listener::sliceClicked, activeSlices );
}

void Preview::setSlices(Array<int> activeSlices)
{
    //turn all the slices off
    for ( int i = 0; i < sliceButtons.size(); i++ )
    {
        sliceButtons[i]->setToggleState( false, dontSendNotification );
    }
    
    //turn on the ones that should be on
    for ( int i = 0; i < activeSlices.size(); i++ )
    {
        //it could be that the xml we have loaded has more slices than the current button layout
        //for instance because it has been changed in Resolume
        //so here we make sure we aren't trying to change a buttonstate that no longer exists
        if ( activeSlices[i] < sliceButtons.size() )
            sliceButtons[activeSlices[i]]->setToggleState(true, dontSendNotification);
    }
    
}

void Preview::addSlice( Slice* slice )
{
    SliceButton* newButton = new SliceButton ( slice );
    
    newButton->setLookAndFeel(sliceLaf);
    newButton->setColour(TextButton::buttonColourId, Colours::darkgrey);
    newButton->setColour(TextButton::buttonOnColourId, sliceLaf->getOutlineColour());
    newButton->setColour(TextButton::textColourOffId, Colours::whitesmoke);
    newButton->setColour(TextButton::textColourOnId, Colours::black);
    newButton->addListener(this);
    sliceButtons.add( newButton );

    if ( newButton->enabled )
        addAndMakeVisible( newButton );
    else
        addChildComponent( newButton );
}

void Preview::clearSlices()
{
    sliceButtons.clear();
}

void Preview::update(Slice* slice, int i)
{
    sliceButtons[i]->setVisible( slice->enabled );
}

void Preview::paint (Graphics& g)
{
    
    g.fillAll (Colours::black);   // clear the background
    
    g.setColour (sliceLaf->getOutlineColour());
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
    
    g.setColour (sliceLaf->getOutlineColour());
    g.setFont (14.0f);
    g.drawText ("Preview", getLocalBounds(),
                Justification::centred, true);   // draw some placeholder text
}

void Preview::resized()
{
    
    for ( int i = 0; i < sliceButtons.size(); i++ )
    {
        sliceButtons[i]->setBoundsRelative(sliceButtons[i]->proportionalX, sliceButtons[i]->proportionalY, sliceButtons[i]->proportionalW, sliceButtons[i]->proportionalH);
    }

}

