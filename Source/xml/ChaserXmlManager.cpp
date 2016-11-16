/*
  ==============================================================================

    ChaserXmlManager.cpp
    Created: 26 Sep 2016 6:38:51pm
    Author:  Joris

  ==============================================================================
*/

#include "ChaserXmlManager.h"
#include "../../HybridApi/Source/JuceBased/ArenaHelpers/File/FileHelper.h"

ChaserXmlManager::ChaserXmlManager()
{
	saveFile = File();
}

ChaserXmlManager::~ChaserXmlManager()
{

}

void ChaserXmlManager::setSaveFile( File newSaveFile )
{
	saveFile = newSaveFile;
	File prefFile = FileHelper::getChaserPreferencesFile();

	//try to parse it
	ScopedPointer<XmlElement> lastUsedFileData (XmlDocument::parse( prefFile ));
		
	if (!lastUsedFileData)
		lastUsedFileData = new XmlElement( "preferences" );

	//try to get the lastusedfile child
	XmlElement* lastUsedFile = lastUsedFileData->getChildByName("lastusedfile");
	if ( !lastUsedFile )
	{
		lastUsedFile = new XmlElement( "lastusedfile" );
		lastUsedFileData->addChildElement( lastUsedFile );
	}

	lastUsedFile->setAttribute( "fullpathname", newSaveFile.getFullPathName() );
	

	lastUsedFileData->writeToFile( prefFile, "" );
}

XmlElement* ChaserXmlManager::getMainElement()
{
	XmlElement* mainElement = nullptr;
	
	//try to read existing data from the saveFile
	if ( FileHelper::isFileValid( saveFile ) )
		mainElement =  XmlDocument::parse( saveFile );
	
	//if there is no valid xml yet, create it
	if ( !mainElement )
		mainElement = new XmlElement( "ChaserData" );

	return mainElement;
}


void ChaserXmlManager::saveXmlElement( juce::XmlElement* data )
{
	ScopedPointer<XmlElement> mainElement = getMainElement();

	//remove the old sequenceData if it exists
	mainElement->deleteAllChildElementsWithTagName( data->getTagName() );

	//add the sequence data
	mainElement->addChildElement( data );

	save( mainElement );
}

XmlElement* ChaserXmlManager::getXmlElement( String tagName )
{
	ScopedPointer<XmlElement> mainElement = getMainElement();

	return mainElement->getChildByName( tagName );
}

bool ChaserXmlManager::save( XmlElement* toSave )
{
	if ( saveFile == File() )
	{
		DBG( "SAVE ERROR!" );
		AlertWindow::showMessageBoxAsync( AlertWindow::AlertIconType::WarningIcon,
			"Data not saved!",
			"There is no savefile specified.",
			"Ok" );

		return false;
	}

	else
	{
		if ( !saveFile.existsAsFile() )
			saveFile.create();
	}

	if ( !toSave )
	{
		DBG( "SAVE ERROR!" );
		AlertWindow::showMessageBoxAsync( AlertWindow::AlertIconType::WarningIcon,
			"Data not saved!",
			"No data found for saving.",
			"Ok" );

		return false;
	}

	else if ( toSave->writeToFile( saveFile, "" ) )
	{
		DBG( "SAVE SUCCESFUL!" );
		return true;
	}

	else
	{
		DBG( "SAVE ERROR!" );
		AlertWindow::showMessageBoxAsync( AlertWindow::AlertIconType::WarningIcon,
			"Data not saved!",
			"Something went wrong during saving.",
			"Ok" );

		return false;
	}
}