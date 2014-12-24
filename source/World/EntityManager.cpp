#include <Nephilim/World/EntityManager.h>

NEPHILIM_NS_BEGIN

/// Destroy this entity
void EntityManager::destroy(Entity entity)
{
	for (std::size_t i = 0; i < entities.size(); ++i)
	{
		if (entities[i].id == entity.id)
		{
			entities.erase(entities.begin() + i);
			return;
		}
	}
}

NEPHILIM_NS_END