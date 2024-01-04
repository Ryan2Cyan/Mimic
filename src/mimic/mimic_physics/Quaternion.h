#pragma once
#include <glm/glm.hpp>
#include <cmath>

# define PI           3.14159265358979323846

namespace MimicPhysics
{
	// #############################################################################
	// Quaternion Struct:
	// #############################################################################
	// Source: https://books.google.co.uk/books/about/Physics_for_Game_Developers.html?id=YHamXqda_swC&redir_esc=y
	struct Quaternion
	{
		Quaternion();
		Quaternion(const float& e0, const glm::vec3& vector);
		Quaternion(const float& e0, const float& e1, const float& e2, const float& e3);

		/// <summary> Returns the magnitude of the quaternion's vector, squaring all the elements,
		/// returning the square-root of the sum.</summary>
		float Magnitude() const;

		glm::vec3 GetVector() const;
		float GetScalar() const;

		friend Quaternion operator+(const Quaternion& quatA, const Quaternion& quatB); 
		friend Quaternion operator-(const Quaternion& quatA, const Quaternion& quatB);
		friend Quaternion operator*(const Quaternion& quatA, const Quaternion& quatB);
		friend Quaternion operator*(const Quaternion& quat, const float& scalar);
		friend Quaternion operator*(const float& scalar, const Quaternion& quat);
		friend Quaternion operator*(const Quaternion& quat, const glm::vec3& vector);
		friend Quaternion operator*(const glm::vec3& vector, const Quaternion& quat);
		friend Quaternion operator/(const Quaternion& quat, const float& scalar);

		Quaternion& Quaternion::operator+=(const Quaternion& q)
		{
			_numberScalar += q.GetScalar();
			_vector += q.GetVector();
			return *this;
		};

		Quaternion& Quaternion::operator-=(const Quaternion& q)
		{
			_numberScalar -= q.GetScalar();
			_vector -= q.GetVector();
			return *this;
		};

		Quaternion& Quaternion::operator*=(const float& scalar)
		{
			_numberScalar *= scalar;
			_vector.x *= scalar;
			_vector.y *= scalar;
			_vector.z *= scalar;
			return *this;
		};

		Quaternion& Quaternion::operator/=(const float& scalar)
		{
			_numberScalar /= scalar;
			_vector.x /= scalar;
			_vector.y /= scalar;
			_vector.z /= scalar;
			return *this;
		};

		Quaternion& Quaternion::operator~() const
		{
			// Return the conjugate of the quaternion:
			return Quaternion(_numberScalar, -_vector);
		};

	private:
		float _numberScalar;
		glm::vec3 _vector;
	};

	/// <summary> Returns angle of rotation about axis represented by the vector part of
	/// inputted the quaternion. </summary>
	static float QuatGetAngle(const Quaternion& quat)
	{
		return 2.0f * acos(quat.GetScalar());
	};

	/// <summary> Returns a unit vector along the axis of rotation represented by the vector
	/// part of the inputted quaternion.</summary>
	static glm::vec3 QuatGetAxis(const Quaternion& quat)
	{
		return quat.GetVector() / quat.Magnitude();
	};

	/// <summary> Rotate quaternion. </summary>
	static Quaternion QuatRotate(const Quaternion& quatA, const Quaternion& quatB)
	{
		return quatA * quatB * (~quatA);
	}

	/// <summary> Rotates input vector by the input unit quaternion. </summary>
	static glm::vec3 QuatVec3Rotate(const glm::vec3& vector, const Quaternion& quat)
	{
		return (quat * vector * (~quat)).GetVector();
	}

	/// <summary> Convert a value from degrees to radians. </summary>
	static float DegreesToRadians(const float& degrees)
	{
		return degrees * PI / 180.0f;
	}

	/// <summary> Convert a value from radians to degrees. </summary>
	static float RadiansToDegrees(const float& radians)
	{
		return radians * 180.0f / PI;
	}

	/// <summary> Constructs a quaternion from a set of eular angles (yaw, pitch, roll) by
	/// creating a unique quaternion for each Eular angle, then returning the three multiplied
	/// together. </summary>
	static Quaternion QuatFromEularAngles(const glm::vec3& eularAngles)
	{
		// Extract the roll (x), pitch(y), and yaw(z):
		const glm::vec3 radianEularAngles = glm::vec3(
			DegreesToRadians(eularAngles.x),
			DegreesToRadians(eularAngles.y),
			DegreesToRadians(eularAngles.z)
		);

		const auto cosRoll = cos(0.5f * radianEularAngles.x);
		const auto cosPitch = cos(0.5f * radianEularAngles.y);
		const auto cosYaw = cos(0.5f * radianEularAngles.z);

		const auto sinRoll = sin(0.5f * radianEularAngles.x);
		const auto sinPitch = sin(0.5f * radianEularAngles.y);
		const auto sinYaw = sin(0.5f * radianEularAngles.z);

		const auto cosYawCosPitch = cosYaw * cosPitch;
		const auto sinYawSinPitch = sinYaw * sinPitch;
		const auto cosYawSinPitch = cosYaw * sinPitch;
		const auto sinYawCosPitch = sinYaw * cosPitch;

		return Quaternion(
			cosYawCosPitch * cosRoll + sinYawSinPitch * sinRoll,
			cosYawCosPitch * sinRoll - sinYawSinPitch * cosRoll,
			cosYawSinPitch * cosRoll + sinYawCosPitch * sinRoll,
			sinYawCosPitch * cosRoll - cosYawSinPitch * sinRoll
		);
	}

