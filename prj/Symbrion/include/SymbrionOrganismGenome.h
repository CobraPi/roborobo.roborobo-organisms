#ifndef SYMBRIONORGANISMGENOME_H
#define SYMBRIONORGANISMGENOME_H

#include <cstdlib>
#include <vector>
#include <ostream>

#include "Coord2d.h"
#include "Symbrion/include/Gene.h"
#include "Symbrion/include/SymbrionAgentWorldModel.h"

class SymbrionOrganismGenome
{
	public:
		SymbrionOrganismGenome( SymbrionAgentWorldModel* wm, int parent1 = 0, int parent2 = 0 );
        SymbrionOrganismGenome( SymbrionAgentWorldModel* wm, SymbrionOrganismGenome *other );
		SymbrionOrganismGenome* crossover( SymbrionOrganismGenome* second );
		SymbrionOrganismGenome* mutate();
		
		std::map<int, Coord2d> getCoords();
		bool isComplete();
		int size();
        int coordSize();
		
		//bool isValid();
		
		Coord2d current();
		void append();
		
		int getId() { return _id; };
		
		void print();
		void log();
	protected:
		SymbrionAgentWorldModel* _wm;
		
		int _id;
		int _parent1;
		int _parent2;
		
		int _size;
		
		std::vector<Gene> _genome;
		std::map<int,Coord2d> _coords;
		
		std::map<int,Coord2d>::iterator _index;
		int _nr_of_extermities;
		int _nr_of_joints;
		
		static int _counter;
		
		void generateCoords();
		
		int checkOverlapping(Coord2d coord);
		
		//mutate helpers
		void add();
		void remove();
		void modify();
};

#endif
