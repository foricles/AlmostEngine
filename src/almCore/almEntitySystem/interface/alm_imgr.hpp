#ifndef _ALM_MANAGER_INTERFACE_HPP_
#define _ALM_MANAGER_INTERFACE_HPP_

#include "../src/almCore/alm_platform.hpp"
#include "../src/almCore/alm_engsystem.hpp"

namespace alme
{
class AlmEntity;
class IAlmEntityManager : protected AlmEngineSystem
{
public:
	IAlmEntityManager(AlmostEngine *engine) : AlmEngineSystem(engine) {};

	virtual AlmEntity*			CreateEntity(const std::string &name) = 0;
	virtual void				ReleaseEntity(AlmEntity*entity) = 0;
	virtual AlmEntity*			FindByName(const std::string &name) const = 0;

	virtual void				ReleaseAllEntities() = 0;
	virtual void				UpdateAllEntities() = 0;

	virtual uint32_t			AllocatedMemory() const = 0;
	virtual uint32_t			EntitiesCount() const = 0;
	virtual uint32_t			EntitiesCountWithUpdate() const = 0;
};
}
#endif // !_ALM_MANAGER_INTERFACE_HPP_
