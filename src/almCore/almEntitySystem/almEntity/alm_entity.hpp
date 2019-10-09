#ifndef _ALM_ENTITY_HPP_
#define _ALM_ENTITY_HPP_

#include "alm_transform.hpp"

namespace alme
{

class AlmEntity
{
public:
	AlmEntity();
	AlmEntity(const AlmEntity &rhv) = delete;
	AlmEntity & operator=(const AlmEntity &rhv) = delete;
	virtual ~AlmEntity();

	const uint32_t			GetId() const;
	const std::string &		GetName() const;
	AlmTransform &			GetTransform();
	const AlmTransform &	GetTransform()const;

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