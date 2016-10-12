/*
  ==============================================================================

  This file was auto-generated!

  ==============================================================================
  */

#include "MainComponent.h"
#include "../../HybridApi/Source/HybridApi.h"

//==============================================================================
MainContentComponent::MainContentComponent()
{
	laf = new ColourLookAndFeel();

	setLookAndFeel( laf );

	version = ProjectInfo::versionString;

	xmlManager = new ChaserXmlManager();
	xmlManager->setSaveFile( File::getSpecialLocation( File::userDocumentsDirectory ).getChildFile( "Chaser/chaserBeta.xml" ) );

	chaseManager = new ChaseManager( xmlManager );
	sliceManager = new SliceManager( xmlManager );

	previewWindow = new Preview();
	addAndMakeVisible( previewWindow );

	sliceList = new SliceList();
	addAndMakeVisible( sliceList );

	sequencer = new Sequencer( *this );
	addAndMakeVisible( sequencer );

	copier = new Copier();
	addAndMakeVisible( copier );

	//add a menu bar
	menuBar = new MenuBarComponent( this );
#if JUCE_WINDOWS
	addAndMakeVisible( menuBar );
#elif JUCE_MAC
	setMacMainMenu(this);
#endif

	addKeyListener( this );

	setSize( 1280, 720 );

	//start a timer to update the window name
	startTimer( 1000 );
}

MainContentComponent::~MainContentComponent()
{
	previewWindow = nullptr;
	sliceList = nullptr;
	xmlManager = nullptr;
#if JUCE_MAC
	setMacMainMenu(nullptr);
#endif
	stepToCopy.clear();
	sequenceToCopy = Sequence();

	chaseManager = nullptr;
	sliceManager = nullptr;
}

void MainContentComponent::timerCallback()
{
	stopTimer();

	//if we can't load the existing chaser, create a new one
	if ( !createChaserFromChaserFile() )
		createChaserFromAssFile( FileHelper::getAssFileAutomagically( true ), false );

	//set the name
	getTopLevelComponent()->setName( FileHelper::getLastUsedChaserFile().getFileNameWithoutExtension());

	//resize to update preview window
	resized();
}

StringArray MainContentComponent::getMenuBarNames()
{
	const char* const names[] = { "File", "Edit", nullptr };
	return StringArray( names );
}

PopupMenu MainContentComponent::getMenuForIndex( int menuIndex, const juce::String & )
{
	PopupMenu menu;

	if ( menuIndex == 0 )
	{
		menu.addItem( 1, "New Chaser" );
		menu.addSeparator();

		menu.addItem( 2, "Load Chaser", false, false );
		menu.addItem( 3, "Save Chaser as...", false, false );
		menu.addSeparator();

		menu.addItem( 4, "Load Arena Setup", false, false );
		//if the last used arena file still exists, enable the option to reload it
		bool isAvailable = FileHelper::isFileValid( sliceManager->getAssFile() );
		menu.addItem( 5, "Reload Arena Setup", isAvailable );
		menu.addItem( 6, "Autoload", true, autoload );
	}

	else if ( menuIndex == 1 )
	{
		menu.addItem( 1, "Copy Step " + KeyPress( 'c', ModifierKeys::commandModifier, NULL ).getTextDescriptionWithIcons() );
		menu.addItem( 2, "Paste Step " + KeyPress( 'v', ModifierKeys::commandModifier, NULL ).getTextDescriptionWithIcons() );
		menu.addSeparator();
		menu.addItem( 3, "Copy Sequence" );
		menu.addItem( 4, "Paste Sequence" );
	}

	return menu;
}

