#include "animationFlip.h"
#include  "sprite.h"


void FlipAnimation_Initialize()
{

}
void FlipAnimation_Finalize()
{

}

struct FLIP_ANIMATION
{
    int texture_id;

    int pattern_width;
    int pattern_height;

    int pattern_count;

    int pattern_column_count_max;

    float pattern_update_time;

    int current_pattern;

    float timer;
};

static FLIP_ANIMATION g_Animations[100];
static int g_AnimationCount = 0;

int FlipBookAnimation_Create(
    int texture_id,
    int pattern_width,
    int pattern_height,
    int pattern_count,
    int pattern_column_count_max,
    float pattern_update_time)
{
    int id = g_AnimationCount++;

    g_Animations[id].texture_id = texture_id;

    g_Animations[id].pattern_width = pattern_width;
    g_Animations[id].pattern_height = pattern_height;

    g_Animations[id].pattern_count = pattern_count;
    g_Animations[id].pattern_column_count_max = pattern_column_count_max;
    //这里是动画数组里每个帧的切图大小定义和时间间隔

    g_Animations[id].pattern_update_time = pattern_update_time;

    g_Animations[id].current_pattern = 0;
    g_Animations[id].timer = 0.0f;

    return id;
}


void FlipBookAnimation_Update(float delta_time)
{
    for (int i = 0; i < g_AnimationCount; i++)
    {
        g_Animations[i].timer += delta_time;

        if (g_Animations[i].timer >=
            g_Animations[i].pattern_update_time)
        {
            g_Animations[i].timer = 0.0f;

            g_Animations[i].current_pattern++;
            //这里是单纯的计算器，计数完了跳下一帧
            if (g_Animations[i].current_pattern >=
                g_Animations[i].pattern_count)
            {
                g_Animations[i].current_pattern = 0;
            }
            //循环完了数组后跳到第一帧
        }
    }
}

void FlipBookAnimation_Draw(
    int animation_id,
    float x,
    float y,
    float sizeX,
    float sizeY)
{
    FLIP_ANIMATION& anim =g_Animations[animation_id];

    int pattern = anim.current_pattern;

    int col =
        pattern % anim.pattern_column_count_max;

    int row =
        pattern / anim.pattern_column_count_max;

    Sprite_Draw(
        anim.texture_id,
        x,
        y,
        sizeX,
        sizeY,

        col * anim.pattern_width,
        row * anim.pattern_height,

        anim.pattern_width,
        anim.pattern_height,

        DirectX::XMFLOAT4(
            1.0f,
            1.0f,
            1.0f,
            1.0f));
}