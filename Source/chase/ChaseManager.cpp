/*
  ==============================================================================

  ChaseManager.cpp
  Created: 21 Feb 2016 5:04:36pm
  Author:  Joris de Jong

  ==============================================================================
  */

#include "ChaseManager.h"


ChaseManager::ChaseManager( ChaserXmlManager* xmlManager, SliceManager* sliceManager, AchievementManager* achievementManager ) : 
	xmlManager( xmlManager ), sliceManager( sliceManager ), achievementManager ( achievementManager )
{
	setDefaults();
}

ChaseManager::~ChaseManager()
{

}

void ChaseManager::timerCallback()
{
	updateComponents();

	if ( xmlManager )
		xmlManager->saveXmlElement( getSequencesAsXml() );

	stopTimer();
	
}

void ChaseManager::setDefaults()
{
	//create 16 sequences with 16 empty steps
	sequences.resize( 16 );
	for ( int i = 0; i < sequences.size(); i++ )
	{
		Sequence sequence;
		sequence.steps.resize( 16 );
		sequence.name = "Sequence " + String( i + 1 );

		sequences.set( i, sequence );
	}

	currentStep = 0;
	currentSequence = 0;
}

void ChaseManager::clearAll()
{
	sequences.clear();
	setDefaults();
    
    for ( auto component : updateables )
        component->reload(); //this will make sure I only update the sequence settings on a load action

	writeToXml();
}

void ChaseManager::setStep( int sequenceIndex, int stepIndex, Step activeSlices )
{
	if ( sequences.size() <= sequenceIndex )
	{
		DBG( "Trying to set a sequence which doesn't exist!" );
		return;
	}
	if ( sequences[sequenceIndex].steps.size() <= stepIndex )
	{
		DBG( "Trying to set a step which doesn't exist!" );
		return;
	}

	Sequence sequence = sequences[sequenceIndex];
	sequence.steps.set( stepIndex, activeSlices );
	sequences.set( sequenceIndex, sequence );
	writeToXml();
}

void ChaseManager::setCurrentStep( Step activeSlices )
{
	setStep( getCurrentSequenceIndex(), getCurrentStepIndex(), activeSlices );
}

Step ChaseManager::getStep( int sequence, int step )
{
	if ( sequences.size() > sequence )
		if ( sequences[sequence].steps.size() > step )
			return sequences[sequence].steps[step];

	return Array < int64 > {};
}

Step ChaseManager::getCurrentStep()
{
	return getStep( getCurrentSequenceIndex(), getCurrentStepIndex() );
}

void ChaseManager::fillSequence()
{
	//first make sure we have enough sequences
	if ( sequences.size() <= currentSequence )
		sequences.resize( currentSequence + 1 );

	//then make sure the sequence is empty
	//otherwise we can just leave it
	if ( sequences[currentSequence].steps.size() == 0 )
	{
		//create 16 steps for this sequence and name it
		Sequence sequence;
		sequence.steps.resize( 16 );
		sequence.name = "Sequence " + String( currentSequence + 1 );
		sequences.set( currentSequence, sequence );
	}
}

void ChaseManager::skipToSequence( int i )
{
	if ( i > 512 )
		return;

	while ( i > getLastSequenceIndex() )
	{
		sequences.resize( sequences.size() + 1 );
		writeToXml();
	}

	currentSequence = i;
	
	//make sure the sequence is filled
	fillSequence();

	//when skipping sequences, the current step always resets
	skipToStep( 0 );
}

int ChaseManager::skipToNextSequence()
{
	currentSequence++;

	if ( currentSequence > getLastSequenceIndex() )
		currentSequence = 0;

	//make sure the sequence is filled
	fillSequence();

	//when skipping sequences, the current step always resets
	skipToStep( 0 );

	return currentSequence;
}

int ChaseManager::skipToPreviousSequence()
{
	currentSequence--;
	if ( currentSequence < 0 )
		currentSequence = getLastSequenceIndex();

	//make sure the sequence is filled
	fillSequence();

	//when skipping sequences, the current step always resets
	skipToStep( 0 );

	return currentSequence;
}

void ChaseManager::skipToStep( int i )
{
	if ( i > getLastStepIndex() )
		i = getLastStepIndex();

	currentStep = i;

	updateComponents();
}

int ChaseManager::skipToNextStep()
{
	currentStep++;

	if ( currentStep > getLastStepIndex() )
		currentStep = 0;

	updateComponents();

	return currentStep;
}

