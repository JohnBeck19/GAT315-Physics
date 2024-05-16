#pragma once
#include "raylib.h"

typedef struct ncEditorData
{
    Vector2 anchor01;
    Vector2 anchor02;
    Vector2 anchor03;

    bool EditorBoxActive;
    float MassMinValue;
    float MassMaxValue;
    float GravitationValue;
    bool BodyTypeEditMode;
    int BodyTypeActive;
    float GravityScaleValue;
    float DampingSliderValue;
} ncEditorData_t;

extern ncEditorData_t ncEditorData;

extern bool ncEditorActive;
extern bool ncEditorIntersect;
extern Texture2D cursorTexture;

void InitEditor();
void UpdateEditor(Vector2 mousePosition);
void DrawEditor(Vector2 position);

struct ncBody* GetBodyIntersect(struct ncBody* bodies, Vector2 position); 
void DrawLineBodyToPosition(struct ncBody* body, Vector2 position);