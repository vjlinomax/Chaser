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
	tabs.clear();
}

void ListBrowser::currentTabChanged( int , const String&  )
{
	
}

void ListBrowser::addComponentAsTab( Component* c, String name )
{
	addTab( name, Colours::lightgrey, c, false );
	tabs.add( c );
}

void ListBrowser::paint( Graphics& )
{

}

void ListBrowser::resized()
{
	//put the button bar in the right place
	getTabbedButtonBar().setBounds( 0, 0, getWidth(), getTabBarDepth() );

	for ( auto c : tabs )
	{
		Rectangle<int> tab = getLocalBounds();
		tab.removeFromTop( getTabBarDepth() );
		c->setBounds( tab );
	}
}
