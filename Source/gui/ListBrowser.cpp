/*
  ==============================================================================

    ListBrowser.cpp
    Created: 17 Dec 2016 1:17:01pm
    Author:  Joris

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"
#include "ListBrowser.h"

//==============================================================================
ListBrowser::ListBrowser() : TabbedComponent( TabbedButtonBar::TabsAtTop )
{
	setTabBarDepth( 30 );
	setCurrentTabIndex( 0 );
}

ListBrowser::~ListBrowser()
{

}

void ListBrowser::paint (Graphics& g)
{

}

void ListBrowser::resized()
{
	getTabbedButtonBar().setBounds( 0, 0, getWidth(), getTabBarDepth() );
}