void MainContentComponent::menuItemSelected( int menuItemID, int topLevelMenuIndex )
{
	if ( topLevelMenuIndex == 0 )
	{
		switch ( menuItemID )
		{
		case 1:
			//new chaser
			createChaserFromAssFile( FileHelper::getAssFileAutomagically( true ), true );
			break;
		case 2:
			//load existing chaser
			break;
		case 3:
			//save the chaser under a new name
			break;
		case 4:
			//load the arena setup
			break;
		case 5:
			//reload the arena setup
			createChaserFromAssFile( FileHelper::getAssFileAutomagically( false ), false );
			break;
		case 0:
		default:
			return;
		}
	}

	else if ( topLevelMenuIndex == 1 )
	{
		switch ( menuItemID )
		{
		case 1:
			copyStep();
			break;
		case 2:
			pasteStep();
			break;
		case 3:
			copySequence();
			break;
		case 4:
			pasteSequence();
			break;

		case 0:
		default:
			return;
		}
	}
}

void MainContentComponent::createChaserFromAssFile( File assFile, bool createNew )
{
	ResXmlParser::parseAssFile( assFile, sliceManager->getSlices(), sliceManager->getResolution() );
	sliceManager->getAssFile() = assFile;

	//now populate the previewwindow with buttons for these slices
	previewWindow->createSliceButtons( sliceManager->getSlices() );
	previewWindow->resized();

	//now populate the slicelist with entries for these slices
	sliceList->addSlices( sliceManager->getSlices() );
	sliceList->resized();

	//at this point, all the slices have their position and screens assigned
	//so we can save this to xml
	sliceManager->writeToXml();

	//see if we need to reload an existing chaser or create a fresh one
	if ( createNew )
		chaseManager->clearAll();

	//make the first step active
	sequencer->selectStep( 0 );
}

bool MainContentComponent::createChaserFromChaserFile()
{
	//check if we have a Chaser file previously saved
	//if so, load that bad boy
	File lastUsedChaser = FileHelper::getLastUsedChaserFile();
	
	if ( FileHelper::isFileValid( lastUsedChaser ) )
	{
		//this will return 1920x1080 if no resolution was saved
		ChaserXmlParser::parseResolution( lastUsedChaser, sliceManager->getResolution() );
		ChaserXmlParser::parseAssFile( lastUsedChaser, sliceManager->getAssFile() );
		ChaserXmlParser::parseSlices( lastUsedChaser, sliceManager->getSlices() );

		//now populate the previewwindow with buttons for these slices
		previewWindow->createSliceButtons( sliceManager->getSlices() );
		previewWindow->resized();

		//now populate the slicelist with entries for these slices
		sliceList->addSlices( sliceManager->getSlices() );
		sliceList->resized();

		//this will try its best to get useful info from the chaserfile
		chaseManager->createSequencesFromXml( ChaserXmlParser::parseSequences( lastUsedChaser ) );

		previewWindow->setActiveSlices( chaseManager->getCurrentStep() );

		//make the first step active
		sequencer->selectStep( 0 );

		return true;
	}

	return false;
}

void MainContentComponent::saveXml()
{
	/*
	if (!xmlSequence->save())
	{
	DBG("SAVE ERROR!");
	throwSaveError();
	}
	*/
}

bool MainContentComponent::saveAsXml()
{
	/*
	//open a save dialog
	File chaserLocation = File::getSpecialLocation( File::SpecialLocationType::userDocumentsDirectory ).getFullPathName() + "/Chaser/";
	if (!chaserLocation.exists())
	chaserLocation.createDirectory();
	FileChooser fc ( "Save chaser as...", chaserLocation, "*.xml", true );
	if ( fc.browseForFileToSave(true) )
	{
	File f = fc.getResult();

	xmlSequence->setXmlFile( f );
	saveXml();

	getTopLevelComponent()->setName(f.getFileNameWithoutExtension());
	return true;
	}
	*/
	return false;
}

void MainContentComponent::loadXml()
{
	/*
	//open a load dialog
	File chaserLocation = File::getSpecialLocation( File::SpecialLocationType::userDocumentsDirectory ).getFullPathName() + "/Chaser/";
	FileChooser fc ( "Pick a Chaser file...", chaserLocation, "*.xml", true );

	if ( fc.browseForFileToOpen() )
	{
	File f = fc.getResult();

	if ( !xmlSequence->loadXmlFile( f ))
	{
	throwLoadError();
	}
	else
	{
	reloadSliceData();
	getTopLevelComponent()->setName(f.getFileNameWithoutExtension());
	resized();
	}
	}
	*/
}

