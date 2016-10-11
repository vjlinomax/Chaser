/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "gui/Preview.h"
#include "../../HybridApi/Source/HybridApi.h"
#include "gui/SliceList.h"
#include "gui/Sequencer.h"
#include "gui/Copier.h"
#include "gui/ColourLookAndFeel.h"
#include "chase/ChaseManager.h"
#include "slice/SliceManager.h"
#include "xml\ChaserXmlManager.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   :  public Component,
								public MenuBarModel,
								public KeyListener,
								public Timer
{
public:
    //==============================================================================
    MainContentComponent();
    ~MainContentComponent();

    void resized() override;

	virtual void timerCallback() override;

	//menubar methods
	virtual StringArray getMenuBarNames() override;
	virtual PopupMenu getMenuForIndex (int topLevelMenuIndex,
									   const String& menuName) override;
	virtual void menuItemSelected (int menuItemID,
								   int topLevelMenuIndex) override;

	//keyboard listener
	virtual bool keyPressed (const KeyPress& key,
							 Component* originatingComponent) override;

	void saveXml();
	bool saveAsXml();
	void loadXml();

	void reloadSliceData();

	void copyStep();
	void pasteStep();
	void copySequence();
	void pasteSequence();

	ScopedPointer<ChaseManager> chaseManager;
	ScopedPointer<SliceManager> sliceManager;

	ScopedPointer<Preview> previewWindow;

private:
	ScopedPointer<MenuBarComponent> menuBar;

	Step stepToCopy;
	Sequence sequenceToCopy;

    ScopedPointer<ColourLookAndFeel> laf;

    ScopedPointer<SliceList> sliceList;
    ScopedPointer<Sequencer> sequencer;
	ScopedPointer<ChaserXmlManager> xmlManager;
	ScopedPointer<Copier> copier;

	void createChaserFromAssFile( File assFile, bool createNew );
	//will return true if succesful
	bool createChaserFromChaserFile();

	String version;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};

#endif  // MAINCOMPONENT_H_INCLUDED
