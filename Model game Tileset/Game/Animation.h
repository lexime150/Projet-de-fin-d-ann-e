//#ifndef ANIMATION_H
//#define ANIMATION_H
//
//#include "Common.h"
//
//#define ANIMATION_FRAME_RATE 8
//
//typedef struct Animation
//{
//	sfSprite* sprite;
//	sfIntRect firstFrame;
//
//	unsigned int frameNumber;
//	unsigned int frameCount;
//	unsigned int frameRate;
//
//	float timer;
//
//	sfBool isLooping;
//	sfBool isFinished;
//}Animation;
//
//Animation CreateAnimation(sfSprite* const _sprite, unsigned _frameCount, unsigned _frameRate, sfIntRect _firstFrame, sfBool _isLooping);
//void UpdateAnimation(Animation* const _animation, float _dt);
//
//void LoadAnimation(Animation* const _animation);
//sfBool AnimationIsFinished(const Animation* const _animation);
//
//#endif