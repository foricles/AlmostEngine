#include "alm_entity.hpp"

using namespace alme;

AlmEntity::AlmEntity(AlmEntityManager * manager)
	: m_id(0)
	, m_name("Entity")
	, m_transform(this, manager)
{
}

AlmEntity::~AlmEntity()
{
}

IAlmTransform * AlmEntity::GetTransform()
{
	return &m_transform;
}

const IAlmTransform * AlmEntity::GetTransform() const
{
	return &m_transform;
}

bool AlmEntity::operator<(const AlmEntity & rhv) const
{
	return GetId() < rhv.GetId();
}

bool AlmEntity::operator==(const AlmEntity & rhv) const
{
	return GetId() == rhv.GetId();
}

const uint32_t AlmEntity::GetId() const
{
	return m_id;
}

const std::string & AlmEntity::GetName() const
{
	return m_name;
}
