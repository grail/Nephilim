#ifndef NephilimStateTransitionFade_h__
#define NephilimStateTransitionFade_h__

#include <Nephilim/Platform.h>
#include "StateTransition.h"
#include <Nephilim/Graphics/RectangleShape.h>
#include "AxEasing.h"

NEPHILIM_NS_BEGIN

/**
	\ingroup Core
	\class StateTransitionFade
	\brief Native state transition animation
*/
class StateTransitionFade : public StateStackTransition
{
public:
	/// Constructor
	StateTransitionFade();

protected:
	/// Update the transition
	virtual void onUpdate(const Time& time);

	/// Draw the transition
	virtual void draw(GraphicsDevice* renderer);

private:
	RectangleShape m_rect;
	bool m_secondPhase;
	AxEasingFunction function;

	float alpha;
	vec2 lastp;
	float m_duration;
	float elapsedTime;
};

NEPHILIM_NS_END
#endif // NephilimStateTransitionFade_h__
