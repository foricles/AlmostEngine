#ifndef _ALM_TRANSFORM_HPP_
#define _ALM_TRANSFORM_HPP_

#include <vector>
#include "../almMath/matrices.hpp"

namespace alme
{
	class AlmTransform
	{
	public:
		AlmTransform();
		virtual ~AlmTransform();

		bool HasChild(const AlmTransform* candidat) const;

		void SetParent(AlmTransform* parent);
		AlmTransform*	GetParent() ;

		void AddChild(AlmTransform* child) ;
		void RemoveChild(AlmTransform* child) ;
		void RemoveAllChildren() ;

		void SetScale(const kmu::vec3 &scale) ;
		void SetScale(float x = 1, float y = 1, float z = 1) ;

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

		void UpdateModelMatrix();
		const kmu::mat4 & GetModelMatrix();

		void SwapParents(AlmTransform* other);

		static void UpdateModelMatrix(AlmTransform*ihead);

	private:
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