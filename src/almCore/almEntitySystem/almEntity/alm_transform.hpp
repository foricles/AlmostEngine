#ifndef _ALM_TRANSFORM_HPP_
#define _ALM_TRANSFORM_HPP_

#include <vector>
#include "../src/almCore/almMath/matrices.hpp"

namespace alme
{

class AlmEntity;
class AlmTransform
{
friend class AlmEntity;
public:
	AlmTransform(AlmTransform &&rhv) = delete;
	AlmTransform(const AlmTransform &rhv) = delete;
	AlmTransform &operator=(AlmTransform &&rhv) = delete;
	AlmTransform &operator=(const AlmTransform &rhv) = delete;
	virtual ~AlmTransform();

	AlmEntity *GetEntity();
	const AlmEntity *GetEntity() const;

	bool HasChild(const AlmTransform *candidat)const;

	void SetParent(AlmTransform *parent);
	void SetParent(const AlmTransform &parent);

	void AddChild(AlmTransform *child);
	void AddChild(const AlmTransform &child);

	void RemoveChild(AlmTransform *child);
	void RemoveChild(const AlmTransform &child);

	void RemoveAllChildren();

	void SetScale(const kmu::vec3 &scale);
	void SetScale(float x = 1, float y = 1, float z = 1);

	void SetPosition(const kmu::vec3 &pos);
	void SetPosition(float x = 0, float y = 0, float z = 0);

	void SetRotation(const kmu::quaternion &rot);
	void SetRotation(const kmu::vec3 &euler);

	kmu::vec3 GetScale();
	kmu::vec3 GetPosition();
	kmu::quaternion GetRotation();

	const kmu::vec3 & GetLocalScale() const;
	const kmu::vec3 & GetLocalPosition() const;
	const kmu::quaternion & GetLocalRotation() const;

	const kmu::mat4 & GetModelMatrix();

	void UpdateModelMatrix();

private:
	AlmTransform(AlmEntity *owner);

private:
	AlmEntity *m_entity;
	AlmTransform *m_parent;
	std::vector<AlmTransform*> m_children;

	kmu::vec3 m_scale;
	kmu::vec3 m_position;
	kmu::quaternion m_rotation;

	bool m_recalModelMatrix;

	kmu::mat4 m_modelMatrix;
};

}


#endif // !_ALM_TRANSFORM_HPP_