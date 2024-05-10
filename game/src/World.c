#pragma once
#include "World.h"
#include "Body.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

ncBody* ncBodies = NULL;
int ncBodyCount = 0;
Vector2 ncGravity;

ncBody* CreateBody(Vector2 position, float mass, ncBodyType bodyType)
{
	//allocate
	ncBody* body = (ncBody*)malloc(sizeof(ncBody));
	assert(body);
	memset(body, 0, sizeof(ncBody));

	body->position = position;
	body->mass = mass;
	body->inverseMass = (bodyType == BT_DYNAMIC) ? 1 / mass : 0;
	body->type = bodyType;

	return body;
}

void AddBody(ncBody* body)
{
	assert(body);

	body->prev = NULL;
	body->next = ncBodies;

	//if list is not empty
	if (ncBodyCount > 0)
	{
		//update prev pointer of head
		ncBodies->prev = body;
	}
	//set the element to the head
	ncBodies = body;
	//increment list amount
	ncBodyCount++;
}
void DestroyBody(ncBody* body)
{
	
	assert(body);
	//if body has previous set the previous' next to the next body 
	if (body->prev) body->prev->next = body->next;
	//if body has next then set the next's previous to the previous body
	if (body->next) body->next->prev = body->prev;
	//if body is head set the next body to the head
	if (body == ncBodies) ncBodies = ncBodies->next;
	//deincrement bodycount
	ncBodyCount--;
	//deallocate memory
	free(body);

}