#include "WorldGrid.h"
#include "MoveTypes.h"

const int WorldGrid::gridSize = 32;

WorldGrid::WorldGrid()
{

}

bool WorldGrid::isPositionOccupied(int row, int col)
{
	int hash = hashPosition(row, col);

	//note that bucks are for hash collisions
	auto bucketPair = spatialHash.find(hash);
	if (bucketPair != spatialHash.end())
	{
		auto& bucket = bucketPair->second;
		for (auto& loc : bucket)
		{
			if (loc.row == row && loc.col == col)
			{
				return true;
			}
		}
	}
	return false;
}

int WorldGrid::hashPosition(int row, int col)
{
	int prime = 7;

	//super simple polynomial hash
	int hash = (row * prime) + (col * prime * prime);
	return hash;
}

bool WorldGrid::insert(int row, int col)
{
	bool bInserted = false;
	int hash = hashPosition(row, col);

	//insert should allow collisions, equality checks will be used.
	auto bucketPair = spatialHash.find(hash);
	if (bucketPair == spatialHash.end())
	{
		//create new bucket with the first grid
		spatialHash.emplace(hash, std::list<GridBlock>{ GridBlock{ row, col } });
		bInserted = true;
	}
	else
	{
		auto& bucket = bucketPair->second;
		bucket.push_back(GridBlock{ row, col });
		bInserted = true;
	}
	//if (bInserted) std::cerr << "claimed: " << row << " " << col << std::endl;

	return bInserted;
}

bool WorldGrid::claimPosition(int row, int col)
{
	if (!isPositionOccupied(row, col))
	{
		return insert(row, col);
	}

	return false;
}

bool WorldGrid::leavePosition(int row, int col)
{
	//there will be some extra copying of target, but that is okay since it is POD.
	GridBlock target{ row, col };
	int hash = hashPosition(row, col);

	auto bucketPair = spatialHash.find(hash);
	if (bucketPair != spatialHash.end())
	{
		auto& bucket = bucketPair->second;

		//extra O(n) but these buckets should be small. I want to know if we're actually removing something.
		auto found = std::find(bucket.begin(), bucket.end(), target);
		if (found != bucket.end())
		{
			bucket.remove(target);
			if (bucket.size() == 0)
			{
				spatialHash.erase(hash);
			}
		}
		else
		{
			std::cerr << "spatial hash remove request failed. " << row << " " << col << " is not in the map's bucket." << std::endl;
		}
		return true;
	}
	else
	{
		std::cerr << "spatial hash remove request failed. " << row << " " << col << " does not have an associated bucket." << std::endl;
	}
	return false;
}

int WorldGrid::hashGrid(sf::Vector2i loc)
{
	int col = Utils::floatToGridLoc((float)loc.x);
	int row = Utils::floatToGridLoc((float)loc.y);
	return hashPosition(row, col);
}
