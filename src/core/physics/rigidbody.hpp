#pragma once

#include "../math/essentials.hpp"

#include <vector>
#include <algorithm>

#include "collision.hpp"



struct RigidBody;

struct Contact {
    vec3 point = vec3(0.0f);
    vec3 normal = vec3(0.0f, 1.0f, 0.0f);
    float penetration = 0.0f;
    RigidBody* A = nullptr;
    RigidBody* B = nullptr;
};

struct RigidBody {
    float mass = 1.0f;
    float invMass = 1.0f;

    vec3 position = vec3(0.0f);
    vec3 rotation = vec3(0.0f);
    vec3 scale = vec3(1.0f);
    vec3 localCenter = vec3(0.0f);
    vec3 localHalfExtents = vec3(0.5f);

    vec3 velocity = vec3(0.0f);
    vec3 omega = vec3(0.0f);

    vec3 force = vec3(0.0f);
    vec3 torque = vec3(0.0f);

    vec3 invInertia = vec3(0.0f);

    float restitution = 0.2f;
    float staticFriction = 0.6f;
    float dynamicFriction = 0.45f;
    float linearDamping = 0.2f;
    float angularDamping = 0.2f;
    bool isStatic = false;

    void setMass(float m) {
        mass = m;
        if (isStatic || m <= 0.0f) {
            invMass = 0.0f;
        } else {
            invMass = 1.0f / m;
        }
    }

    void setBoxCollider(const vec3& halfExtents, const vec3& localOffset = vec3(0.0f)) {
        localHalfExtents = max(abs(halfExtents), 1e-4f);
        localCenter = localOffset;
        recalculateInertia();
    }

    void recalculateInertia() {
        if (isStatic || mass <= 0.0f) {
            invInertia = vec3(0.0f);
            return;
        }

        vec3 s = localHalfExtents * scale;
        float wx = 2.0f * s.x;
        float wy = 2.0f * s.y;
        float wz = 2.0f * s.z;

        float ix = (mass / 12.0f) * (wy * wy + wz * wz);
        float iy = (mass / 12.0f) * (wx * wx + wz * wz);
        float iz = (mass / 12.0f) * (wx * wx + wy * wy);

        invInertia = vec3(ix > 1e-6f ? 1.0f / ix : 0.0f,
                          iy > 1e-6f ? 1.0f / iy : 0.0f,
                          iz > 1e-6f ? 1.0f / iz : 0.0f);
    }

    OBB worldOBB() const {
        vec3 worldCenter = position + rotateEulerXYZ(localCenter * scale, rotation);
        vec3 worldHalf = max(abs(localHalfExtents * scale), 1e-4f);
        return makeOBB(worldCenter, worldHalf, rotation);
    }

    void applyForce(vec3 f) {
        if (isStatic) {
            return;
        }
        force = force + f;
    }

    void applyForceAtPoint(const vec3& f, const vec3& worldPoint) {
        if (isStatic) {
            return;
        }
        force = force + f;
        vec3 r = worldPoint - position;
        torque = torque + cross(r, f);
    }

    void applyImpulse(const vec3& impulse, const vec3& worldPoint) {
        if (isStatic) {
            return;
        }

        velocity = velocity + impulse * invMass;

        vec3 r = worldPoint - position;
        vec3 angularImpulse = cross(r, impulse);
        omega.x += angularImpulse.x * invInertia.x;
        omega.y += angularImpulse.y * invInertia.y;
        omega.z += angularImpulse.z * invInertia.z;
    }

    void integrate(float dt, const vec3& gravity = vec3(0.0f)) {
        if (isStatic) {
            force = vec3(0.0f);
            torque = vec3(0.0f);
            velocity = vec3(0.0f);
            omega = vec3(0.0f);
            return;
        }

        vec3 acceleration = gravity + force * invMass;
        velocity = velocity + acceleration * dt;
        velocity = velocity * std::max(0.0f, 1.0f - linearDamping * dt);

        vec3 angularAcceleration(
            torque.x * invInertia.x,
            torque.y * invInertia.y,
            torque.z * invInertia.z
        );
        omega = omega + angularAcceleration * dt;
        omega = omega * std::max(0.0f, 1.0f - angularDamping * dt);

        position = position + velocity * dt;
        rotation = rotation + degrees(omega * dt);

        force = vec3(0.0);
        torque = vec3(0.0);
    }
};

