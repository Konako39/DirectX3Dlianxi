#ifndef FLIP_ANIMATION_H
#define FLIP_ANIMATION_H
#endif

void FlipAnimation_Initialize();
void FlipAnimation_Finalize();

int FlipBookAnimation_Create(int texture_id,
	int pattern_width, int pattern_height,
	int pattern_count, int pattern_column_count_max,
	float pattern_update_time
	);
void FlipBookAnimation_Update(float delta_time);
void FlipBookAnimation_Draw(int animation_id, float x, float y, float sizeX, float sizeY);



