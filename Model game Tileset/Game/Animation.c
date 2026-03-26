#include "Animation.h"

Animation CreateAnimation(sfSprite* _sprite, unsigned _frameCount, unsigned _frameSpeed, sfBool _isPlaying, sfBool _isLooping, sfIntRect _firstFrame)
{
    Animation createAnim = { 0 };

    createAnim.sprite = _sprite;
    createAnim.frameCount = _frameCount;
    createAnim.frameSpeed = _frameSpeed;
    createAnim.isPlaying = _isPlaying;
    createAnim.isLooping = _isLooping;
    createAnim.firstFrame = _firstFrame;

    return createAnim;
}



void UpdateAnimation(Animation* _animation, float _dt)
{
    // _animation->isPlaying = sfTrue;
    if (_animation->isPlaying)
    {
        _animation->timer += _dt;

        if (_animation->timer > 1.f / _animation->frameSpeed)
        {
            _animation->timer -= 1.f / _animation->frameSpeed;
            _animation->currentFrame++;

            if (_animation->currentFrame == _animation->frameCount)
            {
                if (_animation->isLooping)
                {
                    _animation->currentFrame = 0;
                    //  _animation->firstFrame.left = 0;
                }
                else
                {
                    _animation->currentFrame--;
                    _animation->isPlaying = sfFalse;
                }


            }


        }


    }

    sfIntRect frame = _animation->firstFrame;
    frame.left = frame.left + _animation->currentFrame * frame.width;
    sfSprite_setTextureRect(_animation->sprite, frame);

}

void LoadAnimation(Animation* _animation)
{
    _animation->timer = 0.f;
    _animation->isPlaying = sfTrue;
    _animation->currentFrame = 0;

}