void MainContentComponent::reloadSliceData()
{
	/*
	//sliceList->clearSlices();
	previewWindow->clearSlices();
	slices.clear();

	//load the slices from the xml if they exist
	Array<Slice*> slicesInXml = xmlSequence->getSlices();
	for ( int i = 0; i < slicesInXml.size(); i++ )
	{
	Slice* s = slicesInXml[i];
	previewWindow->addSlice (s) ;

	slices.add(s);
	}

	sliceList->addSlices( slices );

	//update the view for the first step
	activeSlices = xmlSequence->getStep( currentSequence, currentStep );
	previewWindow->setSlices( activeSlices );
	sequencer->setSequenceNames ( xmlSequence->getSequenceNames() );
	sequencer->setSequenceLengths ( xmlSequence->getSequenceLengths() );
	currentSequenceLength = xmlSequence->getSequenceLengths()[ currentSequence ];

	resolution = xmlSequence->getResolution();
	*/
	resized();
}

bool MainContentComponent::keyPressed( const juce::KeyPress &key, juce::Component * )
{

	if ( key == KeyPress::leftKey )
	{
		sequencer->previousStep();
	}
	else if ( key == KeyPress::rightKey )
	{
		sequencer->nextStep();
	}
	else if ( key == KeyPress( 'c', ModifierKeys::commandModifier, NULL ) )
	{
		copyStep();
	}
	else if ( key == KeyPress( 'v', ModifierKeys::commandModifier, NULL ) )
	{
		pasteStep();
	}

	return true;
}

void MainContentComponent::copyStep()
{
	stepToCopy = chaseManager->getCurrentStep();
}

void MainContentComponent::pasteStep()
{
	previewWindow->setActiveSlices( stepToCopy );
}

void MainContentComponent::copySequence()
{
	//sequenceToCopy = chaseManager->getCurrentSequence();
	/*
	//copy the name
	sequenceNameToCopy = xmlSequence->getSequenceNames()[currentSequence];
	//create an array with an array
	currentSequenceSlices.clear();
	for ( int i = 0; i < currentSequenceLength; i++ )
	{
	Array<int> step = xmlSequence->getStep(currentSequence, i);
	currentSequenceSlices.add( step );
	}
	*/
}

void MainContentComponent::pasteSequence()
{
	
}

void MainContentComponent::resized()
{
	Rectangle<int> area( getLocalBounds() );

	int menuBarHeight = 0;
#if JUCE_WINDOWS
	menuBarHeight = LookAndFeel::getDefaultLookAndFeel().getDefaultMenuBarHeight();
#elif JUCE_MAC
	menuBarHeight = 0;
#endif

	menuBar->setBounds( area.removeFromTop( menuBarHeight ) );

	Rectangle<int> previewArea = Rectangle < int > {0, 0, int( getWidth() * 0.83 ), int( (getWidth() / 16.0) * 9.0 * 0.83 ) };
	previewArea.setPosition( 0, menuBarHeight );

	previewWindow->setBounds( 0, 0, sliceManager->getResolution().x, sliceManager->getResolution().y );
	Rectangle<int> previewWindowArea = previewArea.reduced( 5 );

	if ( previewWindow->getWidth() > 0 && previewWindow->getHeight() > 0 )
		previewWindow->setBoundsToFit( previewWindowArea.getX(), previewWindowArea.getY(), previewWindowArea.getWidth(), previewWindowArea.getHeight(), Justification::centred, false );

	Rectangle<int> sliceArea = Rectangle < int > { previewArea.getWidth(), menuBarHeight, area.getWidth() - previewArea.getWidth(), previewArea.getHeight()};
	sliceList->setBounds( sliceArea.reduced( 5 ) );

	Rectangle<int> bottomArea = area.removeFromBottom( area.getHeight() - previewArea.getHeight() );
	copier->setBounds( bottomArea.removeFromLeft( 75 ).reduced( 5 ) );
	sequencer->setBounds( bottomArea.reduced( 5 ) );
}
