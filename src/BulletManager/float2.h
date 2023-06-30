#pragma once

#include <math.h>

struct float2
{

	float x;
	float y;

	// overloads

	inline float2 operator* (const float& other) const
	{
		return { x * other, y * other };
	}

	inline float2 operator+ (const float2& other) const
	{
		return { x + other.x, y + other.y };
	}

	inline float2 operator- (const float2& other) const
	{
		return { x - other.x, y - other.y };
	}

	//

	inline float Dot(const float2& other) const
	{
		return x * other.x + y * other.y;
	}

	inline float Cross(const float2& other) const 
	{
		return x * other.y - y * other.x;
	}

	//

	inline float Magnitude() const
	{
		return sqrtf(x * x + y * y);
	}

	inline float SqrMagnitude() const
	{
		return (x * x + y * y);
	}

	inline void Normalize()
	{
		float magnitude = Magnitude();
		if (magnitude > 0)
		{
			x /= magnitude;
			y /= magnitude;
		}
	}

	inline float2 Normalized() const
	{
		float magnitude = Magnitude();

		if (magnitude > 0)
		{
			return { x / magnitude, y / magnitude };
		}
		else
			return { x, y };
	}

	//

	static float2 Reflect(const float2& inDirection, const float2& inNormal)
	{
		float factor = -2.0f * inNormal.Dot(inDirection);
		return float2(factor * inNormal.x + inDirection.x, factor * inNormal.y + inDirection.y);
	}

	inline
	float2 Perpendicular() const
	{
		return { -y, x };
	}

	/// <summary>
	/// Returns true if intersection is possible.
	/// Writes intersection point into &out.
	/// A->B C->D are segment points.
	/// </summary>
	inline
	static bool GetIntersectPoint(float2& out,
								  const float2& A, const float2& BA, const float2& C, const float2& D)
	{
		//

		//float2 BA = B - A;
		float2 DC = D - C;

		float crossProduct = BA.Cross(DC);

		if (crossProduct == 0)
			return false; // segments are parallel or lie on the same line

		float2 CA = (C - A);

		float t = CA.Cross(DC) / crossProduct;
		float u = CA.Cross(BA) / crossProduct;

		if (!(t >= 0 && t <= 1 && u >= 0 && u <= 1))
			return false;

		//

		out = A + BA * t;

		//

		return true;
	}

};