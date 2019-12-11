#ifndef _ALM_ENTITY_HPP_
#define _ALM_ENTITY_HPP_

#include "alm_transform.hpp"
#include "interface/alm_ientity.hpp"
#include "interface/alm_itransform.hpp"

namespace alme
{

class AlmEntity : public IAlmEntity
{
	friend class AlmEntityManager;
public:
	AlmEntity();
	AlmEntity(const AlmEntity &rhv) = delete;
	AlmEntity & operator=(const AlmEntity &rhv) = delete;
	~AlmEntity();

	const uint32_t			GetId() const override;
	const std::string &		GetName() const override;
	IAlmTransform *			GetTransform() override;
	const IAlmTransform *	GetTransform() const override;

public:
	bool operator < (const AlmEntity &rhv) const;
	bool operator == (const AlmEntity &rhv) const;

private:
	uint32_t m_id;
	std::string m_name;
	AlmTransform m_transform;

};

}

#endif // !_ALM_ENTITY_HPP_