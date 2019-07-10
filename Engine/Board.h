#pragma once

#include "Graphics.h"
#include "Location.h"
#include <random>
#include "GameSettings.h"
#include <vector>

class Board
{
public:
	enum class CellContents
	{
		Empty,
		Obstacle,
		Food,
		Poison
	};
public:
	Board(const GameSettings& settings, Graphics& gfx);
	void DrawCell(const Location & loc, Color c);
	void DrawCell(const Location & loc, const Surface& surf);
	int GetGridWidth() const;
	int GetGridHeight() const;
	bool IsInsideBoard(const Location& loc) const;
	CellContents GetContents(const Location& loc) const;
	void ConsumeContents(const Location& loc);
	void SpawnContents(std::mt19937& rng, const class Snake& snake, CellContents contents);
	void DrawBorder();
	void DrawCells(const Surface& surf);
private:
	static constexpr Color borderColor = Colors::Blue;
	//static constexpr Color obstacleColor = Colors::Gray;
	Surface obstacleImage = L"obstacle.png";
	Surface poisonImage = L"poison.png";
	//static constexpr Color poisonColor = { 100,8,64 };
	//static constexpr Color foodColor = Colors::Red;
	int dimension;
	static constexpr int cellPadding = 1;
	int width;
	int height;
	static constexpr int borderWidth = 4;
	static constexpr int borderPadding = 2;
	static constexpr int x = 70;
	static constexpr int y = 50;
	std::vector<CellContents> contents;
	Graphics& gfx;
};