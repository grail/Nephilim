#ifndef NephilimUILoader_h__
#define NephilimUILoader_h__

#include "Platform.h"
#include "Strings.h"
#include "pugixml/pugixml.hpp"

#include "UIViewComponent.h"

NEPHILIM_NS_BEGIN

class UIView;

/**
	\class UILoaderXML
	\brief Loads and accesses the XML file with the UI
*/
class NEPHILIM_API UILoaderXML
{
public:
	bool loadFromFile(const String& filename);

	pugi::xml_document document; ///< Contains all the UI file data (xml)
};

class NEPHILIM_API UILoader
{
public:

	void configure(UIView* view, const String& filename); // v1 test
};

/**
	\ingroup UI
	\class UIReloadComponent
	\brief UI attachment for reloading itself from a file
*/
class NEPHILIM_API UIReloadComponent : public UIViewComponent
{
public:
	UIReloadComponent(const String& file);

	void onCreate()
	{

	}

	String filename; ///< Where to load the UI from
};

NEPHILIM_NS_END
#endif // NephilimUILoader_h__