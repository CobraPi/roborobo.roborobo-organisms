#include "Symbrion/include/SymbrionOrganismGenome.h"
#include "Symbrion/include/SymbrionSharedData.h"

#include "Coord2d.h"

#include <iostream>
#include <cstdlib>

int SymbrionOrganismGenome::_counter = 0;

SymbrionOrganismGenome::SymbrionOrganismGenome( SymbrionAgentWorldModel* wm, int parent1, int parent2 )
{
	_wm = wm;
	_id = SymbrionOrganismGenome::_counter;
	SymbrionOrganismGenome::_counter++;
	
	_parent1 = -1;
	_parent2 = -1;
	
	_size = rand() % (SymbrionSharedData::maxOrganismSize - SymbrionSharedData::minOrganismSize) + SymbrionSharedData::minOrganismSize;
	
	int next_from = -1;
	
	std::map<int, Coord2d>::iterator it;
	
	for( int i = 1; i <= _size; i++ )
	{
		int from = i;
		int dir = rand() % 4;
		int to = i + 1;
		
		if( next_from != -1 )
		{
			from = next_from;
		}
		
		Gene gene( from, dir, to );
		Coord2d parent_coord;
		Coord2d coord;
		
		// if adding the first we can just add to the coordinates else we need to find the coords of the from and check collisions
		if( i == 1 )
		{
			
			parent_coord._x = 0;
			parent_coord._y = 0;
			_coords[from] = parent_coord;
			switch (dir )
			{
				case 0:
					coord._x = parent_coord._x - 1;
					coord._y = parent_coord._y;
					break;
				case 1:
					coord._x = parent_coord._x;
					coord._y = parent_coord._y + 1;
					break;
				case 2:
					coord._x = parent_coord._x + 1;
					coord._y = parent_coord._y;
					break;
				case 3:
					coord._x = parent_coord._x;
					coord._y = parent_coord._y - 1;
					break;
			}
			
			_coords[to] = coord;
			_genome.push_back( gene );
		}
		else
		{
			it = _coords.find( from );
			if( it != _coords.end() )
			{
				parent_coord = it->second;
				
				switch (dir )
				{
					case 0:
						coord._x = parent_coord._x - 1;
						coord._y = parent_coord._y;
						break;
					case 1:
						coord._x = parent_coord._x;
						coord._y = parent_coord._y + 1;
						break;
					case 2:
						coord._x = parent_coord._x + 1;
						coord._y = parent_coord._y;
						break;
					case 3:
						coord._x = parent_coord._x;
						coord._y = parent_coord._y - 1;
						break;
				}
			}
			else
			{
				std::cerr << "Could not find parent2\n";
			}
			int collision = checkOverlapping( coord );
			
			if( collision != -1 )
			{
				next_from = collision;
				i--;
			}
			else
			{
				next_from = -1;
				_coords[to] = coord;
				_genome.push_back( gene );
			}
		}
	}
	
	this->_index = _coords.begin();
	this->_nr_of_extermities = 0;
	this->_nr_of_joints = 0;
	//generateCoords();
}

SymbrionOrganismGenome::SymbrionOrganismGenome(SymbrionAgentWorldModel* wm, SymbrionOrganismGenome *other){
    _wm = wm;
    _parent1 = other->_parent1;
    _parent2 = other->_parent2;
	
	_genome.clear();
	_coords.clear();
	
	for( unsigned int i = 0; i < other->_genome.size(); i++ )
	{
        _genome.push_back(other->_genome.at(i));
	}
    
	std::map<int,Coord2d>::iterator it;
	for( it = other->_coords.begin(); it != other->_coords.end(); it++ )
	{
		_coords[it->first] = it->second;
	}
}

SymbrionOrganismGenome* SymbrionOrganismGenome::crossover( SymbrionOrganismGenome* second )
{	
	SymbrionOrganismGenome* result = new SymbrionOrganismGenome( this->_wm, this->_id, second->_id );
	
	return result;
}

SymbrionOrganismGenome* SymbrionOrganismGenome::mutate()
{
	SymbrionOrganismGenome* result = new SymbrionOrganismGenome( this->_wm, this->_id, -1 );
	result->_parent1 = this->_id; 
	
	result->_genome.clear();
	result->_coords.clear();
	
	for( unsigned int i = 0; i < _genome.size(); i++ )
	{
		result->_genome.push_back(_genome.at(i));
	}

	std::map<int,Coord2d>::iterator it;
	for( it = _coords.begin(); it != _coords.end(); it++ )
	{
		result->_coords[it->first] = it->second;
	}
	
	int type = rand() % 2; //0=add, 1=remove
	
	if( type == 0 )
	{
		result->add();
	}
	if( type == 1 )
	{
		result->remove();
	}
    
    result->_index = result->_coords.begin();
    
	return result;
}

