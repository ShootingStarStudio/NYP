#include "Camera3.h"

Camera3::Camera3()
{
}

Camera3::~Camera3()
{
}

void Camera3::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	view = (target - position).Normalized();
	right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();
}

void Camera3::Update(double dt)
{
	float CAMERA_SPEED = 45.f;
	float ZOOM_SPEED = 20.f;
	view = (target - position).Normalized();
	right = view.Cross(up);
	float pitch = -CAMERA_SPEED * static_cast<float>(dt);
	Vector3 pos = view;
	pos.y = 0;
	Mtx44 rotation;

	//sprint
	if (Application::IsKeyPressed(VK_SHIFT))
	{
		ZOOM_SPEED = 50.f;
		CAMERA_SPEED = 100.f;
	}
	else {
		ZOOM_SPEED = 20.f;
		CAMERA_SPEED = 45.f;
	}
	//delete when done modeling
	{
		/*if (Application::IsKeyPressed(VK_RIGHT))
		{
			float yaw = -CAMERA_SPEED * static_cast<float>(dt);
			rotation.SetToRotation(yaw, 0, 1, 0);
			up = rotation * up;
			view = rotation * view;
		}
		if (Application::IsKeyPressed(VK_LEFT))
		{
			float yaw = CAMERA_SPEED * static_cast<float>(dt);
			rotation.SetToRotation(yaw, 0, 1, 0);
			up = rotation * up;
			view = rotation * view;
		}
		if (Application::IsKeyPressed(VK_DOWN))
		{
			view = (target - position).Normalized();
			right = view.Cross(up);
			right.y = 0;
			right.Normalize();
			up = right.Cross(view).Normalized();
			rotation.SetToRotation(pitch, right.x, right.y, right.z);
			view = rotation * view;
			if (view.y < -0.9) {
				view.y = -0.9;
			}
		}
		if (Application::IsKeyPressed(VK_UP))
		{
			view = (target - position).Normalized();
			right = view.Cross(up);
			right.y = 0;
			right.Normalize();
			up = right.Cross(view).Normalized();
			rotation.SetToRotation(-pitch, right.x, right.y, right.z);
			view = rotation * view;
			if (view.y > 0.9) {
				view.y = 0.9;
			}
		}*/
	}
	/*if (Application::IsKeyPressed('W'))
	{
		position += pos * ZOOM_SPEED * static_cast<float>(dt);
	}
	if (Application::IsKeyPressed('S'))
	{
		position -= pos * ZOOM_SPEED * static_cast<float>(dt);
	}
	if (Application::IsKeyPressed('A'))
	{
		position -= right * ZOOM_SPEED * static_cast<float>(dt);
	}
	if (Application::IsKeyPressed('D'))
	{
		position += right * ZOOM_SPEED * static_cast<float>(dt);
	}*/
	if (Application::IsKeyPressed('R'))
	{
		Reset();
	}

	//bounds check
	/*{
		if (position.x < minx) {
			position.x = minx;
		}
		if (position.x > maxx) {
			position.x = maxx;
		}
		if (position.z < minz) {
			position.z = minz;
		}
		if (position.z > maxz) {
			position.z = maxz;
		}
	}*/
	target = position + view;
}

void Camera3::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}

