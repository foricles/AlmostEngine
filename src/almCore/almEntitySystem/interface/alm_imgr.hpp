#ifndef _ALM_MANAGER_INTERFACE_HPP_
#define _ALM_MANAGER_INTERFACE_HPP_

#include <string>
#include "../src/almCore/alm_engsystem.hpp"

namespace alme
{
class IAlmEntity;
class IAlmEntityManager : protected AlmEngineSystem
{
public:
	IAlmEntityManager(AlmostEngine *engine) : AlmEngineSystem(engine) {};

	virtual IAlmEntity		*CreateEntity(const std::string &name) = 0;
	virtual void			 ReleaseEntity(IAlmEntity *entity) = 0;
	virtual IAlmEntity		*FindByName(const std::string &name) const = 0;

};
}
#endif // !_ALM_MANAGER_INTERFACE_HPP_
