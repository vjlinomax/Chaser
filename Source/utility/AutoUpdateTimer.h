/*
  ==============================================================================

  AutoUpdateTimer.h
  Created: 12 Oct 2016 8:36:04pm
  Author:  Joris

  ==============================================================================
  */

#ifndef AUTOUPDATETIMER_H_INCLUDED
#define AUTOUPDATETIMER_H_INCLUDED

#include "JuceHeader.h"
#include "ChaserCreator.h"

class AutoUpdateTimer :
	public Timer
{
public:
	AutoUpdateTimer( ChaserCreator* creator );
	~AutoUpdateTimer();

	void timerCallback() override;

	void start();
	void stop();

private:
	ChaserCreator* creator;
	Time lastModifiedTime;
};




#endif  // AUTOUPDATETIMER_H_INCLUDED
