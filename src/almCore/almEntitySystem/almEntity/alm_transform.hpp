#ifndef _ALM_TRANSFORM_HPP_
#define _ALM_TRANSFORM_HPP_

#include <vector>

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

	bool HasChild(AlmTransform *candidat)const;
	void SetParent(AlmTransform *parent);
	void AddChild(AlmTransform *child);
	void RemoveChild(AlmTransform *child);
	void RemoveAllChildren();

private:
	AlmTransform(AlmEntity *owner);

private:
	AlmEntity *m_entity;
	AlmTransform *m_parent;
	std::vector<AlmTransform*> m_children;

};

}


#endif // !_ALM_TRANSFORM_HPP_