#ifndef ANIMATION_H
#define ANIMATION_H

#include "Common.h"


typedef struct Animation
{
	sfSprite* sprite;
	unsigned frameSpeed;
	unsigned frameCount;
	unsigned currentFrame;
	float timer;
	sfIntRect firstFrame;
	sfBool isPlaying;
	sfBool isLooping;

}Animation;


Animation CreateAnimation(sfSprite* _sprite, unsigned _frameCount, unsigned _frameSpeed, float _timer, sfBool _isPlaying, sfBool _isLooping);
void UpdateAnimation(Animation* _animation, float _dt);


#endif 