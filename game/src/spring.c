#include "spring.h"
#include "Body.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>


ncSpring_t* ncSprings;
int ncSpringCount = 0;
Vector2 ncGravity;

ncSpring_t* CreateSpring(struct NcBody* body1, struct NcBody* body2, float restLength, float k)
{

	ncSpring_t* spring = (ncSpring_t*)malloc(sizeof(ncSpring_t));
	assert(spring);
	memset(spring, 0, sizeof(ncSpring_t));

	spring->body1 = body1;
	spring->body2 = body2;
	spring->restLength = restLength;
	spring->k = k;

	return spring;
}


void AddSpring(ncSpring_t* spring) {

	assert(spring);
	spring->next = ncSprings;
	spring->prev = NULL;


	if (ncSprings) ncSprings->prev = spring;
	//set head of elements to new element
	ncSprings = spring;

}

void DestorySpring(ncSpring_t* spring)
{
	assert(spring);

	if (spring->prev != NULL) spring->prev->next = spring->next;
	if (spring->next != NULL) spring->next->prev = spring->prev;

	if (spring == ncSprings) { ncSprings = spring->next; }
	free(spring);

}

void DestoryAllSprings() {

}

void ApplySpringForce(ncSpring_t* spring) {
	for (ncSpring_t* spring = ncSprings; spring; spring = spring->next)
	{
		Vector2 dir = Vector2Subtract(spring->body1->position, spring->body2->position);
		if (dir.x == 0 && dir.y == 0) continue;

		float length = Vector2Length(dir);
		float x = length - spring->restLength;
		float force = -spring->k * x;

		Vector2 ndirection = Vector2Normalize(dir);


		ApplyForce(spring->body1, Vector2Scale(ndirection, force), FM_FORCE);
		ApplyForce(spring->body2, Vector2Scale(ndirection, -force), FM_FORCE);
	}

}
