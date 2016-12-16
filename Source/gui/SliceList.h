/*
  ==============================================================================

    SliceList.h
    Created: 10 Jan 2015 7:14:14pm
    Author:  Joris de Jong

  ==============================================================================
*/

#ifndef SLICELIST_H_INCLUDED
#define SLICELIST_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "../../HybridApi/Source/HybridApi.h"
#include "../slice/SliceManager.h"
#include "../gui/Preview.h"
#include "ColourLookAndFeel.h"
#include <map>

//==============================================================================

typedef std::map<int64, Array<PropertyComponent*>> SectionMap;

class SliceList;

class MyPropertyPanel : public PropertyPanel
{
public:
	MyPropertyPanel( SliceList& parent );
	~MyPropertyPanel();
	
	//I need this to get a call back for when one of the sections is closed or opened
	void resized() override;
	
private:
	SliceList& parent;
};

class SliceList : public Component
{
public:
	SliceList( SliceManager* sliceManager, Preview* preview );
	~SliceList();
	
	void paint(Graphics&) override;
	void resized() override;
	
	void setSlices();
	void clear();
	
	void sliceVisibilityChanged();
	void screenVisibilityChanged( int foldedSectionIndex );
	
	Array<Slice*> getSlicesFromSection ( int i );
	
private:
	SliceManager* sliceManager;
	Preview* preview;

	ColourLookAndFeel claf;
	SectionMap sections;
	
	ScopedPointer<MyPropertyPanel> panel;
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SliceList)
	
};


class SlicePropertyButton : public BooleanPropertyComponent
{
public:
	SlicePropertyButton( SliceList& parent, Slice& slice );
	~SlicePropertyButton();
	
	void buttonClicked ( Button* b ) override;
	void setState (bool newState) override;
	bool getState() const override;

	Slice& getSlice();
	
	void paint (Graphics& g) override
	{
		PropertyComponent::paint (g);
	}
	
private:
	SliceList& parent;
	Slice& slice;
	bool state;	
};





#endif  // SLICELIST_H_INCLUDED
