#pragma once
#include "World.h"
#include <stdlib.h>
#include <assert.h>

Body* bodies = NULL;
int bodyCount = 0;

Body* CreateBody()
{
	//allocate
	Body* body = (Body*)malloc(sizeof(Body));
	assert(body);

	//set the prev and next of the new body
	body->prev = NULL;
	body->next = bodies;
	
	//if list is not empty
	if (bodyCount > 0)
	{
		//update prev pointer of head
		bodies->prev = body;
	}
	//set the new body to the head
	bodies = body;
	//increment list amount
	bodyCount++;
	//return the body
	return body;
}

void DestroyBody(Body* body)
{
	
	assert(body);
	//if body has previous set the previous' next to the next body 
	if (body->prev) body->prev->next = body->next;
	//if body has next then set the next's previous to the previous body
	if (body->next) body->next->prev = body->prev;
	//if body is head set the next body to the head
	if (body == bodies) bodies = bodies->next;
	//deincrement bodycount
	bodyCount--;
	//deallocate memory
	free(body);

}