inline bool collide(RigidBody& a, RigidBody& b, Contact& outContact) {
    if (&a == &b) {
        return false;
    }

    if (a.isStatic && b.isStatic) {
        return false;
    }

    CollisionInfo info;
    if (!intersectOBB(a.worldOBB(), b.worldOBB(), info)) {
        return false;
    }

    outContact.A = &a;
    outContact.B = &b;
    outContact.normal = info.normal;
    outContact.point = info.point;
    outContact.penetration = info.penetration;
    return true;
}

inline void positionalCorrection(Contact& contact) {
    const float percent = 0.75f;
    const float slop = 0.001f;

    float totalInvMass = contact.A->invMass + contact.B->invMass;
    if (totalInvMass <= 1e-6f) {
        return;
    }

    float correction = std::max(contact.penetration - slop, 0.0f);
    vec3 correctionVector = contact.normal * (correction * percent / totalInvMass);

    if (!contact.A->isStatic) {
        contact.A->position = contact.A->position - correctionVector * contact.A->invMass;
    }

    if (!contact.B->isStatic) {
        contact.B->position = contact.B->position + correctionVector * contact.B->invMass;
    }
}

inline float invInertiaAlong(const RigidBody& body, const vec3& r, const vec3& n) {
    vec3 rn = cross(r, n);
    vec3 irn = vec3(rn.x * body.invInertia.x, rn.y * body.invInertia.y, rn.z * body.invInertia.z);
    return dot(cross(irn, r), n);
}

inline void resolve(Contact& contact) {
    positionalCorrection(contact);

    vec3 ra = contact.point - contact.A->position;
    vec3 rb = contact.point - contact.B->position;

    vec3 va = contact.A->velocity + cross(contact.A->omega, ra);
    vec3 vb = contact.B->velocity + cross(contact.B->omega, rb);
    vec3 rv = vb - va;

    float velAlongNormal = dot(rv, contact.normal);
    if (velAlongNormal > 0.0f) {
        return;
    }

    float invMassSum = contact.A->invMass + contact.B->invMass +
        invInertiaAlong(*contact.A, ra, contact.normal) +
        invInertiaAlong(*contact.B, rb, contact.normal);
    if (invMassSum <= 1e-6f) {
        return;
    }

    float restitution = std::min(contact.A->restitution, contact.B->restitution);
    float j = -(1.0f + restitution) * velAlongNormal;
    j /= invMassSum;

    vec3 impulse = contact.normal * j;
    contact.A->applyImpulse(scaleVec(impulse, -1.0f), contact.point);
    contact.B->applyImpulse(impulse, contact.point);

    va = contact.A->velocity + cross(contact.A->omega, ra);
    vb = contact.B->velocity + cross(contact.B->omega, rb);
    rv = vb - va;

    vec3 tangent = rv - contact.normal * dot(rv, contact.normal);
    float tangentLen = length(tangent);
    if (tangentLen <= 1e-6f) {
        return;
    }

    tangent = tangent / tangentLen;
    float jt = -dot(rv, tangent);
    jt /= invMassSum;

    float muS = std::sqrt(contact.A->staticFriction * contact.B->staticFriction);
    float muD = std::sqrt(contact.A->dynamicFriction * contact.B->dynamicFriction);

    vec3 frictionImpulse;
    if (std::fabs(jt) < j * muS) {
        frictionImpulse = tangent * jt;
    } else {
        frictionImpulse = tangent * (-j * muD);
    }

    contact.A->applyImpulse(scaleVec(frictionImpulse, -1.0f), contact.point);
    contact.B->applyImpulse(frictionImpulse, contact.point);
}

struct PhysicsWorld {
    std::vector<RigidBody*> bodies;
    vec3 gravity = vec3(0.0f);
    int solverIterations = 8;

    void clear() {
        bodies.clear();
    }

    void addBody(RigidBody& body) {
        bodies.push_back(&body);
    }

    void step(float dt) {
        for (RigidBody* body : bodies) {
            body->integrate(dt, gravity);
        }

        for (int iteration = 0; iteration < solverIterations; ++iteration) {
            for (size_t i = 0; i < bodies.size(); ++i) {
                for (size_t j = i + 1; j < bodies.size(); ++j) {
                    Contact c;
                    if (collide(*bodies[i], *bodies[j], c)) {
                        resolve(c);
                    }
                }
            }
        }
    }
};