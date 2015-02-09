#ifndef NephilimUIView_h__
#define NephilimUIView_h__

#include <Nephilim/UI/UIObject.h>

#include <Nephilim/UI/UIPainter.h>
#include <Nephilim/Foundation/Rect.h>
#include <Nephilim/Foundation/Event.h>
#include <Nephilim/Foundation/String.h>
#include <Nephilim/Graphics/RectangleShape.h>
#include <Nephilim/Graphics/GraphicsDevice.h>
#include <Nephilim/Foundation/Event.h>
#include <Nephilim/Animation/AxList.h>
#include <Nephilim/Animation/AxTarget.h>
#include <Nephilim/Foundation/Matrix.h>


#include <vector>
#include <map>
#include <memory>

#include "UILayout.h"
#include "UICore.h"
#include "UIProperty.h"
#include "UIComponent.h"

#include <Nephilim/UI/UIAnimation.h>

NEPHILIM_NS_BEGIN

class Animation;
class UIComponent;

struct UIPointerEvent
{ 
	Vector2D screenSpace; ///< Location of the pointer event in screen coordinates
	Vector2D screenSpaceDelta; ///< Difference between this event and the last event handled

	Mouse::Button effectingButton;

	enum
	{
		None,
		Pressed,
		Released,
		Moved
	};

	int eventType = None;

	vec2i windowSpace;

	/// Get the offset since the last pointer's event handled
	Vector2D getCursorDelta()
	{
		return screenSpaceDelta;
	}

	/// Get the button that raised this event
	Mouse::Button getButton()
	{
		return effectingButton;
	}

	bool isPointerDown()
	{
		return eventType == Pressed;
	}
};


namespace UIPositionFlag
{
	enum Flags
	{
		AttachBottom = 1 << 0,
		AttachRight = 1 << 1,
		KeepRelativePositionX = 1 << 2,
		KeepRelativePositionY = 1 << 3
	};
}

namespace UISizeFlag
{
	enum Flags
	{
		KeepRelativeSizeX = 1 << 0,
		KeepRelativeSizeY = 1 << 1
	};
}

namespace UIComponentList
{
	enum Types
	{
		Button = 0,
		Background,
		Text,
	};
};

namespace UIEventList
{
	enum EventTypes
	{
		Click
	};
}

namespace UIFlag
{
	enum UIFlagTypes
	{
		FIXED_WIDTH        = 1 << 0,  ///< The width can not be mutated by layouting
		FIXED_HEIGHT       = 1 << 1,  ///< The height can not be mutated by layouting
		FILL_PARENT_WIDTH  = 1 << 2,  ///< The view always resizes to fill the parent's width
		FILL_PARENT_HEIGHT = 1 << 3,  ///< The view always resizes to fill the parent's height
		ANCHOR_BOTTOM      = 1 << 4,  ///< The view anchors to its parent bottom line
		ANCHOR_LEFT        = 1 << 5,  ///< The view anchors to its parent left line
		ANCHOR_TOP         = 1 << 6,  ///< The view anchors to its parent top line
		ANCHOR_RIGHT       = 1 << 7,  ///< The view anchors to its parent right line
		RELATIVE_WIDTH     = 1 << 8,  ///< As the parent grows, this view maintains horizontal proportion
		RELATIVE_HEIGHT    = 1 << 9,  ///< As the parent grows, this view maintains horizontal proportion
	};
}

class UIAnimation;
class UIController;

class NEPHILIM_API UIView : public UIObject, public AxTarget, public RefCountable
{
public:
	vec2    size;            ///< Size of the view rectangle
	vec2    pivot;           ///< Pivot for positioning and rotations
	float   rotation_x;      ///< Rotation around the X axis; 0 means no rotation;
	float   rotation_y;      ///< Rotation around the Y axis; 0 means no rotation;
	float   rotation_z;      ///< Rotation around the Z axis; 0 means no rotation;
	
	mat4    matrix;

	vec2 scrolling_offset;   ///< Scroll bars just move this variable around, effectively shifting all elements all around 
	
	Uint32 mFlags;

