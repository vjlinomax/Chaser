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
	//try to get the last used chaser file
	File lastChaser = FileHelper::getLastUsedChaserFile();
	if ( !FileHelper::isFileValid( lastChaser ) )
		lastChaser = File::getSpecialLocation( File::userDocumentsDirectory ).getChildFile( "Chaser/chaserBeta.xml" );
	xmlManager->setSaveFile( lastChaser );

	//save the version
	XmlElement* versionXml = new XmlElement( "version" );
	versionXml->setAttribute( "nr", version );
	xmlManager->saveXmlElement( versionXml );

	chaseManager = new ChaseManager( xmlManager );
	sliceManager = new SliceManager( xmlManager );

	previewWindow = new Preview( chaseManager, sliceManager );
	addAndMakeVisible( previewWindow );

	sliceList = new SliceList( sliceManager, previewWindow );
	
	listBrowser = new ListBrowser( );
	
	listBrowser->addTab( "Slices", Colours::lightgrey, sliceList, false );
	listBrowser->addTab( TRANS( "Tab 1" ), Colours::lightgrey, 0, false );
	addAndMakeVisible( listBrowser );

	sequencer = new Sequencer( chaseManager, previewWindow );
	addAndMakeVisible( sequencer );

	copier = new Copier( chaseManager );
	addAndMakeVisible( copier );

	creator = new ChaserCreator( sliceManager, chaseManager, previewWindow, sliceList, sequencer );

	//start checking for ass file updates once per second
	autoUpdate = new AutoUpdateTimer( creator );
	autoUpdate->start();

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

	stepToCopy.clear();
	sequenceToCopy = Sequence();

#if JUCE_MAC
	setMacMainMenu(nullptr);
#endif

	chaseManager = nullptr;
	sliceManager = nullptr;
	xmlManager = nullptr;
}

void MainContentComponent::timerCallback()
{
	stopTimer();

	//if we can't load the existing chaser, create a new one
	if ( !creator->createChaserFromChaserFile() )
		creator->createChaserFromAssFile( FileHelper::getAssFileAutomagically( true ), false );

	//set the name
	getTopLevelComponent()->setName( FileHelper::getLastUsedChaserFile().getFileNameWithoutExtension() );

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

		menu.addItem( 2, "Load Chaser" );
		menu.addItem( 3, "Save Chaser as..." );
		menu.addSeparator();

		//if the last used arena file still exists, enable the option to reload it
		bool isAvailable = FileHelper::isFileValid( sliceManager->getAssFile(), false );

		//if there is no active assfile, stop the timer
		if ( !isAvailable )
			autoUpdate->stop();

		menu.addItem( 4, "Autoload", isAvailable, autoUpdate->isTimerRunning() );
	}

	else if ( menuIndex == 1 )
	{
		menu.addItem( 1, "Copy Step " + KeyPress( 'c', ModifierKeys::commandModifier, NULL ).getTextDescriptionWithIcons() );
		menu.addItem( 2, "Paste Step " + KeyPress( 'v', ModifierKeys::commandModifier, NULL ).getTextDescriptionWithIcons() );
		menu.addItem( 3, "Clear Step" );
		menu.addItem( 4, "Invert Step" );
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
		{
			//new chaser
			File defaultChaser = File::getSpecialLocation( File::userDocumentsDirectory ).getChildFile( "Chaser/chaserBeta.xml" );
			xmlManager->setSaveFile( defaultChaser );
			creator->createChaserFromAssFile( FileHelper::getAssFileAutomagically( true ), true );
			getTopLevelComponent()->setName( defaultChaser.getFileNameWithoutExtension() );
		}
			break;
		case 2:
			//load existing chaser
			loadChaser();
			break;
		case 3:
			//save the chaser under a new name
			break;
		case 4:
			//toggle autoreload
			if ( autoUpdate->isTimerRunning() )
				autoUpdate->stop();
			else
				autoUpdate->start();
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
		{
			//clear the step
			Step empty;
			chaseManager->setCurrentStep( empty );
			previewWindow->setActiveSlices();
			break;
		}
		case 4:
			//invert step
			previewWindow->invertActiveSlices();
			break;
		case 0:
		default:
			return;
		}
	}
}


void MainContentComponent::saveChaser()
{
	/*
	if (!xmlSequence->save())
	{
	DBG("SAVE ERROR!");
	throwSaveError();
	}
	*/
}

bool MainContentComponent::saveChaserAs()
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

void MainContentComponent::loadChaser()
{

	//open a load dialog
	File chaserLocation = File::getSpecialLocation( File::SpecialLocationType::userDocumentsDirectory ).getFullPathName() + "/Chaser/";
	FileChooser fc( "Pick a Chaser file...", chaserLocation, "*.xml", true );

	if ( fc.browseForFileToOpen() )
	{
		File f = fc.getResult();
		if ( creator->createChaserFromChaserFile( f ) )
		{
			getTopLevelComponent()->setName( f.getFileNameWithoutExtension() );
			xmlManager->setSaveFile( f );
		}
	}
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
	chaseManager->setCurrentStep( stepToCopy );
	previewWindow->setActiveSlices();
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
	listBrowser->setBounds( sliceArea.reduced( 5 ) );

	float relativeBarHeight = float( listBrowser->getTabBarDepth() ) / float( listBrowser->getHeight() );
	sliceList->setBoundsRelative( 0.0f, relativeBarHeight, 1.0f, 1.0f - relativeBarHeight );

	Rectangle<int> bottomArea = area.removeFromBottom( area.getHeight() - previewArea.getHeight() );
	copier->setBounds( bottomArea.removeFromLeft( 75 ).reduced( 5 ) );
	sequencer->setBounds( bottomArea.reduced( 5 ) );
}
