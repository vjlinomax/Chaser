/*
  ==============================================================================

    ListBrowser.h
    Created: 17 Dec 2016 1:17:01pm
    Author:  Joris

  ==============================================================================
*/

#ifndef LISTBROWSER_H_INCLUDED
#define LISTBROWSER_H_INCLUDED

#include "../../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class ListBrowser    : public juce::TabbedComponent
{
public:
    ListBrowser();
    ~ListBrowser();

    void paint (Graphics&) override;
    void resized() override;

	void addComponentAsTab( Component* c, String name );

	void currentTabChanged( int newCurrentTabIndex, const String& newCurrentTabName ) override;
private:
	OwnedArray<Component> tabs;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ListBrowser)
};


#endif  // LISTBROWSER_H_INCLUDED