	/// <summary> Extract three eular angles from the input quaternion. The quaternion is
	/// converted into a rotation matrix, in which the eular angles are extracted from. </summary>
	static glm::vec3 EularAnglesFromQuat(const Quaternion& quat)
	{
		const auto qScalar = quat.GetScalar();
		const auto qVector = quat.GetVector();

		const auto q00 = qScalar * qScalar;
		const auto q11 = qVector.x * qVector.x;
		const auto q22 = qVector.y * qVector.y;
		const auto q33 = qVector.z * qVector.z;

		const auto r11 = q00 + q11 - q22 - q33;
		const auto r21 = 2.0f * (qVector.x * qVector.y + qScalar * qVector.z);
		const auto r31 = 2.0f * (qVector.x * qVector.z - qScalar * qVector.y);
		const auto r32 = 2.0f * (qVector.y * qVector.z + qScalar * qVector.x);
		const auto r33 = q00 - q11 - q22 + q33;

		const auto temp = fabs(r31);
		if (temp > 0.999999f)
		{
			const auto r12 = 2.0f * (qVector.x * qVector.y - qScalar * qVector.z);
			const auto r13 = 2.0f * (qVector.x * qVector.y + qScalar * qVector.y);

			return glm::vec3(
				RadiansToDegrees(0.0f),							// roll
				RadiansToDegrees((-PI / 2.0f) * r31 / temp),	// pitch
				RadiansToDegrees(atan2(-r12, -r31 * r13))		// yaw
			);
		}

		return glm::vec3(
			RadiansToDegrees(atan2(r32, r33)),	// roll
			RadiansToDegrees(asin(-r31)),		// pitch
			RadiansToDegrees(atan2(r21, r11))	// yaw
		);
	}

	static Quaternion operator+(const Quaternion& quatA, const Quaternion& quatB)
	{
		return Quaternion(quatA.GetScalar() + quatB.GetScalar(), quatA.GetVector() + quatB.GetVector());
	};

	static Quaternion operator-(const Quaternion& quatA, const Quaternion& quatB)
	{
		return Quaternion(quatA.GetScalar() - quatB.GetScalar(), quatA.GetVector() - quatB.GetVector());
	};

	static Quaternion operator*(const Quaternion& quatA, const Quaternion& quatB)
	{
		const auto aScalar = quatA.GetScalar();
		const auto aVector = quatA.GetVector();

		const auto bScalar = quatB.GetScalar();
		const auto bVector = quatB.GetVector();

		return Quaternion(
			aScalar * bScalar - aVector.x * bVector.x - aVector.y * bVector.y - aVector.z * bVector.z,
			aScalar * bVector.x + aVector.x * bScalar + aVector.y * bVector.z - aVector.z * bVector.y,
			aScalar * bVector.y + aVector.y * bScalar + aVector.z * bVector.x - aVector.x * bVector.z,
			aScalar * bVector.z + aVector.z * bScalar + aVector.x * bVector.y - aVector.y * bVector.x
		);
	};

	static Quaternion operator*(const Quaternion& quat, const float& scalar)
	{
		return Quaternion(quat.GetScalar() * scalar, quat.GetVector() * scalar);
	};

	static Quaternion operator*(const float& scalar, const Quaternion& quat)
	{
		return Quaternion(quat.GetScalar() * scalar, quat.GetVector() * scalar);
	};

	static Quaternion operator*(const Quaternion& quat, const glm::vec3& vector)
	{
		const auto qScalar = quat.GetScalar();
		const auto qVector = quat.GetVector();

		return Quaternion(
			-(qVector.x * vector.x + qVector.y * vector.y, qVector.z * vector.z),
			qScalar * vector.x + qVector.z * vector.y - qVector.z * vector.y,
			qScalar * vector.y + qVector.x * vector.x - qVector.x * vector.z,
			qScalar * vector.z + qVector.x * vector.y - qVector.y * vector.x
		);
	};

	static Quaternion operator*(const glm::vec3& vector, const Quaternion& quat)
	{
		const auto qScalar = quat.GetScalar();
		const auto qVector = quat.GetVector();

		return Quaternion(
			-(qVector.x * vector.x + qVector.y * vector.y, qVector.z * vector.z),
			qScalar * vector.x + qVector.z * vector.y - qVector.z * vector.y,
			qScalar * vector.y + qVector.x * vector.x - qVector.x * vector.z,
			qScalar * vector.z + qVector.x * vector.y - qVector.y * vector.x
		);
	};

	static Quaternion operator/(const Quaternion& quat, const float& scalar)
	{
		return Quaternion(quat.GetScalar() / scalar, quat.GetVector() / scalar);
	};
}