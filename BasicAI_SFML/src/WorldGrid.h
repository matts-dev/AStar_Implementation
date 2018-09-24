#pragma once
#include <unordered_map>
#include <list>
#include <algorithm>
#include <iostream>
#include <SFML/Graphics.hpp>

struct GridBlock
{
	int row;
	int col;

	bool operator==(const GridBlock& other)
	{
		return (row == other.row) && (col == other.col);
	}
};

/** Spatial hash to represent a world grid*/
class WorldGrid
{
	/** In real applications this class should probably be more secure by allowing an object/address to claim a position. Right now, anyone can cause the removal of an occupied position.*/
private://fields
	std::unordered_map<int, std::list<GridBlock>> spatialHash;

private: //methods
	static int hashPosition(int row, int col);
	bool insert(int row, int col);

	WorldGrid(const WorldGrid& copy) = delete;
	WorldGrid(WorldGrid&& move) = delete;
	WorldGrid& operator=(const WorldGrid& copy) = delete;
	WorldGrid& operator=(WorldGrid&& move) = delete;

public: //constants
	static const int gridSize;

public: //api
	explicit WorldGrid();
	bool isPositionOccupied(int row, int col);
	bool claimPosition(int row, int col);
	bool leavePosition(int row, int col);
	static int hashGrid(sf::Vector2i loc);
};