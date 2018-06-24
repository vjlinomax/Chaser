/*
  ==============================================================================

	MultiDragButton.cpp
	Created: 15 May 2018 7:38:46am
	Author:  Joris

  ==============================================================================
*/

#include "MultiDragButton.h"

MultiDragButton::MultiDragButton() : Button("")
{
}

MultiDragButton::~MultiDragButton()
{
	lastDraggedButton = nullptr;
}


void MultiDragButton::mouseDrag( const MouseEvent &event )
{
	Button::mouseDrag( event );

	Component* parent = getParentComponent();
	MultiDragButton* button = dynamic_cast<MultiDragButton*>( parent->getComponentAt( event.getPosition() + getPosition() ) );

	//if we're actually over a slicebutton and it wasn't the last button we toggled
	if ( button && button != lastDraggedButton )
	{
		bool state = button->getToggleState();
		button->setToggleState( !state, sendNotification );
		lastDraggedButton = button;
	}
}