int ChaseManager::skipToPreviousStep()
{
	currentStep--;

	if ( currentStep < 0 )
		currentStep = getLastStepIndex();

	updateComponents();

	return currentStep;
}

int ChaseManager::setStepCount( int i )
{
	if ( i <= 1024 && i > 0 )
	{
		while ( i > getLastStepIndex() + 1 )
			addStep( false );
		while ( i < getLastStepIndex() + 1 )
			removeStep( false );

		if ( i == 666 )
			if ( achievementManager )
				achievementManager->getAchievement( Achievement::Types::Darklord )->trigger();

	}

	writeToXml();

	return getLastStepIndex();
}

int ChaseManager::addSequence()
{
	//this will make sure the sequence gets created and filled
	currentSequence = getLastSequenceIndex() + 1;
	fillSequence();

	writeToXml();

	return getLastSequenceIndex() + 1;
}

int ChaseManager::removeSequence( int index )
{
	if ( index <= getLastSequenceIndex() && index > -1 )
	{
		sequences.removeRange( index, 1 );
	}

	writeToXml();

	return getLastSequenceIndex() + 1;
}

int ChaseManager::addStep( bool write )
{
	if ( sequences.size() == 0 )
		return 0;

	Sequence sequence = sequences[getCurrentSequenceIndex()];
	sequence.steps.resize( sequence.steps.size() + 1 );

	sequences.set( getCurrentSequenceIndex(), sequence );

	if ( write )
		writeToXml();

	return getLastStepIndex();
}

int ChaseManager::removeStep( bool write )
{
	//only remove as long as we still have more than 1 step
	if ( getLastStepIndex() > 0 )
	{
		Sequence sequence = sequences[getCurrentSequenceIndex()];
		sequence.steps.resize( sequence.steps.size() - 1 );

		sequences.set( getCurrentSequenceIndex(), sequence );

		if ( write )
			writeToXml();
	}

	return getLastStepIndex();
}

int ChaseManager::getLastStepIndex()
{
	return sequences[getCurrentSequenceIndex()].steps.size() - 1;
}

int ChaseManager::getLastSequenceIndex()
{
	return sequences.size() - 1;
}

int ChaseManager::getCurrentStepIndex()
{
	//check if we're still in bounds
	if ( currentStep > getLastStepIndex() )
		currentStep = getLastStepIndex();

	return currentStep;
}

int ChaseManager::getCurrentSequenceIndex()
{
	if ( currentSequence > getLastSequenceIndex() )
		currentSequence = getLastSequenceIndex();

	return currentSequence;
}

String ChaseManager::getCurrentSequenceName()
{
	return sequences[getCurrentSequenceIndex()].name;
}

void ChaseManager::setCurrentSequenceName( juce::String newName )
{
	Sequence sequence = sequences[getCurrentSequenceIndex()];
	sequence.name = newName;

	sequences.set( getCurrentSequenceIndex(), sequence );

	writeToXml();
}

StringArray ChaseManager::getSequenceNames()
{
	StringArray returnArray;

	for ( int i = 0; i < getLastSequenceIndex() + 1; i++ )
	{
		//fill the array with default names
		//unless the sequence exists and has its own name
		String name = "Sequence " + String( i + 1 );
		if ( sequences[i].name != String() )
			name = sequences[i].name;
		returnArray.add( name );
	}

	return returnArray;
}

XmlElement* ChaseManager::getSequencesAsXml()
{
	XmlElement* sequencesXml = new XmlElement( "sequences" );
	sequencesXml->setAttribute( "count", sequences.size() );

	//loop through all the sequences
	for ( int i = 0; i < sequences.size(); i++ )
	{
		Sequence sequence = sequences[i];
		//check if the sequence has anything to save
		if ( !sequence.isEmpty() || //only save if there are any steps to save
			(sequence.steps.size() != 16 && sequence.steps.size() != 0) || // or if the stepcount is unusual, but more than 0
			(sequence.name != "Sequence " + String( i + 1 ) && sequence.name != "") ) //of if the name is unusual, but not empty
		{
			//for every sequence, create an xmlelement and store the name
			XmlElement* sequenceXml = new XmlElement( "sequence" );
			sequenceXml->setAttribute( "nr", i );
			sequenceXml->setAttribute( "name", sequences[i].name );
			sequencesXml->addChildElement( sequenceXml );

			//then create an xmlelement to store the steps
			XmlElement* stepsXml = new XmlElement( "steps" );
			stepsXml->setAttribute( "count", sequence.steps.size() );
			sequenceXml->addChildElement( stepsXml );

			//loop through this sequence's steps
			for ( int j = 0; j < sequence.steps.size(); j++ )
			{
				Step step = sequence.steps[j];
				//only store filled steps
				if ( step.size() > 0 )
				{
					//for every step, create an xmlelement and store the step nr
					XmlElement* stepXml = new XmlElement( "step" );
					stepXml->setAttribute( "nr", j );
					stepsXml->addChildElement( stepXml );

					//loop through this step's active slices
					for ( int64 sliceUid : step )
					{
						//for every active slice, create an xmlelement and store the slice nr
						//only store visible slices
						if ( sliceManager->isSliceShowing( sliceUid ) )
						{
							XmlElement* sliceXml = new XmlElement( "slice" );
							sliceXml->setAttribute( "uniqueId", String( sliceUid ) );
							stepXml->addChildElement( sliceXml );
						}
					}
				}
			}
		}
	}

	return sequencesXml;
}

