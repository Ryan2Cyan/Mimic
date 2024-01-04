#pragma once
#include <glm/glm.hpp>
#include <cmath>

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