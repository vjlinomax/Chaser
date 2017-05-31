/*
  ==============================================================================

  Copier.h
  Created: 1 Feb 2015 3:36:41pm
  Author:  Joris de Jong

  ==============================================================================
  */

#ifndef COPIER_H_INCLUDED
#define COPIER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "ColourLookAndFeel.h"
#include "../chase/ChaseManager.h"




/*

 This class is used to copy the current step to other steps
 */
class Copier : public Component, public Button::Listener
{
public:
	Copier( ChaseManager* chaseManager );
	~Copier();

	//listener functions
	virtual void buttonClicked( Button* );

	//component functions
	void paint( Graphics& );
	void resized();

private:
	OwnedArray<Button> buttons;
	ChaseManager* chaseManager;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR( Copier )
};


#endif  // COPIER_H_INCLUDED
