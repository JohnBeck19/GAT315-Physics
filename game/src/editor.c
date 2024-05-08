#include "editor.h"
#define RAYGUI_IMPLEMENTATION
#include "../../raygui/src/raygui.h"

bool ncEditorActive = true;
bool ncEditorIntersect = false;
ncEditorData_t ncEditorData;
Rectangle editorRect;

Vector2 anchor01 = {950, 25 };

bool EditorBoxActive = true;


void InitEditor()
{
    GuiLoadStyle("raygui/styles/lavanda/style_lavanda.rgs");
    ncEditorData.GravitationValue = 2;
    ncEditorData.MassMinValue = 0.1f;
    ncEditorData.MassMaxValue = 1.5f;
}

void UpdateEditor(Vector2 position)
{
    //
}

void DrawEditor()
{
    if (EditorBoxActive)
    {
        EditorBoxActive = !GuiWindowBox((Rectangle) { anchor01.x + 0, anchor01.y + 0, 312, 600 }, "Editor");
        GuiSliderBar((Rectangle) { anchor01.x + 72, anchor01.y + 72, 120, 16 }, "Mass Min", NULL, & ncEditorData.MassMinValue, 0, 100);
        GuiSliderBar((Rectangle) { anchor01.x + 72, anchor01.y + 104, 120, 16 }, "Mass Max", NULL, & ncEditorData.MassMaxValue, 0, 100);
        GuiSliderBar((Rectangle) { anchor01.x + 72, anchor01.y + 136, 120, 16 }, "Gravitation", NULL, & ncEditorData.GravitationValue, 0, 100);
    }
}
