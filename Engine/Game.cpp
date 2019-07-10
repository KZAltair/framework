/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"
#include "SpriteEffect.h"

Game::Game( MainWindow& wnd )
	:
	wnd(wnd),
	gfx(wnd),
	brd(settings, gfx),
	rng(std::random_device()()),
	snek({ 2,2 }),
	nPoison(settings.GetPoisonAmount()),
	nFood(settings.GetFoodAmount()),
	snekSpeedupFactor(settings.GetSpeedupRate()),
	font(L"Customfont16x28.bmp")
{
	for (int i = 0; i < nPoison; i++)
	{
		brd.SpawnContents(rng, snek, Board::CellContents::Poison);
	}
	for (int i = 0; i < nFood; i++)
	{
		brd.SpawnContents(rng, snek, Board::CellContents::Food);
	}
	if (!gameIsStarted)
	{
		sndTitle.Play(1.0f, 1.0f);
	}
	if (gameIsOver)
	{
		sndTitle.Play(1.0f, 1.0f);
	}
	
}

void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	const float dt = ft.Mark();

	if (gameIsStarted)
	{
		if (!gameIsOver)
		{
			if (wnd.kbd.KeyIsPressed(VK_UP))
			{
				const Location new_delta_loc = { 0,-1 };
				if (delta_loc != -new_delta_loc || snek.GetLength() <= 2)
				{
					delta_loc = new_delta_loc;
				}
			}
			else if (wnd.kbd.KeyIsPressed(VK_DOWN))
			{
				const Location new_delta_loc = { 0,1 };
				if (delta_loc != -new_delta_loc || snek.GetLength() <= 2)
				{
					delta_loc = new_delta_loc;
				}
			}
			else if (wnd.kbd.KeyIsPressed(VK_LEFT))
			{
				const Location new_delta_loc = { -1,0 };
				if (delta_loc != -new_delta_loc || snek.GetLength() <= 2)
				{
					delta_loc = new_delta_loc;
				}
			}
			else if (wnd.kbd.KeyIsPressed(VK_RIGHT))
			{
				const Location new_delta_loc = { 1,0 };
				if (delta_loc != -new_delta_loc || snek.GetLength() <= 2)
				{
					delta_loc = new_delta_loc;
				}
			}

			float snekModifiedMovePeriod = snekMovePeriod;
			if (wnd.kbd.KeyIsPressed(VK_CONTROL))
			{
				snekModifiedMovePeriod = std::min(snekMovePeriod, snekMovePeriodSpeedup);
			}

			snekMoveCounter += dt;
			if (snekMoveCounter >= snekModifiedMovePeriod)
			{
				snekMoveCounter -= snekModifiedMovePeriod;
				const Location next = snek.GetNextHeadLocation(delta_loc);
				const Board::CellContents contents = brd.IsInsideBoard(next) ? brd.GetContents(next)
					: Board::CellContents::Empty;
				if (!brd.IsInsideBoard(next) ||
					snek.IsInTileExceptEnd(next) ||
					contents == Board::CellContents::Obstacle)
				{
					gameIsOver = true;
					sndFart.Play(rng, 1.2f);
					sndMusic.StopAll();
					sndTitle.Play(1.0f, 1.0f);
				}
				else if (contents == Board::CellContents::Food)
				{
					snek.GrowAndMoveBy(delta_loc);
					brd.ConsumeContents(next);
					brd.SpawnContents(rng, snek, Board::CellContents::Obstacle);
					brd.SpawnContents(rng, snek, Board::CellContents::Food);
					sfxEat.Play(rng, 0.8f);
					++points;
				}
				else if (contents == Board::CellContents::Poison)
				{
					snek.MoveBy(delta_loc);
					brd.ConsumeContents(next);
					snekMovePeriod = std::max(snekMovePeriod * snekSpeedupFactor, snekMovePeriodMin);
					sndFart.Play(rng, 0.6f);
				}
				else
				{
					snek.MoveBy(delta_loc);
					sfxSlither.Play(rng, 0.08f);
				}
			}
		}
	}
	else
	{
		if (wnd.kbd.KeyIsPressed(VK_RETURN))
		{
			sndTitle.StopAll();
			sndMusic.Play(1.0f, 0.6f);
			gameIsStarted = true;
		}
	}
	if (gameIsOver)
	{
		sndMusic.StopAll();
		if (wnd.kbd.KeyIsPressed(VK_SPACE))
		{
			gameIsOver = false;
			gameIsStarted = false;
			rng = std::mt19937(std::random_device()());
			brd.ClearBoard(settings, rng, snek);
			snek = Snake({ 2,2 });
			delta_loc = Location{ 1,0 };
			snekSpeedupFactor = settings.GetSpeedupRate();
			snekMovePeriod = 0.4f;
			snekMoveCounter = 0.0f;
			points = 0;
		}
	}
}

void Game::ComposeFrame()
{
	if (gameIsStarted)
	{
		gfx.DrawSprite(0, 0, background);
		snek.Draw(brd);
		brd.DrawCells(apple);
		brd.DrawBorder();
		font.DrawText(std::to_string(points), { 700, 30 }, Colors::White, gfx);
		if (gameIsOver)
		{
			gfx.DrawSprite(0, 0, over);
		}
	}
	else
	{
		gfx.DrawSprite(0, 0, title);
	}
}
