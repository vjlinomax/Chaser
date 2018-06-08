/*
  ==============================================================================

  This file was auto-generated!

  ==============================================================================
  */

#include "MainComponent.h"
#include "../../HybridApi/Source/JuceBased/ArenaHelpers/File/FileHelper.h"
#include "../../HybridApi/Source/JuceBased/Fileless/FileLess.h"
#include "utility/PixelMapCreator.h"

  //==============================================================================
MainContentComponent::MainContentComponent()
{
	laf = new ColourLookAndFeel();

	setLookAndFeel( laf );

	version = ProjectInfo::versionString;

	xmlManager = new ChaserXmlManager();

	//try to get the last used chaser file
	//this will create and return a deafult filename if nothing is found
	File lastChaser = FileLess::getLastUsedFileName( FileLess::Chaser );
	xmlManager->setSaveFile( lastChaser );

	//save the version
	XmlElement* versionXml = new XmlElement( "version" );
	versionXml->setAttribute( "nr", version );
	xmlManager->saveXmlElement( versionXml );

	achievementManager = new AchievementManager();

	sliceManager = new SliceManager( xmlManager );
	chaseManager = new ChaseManager( xmlManager, sliceManager, achievementManager );
	
	previewWindow = new Preview( chaseManager, sliceManager, achievementManager );
	addAndMakeVisible( previewWindow );

	sequencer = new Sequencer( chaseManager );
	addAndMakeVisible( sequencer );

	SliceList* sliceList = new SliceList( chaseManager, sliceManager );
	//SequenceList* sequenceList = new SequenceList( chaseManager );
	listBrowser = new ListBrowser();
	listBrowser->addComponentAsTab( sliceList, "Slices" );
	//listBrowser->addComponentAsTab( sequenceList, "Sequences" );
	addAndMakeVisible( listBrowser );

	//chaseManager->addUpdateable( sequenceList );
	chaseManager->addUpdateable( sequencer );
	chaseManager->addUpdateable( previewWindow );

	sliceManager->addUpdateable( sliceList );
	sliceManager->addUpdateable( previewWindow );

	copier = new Copier( chaseManager );
	addAndMakeVisible( copier );

	creator = new ChaserCreator( sliceManager, chaseManager, previewWindow, sliceList );

	//start checking for ass file updates once per second
	autoUpdate = new AutoUpdateTimer( creator );
	autoUpdate->start();

	//add a menu bar
	menuBar = new MenuBarComponent( this );
#if JUCE_WINDOWS
	addAndMakeVisible( menuBar );
#elif JUCE_MAC
	setMacMainMenu( this );
#endif

	addKeyListener( this );

	setSize( 1280, 720 );

	//start a timer to update the window name
	startTimer( 1000 );
}

MainContentComponent::~MainContentComponent()
{
	previewWindow = nullptr;

	stepToCopy.clear();
	sequenceToCopy = Sequence();

#if JUCE_MAC
	setMacMainMenu( nullptr );
#endif

	chaseManager = nullptr;
	sliceManager = nullptr;
	xmlManager = nullptr;
	achievementManager = nullptr;

	setLookAndFeel( nullptr );
}