void ChaseManager::createSequencesFromXml( XmlElement sequencesXml )
{
	//clear the current sequences
	sequences.clear();

	//keep track of how many sequences we've processed
	//we use this to give names and numbers when we
	//can't retrieve them from xml
	int sequenceCount = 0;

	//see how many sequences there should be in total
	int totalSequenceCount = sequencesXml.getIntAttribute( "count", 16 );
	sequences.resize( totalSequenceCount );

	//read out the data for each sequence
	forEachXmlChildElement( sequencesXml, sequenceXml )
	{
		int sequenceNr = sequenceXml->getIntAttribute( "nr", sequenceCount );
		String sequenceName = sequenceXml->getStringAttribute( "name", "Sequence " + String( sequenceNr + 1 ) );

		Sequence sequence = sequences[sequenceNr];
		sequence.name = sequenceName;

		//if we're out of bounds, resize the sequence array to fit
		if ( sequenceNr >= sequences.size() )
			sequences.resize( sequenceNr + 1 );

		forEachXmlChildElement( *sequenceXml, stepsXml )
		{
			int totalStepCount = stepsXml->getIntAttribute( "count", 16 );
			sequence.steps.resize( totalStepCount );

			//keep track of how many steps we've processed
			//we use this to give numbers when we
			//can't retrieve them from xml
			int stepCount = 0;

			//read out the data for each step
			forEachXmlChildElement( *stepsXml, stepXml )
			{
				int stepNr = stepXml->getIntAttribute( "nr", stepCount );

				//if we're out of bounds, resize the step array to fit
				if ( stepNr >= sequence.steps.size() )
					sequence.steps.resize( stepNr + 1 );

				//keep track of how many slices we've processed
				//we use this to give numbers when we
				//can't retrieve them from xml
				int sliceCount = 0;

				//read out the data for each slice
				Step step;
				forEachXmlChildElement( *stepXml, sliceXml )
				{
					int64 sliceUid = sliceXml->getStringAttribute( "uniqueId", String( sliceCount ) ).getLargeIntValue();
					step.add( sliceUid );

					sliceCount++;
				}
				sequence.steps.set( stepNr, step );

				stepCount++;
			}

			//write the data into the array
			sequences.set( sequenceNr, sequence );
		}
	}

	//now go through the entire array of sequences and see if any are empty
	for ( Sequence sequence : sequences )
	{
		//set default values if any values are empty
		if ( sequence.name == "" )
			sequence.name = "Sequence " + String( sequenceCount + 1 );
		if ( sequence.steps.size() == 0 )
			sequence.steps.resize( 16 );

		sequences.set( sequenceCount, sequence );
		sequenceCount++;
	}

	currentSequence = 0;
	currentStep = 0;

	for ( auto component : updateables )
	{
		component->update();
		component->reload(); //this will make sure I only update the sequence settings on a load action
	}
}

void ChaseManager::writeToXml()
{
	if (!isTimerRunning() )
		startTimerHz( 1 );
}

void ChaseManager::removeDeletedSlices( OwnedArray<hybrid::Slice>& sliceList )
{
	for ( Sequence& sequence : sequences )
		for ( Step& step : sequence.steps )
			for ( int64 uid : step )
			{
				bool contains = false;
				for ( hybrid::Slice* slice : sliceList )
				{
					if ( slice->sliceId.second == uid )
						contains = true;
				}
				if ( !contains )
				{
					step.removeAllInstancesOf( uid );
				}
			}

	writeToXml();
}

void ChaseManager::addUpdateable( UpdateableComponent* newUpdateable )
{
	updateables.add( newUpdateable );
}

void ChaseManager::updateComponents()
{
	for ( auto component : updateables )
	{
		component->update();
	}
}
