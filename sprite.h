#pragma once
#ifndef SPRITE_H
#define SPRITE_H
#include <DirectXmath.h>

bool Sprite_Initialize();
void Sprite_Finalize();

void Sprite_Draw(int texture_id, float position_x, float position_y);
void Sprite_Draw(int texture_id, float position_x, float position_y, const DirectX::XMFLOAT4& color);

void Sprite_Draw(
	int texture_id,
	float position_x, float position_y,
	float width, float height
);
void Sprite_Draw(
	int texture_id,
	float position_x, float position_y,
	float width, float height,
	const DirectX::XMFLOAT4& color
);

void Sprite_Draw(
	int texture_id,
	float position_x, float position_y,
	int texture_x, int texture_y,
	int texture_width, int texture_height
);
void Sprite_Draw(
	int texture_id,
	float position_x, float position_y,
	int texture_x, int texture_y,
	int texture_width, int texture_height,
	const DirectX::XMFLOAT4& color
);

void Sprite_Draw(
	int texture_id,
	float position_x, float position_y,
	float width, float height,
	int texture_x, int texture_y,
	int texture_width, int texture_height
);
void Sprite_Draw(
	int texture_id,
	float position_x, float position_y,
	float width, float height,
	int texture_x, int texture_y,
	int texture_width, int texture_height,
	const DirectX::XMFLOAT4& color
);

#endif //SPRITE_H
