#include "Animation.h"

Animation CreateAnimation(sfSprite* _sprite, unsigned _frameCount, unsigned _frameSpeed, float _timer, sfBool _isPlaying, sfBool _isLooping)
{
    Animation createAnim = { 0 };

    createAnim.sprite = _sprite;
    createAnim.frameCount = _frameCount;
    createAnim.frameSpeed = _frameSpeed;
    createAnim.timer = _timer;
    createAnim.isPlaying = _isPlaying;
    createAnim.isLooping = _isLooping;

    return createAnim;
}



void UpdateAnimation(Animation* _animation, float _dt)
{
    if (_animation->isPlaying)
    {
        _animation->timer += _dt;

        if (_animation->timer > 1.f / _animation->frameSpeed)
        {
            _animation->timer -= (1.f / _animation->frameSpeed);
            _animation->currentFrame++;

            if (_animation->currentFrame == _animation->frameCount)
            {
                if (_animation->isLooping)
                {
                    _animation->currentFrame = 0;
                }
                else
                {
                    _animation->isPlaying = sfFalse;
                    _animation->currentFrame--;
                }


            }


        }


    }

    sfFloatRect frame = sfSprite_getGlobalBounds(_animation->sprite);
    frame.left = frame.width * _animation->currentFrame;
    sfSprite_setTextureRect(_animation->sprite, (sfIntRect) { frame.left, frame.width, frame.top, frame.height});
}
