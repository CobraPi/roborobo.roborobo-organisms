#ifndef GENE_HPP
#define GENE_HPP

class SymbrionOrganism;

class Gene
{
	public:
		Gene( int from, int dir, int to );

		friend class SymbrionOrganismGenome;
	protected:
		int _from;
		int _dir;
		int _to;
};

#endif
