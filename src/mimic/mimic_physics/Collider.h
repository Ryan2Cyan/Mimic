#pragma once
#include <mimic_utility/Logger.h>

#include <glm/glm.hpp>
#include <vector>
#include <array>


namespace MimicPhysics
{
	/// <summary>
	/// Base class for collider components containing both the collider's offset (from it's parent GameObject), and 
	/// a bool relaying whether a collision has occured or not.
	/// </summary>
	struct Collider
	{
		virtual ~Collider() = default;

		void SetPosition(const glm::vec3& position);
		void SetOffset(const glm::vec3& offset);

		std::vector<glm::vec3> GetVertices() const;
		glm::vec3 GetPosition() const;
		glm::vec3 GetOffset() const;
		bool IsInitialised() const;

	protected: 
		std::vector<glm::vec3> _vertices;
		glm::vec3 _offset;
		glm::vec3 _position;
		bool _initialised;
	};

	/// <summary>
	/// Collision entity shaped like a cuboid. User can modify its size on the x, y, and z axes.
	/// User can set the 'OnCollisionEnter' and 'OnCollisionExit' functions to execute code.
	/// </summary>
	struct BoxCollider : Collider 
	{
		static std::shared_ptr<BoxCollider> Initialise(const glm::vec3& size, const glm::vec3& offset);

		/// <summary> Returns true if colliding with another mesh collider, otherwise 
		/// false </summary>
		template<typename T>
		bool IsColliding(const std::shared_ptr<T>& collider, const bool& aligned = false) const
		{
			// Check which type of collide has been inputted, and call the appropriate collision
			// detection algorithm:
			if (auto boxCollider = std::dynamic_pointer_cast<BoxCollider>(collider))
			{
				if (aligned) return AxisAlignedCollisionDetection(_self.lock(), boxCollider);
				return MimicPhysics::GJKCollisionDetection(_self.lock(), boxCollider);
			}
			if (auto meshCollider = std::dynamic_pointer_cast<MeshCollider>(collider)) return MimicPhysics::GJKCollisionDetection(_self.lock(), meshCollider);
			
			MIMIC_LOG_WARNING("[MimicPhysics::BoxCollider] Invalid collider data type input, unable to deduce collision detection function");
			return false;
		}

		void SetSize(const glm::vec3& size);

		glm::vec3 GetSize() const;

	private:
		glm::vec3 _size;
		std::weak_ptr<BoxCollider> _self;
	};

	/// <summary>
	/// Will check if the user has a ModelRenderer and valid Model/Meshes to extract vertices, if one of these conditions 
	/// is not met the MeshCollider will fail to initialise correctly. User can modify its size on the x, y, and z axes. 
	/// User can set the 'OnCollisionEnter' and 'OnCollisionExit' functions to execute code.
	/// </summary>
	struct MeshCollider : Collider
	{
		static std::shared_ptr<MeshCollider> Initialise(const std::vector<glm::vec3>& vertices);

		/// <summary> Returns true if colliding with another mesh collider, otherwise 
		/// false </summary>
		template<typename T>
		bool IsColliding(const std::shared_ptr<T>& collider) const
		{
			return MimicPhysics::GJKCollisionDetection(_self.lock(), collider);
		}

 	private:
		std::weak_ptr<MeshCollider> _self;
	};

	/// <summary>
	/// GJK algorithm helper struct. Used to construct a simplex from vertices (1 vertex = point, 2 vertices = line, 
	/// 3 vertices = triangle, 4 vertices = tetrahedron).
	/// </summary>
	struct Simplex
	{
		Simplex() : _size(0), _points({ glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f) }) {}

		Simplex& operator=(const std::initializer_list<glm::vec3>& list)
		{
			for (auto v = list.begin(); v != list.end(); v++) _points[std::distance(list.begin(), v)] = *v;
			_size = list.size();
			return *this;
		}

		glm::vec3& operator[](const unsigned& i)
		{
			return _points[i];
		}

