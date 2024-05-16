#include "collision.h"
#include "Body.h"
#include "contact.h"
#include "Mathf.h"
#include "raymath.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

bool Intersects(ncBody* body1, ncBody* body2)
{
	//if distance is less than radius
	float distance = Vector2Distance(body1->position, body2->position);
	float radius = body1->mass + body2->mass;
	return (distance <= radius);
}

void CreateContacts(ncBody* bodies, ncContact_t** contacts)
{
	for (ncBody* body1 = bodies; body1; body1 = body1->next)
	{
		for (ncBody* body2 = body1->next; body2; body2 = body2->next)
		{
			if (body1 == body2) continue;
			if (body1->type != BT_DYNAMIC && body2->type != BT_DYNAMIC) continue;

			if (Intersects(body1, body2))
			{
				ncContact_t* contact = GenerateContact(body1, body2);
				AddContact(contact, contacts);
			}
		}
	}
}

ncContact_t* GenerateContact(ncBody* body1, ncBody* body2)
{
	ncContact_t* contact = (ncContact_t*)malloc(sizeof(ncContact_t));
	assert(contact);

	memset(contact, 0, sizeof(ncContact_t));

	contact->body1 = body1;
	contact->body2 = body2;

	Vector2 direction = Vector2Subtract(body1->position, body2->position);
	float distance = Vector2Length(direction);
	if (distance == 0)
	{
		direction = (Vector2){ GetRandomFloatValue(-0.05f, 0.05f), GetRandomFloatValue(-0.05f, 0.05f) };
	}

	float radius = (body1->mass + body2->mass);

	contact->depth = radius - distance;
	contact->normal = Vector2Normalize(direction);
	contact->restitution = (body1->restitution + body2->restitution) * 0.5f;

	return contact;
}

void SeparateContacts(ncContact_t* contacts)
{
	// Loop through each contact in the linked list
	for (ncContact_t* contact = contacts; contact; contact = contact->next)
	{
		// Calculate the total inverse mass of the two bodies involved in the contact
		float totalInverseMass = contact->body1->inverseMass + contact->body2->inverseMass;

		// Calculate the separation vector based on the contact normal and depth,
		// scaled by the inverse mass of the bodies
		Vector2 separation = Vector2Scale(contact->normal, (contact->depth / totalInverseMass));

		// Move the first body's position by the scaled separation vector,
		// taking into account its inverse mass
		contact->body1->position = Vector2Add(contact->body1->position, Vector2Scale(separation, contact->body1->inverseMass));

		// Move the second body's position by the negated scaled separation vector,
		// taking into account its inverse mass
		contact->body2->position = Vector2Add(contact->body2->position, Vector2Scale(Vector2Negate(separation), contact->body2->inverseMass));
	}
}


void ResolveContacts(ncContact_t* contacts)
{
	for (ncContact_t* contact = contacts; contact; contact = contact->next)
	{
		Vector2 rv = Vector2Subtract(contact->body1->velocity, contact->body2->velocity);
		float nv = Vector2DotProduct(rv, contact->normal);
		if (nv > 0) continue;

		float tim = contact->body1->inverseMass + contact->body2->inverseMass;
		float impulseMagnitude = (-(1 + contact->restitution) * nv / tim);
		Vector2 iv = Vector2Scale(contact->normal, impulseMagnitude);

		ApplyForce(contact->body1, iv, FM_IMPULSE);
		ApplyForce(contact->body2, Vector2Negate(iv), FM_IMPULSE);



	}
}
