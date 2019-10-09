#include "alm_entity.hpp"

using namespace alme;

AlmEntity::AlmEntity()
	: m_id(0)
	, m_name("Entity")
	, m_transform(this)
{
}

AlmEntity::~AlmEntity()
{
}

AlmTransform & alme::AlmEntity::GetTransform()
{
	return m_transform;
}

const AlmTransform & alme::AlmEntity::GetTransform() const
{
	return m_transform;
}

bool alme::AlmEntity::operator<(const AlmEntity & rhv) const
{
	return GetId() < rhv.GetId();
}

bool alme::AlmEntity::operator==(const AlmEntity & rhv) const
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
