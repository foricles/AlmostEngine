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

	void SetPerspective(float fov, float near, float far, uint32_t wight, uint32_t height);
	void SetOrthographic(int left, int right, int top, int bottom, int near, int far);

	const kmu::mat4 GetCameraMatrix();
	const kmu::mat4 & GetProjectionMatrix();

private:
	eAlmProjection m_projection;
	kmu::mat4 m_projMatrix;
};

}
#endif // !_ALM_RENDER_CAMERA_HPP_