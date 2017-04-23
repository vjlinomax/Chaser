/*
  ==============================================================================

    UpdateableComponent.h
    Created: 17 Dec 2016 8:34:36pm
    Author:  Joris

  ==============================================================================
*/

#ifndef UPDATEABLECOMPONENT_H_INCLUDED
#define UPDATEABLECOMPONENT_H_INCLUDED

//we use this so we can store a bunch of components in a manager
//and let them decide themselves what needs to happen 
//when they need to be updated

class UpdateableComponent
{
public:
	UpdateableComponent();
	virtual ~UpdateableComponent();

	virtual void update() = 0;
};



#endif  // UPDATEABLECOMPONENT_H_INCLUDED
