#ifndef _ALM_ENTITY_MANAGER_HPP_
#define _ALM_ENTITY_MANAGER_HPP_

#include "interface/alm_imgr.hpp"

namespace alme
{
class AlmEntityManager : public IAlmEntityManager
{
public:
	class AlmContainer;
public:
	AlmEntityManager(AlmostEngine *engine);
	~AlmEntityManager();

	AlmEntity*				CreateEntity(const std::string &name) override;
	void					ReleaseEntity(AlmEntity *entity) override;
	AlmEntity*				FindByName(const std::string &name) const override;

	void					ReleaseAllEntities() override;
	void					UpdateAllEntities() override;

	uint32_t				AllocatedMemory() const override;
	uint32_t				EntitiesCount() const override;
	uint32_t				EntitiesCountWithUpdate() const override;

private:
	AlmContainer*			m_container;
	uint32_t				m_memmory;
};

}


#endif // _ALM_ENTITY_MANAGER_HPP_