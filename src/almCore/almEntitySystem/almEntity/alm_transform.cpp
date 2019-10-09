#include "alm_transform.hpp"
#include "alm_entity.hpp"
#include <algorithm>

using namespace alme;



AlmTransform::AlmTransform(AlmEntity *owner)
	: m_entity(owner)
	, m_parent(nullptr)
{
}

AlmTransform::~AlmTransform()
{
}


AlmEntity *AlmTransform::GetEntity()
{
	return m_entity;
}

const AlmEntity *AlmTransform::GetEntity() const
{
	return m_entity;
}


bool AlmTransform::HasChild(AlmTransform * candidat) const
{
	return (std::find(m_children.begin(), m_children.end(), candidat) != m_children.end());
}

void AlmTransform::SetParent(AlmTransform * parent)
{
	if (!parent) return;
	if (m_parent == parent) return;
	if (m_parent) RemoveChild(this);
	m_parent = parent;
	parent->AddChild(this);
}

void AlmTransform::AddChild(AlmTransform * child)
{
	if (HasChild(child)) return;
	m_children.push_back(child);
	child->SetParent(this);
}

void AlmTransform::RemoveChild(AlmTransform *child)
{
	auto found = std::find(m_children.begin(), m_children.end(), child);
	if (found != m_children.end()) m_children.erase(found);
}

void alme::AlmTransform::RemoveAllChildren()
{
	for (auto child : m_children)
		child->SetParent(m_parent);
	m_children.clear();
}
