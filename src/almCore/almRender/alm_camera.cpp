#include "alm_camera.hpp"

using namespace alme;

AlmCamera::AlmCamera() : m_projection(eAlmProjection::ePerspective)
{
}

AlmCamera::AlmCamera(AlmCamera && camera)
{
}

AlmCamera::~AlmCamera()
{
}

AlmCamera & alme::AlmCamera::operator=(AlmCamera && camera)
{
	return *this;
}

void AlmCamera::SetPerspective(float fFov, float fNear, float fFar, uint32_t uWight, uint32_t wHeight)
{
	m_projection = eAlmProjection::ePerspective;
	m_projMatrix = kmu::Perspective(fFov, uWight, wHeight, fNear, fFar);
}

void AlmCamera::SetOrthographic(int iLeft, int iRight, int iTop, int iBottom, int iNear, int iFar)
{
	m_projection = eAlmProjection::eOrthographic;
	m_projMatrix = kmu::Orthographic(iLeft, iRight, iTop, iBottom, iNear, iFar);
}

const kmu::mat4 AlmCamera::GetCameraMatrix()
{
	kmu::vec3 pos = GetPosition();
	kmu::quaternion rot = GetRotation();

	kmu::vec3 U = rot.rotate(VEC3_UP);
	kmu::vec3 F = rot.rotate(VEC3_FRONT);

	return std::move(kmu::CameraMatrix(F, U) * kmu::Translation(-pos.x, -pos.y, -pos.z));
}

const kmu::mat4 & AlmCamera::GetProjectionMatrix()
{
	return m_projMatrix;
}
