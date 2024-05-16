#include "Body.h"
#include "raylib.h"
#include "Mathf.h"
#include "Integrator.h"
#include "raymath.h"
#include "World.h"
#include "Force.h"
#include "editor.h"
#include "render.h"
#include "collision.h"
#include "contact.h"
#include "spring.h"
#include <stdlib.h>
#include <assert.h>



#define MAX_BODIES 100
int main(void)
{
	ncBody* selectedBody = NULL;
	ncBody* connectBody = NULL;

	InitWindow(1280, 720, "Physics Engine");
	InitEditor();
	SetTargetFPS(60);

	//initalize world
	ncScreenZoom = 5;
	ncGravity = (Vector2){ 0, -1};

	//game loop
	while (!WindowShouldClose())
	{
		//update 
		float dt = GetFrameTime();
		float fps = (float)GetFPS();

		Vector2 position = GetMousePosition();
		ncScreenZoom += GetMouseWheelMove() * 0.2f;
		ncScreenZoom = Clamp(ncScreenZoom, 0.1f, 10);
		UpdateEditor(position);
		selectedBody = GetBodyIntersect(ncBodies, position);
		if (selectedBody)
		{
			Vector2 screen = ConvertWorldToScreen(selectedBody->position);
			DrawCircleLines(screen.x, screen.y, ConvertWorldToPixel(selectedBody->mass * 0.5) + 5, YELLOW);
		}
		//create body
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && IsKeyDown(KEY_LEFT_SHIFT)))
		{
				ncBody* body = CreateBody(ConvertScreenToWorld(position), GetRandomFloatValue(ncEditorData.MassMinValue, ncEditorData.MassMaxValue), ncEditorData.BodyTypeActive);
				AddBody(body);
				//body->damping = 0.25f;
				body->gravityScale = 10;
				body->color = (Color){ 0,0,GetRandomFloatValue(100,255),255};
				body->restitution = 0.8f;
				//Vector2 force = Vector2Scale(GetVector2FromAngle(GetRandomFloatValue(0, 360) * DEG2RAD), GetRandomFloatValue(1000, 2000));
		}

		if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && selectedBody) connectBody = selectedBody;
		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && connectBody) DrawLineBodyToPosition(connectBody, position);
		if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT) && connectBody)
		{
			if (selectedBody && selectedBody != connectBody)
			{
				ncSpring_t* spring = CreateSpring(connectBody, selectedBody, Vector2Distance(connectBody->position, selectedBody->position), 20);
				AddSpring(spring);
			}
		}

		ApplyGravitation(ncBodies, ncEditorData.GravitationValue);
		ApplySpringForce(ncSprings);
		for (ncBody* body = ncBodies; body; body = body->next)
		{
			//ApplyForce(body, CreateVector2(0,-100),FM_FORCE);
			Step(body, dt);
		}

		//collision
		ncContact_t* contacts = NULL;
		CreateContacts(ncBodies, &contacts);
		SeparateContacts(contacts);
		ResolveContacts(contacts);

		//draw
		BeginDrawing();
		ClearBackground(BLACK);


		//DrawCircle((int)position.x, (int)position.y, 10, BLUE);

		//update bodies
		// update / draw bodies
		
		for (ncBody* body = ncBodies; body; body = body->next)
		{
			Vector2 screen = ConvertWorldToScreen(body->position);
			DrawCircle((int)screen.x, (int)screen.y, ConvertWorldToPixel(body->mass*0.5f), body->color);
			//if (body->gravityScale == 10)
			//{
			//	Vector2 vel = Vector2Scale(Vector2Normalize(body->velocity), ConvertWorldToPixel(body->mass)*5);
			//	Vector2 trail = ConvertWorldToScreen((Vector2){ (int)body->position.x - vel.x,(int)body->position.y - vel.y });
			//	DrawLineEx(screen, trail, 5, RED);
			//}

		}
		//draw springs
		for (ncSpring_t* spring = ncSprings; spring; spring = spring->next)
		{
			Vector2 screen1 = ConvertWorldToScreen(spring->body1->position);
			Vector2 screen2 = ConvertWorldToScreen(spring->body2->position);
			DrawLine((int)screen1.x, (int)screen1.y, (int)screen2.x, (int)screen2.y, YELLOW);
		}
		//draw contact
		for (ncContact_t* contact = contacts; contact; contact = contact->next)
		{
			Vector2 screen = ConvertWorldToScreen(contact->body1->position);
			DrawCircle((int)screen.x, (int)screen.y, ConvertWorldToPixel(contact->body1->mass*0.5f), RED);


		}
		//stats
		DrawText(TextFormat("FPS: %.2f (%.2fms)", fps,1000/fps), 10, 10, 20, LIME);
		DrawEditor(position);
		HideCursor();
		DrawText(TextFormat("FRAME: %.4f", dt), 10, 30, 20, LIME);

		EndDrawing();
	}
	CloseWindow();
	return 0;
}
