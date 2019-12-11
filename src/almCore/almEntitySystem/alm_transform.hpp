#ifndef _ALM_TRANSFORM_HPP_
#define _ALM_TRANSFORM_HPP_

#include <vector>
#include "interface/alm_itransform.hpp"

namespace alme
{

class AlmTransform : public IAlmTransform
{
	friend class AlmEntity;
public:
	~AlmTransform();

	IAlmEntity *GetEntity() override;
	const IAlmEntity *GetEntity() const override;

	bool HasChild(const IAlmTransform *candidat)const override;

	void SetParent(IAlmTransform *parent) override;
	void SetParent(const IAlmTransform &parent) override;

	void AddChild(IAlmTransform *child) override;
	void AddChild(const IAlmTransform &child) override;

	void RemoveChild(IAlmTransform *child) override;
	void RemoveChild(const IAlmTransform &child) override;

	void RemoveAllChildren() override;

	void SetScale(const kmu::vec3 &scale) override;
	void SetScale(float x = 1, float y = 1, float z = 1) override;

	void SetPosition(const kmu::vec3 &pos) override;
	void SetPosition(float x = 0, float y = 0, float z = 0) override;

	void SetRotation(const kmu::quaternion &rot) override;
	void SetRotation(const kmu::vec3 &euler) override;

	kmu::vec3 GetScale() override;
	kmu::vec3 GetPosition() override;
	kmu::quaternion GetRotation() override;

	const kmu::vec3 & GetLocalScale() const override;
	const kmu::vec3 & GetLocalPosition() const override;
	const kmu::quaternion & GetLocalRotation() const override;

	const kmu::mat4 & GetModelMatrix() override;

	static void UpdateModelMatrix(IAlmTransform *ihead);

private:
	AlmTransform(IAlmEntity *owner);

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