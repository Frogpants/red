#pragma once

#include "../math/essentials.hpp"



struct Contact {
    vec3 point;

    vec3 normal;

    float penetration;

    RigidBody* A;

    RigidBody* B;
};

struct RigidBody {
    float mass;
    float invMass;

    vec3 position;
    vec4 rotation;

    vec3 velocity;
    vec3 omega;

    vec3 force;
    vec3 torque;

    float inertia;
    float invInertia;

    float e;

    bool isStatic = false;


    void applyForce(vec3 f) {
        force = force + f;
    }

    void applyForce(float f, vec3 dir) {
        force = force + normalize(dir) * f;
    }

    void applyForce(vec3 f, vec3 point) {
        force = force + normalize(position - point) * f;
    }

    void step(float dt) {
        vec3 acceleration = force * invMass;
        velocity = velocity + acceleration * dt;
        position = position + velocity * dt;

        force = vec3(0.0);


        vec3 alpha = torque * invInertia;
        omega = omega + alpha * dt;

        torque = vec3(0.0);

        vec4 spin = vec4(0.0, omega.x, omega.y, omega.z);
        rotation = rotation + normalize(spin * rotation * dt * 0.5);
    }

    

};

void resolve(Contact contact, RigidBody& other) {
    const float percent = 0.8f;
    const float slop = 0.01f;

    float correction = max(contact.penetration - slop, 0.0f);

    vec3 correctionVector = contact.normal * correction * percent;

    contact.A->position -= correctionVector * contact.A->invMass / (contact.A->invMass + contact.B->invMass);
    contact.B->position += correctionVector * contact.B->invMass / (contact.A->invMass + contact.B->invMass);

    vec3 ra = contact.point - contact.A->position;
    vec3 rb = contact.point - contact.B->position;

    vec3 va = contact.A->velocity + cross(contact.A->omega, ra);
    vec3 vb = contact.B->velocity + cross(contact.B->omega, rb);

    vec3 rv = vb - va;

    float velAlongNormal = dot(rv, contact.normal);
    if (velAlongNormal > 0) {
        return;
    }

    float effectiveMass = contact.A->invMass + contact.B->invMass + dot(cross(cross(ra, contact.normal) * contact.A->invInertia, ra) + cross(cross(rb, contact.normal), rb) * contact.B->invInertia, contact.normal);
    float j = -(1 + max(contact.A->e, contact.B->e)) * velAlongNormal / effectiveMass;
    vec3 impulse = contact.normal * j;

    contact.A->velocity -= impulse * contact.A->invMass;
    contact.B->velocity += impulse * contact.B->invMass;

    contact.A->omega -= cross(ra, impulse) * contact.A->invInertia;
    contact.B->omega += cross(rb, impulse) * contact.B->invInertia;

    vec3 tangent = rv - contact.normal * dot(rv, contact.normal);
    tangent = normalize(tangent);

    float jt = -dot(rv,tangent) / effectiveMass;
    float mu = 0.7;
    jt = clamp(jt, -j*mu, j*mu);
}