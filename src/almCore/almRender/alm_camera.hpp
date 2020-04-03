#ifndef _ALM_RENDER_CAMERA_HPP_
#define _ALM_RENDER_CAMERA_HPP_

#include "../almMath/matrices.hpp"
#include "../src/almCore/almEntitySystem/alm_entity.hpp"

namespace alme
{

enum class eAlmProjection
{
	ePerspective,
	eOrthographic
};

class AlmCamera : public AlmEntity
{
public:
	AlmCamera();
	AlmCamera(AlmCamera &&camera);
	AlmCamera(const AlmCamera &camera) = delete;

	~AlmCamera();

	AlmCamera & operator=(AlmCamera &&camera);
	AlmCamera & operator=(const AlmCamera &camera) = delete;

	void SetPerspective(float fFov, float fNear, float fFar, uint32_t uWight, uint32_t wHeight);
	void SetOrthographic(int iLeft, int iRight, int iTop, int iBottom, int iNear, int iFar);

	const kmu::mat4 GetCameraMatrix();
	const kmu::mat4 & GetProjectionMatrix();

private:
	eAlmProjection m_projection;
	kmu::mat4 m_projMatrix;
};

}
#endif // !_ALM_RENDER_CAMERA_HPP_