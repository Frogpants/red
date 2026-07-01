#pragma once

#include "../math/essentials.hpp"

#include <array>
#include <limits>

struct OBB {
	vec3 center = vec3(0.0f);
	vec3 halfExtents = vec3(0.5f);
	std::array<vec3, 3> axis = {
		vec3(1.0f, 0.0f, 0.0f),
		vec3(0.0f, 1.0f, 0.0f),
		vec3(0.0f, 0.0f, 1.0f)
	};
};

struct CollisionInfo {
	bool hit = false;
	vec3 normal = vec3(0.0f, 1.0f, 0.0f);
	vec3 point = vec3(0.0f);
	float penetration = 0.0f;
};

inline vec3 scaleVec(const vec3& v, float s) {
	return vec3(v.x * s, v.y * s, v.z * s);
}

inline vec3 safeNormalize(const vec3& v) {
	float l = length(v);
	if (l <= 1e-6f) {
		return vec3(0.0f, 1.0f, 0.0f);
	}

	return vec3(v.x / l, v.y / l, v.z / l);
}

inline vec3 rotateEulerXYZ(const vec3& p, const vec3& deg) {
	vec3 r = radians(deg);
	float sx = std::sin(r.x);
	float cx = std::cos(r.x);
	float sy = std::sin(r.y);
	float cy = std::cos(r.y);
	float sz = std::sin(r.z);
	float cz = std::cos(r.z);

	vec3 out = p;

	float yx = out.y * cx - out.z * sx;
	float zx = out.y * sx + out.z * cx;
	out.y = yx;
	out.z = zx;

	float xx = out.x * cy + out.z * sy;
	float zz = -out.x * sy + out.z * cy;
	out.x = xx;
	out.z = zz;

	float xy = out.x * cz - out.y * sz;
	float yy = out.x * sz + out.y * cz;
	out.x = xy;
	out.y = yy;

	return out;
}

inline OBB makeOBB(const vec3& center, const vec3& halfExtents, const vec3& rotationDeg) {
	OBB box;
	box.center = center;
	box.halfExtents = halfExtents;
	box.axis[0] = safeNormalize(rotateEulerXYZ(vec3(1.0f, 0.0f, 0.0f), rotationDeg));
	box.axis[1] = safeNormalize(rotateEulerXYZ(vec3(0.0f, 1.0f, 0.0f), rotationDeg));
	box.axis[2] = safeNormalize(rotateEulerXYZ(vec3(0.0f, 0.0f, 1.0f), rotationDeg));
	return box;
}

inline float projectedRadius(const OBB& b, const vec3& axis) {
	return std::fabs(dot(b.axis[0], axis)) * b.halfExtents.x +
		   std::fabs(dot(b.axis[1], axis)) * b.halfExtents.y +
		   std::fabs(dot(b.axis[2], axis)) * b.halfExtents.z;
}

inline bool testAxis(const OBB& a, const OBB& b, const vec3& axisRaw, const vec3& toB, float& bestDepth, vec3& bestAxis) {
	float axisLen = length(axisRaw);
	if (axisLen <= 1e-6f) {
		return true;
	}

	vec3 axis = vec3(axisRaw.x / axisLen, axisRaw.y / axisLen, axisRaw.z / axisLen);

	float distance = std::fabs(dot(toB, axis));
	float ra = projectedRadius(a, axis);
	float rb = projectedRadius(b, axis);
	float overlap = (ra + rb) - distance;
	if (overlap <= 0.0f) {
		return false;
	}

	if (overlap < bestDepth) {
		bestDepth = overlap;
		bestAxis = axis;
		if (dot(toB, bestAxis) < 0.0f) {
			bestAxis = scaleVec(bestAxis, -1.0f);
		}
	}

	return true;
}

inline bool intersectOBB(const OBB& a, const OBB& b, CollisionInfo& info) {
	vec3 toB = b.center - a.center;

	float bestDepth = std::numeric_limits<float>::max();
	vec3 bestAxis = vec3(0.0f, 1.0f, 0.0f);

	for (int i = 0; i < 3; ++i) {
		if (!testAxis(a, b, a.axis[i], toB, bestDepth, bestAxis)) {
			return false;
		}
	}

	for (int i = 0; i < 3; ++i) {
		if (!testAxis(a, b, b.axis[i], toB, bestDepth, bestAxis)) {
			return false;
		}
	}

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			if (!testAxis(a, b, cross(a.axis[i], b.axis[j]), toB, bestDepth, bestAxis)) {
				return false;
			}
		}
	}

	float ra = projectedRadius(a, bestAxis);
	float rb = projectedRadius(b, bestAxis);
	vec3 pointA = a.center + scaleVec(bestAxis, ra);
	vec3 pointB = b.center - scaleVec(bestAxis, rb);

	info.hit = true;
	info.normal = bestAxis;
	info.penetration = bestDepth;
	info.point = (pointA + pointB) / 2.0f;
	return true;
}



