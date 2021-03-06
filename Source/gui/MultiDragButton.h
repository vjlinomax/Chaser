/*
  ==============================================================================

    MultiDragButton.h
    Created: 15 May 2018 7:38:46am
    Author:  Joris

	MultiDragButton will let you click and drag across a set of buttons and toggle them
  ==============================================================================
*/

#include "JuceHeader.h"

class MultiDragButton : public Button
{
public:
	MultiDragButton();
	~MultiDragButton();

	void mouseDown( const MouseEvent& ) override;
	void mouseUp( const MouseEvent& ) override;
	void mouseDrag( const MouseEvent& ) override;

private:
	MultiDragButton* lastDraggedButton;
	bool weWereDragging;


};

#pragma once
