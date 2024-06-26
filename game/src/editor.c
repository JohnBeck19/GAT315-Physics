#include "editor.h"
#include "Body.h"
#include "render.h"
#define RAYGUI_IMPLEMENTATION
#include "../../raygui/src/raygui.h"

bool ncEditorActive = true;
bool ncEditorIntersect = false;
ncEditorData_t ncEditorData;
Rectangle editorRect;

Vector2 anchor01 = {950, 25 };

bool EditorBoxActive = true;
Texture2D cursorTexture;


void InitEditor()
{
    GuiLoadStyle("raygui/styles/lavanda/style_lavanda.rgs");

    Image image = LoadImage("resources/reticle.png");
    cursorTexture = LoadTextureFromImage(image);
    ncEditorData.anchor01 = (Vector2){ 792, 24 };
    ncEditorData.anchor02 = (Vector2){ 816, 368 };
    ncEditorData.anchor03 = (Vector2){ 816, 72 };
    ncEditorData.EditorBoxActive = true;
    ncEditorData.MassMinValue = 10.0f;
    ncEditorData.MassMaxValue = 15.0f;
    ncEditorData.GravitationValue = 0.1f;
    ncEditorData.BodyTypeEditMode = false;
    ncEditorData.BodyTypeActive = 0;
    ncEditorData.GravityScaleValue = 0.5f;
    ncEditorData.DampingSliderValue = 0.0f;
    editorRect = (Rectangle){ anchor01.x + 100, anchor01.y + 0, 312,600 };
}

void UpdateEditor(Vector2 position)
{
    //
}

void DrawEditor(Vector2 position)
{
    if (ncEditorData.BodyTypeEditMode) GuiLock();

    if (ncEditorData.EditorBoxActive)
    {
        ncEditorData.EditorBoxActive = !GuiWindowBox((Rectangle) { ncEditorData.anchor01.x + 100, ncEditorData.anchor01.y + 0, 312, 600 }, "Editor");
    }
    GuiSliderBar((Rectangle) { ncEditorData.anchor03.x + 212, ncEditorData.anchor03.y + 56, 120, 16 }, "Mass Min", NULL, & ncEditorData.MassMinValue, 0, 100);
    GuiSliderBar((Rectangle) { ncEditorData.anchor03.x + 212, ncEditorData.anchor03.y + 80, 120, 16 }, "Mass Max", NULL, & ncEditorData.MassMaxValue, 0, 100);
    GuiSliderBar((Rectangle) { ncEditorData.anchor02.x + 172, ncEditorData.anchor02.y + 40, 120, 16 }, "Gravitation", NULL, & ncEditorData.GravitationValue, 0, 100);
    GuiGroupBox((Rectangle) { ncEditorData.anchor02.x + 100, ncEditorData.anchor02.y + 8, 256, 232 }, "World");
    GuiGroupBox((Rectangle) { ncEditorData.anchor03.x + 100, ncEditorData.anchor03.y + 8, 256, 248 }, "Body");
    GuiSliderBar((Rectangle) { ncEditorData.anchor03.x + 212, ncEditorData.anchor03.y + 128, 120, 16 }, "Gravity Scale", NULL, & ncEditorData.GravityScaleValue, 0, 100);
    GuiSliderBar((Rectangle) { ncEditorData.anchor03.x + 212, ncEditorData.anchor03.y + 104, 120, 16 }, "Damping", NULL, & ncEditorData.DampingSliderValue, 0, 100);
    if (GuiDropdownBox((Rectangle) { ncEditorData.anchor03.x + 172, ncEditorData.anchor03.y + 24, 120, 24 }, "DYNAMIC;STATIC;KINEMATIC", & ncEditorData.BodyTypeActive, ncEditorData.BodyTypeEditMode)) ncEditorData.BodyTypeEditMode = !ncEditorData.BodyTypeEditMode;

    DrawTexture(cursorTexture, position.x-cursorTexture.width/2, position.y-cursorTexture.height/2, WHITE);
    GuiUnlock();
}

ncBody* GetBodyIntersect(ncBody* bodies, Vector2 position)
{
    for (ncBody* body = bodies; body; body = body->next)
    {
        Vector2 screen = ConvertWorldToScreen(body->position);
        if (CheckCollisionPointCircle(position, screen, ConvertWorldToPixel(body->mass)))
        {
            return body;
        }
    }

    return NULL;
}

void DrawLineBodyToPosition(ncBody* body, Vector2 position)
{
    Vector2 screen = ConvertWorldToScreen(body->position);
    DrawLine((int)screen.x, (int)screen.y, (int)position.x - cursorTexture.width / 2, (int)position.y - cursorTexture.height / 2, YELLOW);
}