	/// Holds the animations currently active on this view
	/// Each animation is bound to the view itself, and is automatically destroyed with it
	//std::vector<std::unique_ptr<UIAnimation> > animations;


	vec3  position;   ///< The 3D position of this view
	String     m_name;             ///< Name
	UIView* m_parent;           ///< The parent control
	
	UIController* _controller = nullptr;
	
// Inheritance
protected:

	/// Called on the subclass to have it paint its contents
	virtual void onPaint(UIPainter& painter);

	/// Called on subclasses to notify the widget was just resized
	virtual void onResize();

	/// Called when the view receives a mouse/touch related event
	virtual void onPointerEvent(const UIPointerEvent& event);

// Signals
public:

	sigc::signal<void> sizeChanged; ///< Emitted when the widget changes size


public:


	/// Allows cleaner code, which uses UIView::Ptr as the type which can be changed anytime between other types of smart pointers without breaking code
	typedef UIView* Ptr;

	/// Base constructor
	UIView();

	/// Construct and assign a parent directly
	UIView(UIView* parent);

	/// Base destructor
	virtual ~UIView();

	/// Get the current width of this widget
	float width();

	/// Get the current height of this widget
	float height();

	/// Set the new Z value
	void setZ(float _z);

	/// This will instance and bind a new controller to this view
	template<typename T>
	T* createController();

	/// This function allows to start an animation out of its definition file
	void startAnimation(const String& animationAsset);

	/// Refresh the visual styles on this view
	void updateStyles();

	/// Set styles directly to the view
	void setStyleSheet(const String& stylesheet);

	/// Get the bounding box of the children
	/// Coordinates in local space, relative to this view
	FloatRect childrenRect();

	/// Set any flags for the view
	void setFlag(Uint32 flags);

	bool hasFlags(Uint32 flags);

	/// Load the hierarchy of this view from a file and configure itself
	void load(const String& filename);

	/// Template to instance directly given types as children
	template<typename T>
	T* createChild(const String& name);

	/// Get the current world position
	vec3 getWorldPosition();

	/// Add a component to the view
	void addComponent(UIComponent* component);

	/// Add a new component from a pre registered type
	void addComponent(const String& name);

	void addComponent(int type);

	void addStringProperty(const String& propertyName, const String& propertyValue);

	String getStringProperty(const String& propertyName);

	/// Returns the first component with the given type
	UIComponent* getComponentByType(UIComponent::Type type);

	void printHierarchy(int tabs = 0);

	bool isScheduledForRemoval(UIView* v);

	/// Called before rendering the children UIView
	virtual void preRender(GraphicsDevice* renderer);

	/// Called after rendering the children UIView
	virtual void postRender(GraphicsDevice* renderer);

	/// Set the position of the control's top-left corner
	void setPosition(float x, float y);
	
	/// Set the position of the control's top-left corner
	void setPosition(vec2 position);

	/// Get the position of the top-left corner of the control
	vec2 getPosition();

	/// Set the position of the top-left corner of the control relative to its parent
	/// Note that this function has no effect if the control is not inserted in an hierarchy
	void setLocalPosition(float x, float y);

	/// Set the position of the top-left corner of the control relative to its parent
	/// Note that this function has no effect if the control is not inserted in an hierarchy
	void setLocalPosition(vec2 localPosition);

	/// Get the position of the top-left corner of the control relative to its parent
	/// Note that this function has no effect if the control is not inserted in an hierarchy,
	/// effectively returning (0,0)
	vec2 getLocalPosition();

	/// Get the parent control
	UIView* getParent();

	/// Find a control by its name in the control tree
	UIView* findByName(const String& name);

	/// Creates a new UIView, names it and attaches it as a child, then returns it
	UIView* createChild(const String& name);

	bool isHit(vec2 point);

	/// Adds a child control
	void attach(UIView* control);

	/// Detach a child control without destroying it
	void detach(UIView* control);

	/// Destroy all children
	void clear();

	/// Destroys the control and removes from the hierarchy
	void destroy();

	/// Makes the control invisible
	void hide();

	/// Makes the control visible
	void show();

	/// Destroy the child
	void destroyChild(UIView* child);

