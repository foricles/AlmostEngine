#ifndef _ALM_ENTITY_INTERFACE_HPP_
#define _ALM_ENTITY_INTERFACE_HPP_

#include "../src/almCore/alm_platform.hpp"

namespace alme
{
class IAlmTransform;
class IAlmEntity
{
public:
	virtual ~IAlmEntity() {};

	virtual const uint32_t			GetId() const = 0;
	virtual const std::string &		GetName() const = 0;
	virtual IAlmTransform *			GetTransform() = 0;
	virtual const IAlmTransform *	GetTransform() const = 0;

	bool m_hasUpdate;
};
}

#endif // !_ALM_ENTITY_INTERFACE_HPP_
