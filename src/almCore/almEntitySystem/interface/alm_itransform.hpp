#ifndef _ALM_TRANSFORM_INTERFACE_HPP_
#define _ALM_TRANSFORM_INTERFACE_HPP_

#include "alm_ientity.hpp"
#include "../src/almCore/alm_platform.hpp"
#include "../src/almCore/almMath/matrices.hpp"

namespace alme
{

class IAlmTransform
{
public:
	IAlmTransform() {};
	IAlmTransform(IAlmTransform &&rhv) = delete;
	IAlmTransform(const IAlmTransform &rhv) = delete;
	IAlmTransform &operator=(IAlmTransform &&rhv) = delete;
	IAlmTransform &operator=(const IAlmTransform &rhv) = delete;
	virtual ~IAlmTransform() {};

	virtual IAlmEntity *				GetEntity() = 0;
	virtual const IAlmEntity *			GetEntity() const = 0;

	virtual bool						HasChild(const IAlmTransform *candidat)const = 0;

	virtual void						SetParent(IAlmTransform *parent) = 0;
	virtual IAlmTransform *				GetParent() = 0;

	virtual void						AddChild(IAlmTransform *child) = 0;
	virtual void						RemoveChild(IAlmTransform *child) = 0;
	virtual void						RemoveAllChildren() = 0;

	virtual void						SetScale(const kmu::vec3 &scale) = 0;
	virtual void						SetScale(float x, float y, float z) = 0;

	virtual void						SetPosition(const kmu::vec3 &pos) = 0;
	virtual void						SetPosition(float x, float y, float z) = 0;

	virtual void						SetRotation(const kmu::quaternion &rot) = 0;
	virtual void						SetRotation(const kmu::vec3 &euler) = 0;

	virtual kmu::vec3					GetScale() = 0;
	virtual kmu::vec3					GetPosition() = 0;
	virtual kmu::quaternion				GetRotation() = 0;

	virtual const kmu::vec3 &			GetLocalScale() const = 0;
	virtual const kmu::vec3 &			GetLocalPosition() const = 0;
	virtual const kmu::quaternion &		GetLocalRotation() const = 0;

	virtual void						UpdateModelMatrix() = 0;
	virtual const kmu::mat4 &			GetModelMatrix() = 0;

	virtual void						SwapParents(IAlmTransform * other) = 0;

};

}


#endif // !_ALM_TRANSFORM_INTERFACE_HPP_