	/// Submit an animation to be processed by the control
	void commitAnimation(Animation* animation);

	/// Check if this control has animated children
	bool hasAnimatedChildren();

	/// Check if this control has any animation going on
	bool hasAnimations();

	/// Set a new layout to the control
	void setLayout(UILayout* layout);

	/// Set positioning flags to the control
	void setPositionFlags(Uint64 flags);

	/// Get the current positioning flags
	Uint64 getPositionFlags();

	/// Check the existence of a particular flag for positioning
	bool hasPositionFlag(Uint64 flag);

	/// Set the flags for sizing
	void setSizeFlags(Uint64 flags);

	/// Get the sizing flags
	Uint64 getSizeFlags();

	void enableAutoResize(bool enable);

	/// Check if there is a particular sizing flag
	bool hasSizeFlag(Uint64 flag);

	/// Set a property from a string
	void setProperty(const String& str);

	/// Get the currently assigned layout controller
	/// \return NULL if there is no layout controller assigned
	UILayout* getLayout();

	/// Get the number of children of this control
	int getChildCount();

	/// Is the control able to get input focus or not?
	virtual bool isFocusable();

	vec2 getCenter();

	/// Check if the control currently has focus
	bool hasFocus();

	/// Attempts focus on this control
	bool focus();

	/// Remove focus from the element
	void blur();

	/// Get a child at a index
	UIView* getChild(int index);

	/// Get the current size of the control
	vec2 getSize();

	/// Returns true when the control is subject of being layout in a grid or other organization form
	/// Most controls don't implement this function, as their default behavior is to respond to layouts always
	virtual bool respondsToLayouts();

	/// Requests a tool tip text label from the control
	/// If only a empty string is returned, no tooltip is shown
	virtual String getToolTipLabel();

	/// Tell the UIView to draw itself, giving it its parent transform
	void drawItself(GraphicsDevice* renderer, const mat4& transform = mat4::identity);

	/// Callback to render itself, renders children
	virtual void draw(GraphicsDevice* renderer, mat4 transform);

	/// Dispatches an event through the hierarchy
	void dispatchEvent(const Event& event);

	/// Callback to handle events
	virtual bool onEventNotification(Event& event);

	/// Attempt to process positional flags
	virtual void processPositionFlags();

	/// Attempt to process size flags
	virtual void processSizeFlags();

	/// Process a mouve movement event
	/// Returns false if the mouse isnt on any control
	virtual bool processMouseMove(int x, int y);

	virtual bool processTouchMove(int x, int y);

	/// Process a mouse press event
	virtual bool processMouseButtonPressed(int x, int y, Mouse::Button button);

	/// Process a mouse release event
	virtual void processMouseButtonReleased(int x, int y, Mouse::Button button, UIEventResult& info);

	virtual bool onKeyPressed(Keyboard::Key key);
	virtual bool onTextEvent(Uint32 code);

	virtual void onAttach(){}

	/// Move the widget around, relative to where it currently is
	void move(float x, float y);

	/// Immediately sets the new size of the control 
	void setSize(float width, float height);

	/// Immediately sets the center of the control to a new position
	void setCenter(float x, float y);

	/// Immediately sets the center of the control to a new position
	void setCenter(Vec2f position);

	/// Define a new name for this control
	void setName(const String& name);

	/// Deep clone of the control and its hierarchy
	virtual UIView* clone();

	/// Adjusts children to the new sizes according to UISizeFlags
	virtual void processSizeChange(float previousWidth, float previousHeight, float newWidth, float newHeight);

	/// Get bounds of the control
	FloatRect getBounds();

	/// Get the name of the control
	String getName();

	/// Notification for the custom controls for updating
	virtual void onUpdate(float elapsedTime);

	/// Update the control hierarchy
	void update(float elapsedTime);

	/// Update layout of children
	void updateLayout();

	void setRect(FloatRect rect);

	void setRect(float left, float top, float width, float height);

	FloatRect getRect();

	void switchLanguage();

	virtual void innerLanguageSwitch(){}

	Vec2f getMiddlePosition();

	/// Resizes the control over a defined time
	/// The lower border of the control will become at target position
	void resizeToPoint(float x, float y, float duration);

