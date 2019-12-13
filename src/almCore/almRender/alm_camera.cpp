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

void AlmCamera::SetPerspective(float fov, float near, float far, uint32_t wight, uint32_t height)
{
	m_projection = eAlmProjection::ePerspective;
	m_projMatrix = kmu::Perspective(fov, wight, height, near, far);
}

void AlmCamera::SetOrthographic(int left, int right, int top, int bottom, int near, int far)
{
	m_projection = eAlmProjection::eOrthographic;
	m_projMatrix = kmu::Orthographic(left, right, top, bottom, near, far);
}

const kmu::mat4 AlmCamera::GetCameraMatrix()
{
	kmu::vec3 pos = GetTransform()->GetPosition();
	kmu::quaternion rot = GetTransform()->GetRotation();

	kmu::vec3 U = rot.rotate(VEC3_UP);
	kmu::vec3 F = rot.rotate(VEC3_FRONT);

	return std::move(kmu::CameraMatrix(F, U) * kmu::Translation(-pos.x, -pos.y, -pos.z));
}

const kmu::mat4 & AlmCamera::GetProjectionMatrix()
{
	return m_projMatrix;
}