void MainContentComponent::timerCallback()
{
	stopTimer();

	//if we can't load the existing chaser, create a new one
	if ( !creator->createChaserFromChaserFile( false ) ) //no need to give feedback on first startup
		creator->createChaserFromAssFile( FileHelper::getAssFileAutomagically( true ), false );

	//set the name
	getTopLevelComponent()->setName( FileLess::getLastUsedFileName( FileLess::Chaser ).getFileNameWithoutExtension() );

#ifdef DEBUG
	
#endif // DEBUG

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

		menu.addItem( 4, "Export Pixelmap" );

//		//if there is no active assfile, stop the timer
//		bool isAvailable = FileHelper::isFileValid( sliceManager->getAssFile(), false );
//		if ( !isAvailable )
//			autoUpdate->stop();
	}

	else if ( menuIndex == 1 )
	{
		menu.addItem( 1, "Copy Step " + KeyPress( 'c', ModifierKeys::commandModifier, NULL ).getTextDescriptionWithIcons() );
		menu.addItem( 2, "Paste Step " + KeyPress( 'v', ModifierKeys::commandModifier, NULL ).getTextDescriptionWithIcons(), !stepToCopy.isEmpty() );
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
			File defaultChaser = FileLess::getNewFile( FileLess::Chaser );
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
			saveChaserAs();
			break;
		case 4:
		{
			PixelMapCreator c;
			c.saveAsPng( sliceManager->getAssFile().getFileNameWithoutExtension(), sliceManager->getSlices(), sliceManager->getResolution() );
			break;
		}
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

bool MainContentComponent::saveChaserAs()
{

	//open a save dialog
	File chaserLocation = FileLess::getAppFolder( FileLess::Chaser );
	FileChooser fc( "Save chaser as...", chaserLocation, "*.xml", true );
	if ( fc.browseForFileToSave( true ) )
	{
		File f = fc.getResult();
		xmlManager->setSaveFile( f );

		sliceManager->writeToXml();
		chaseManager->writeToXml();

		getTopLevelComponent()->setName( f.getFileNameWithoutExtension() );
		return true;
	}

	return false;
}

void MainContentComponent::loadChaser()
{
	//open a load dialog
	File chaserLocation = FileLess::getAppFolder( FileLess::Chaser );
	FileChooser fc( "Pick a Chaser file...", chaserLocation, "*.xml", true );

	if ( fc.browseForFileToOpen() )
	{
		File f = fc.getResult();
		if ( creator->createChaserFromChaserFile( f, true ) )
		{
			getTopLevelComponent()->setName( f.getFileNameWithoutExtension() );
			xmlManager->setSaveFile( f );
		}
	}
}

void MainContentComponent::loadAssFile()
{
	//open a load dialog
	File docs = File::getSpecialLocation(File::SpecialLocationType::userDocumentsDirectory);
	FileChooser fc("Choose a Resolume ASS file...", docs, "*.xml", true);

	if (fc.browseForFileToOpen())
	{
		File f = fc.getResult();
		if (FileHelper::isFileValid(f, true))
		{
			creator->createChaserFromAssFile(f, false);
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
	//secret shortcut to force opening a particular assfile
	else if (key == KeyPress('o', ModifierKeys::commandModifier + ModifierKeys::shiftModifier, NULL))
	{
		loadAssFile();
	}

	return true;
}

void MainContentComponent::copyStep()
{
	stepToCopy = chaseManager->getCurrentStep();
}

void MainContentComponent::pasteStep()
{
	if ( !stepToCopy.isEmpty() )
	{
		chaseManager->setCurrentStep( stepToCopy );
		previewWindow->setActiveSlices();
	}
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

	Rectangle<int> previewArea = Rectangle < int >{ 0, 0, int( getWidth() * 0.83 ), int( (getWidth() / 16.0) * 9.0 * 0.83 ) };
	previewArea.setPosition( 0, menuBarHeight );

	previewWindow->setBounds( 0, 0, sliceManager->getResolution().x, sliceManager->getResolution().y );
	Rectangle<int> previewWindowArea = previewArea.reduced( 5 );

	if ( previewWindow->getWidth() > 0 && previewWindow->getHeight() > 0 )
		previewWindow->setBoundsToFit( Rectangle<int>{previewWindowArea.getX(), previewWindowArea.getY(), previewWindowArea.getWidth(), previewWindowArea.getHeight()}, Justification::centred, false );

	Rectangle<int> sliceArea = Rectangle < int >{ previewArea.getWidth(), menuBarHeight, area.getWidth() - previewArea.getWidth(), previewArea.getHeight() };
	listBrowser->setBounds( sliceArea.reduced( 5 ) );

	Rectangle<int> bottomArea = area.removeFromBottom( area.getHeight() - previewArea.getHeight() );
	copier->setBounds( bottomArea.removeFromLeft( 75 ).reduced( 5 ) );
	sequencer->setBounds( bottomArea.reduced( 5 ) );
}
