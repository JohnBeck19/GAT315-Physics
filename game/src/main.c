#include "Body.h"
#include "raylib.h"
#include "Mathf.h"
#include "Integrator.h"
#include "raymath.h"
#include "World.h"
#include "Force.h"
#include "editor.h"
#include "render.h"
#include <stdlib.h>
#include <assert.h>


#define MAX_BODIES 100
int main(void)
{
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
		if (IsMouseButtonPressed(0))
		{
			switch (GetRandomValue(1, 3))
			{
				case 1:
					for (int i = 0; i < 1; i++)
					{
						ncBody* body = CreateBody(ConvertScreenToWorld(position), GetRandomFloatValue(ncEditorData.MassMinValue, ncEditorData.MassMaxValue), ncEditorData.BodyTypeActive);
						AddBody(body);
						//body->type = ncEditorData.BodyTypeActive;
						//body->damping = 0.5f;
						body->gravityScale = 0;
						body->color = (Color){ 255,0,GetRandomFloatValue(0,255),255 };
						//ApplyForce(body, (Vector2) { GetRandomFloatValue(-200, 200), GetRandomFloatValue(-200, 200) }, FM_VELOCITY);
					}
					break;
				case 2:
					for (int i = 0; i < 1; i++)
					{
						ncBody* body = CreateBody(ConvertScreenToWorld(position), GetRandomFloatValue(ncEditorData.MassMinValue, ncEditorData.MassMaxValue),BT_DYNAMIC);
						AddBody(body);
						//body->damping = 0.25f;
						body->gravityScale = 5;
						body->color = (Color){ GetRandomFloatValue(0,255),GetRandomFloatValue(0,255),GetRandomFloatValue(0,255),255 };
						//Vector2 force = Vector2Scale(GetVector2FromAngle(GetRandomFloatValue(0, 360) * DEG2RAD), GetRandomFloatValue(1000, 2000));
						//ApplyForce(body, force, FM_IMPULSE);
					}
					break;
				case 3:
					for (int i = 0; i < 1; i++)
					{
						ncBody* body = CreateBody(ConvertScreenToWorld(position), GetRandomFloatValue(ncEditorData.MassMinValue, ncEditorData.MassMaxValue),BT_DYNAMIC);
						AddBody(body);
						//body->damping = 0.25f;
						body->gravityScale = 10;
						body->color = (Color){ 0,0,GetRandomFloatValue(100,255),255};
						//Vector2 force = Vector2Scale(GetVector2FromAngle(GetRandomFloatValue(0, 360) * DEG2RAD), GetRandomFloatValue(1000, 2000));
						//ApplyForce(body, force, FM_IMPULSE);
					}
					break;
		}

			
		}
		ApplyGravitation(ncBodies, ncEditorData.GravitationValue);
		for (ncBody* body = ncBodies; body; body = body->next)
		{
			//ApplyForce(body, CreateVector2(0,-100),FM_FORCE);
			Step(body, dt);
		}
		//draw
		BeginDrawing();
		ClearBackground(BLACK);


		//DrawCircle((int)position.x, (int)position.y, 10, BLUE);

		//update bodies
		// update / draw bodies
		
		for (ncBody* body = ncBodies; body; body = body->next)
		{
			Vector2 screen = ConvertWorldToScreen(body->position);
			if (body->gravityScale == 10)
			{
				Vector2 vel = Vector2Scale(Vector2Normalize(body->velocity), ConvertWorldToPixel(body->mass)*5);
				Vector2 trail = ConvertWorldToScreen((Vector2){ (int)body->position.x - vel.x,(int)body->position.y - vel.y });
				DrawLineEx(screen, trail, 5, RED);
			}
			DrawCircle((int)screen.x, (int)screen.y, ConvertWorldToPixel(body->mass), body->color);

			
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