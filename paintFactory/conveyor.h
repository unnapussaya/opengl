#pragma once

void block_position(float beltTranslation[BLOCK_NUMBER], float beltRotation[BLOCK_NUMBER]);
void move_block(float beltTranslation[BLOCK_NUMBER], float beltRotation[BLOCK_NUMBER]);
void drawSidebelt(float radius, float len, float width, int min);
void draw_conv_leg();
void conveyor(float beltTranslation[BLOCK_NUMBER], float beltRotation[BLOCK_NUMBER]);
void draw_box(float s);
void draw_machine(float beltTranslation[BLOCK_NUMBER], float beltRotation[BLOCK_NUMBER], float angleGear1, float angleGear2);
void draw_allboxes(float doorUp, float doorDown);