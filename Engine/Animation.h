#pragma once

#include "Surface.h"
#include "Graphics.h"
#include "SpriteEffect.h"
#include <vector>

class Animation
{
public:
	Animation(int x, int y, int width, int height, int count, const Surface& sprite, float holdTime);
	void Draw(const Vei2& pos, Graphics& gfx) const;
	void Draw(const Vei2& pos, Graphics& gfx, const RectI& clip) const;
	void Update(float dt);
private:
	void Advance();
private:
	const Surface& sprite;
	std::vector<RectI> frames;
	int iCurFrame = 0;
	float holdTime;
	float curFrameTime = 0.0f;

};