	void resize(float width, float height, float duration);

	/// Enables or disables a pseudo class
	void setPseudoClass(const String& name, bool active);

	/// Callback when a child of the control is removed
	virtual void onChildRemoved(UIView* control);

	/// Feeds the position of the control to the animation systems
	virtual vec2 axGetPosition2D();

	virtual void axSetPosition2D(vec2 position);

	virtual void axSetAlpha(float alpha);

	virtual float axGetAlpha();

	virtual void axKillTrigger();

public:                                                        // Old Properties

	/// Signal emitted whenever the slider value changes
	sigc::signal<void, int> onValueChanged;

	/// Events fired by the control
	sigc::signal<void> onMouseEnter;
	sigc::signal<void> onMouseLeave;
	sigc::signal<void> onMouseMove;
	sigc::signal<void> onClick;
	sigc::signal<void> onDoubleClick;
	sigc::signal<void> onTripleClick;
	
	sigc::signal<void> onFocus;
	sigc::signal<void> onLostFocus;
	sigc::signal<void, UIView*> onNewChild; /// Emitted when a child is attached


	/// Hierarchicly sets the context to all children
	void setContext(UICore* states);

	bool m_clipContents; ///< Whether the contents of the control itself are clipped at the border
	bool m_clipChildren; ///< Whether the children are clipped on the control rectangle
	/// Is the control being rendered?
	bool m_visible;

	bool m_drawBorder; /// temp

	Uint64 m_positionFlags;
	Uint64 m_sizeFlags;

	std::map<String, UIPropertyMap> m_styleInfo;
	std::map<String, bool>          m_classInfo;

	/// Reload all graphics because they were destroyed and are unavailable now
	virtual void reloadGraphicalAssets();

	int m_pointerPressCount;

	///< Animations
	std::vector<UIAnimation*> mAnimations;

	/// Cascaded transform
	mat4 m_transform;

	AxList m_animations; ///< Animation list

	void offsetChildrenPosition(vec2 offset);

	template<class T>
	T* getComponent();


	friend class UILayout;

	std::vector<UIComponent*> components; ///< List of components in this view

	std::map<String, String> mStringProperties;

protected: // functions

	/// Callback when the position of the control changed, for updating nested objects
	virtual void onPositionChanged();


public:

	struct UIControlOperation
	{
		enum UIControlOperationType
		{
			Attachment,
			Destruction,
			Detachment
		};

		UIControlOperationType type;
		UIView* control;
	};

	std::vector<UIControlOperation> m_pendingOperations;

	void applyPendingOperations();


	Vec2f m_minimumDimensions;
	Vec2f m_maximumDimensions;

	bool m_hasFocus;

	bool m_hovered;


	Color m_backgroundColor;
	Color m_topBorderColor, m_bottomBorderColor, m_leftBorderColor, m_rightBorderColor;

	/// Children of the control
	std::vector<UIView*> m_children;
	int m_childrenLock;
	typedef std::vector<UIView*>::iterator UIControlIterator;

	
	UILayout*  m_layoutController; ///< Layouter

	//////////////////////////////////////////////////////////////////////////
	// -- UIView definition

	Rect<float> mRect; ///< Bounds of this UIView
	Rect<float> mPadding; ///< Padding in the view
};


template<class T>
T* UIView::getComponent()
{
	for(std::size_t i = 0; i < components.size(); ++i)
	{
		T* component = dynamic_cast<T*>(components[i]);
		if(component)
			return component;
	}

	return NULL;
}

/// Template to instance directly given types as children
template<typename T>
T* UIView::createChild(const String& name)
{
	// Instance a new view
	T* view = new T();
	view->setName(name);
	//view->setPosition(getPosition());

	// Attach
	attach(view);

	return view;
}

/// This will instance and bind a new controller to this view
template<typename T>
T* UIView::createController()
{
	if (!_controller)
	{
		_controller = new T();
		_controller->_view = this;
	}

	return _controller;
}

/// UIView or UIWidget are the same thing.
typedef UIView UIWidget;

NEPHILIM_NS_END
#endif // NephilimUIView_h__
