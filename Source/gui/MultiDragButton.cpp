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

void MultiDragButton::mouseDown( const MouseEvent &event )
{
	Button::mouseDown( event );
}

void MultiDragButton::mouseUp( const MouseEvent &event )
{
	//if this button was part of the last drag action,
	//don't do anything, the state will already have been toggled
	if ( lastDraggedButton == this )
	{
		lastDraggedButton = nullptr;
		return;
	}

	//otherwise do the regular mouse up action
	Button::mouseUp( event );
	setToggleState( !getToggleState(), sendNotification );
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