		void PushFront(const glm::vec3& point)
		{
			_points = { point, _points[0], _points[1], _points[2] };
			_size = std::min(_size + 1, 4u);
		}

		unsigned GetSize() const
		{
			return _size;
		}

		auto Begin() const
		{
			return _points.begin();
		}

		auto End() const
		{
			return _points.end() - (4 - _size);
		}

	private:
		std::array<glm::vec3, 4> _points;
		unsigned _size;
	};

	/// <summary> 
	/// GJK collision detection helper function. Returns true if the inputted direction
	/// is facing the general direction of the inputted origin vector. 
	/// </summary>
	static bool SameDirection(const glm::vec3& direction, const glm::vec3& origin)
	{
		return glm::dot(direction, origin) > 0;
	};

	/// <summary> 
	/// GJK collision detection helper function. Uses deduction to determine which area
	/// defined by the line contains the origin, then modifies the points & direction accordingly.
	/// </summary>
	static bool Line(Simplex& points, glm::vec3& direction)
	{
		const glm::vec3 a = points[0];
		const glm::vec3 b = points[1];
		const glm::vec3 ab = b - a;
		const glm::vec3 ao = -a;

		if (SameDirection(ab, ao)) direction = glm::cross(glm::cross(ab, ao), ab);
		else
		{
			points = { a };
			direction = ao;
		}
		return false;
	}

	/// <summary> 
	/// GJK collision detection helper function. Uses deduction to determine which area
	/// defined by the triangle contains the origin, then modifies the points & direction accordingly.
	/// </summary>
	static bool Triangle(Simplex& points, glm::vec3& direction)
	{
		const glm::vec3 a = points[0];
		const glm::vec3 b = points[1];
		const glm::vec3 c = points[2];

		const glm::vec3 ab = b - a;
		const glm::vec3 ac = c - a;
		const glm::vec3 ao = -a;

		const glm::vec3 abc = glm::cross(ab, ac);

		if (SameDirection(glm::cross(abc, ac), ao))
		{
			if (SameDirection(ac, ao))
			{
				points = { a, c };
				direction = glm::cross(glm::cross(ac, ao), ac);
			}
			else return Line(points = { a, b }, direction);
		}
		else
		{
			if (SameDirection(glm::cross(ab, abc), ao)) return Line(points = { a, b }, direction);
			else
			{
				if (SameDirection(abc, ao)) direction = abc;
				else
				{
					points = { a, c, b };
					direction = -abc;
				}
			}
		}

		return false;
	}

	/// <summary> 
	/// GJK collision detection helper function. Checks for the origin inside a 
	/// tetrahedron simplex construct. Returns true if the origin is found, otherwise false. 
	/// </summary>
	static bool Tetrahedron(Simplex& points, glm::vec3& direction)
	{
		const glm::vec3 a = points[0];
		const glm::vec3 b = points[1];
		const glm::vec3 c = points[2];
		const glm::vec3 d = points[3];

		const glm::vec3 ab = b - a;
		const glm::vec3 ac = c - a;
		const glm::vec3 ad = d - a;
		const glm::vec3 ao = -a;

		const glm::vec3 abc = glm::cross(ab, ac);
		const glm::vec3 acd = glm::cross(ac, ad);
		const glm::vec3 adb = glm::cross(ad, ab);

		if (SameDirection(abc, ao)) return Triangle(points = { a, b, c }, direction);
		if (SameDirection(acd, ao)) return Triangle(points = { a, c, d }, direction);
		if (SameDirection(adb, ao)) return Triangle(points = { a, d, b }, direction);

		return true;
	}

	/// <summary> 
	/// GJK collision detection helper function. Calls simplex function based on the
	/// number of points in the simplex. 
	/// </summary>
	static bool NextSimplex(Simplex& points, glm::vec3& direction)
	{
		switch (points.GetSize())
		{
			case 2: return Line(points, direction);
			case 3: return Triangle(points, direction);
			case 4: return Tetrahedron(points, direction);
				break;
		}
		return false;
	}

	/// <summary> 
	/// Given any 3D shape's vertices, Returns the furthest vertex in any given direction. 
	/// The translate input is added onto the return vertex to convert its position to world space. 
	/// </summary>
	static glm::vec3 GetFarthestPoint(const std::vector<glm::vec3>& vertices, const glm::vec3& direction, const glm::vec3& translate)
	{
		// Source: https://en.wikipedia.org/wiki/Minkowski_addition

		auto farthestPoint = glm::vec3(0);
		float max = std::numeric_limits<float>::min();

		// Loop through all vertices, check the dot product with the input direction,
		// and cache result if larger than the last:
		for (auto vertex : vertices)
		{
			const float projection = glm::dot(vertex, direction);
			if (projection > max)
			{
				farthestPoint = vertex;
				max = projection;
			}
		}
		return farthestPoint + translate;
	}

	/// <summary> 
	/// GJK collision detection helper function. Based on the Minkowski Difference,
	/// calculates the "most-extreme" points in two shapes (vertices farthest in a given direction),
	/// and returns the difference. 
	/// </summary>
	static glm::vec3 Support(const std::shared_ptr<Collider>& colA, const std::shared_ptr<Collider>& colB, const glm::vec3& direction)
	{
		return GetFarthestPoint(colA->GetVertices(), direction, colA->GetPosition() + colA->GetOffset()) -
			GetFarthestPoint(colB->GetVertices(), -direction, colB->GetPosition() + colB->GetOffset());
	};

	/// <summary> GJK collision detection algorithm. Calculates the outer-hull of the Minkowski   
	/// Difference (difference between two shapes), if the origin is located within this new 
	/// shape, return true, otherwise false. Using a support function, a simplex is created around 
	/// the origin. </summary>
	static bool GJKCollisionDetection(const std::shared_ptr<Collider>& colA, const std::shared_ptr<Collider>& colB)
	{
		// Source: https://winter.dev/articles/gjk-algorithm
		// Source: https://www.youtube.com/watch?v=Qupqu1xe7Io&t=576s
		// Source: https://www.medien.ifi.lmu.de/lehre/ss10/ps/Ausarbeitung_Beispiel.pdf

		// Get initial support point in any direction (unit-x here):
		glm::vec3 support = Support(colA, colB, glm::vec3(1.0f, 0.0f, 0.0f));

		// Simplex is the simplex shapes that can contain a region in space. These include
		// a point, line, triangle, and tetrahedron. The max number of points is 4:
		Simplex points;
		points.PushFront(support);

		// New direction is towards the origin:
		glm::vec3 direction = -support;

		while (true)
		{
			support = Support(colA, colB, direction);

			// If the new support point is behind the new direction, there is no collision:
			if (glm::dot(support, direction) <= 0) return false;

			points.PushFront(support);

			// Check for collision:
			if (NextSimplex(points, direction)) return true;
		}
	}



	/// <summary>
	/// Calculates whether two axis aligned (no rotations) box colliders are colliding.
	/// </summary>
	static bool AxisAlignedCollisionDetection(const std::shared_ptr<BoxCollider>& colA, const std::shared_ptr<BoxCollider>& colB)
	{
		// Source: https://developer.mozilla.org/en-US/docs/Games/Techniques/3D_collision_detection
		auto aPos = colA->GetPosition();
		auto aSize = colA->GetSize() / 2.0f;

		auto bPos = colB->GetPosition();
		auto bSize = colB->GetSize();

		if (aPos.x - aSize.x > bPos.x + bSize.x) return false;
		if (aPos.x + aSize.x < bPos.x - bSize.x) return false;
		if (aPos.y - aSize.y > bPos.y + bSize.y) return false;
		if (aPos.y + aSize.y < bPos.y - bSize.y) return false;
		if (aPos.z - aSize.z > bPos.z + bSize.z) return false;
		if (aPos.z + aSize.z < bPos.z - bSize.z) return false;
		return true;
	}
}