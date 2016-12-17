/*
  ==============================================================================

  ChaseManager.h
  Created: 21 Feb 2016 5:04:36pm
  Author:  Joris de Jong

  ==============================================================================
  */

#ifndef CHASEMANAGER_H_INCLUDED
#define CHASEMANAGER_H_INCLUDED

#include "JuceHeader.h"
#include "../xml/ChaserXmlManager.h"
#include "../gui logic/UpdateableComponent.h"

//the active slices in a step, slices are identified by their uniqueId
typedef Array<int64> Step;

struct Sequence
{
	Array<Step> steps;
	String name;

	bool isEmpty()
	{
		for ( auto step : steps )
			if ( step.size() > 0 )
				return false;

		return true;
	}
};

//ChaseManager gives access to the sequences, their steps and the active slices in the step

class ChaseManager
{
public:
	ChaseManager( ChaserXmlManager* xmlManager );
	~ChaseManager();

	void setStep( int sequence, int step, Step activeSlices );
	void setCurrentStep( Step activeSlices );
	Step getStep( int sequence, int step );
	Step getCurrentStep();

	void clearAll();

	void skipToSequence( int i );
	void skipToStep( int i );

	//will return the new current step
	int skipToNextStep();
	int skipToPreviousStep();

	//will return the new current sequence
	int skipToNextSequence();
	int skipToPreviousSequence();

	//will return the new max amount of steps in the current sequence
	//steps are always added and removed from the end of the sequence
	int addStep( bool write = true);
	int removeStep( bool write = true );
	int setStepCount( int i );

	//will return the new max amount of sequences
	int addSequence();
	int removeCurrentSequence();

	int getLastStepIndex();
	int getLastSequenceIndex();

	int getCurrentStepIndex();
	int getCurrentSequenceIndex();

	String getCurrentSequenceName();
	void setCurrentSequenceName( String newName );
	StringArray getSequenceNames();

	void createSequencesFromXml( XmlElement sequenceElement );

	void addUpdateable( UpdateableComponent* newUpdateable );

private:
	void setDefaults();

	void fillSequence();
	int currentStep;
	int currentSequence;

	Array<Sequence> sequences;

	void writeToXml();
	XmlElement* getSequencesAsXml();

	ChaserXmlManager* xmlManager;

	Array<UpdateableComponent*> updateables;
	void updateComponents();
};



#endif  // CHASEMANAGER_H_INCLUDED
