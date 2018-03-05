/*
  ==============================================================================

	Achievement.cpp
	Created: 5 Mar 2018 6:53:39pm
	Author:  Joris

  ==============================================================================
*/

#include "Achievement.h"

Achievement::Achievement( Types type ) : type ( type )
{
}

Achievement::~Achievement()
{
}


void Achievement::add()
{
}

void Achievement::trigger()
{
	
	switch ( type )
	{
	case Types::Darklord:
		AlertWindow::showMessageBox( AlertWindow::AlertIconType::WarningIcon,
			"Greetings, Hellchild!",
			"You have set your sequence to exactly 666 steps. You are truly the spawn of Satan.",
			"All Hail!", nullptr );
	}
}

Achievement::Types Achievement::getType()
{
	return type;
}

void Achievement::writeToXml()
{
}