std::map<int,Coord2d> SymbrionOrganismGenome::getCoords()
{
	return _coords;
}

bool SymbrionOrganismGenome::isComplete()
{
	if( this->_index != _coords.end())
		return false;

	return true;
}

int SymbrionOrganismGenome::size(){
    return this->_genome.size();
}

int SymbrionOrganismGenome::coordSize(){
    return this->_coords.size();
}

Coord2d SymbrionOrganismGenome::current()
{
	return this->_index->second;
}

void SymbrionOrganismGenome::append()
{
    if(!isComplete()){
        this->_index++;
    }else{
        std::cerr << "Error: cannot append to completed organism: agent: " << this->_wm->_agentId << " index: " << this->_index->first << " of " << _coords.size() << std::endl;
    }
}

int SymbrionOrganismGenome::checkOverlapping(Coord2d coord)
{	
	std::map<int,Coord2d>::iterator it;
	for( it = _coords.begin(); it != _coords.end(); it++ )
	{
		if( coord._x == it->second._x && coord._y == it->second._y )
		{
			return it->first;
		}
	}
	
	return -1;
}

void SymbrionOrganismGenome::print()
{
	std::cerr << _id << ",";
	std::cerr << _parent1 << ",";
	std::cerr << _parent2 << ",";
	std::cerr << _genome.size() << ",";
	for( unsigned int i = 0; i < _genome.size(); i++ )
	{
		std::cerr << _genome.at(i)._from << " " << _genome.at(i)._dir << " " << _genome.at(i)._to << " ";
	}
	std::cerr << "\n";
}

void SymbrionOrganismGenome::log()
{
	SymbrionSharedData::gGenomesOrgLogFile << _wm->_agentId << ",";
	SymbrionSharedData::gGenomesOrgLogFile << _id << ",";
	SymbrionSharedData::gGenomesOrgLogFile << _parent1 << ",";
	SymbrionSharedData::gGenomesOrgLogFile << _parent2 << ",";
	SymbrionSharedData::gGenomesOrgLogFile << _size<< ",";
	SymbrionSharedData::gGenomesOrgLogFile << this->_nr_of_extermities << ",";
	SymbrionSharedData::gGenomesOrgLogFile << this->_nr_of_joints << ",";
	
	for( unsigned int i = 0; i < _genome.size(); i++ )
	{
		SymbrionSharedData::gGenomesOrgLogFile << _genome.at(i)._from << " " << _genome.at(i)._dir << " " << _genome.at(i)._to << " ";
	}
	SymbrionSharedData::gGenomesOrgLogFile << "\n";
}

void SymbrionOrganismGenome::add()
{
	if( _coords.size() >= SymbrionSharedData::maxOrganismSize )
		return;
	
	std::map<int, Coord2d>::iterator it;
	int collision;
	
	do
	{
		int from = rand() % _coords.size() + 1;
		int dir = rand() % 4;
		int to = _coords.size() + 1;
		
		Gene gene( from, dir, to );
		
		//std::cerr << "adding: " << from << " dir " << dir << " to " << to << "\n";
		
		Coord2d coord;
		Coord2d parent_coord;
		
		it = _coords.find( from );
		if( it != _coords.end() )
		{
			parent_coord = it->second;
			
			switch (dir )
			{
				case 0:
					coord._x = parent_coord._x - 1;
					coord._y = parent_coord._y;
					break;
				case 1:
					coord._x = parent_coord._x;
					coord._y = parent_coord._y + 1;
					break;
				case 2:
					coord._x = parent_coord._x + 1;
					coord._y = parent_coord._y;
					break;
				case 3:
					coord._x = parent_coord._x;
					coord._y = parent_coord._y - 1;
					break;
			}
			
			collision = checkOverlapping( coord );
			
			if( collision != -1 )
			{
				//std::cerr << "Collision\n";
			}
			else
			{
				_coords[to] = coord;
				_genome.push_back( gene );
			}
		}
	} while( collision != -1 );
}

//removes last organism
void SymbrionOrganismGenome::remove()
{
	if( _coords.size() <= SymbrionSharedData::minOrganismSize )
	{
		return;
	}

	_genome.pop_back();
	std::map<int,Coord2d>::iterator it;
	it = _coords.end();
	it--;
	_coords.erase( it );
}
