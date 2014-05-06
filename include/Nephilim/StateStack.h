#ifndef NephilimStateStack_h__
#define NephilimStateStack_h__

#include "Platform.h"

#include <vector>
#include <map>

NEPHILIM_NS_BEGIN

class String;
class Time;
class State;
class StateStackTransition;
class Renderer;
class Event;
class GameCore;

/**
	\ingroup Core
	\class StateStack
	\brief Mechanism to control game states like the pause screen, main menu, etc

	This tool uses a delayed action system. In other words, it does not necessarily apply changes immediately.

	Whenever you manipulate a StateStack object from a regular place, ie, not inside a State object execution,
	you must always call process() to push the pending changes and refresh the state. In the other hand,
	when the stack is being manipulated internally by State*, process() is completely irrelevant and other rules apply.

	State - bool active - when a state is about to be added to execution, it becomes active, and goes inactive once it finishes
*/
class NEPHILIM_API StateStack{
public:
	/// Creates an empty state machine
	StateStack();

	/// Safely destroys all states by sending them a destroy callback
	~StateStack();

	/// Processes all the state changes that are pending
	/// Can never be called while the stack is working(rendering,updating or fetching events)
	void process();

	/// Clear all active states
	void clear();

	/// Triggers a transition animation at any time
	/// If there are no pending changes in the state stack, the transition happens but is useless.
	/// Note: All transitions are deallocated when they finish. You must allocate it dynamically using new.
	/// Note: Passing NULL will mean a immediate transition
	/// Note: If a transition is passed while another is in effect, the older is deallocated and the new one is used.
	void performTransition(StateStackTransition* transition);

	/// Finishes the transition animation, deletes the transition
	/// and then commits all needed changes for regular functioning
	void endTransition();

	/// Check if a given state is currently being executed
	bool isActive(const String& name);

	/// Check if a given state is currently being executed
	bool isActive(State* state);
	
	void drawList(std::vector<State*>& list, Renderer* renderer);

	void drawCurrentList(Renderer* renderer);


	/// Push a state for execution directly
	void add(State* state);

	/// Push a binded state for execution by its name
	void add(const String& name);

	/// Remove the top state from the stack
	void pop();

	/// Adds a state to the waiting list
	void addWaiting(State* state);

	/// Checks if the stack is empty and if so adds a state from the wait list
	void processWaitList();

	/// Updates the right states 
	void update(Time &time);

	/// Perform a update on the list
	void updateList(std::vector<State*>& list, const Time& time);

	/// Delivers the event to the right events
	void pushEvent(Event &event);

	/// Draws the right states
	void drawStates(Renderer *renderer);

	int getActiveStateCount();

	/// Bind a new state to the list
	/// Returns false if the name was already taken
	bool bind(const String& name, State* state);

	/// Processes the list of changes to the stack
	void applyChanges();
	void applyChangesTo(std::vector<State*>& list);

	/// Erase a state
	void erase(State* state);

	/// Get the parent game, if any
	GameCore* getParentGame();

	/// Get a state or NULL
	State* getBinding(const String& name);

	bool isTransitionActive(); 

	GameCore* m_parent;

	struct StateStackOperation
	{
		enum Type
		{
			Erase = 0,
			Add
		};

		Type type;
		State* obj;
	};

//private:
	std::vector<State*> mCurrentList; ///< The currently active stack
	std::vector<State*> mFutureList;  ///< The stack that will be active after the transition

	friend class StateStackTransition;



	std::vector<State*> m_waitList; ///< The list of states waiting to become active


	std::map<String, State*> m_bindList; ///< The list of binded states, that "live" under the stack

	std::vector<StateStackOperation> m_pendingOperations; ///< The list of pending operations

	StateStackTransition* m_transition; ///< The active transition

	bool m_stackLock;
};

NEPHILIM_NS_END
#endif // NephilimStateStack_h__