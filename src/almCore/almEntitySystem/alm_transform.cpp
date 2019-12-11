#include "alm_transform.hpp"
#include "alm_entity.hpp"
#include "../src/almCore/alm_log.hpp"
#include <algorithm>

using namespace alme;



AlmTransform::AlmTransform(IAlmEntity *owner)
	: m_entity(dynamic_cast<AlmEntity*>(owner))
	, m_parent(nullptr)
	, m_scale(1, 1, 1)
	, m_position(0, 0, 0)
	, m_rotation()
	, m_recalModelMatrix(true)
	, m_modelMatrix()
{
}

AlmTransform::~AlmTransform()
{
	if (!m_parent) return;
	m_parent->RemoveChild(this);
}


IAlmEntity *AlmTransform::GetEntity()
{
	return m_entity;
}

const IAlmEntity *AlmTransform::GetEntity() const
{
	return m_entity;
}


bool AlmTransform::HasChild(const IAlmTransform * candidat) const
{
	return (std::find(m_children.begin(), m_children.end(), candidat) != m_children.end());
}

void AlmTransform::SetParent(IAlmTransform * parent)
{
	if (!parent) return;
	if (m_parent == parent) return;
	if (m_parent) RemoveChild(this);
	m_parent = dynamic_cast<AlmTransform*>(parent);
	parent->AddChild(this);

	this->m_recalModelMatrix = true;
}

void alme::AlmTransform::SetParent(const IAlmTransform & parent)
{
	SetParent(const_cast<IAlmTransform*>(&parent));
}

void AlmTransform::AddChild(IAlmTransform * child)
{
	if (HasChild(child)) return;
	m_children.push_back(dynamic_cast<AlmTransform*>(child));
	child->SetParent(this);

	dynamic_cast<AlmTransform*>(child)->m_recalModelMatrix = true;
}

void alme::AlmTransform::AddChild(const IAlmTransform & child)
{
	AddChild(const_cast<IAlmTransform*>(&child));
}

void AlmTransform::RemoveChild(IAlmTransform *child)
{
	auto found = std::find(m_children.begin(), m_children.end(), child);
	if (found != m_children.end()) m_children.erase(found);
}

void alme::AlmTransform::RemoveChild(const IAlmTransform & child)
{
	RemoveChild(const_cast<IAlmTransform*>(&child));
}

void AlmTransform::RemoveAllChildren()
{
	for (auto child : m_children)
		child->SetParent(m_parent);
	m_children.clear();
}

void alme::AlmTransform::SetScale(const kmu::vec3 & scale)
{
	m_scale = scale;
	this->m_recalModelMatrix = true;
}

void alme::AlmTransform::SetScale(float x, float y, float z)
{
	m_scale.set(x, y, z);
	this->m_recalModelMatrix = true;
}

void alme::AlmTransform::SetPosition(const kmu::vec3 & pos)
{
	m_position = pos;
	this->m_recalModelMatrix = true;
}

void alme::AlmTransform::SetPosition(float x, float y, float z)
{
	m_position.set(x, y, z);
	this->m_recalModelMatrix = true;
}

void alme::AlmTransform::SetRotation(const kmu::quaternion & rot)
{
	m_rotation = rot;
	this->m_recalModelMatrix = true;
}

void alme::AlmTransform::SetRotation(const kmu::vec3 & euler)
{
	m_rotation.euler(euler.magnitude(), euler.normalized());
	this->m_recalModelMatrix = true;
}

kmu::vec3 alme::AlmTransform::GetScale()
{
	if (m_recalModelMatrix)
		UpdateModelMatrix(this);

	kmu::vec3 ret;

	ret.x = m_modelMatrix.at(0, 0);
	ret.y = m_modelMatrix.at(1, 1);
	ret.z = m_modelMatrix.at(2, 2);

	return ret;
}

kmu::vec3 alme::AlmTransform::GetPosition()
{
	if (m_recalModelMatrix)
		UpdateModelMatrix(this);

	kmu::vec3 ret;

	ret.x = m_modelMatrix.at(0, 3);
	ret.y = m_modelMatrix.at(1, 3);
	ret.z = m_modelMatrix.at(2, 3);

	return ret;
}

kmu::quaternion alme::AlmTransform::GetRotation()
{
	if (m_recalModelMatrix)
		UpdateModelMatrix(this);

	return kmu::Quaternion(m_modelMatrix);
}

const kmu::vec3 & alme::AlmTransform::GetLocalScale() const
{
	return m_scale;
}

const kmu::vec3 & alme::AlmTransform::GetLocalPosition() const
{
	return m_position;
}

const kmu::quaternion & alme::AlmTransform::GetLocalRotation() const
{
	return m_rotation;
}

const kmu::mat4 & alme::AlmTransform::GetModelMatrix()
{
	if (m_recalModelMatrix)
		UpdateModelMatrix(this);

	return m_modelMatrix;
}

void AlmTransform::UpdateModelMatrix(IAlmTransform *ihead)
{
	AlmTransform *head = dynamic_cast<AlmTransform*>(ihead);
	while (head->m_parent && head->m_parent->m_recalModelMatrix) head = head->m_parent;

	if (head->m_recalModelMatrix)
	{
		head->m_recalModelMatrix = false;

		kmu::CalculateModelMatrix(head->m_modelMatrix, head->m_position, head->m_scale, head->m_rotation);

		if (head->m_parent)
			head->m_modelMatrix = head->m_modelMatrix * head->m_parent->m_modelMatrix;

		for (AlmTransform *child : head->m_children)
		{
			child->m_recalModelMatrix = true;
			UpdateModelMatrix(child);
		}
		return;
	}

	for (AlmTransform *child : head->m_children)
		UpdateModelMatrix(child);
}