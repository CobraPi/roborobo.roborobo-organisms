#ifndef GENE_HPP
#define GENE_HPP

class Gene
{
	public:
		Gene( int from, dir, to );
	protected:
		int m_from;
		int m_dir;
		int m_to;
		int m_x;
		int m_y;
};
