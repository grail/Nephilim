#ifndef NephilimContentManager_h__
#define NephilimContentManager_h__

#include <Nephilim/Platform.h>
#include <Nephilim/Game/Resource.h>

#include <Nephilim/Foundation/VirtualFileSystem.h>
#include <Nephilim/Foundation/StringHash.h>
#include <Nephilim/Foundation/File.h>

#include <Nephilim/Graphics/GLTexture.h>
#include <Nephilim/Graphics/Font.h>

#include <Nephilim/Plugins/Plugin.h>

#include <map>
#include <memory>

NEPHILIM_NS_BEGIN

class StaticMesh;
class SkeletalMesh;

class ModelLoader;

class ContentGroupFonts
{
public:
	bool load(const String& filename);

	std::map<String, Font*> mFonts;
};

/// A content group holds an indefinite number of resources of different types
class ContentGroup
{
public:

	ContentGroupFonts mFonts;

	std::map<String, Texture*> mTextures;

};

// used to assist in automatic loading of third party importers
// not needed for native formats
struct ModelImportSettings
{

};

/// Content manager manages all content groups created
class ContentManager
{
public:

	/// List of custom model loaders currently plugged in
	/// This is just for third party formats, all native binary ones are handled automatically
	std::vector<ModelLoader*> modelLoaders;

	std::vector<StaticMesh*> staticMeshContainer;
	std::vector<String>      staticMeshNames;

	std::vector<SkeletalMesh*> skeletalMeshContainer;
	std::vector<String>      skeletalMeshNames;

public:

	/// Creates the default group - no name ""
	ContentManager();

	/// Ensure removal of all content groups and the resources
	~ContentManager();

	/// Create a new static mesh with a given resource name
	StaticMesh* createStaticMesh(const String& urn);

	/// Create a new static mesh with a given resource name
	SkeletalMesh* createSkeletalMesh(const String& urn);

	/// This will cause a static mesh to be loaded from a file with native loader or an importer
	bool loadStaticMesh(const String& urn, const String& filename);
	

	ContentGroup* createGroup(const String& name);

	/// Find a mesh in our storage, so it can be used somewhere
	Resource<StaticMesh> findMesh(const String& name);

	

	void removeGroup(const String& name);

	/// The most elemental form of loading an asset
	/// Simply takes the filename and tries to deduce how to load it from extension
	bool load(const String& filename);

	Texture* getTexture(const String& name);

	String mDefaultGroup; ///< The current default group
	ContentGroupFonts* mDefaultFonts; ///< A pointer to the font holder in the currently "default" group

	std::map<String, ContentGroup*> mGroups;

	VirtualFS* filesystems;

	std::vector<std::unique_ptr<Plugin> > plugins;
};

NEPHILIM_NS_END
#endif // NephilimContentManager_h__