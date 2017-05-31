/*
  ==============================================================================

  Copier.cpp
  Created: 1 Feb 2015 3:36:41pm
  Author:  Joris de Jong

  ==============================================================================
  */

#include "../JuceLibraryCode/JuceHeader.h"
#include "Copier.h"
#include "../MainComponent.h"


//==============================================================================
Copier::Copier( ChaseManager* chaseManager ) :
chaseManager( chaseManager )
{
	//create 4 buttons for x1, x2, x4 and x8
	for ( int i = 0; i < 4; i++ )
	{
		TextButton* b = new TextButton( String( i ) );
		b->setButtonText( "x" + String( pow( 2, i ) ) );
		ColourLookAndFeel claf;
		b->setColour( TextButton::buttonColourId, claf.backgroundColour );
		b->addListener( this );
		addAndMakeVisible( b );
		buttons.add( b );
	}
}

Copier::~Copier()
{

}

void Copier::buttonClicked( Button* b )
{
	int multiplier = int( pow( 2, buttons.indexOf( b ) ) );

	int copiesThatFitInSequence = int( floor( (chaseManager->getLastStepIndex() + 1) / multiplier ) );
	for ( int i = 1; i < copiesThatFitInSequence; i++ )
	{
		int nextStep = chaseManager->getCurrentStepIndex() + i  * multiplier;
		if ( nextStep >= chaseManager->getLastStepIndex() + 1 )
			nextStep -= chaseManager->getLastStepIndex() + 1;

		chaseManager->setStep( chaseManager->getCurrentSequenceIndex(), nextStep, chaseManager->getCurrentStep() );
	}
}

void Copier::paint( Graphics& )
{

}

void Copier::resized()
{
	float h = 1.0f / buttons.size() - 0.01f;
	for ( int i = 0; i < buttons.size(); i++ )
	{
		buttons[ i ]->setBoundsRelative( 0.01f, 0.01f + i * h + 0.01f, 0.98f, h );
	}
}
