/*
    translocations
    Copyright (C) 2014-2016 German Tischler
    Copyright (C) 2014-2015 Genome Research Limited

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#if ! defined(EVIDENCELIST_HPP)
#define EVIDENCELIST_HPP

#include <InstanceLine.hpp>
#include <MappedPairsList.hpp>

struct EvidenceList
{
	InstanceLine instline;
	std::vector<MappedPairsList> MPL;

	// filter break points keeping such with at least thres supporting read pairs
	bool filterMappedPairs(uint64_t const thres)
	{
		std::set<Instance> keepset;
		uint64_t o = 0;
		for ( uint64_t i = 0; i < MPL.size(); ++i )
			if ( MPL[i].filterMappedPairs(thres) )
			{
				keepset.insert(MPL[i].inst);
				MPL[o++] = MPL[i];
			}
		MPL.resize(o);
			
		return instline.filterInstances(keepset);
	}
	
	bool filterInstances(uint64_t const thres) const
	{
		return MPL.size() <= thres;
	}
	
	// filter for one gene pair
	bool filterGenePair(std::string const & nameA, std::string const & nameB) const
	{
		return
			(instline.geneA.name == nameA && instline.geneB.name == nameB)
			||
			(instline.geneA.name == nameB && instline.geneB.name == nameA);
	}
	
	EvidenceList() {}
	EvidenceList(std::istream & in)
	{
		// get instance line
		instline = InstanceLine(0,in);

		// get mapped pair lines
		while ( in.peek() >= 0 && in.peek() == '\t' )
		{
			MappedPairsList MPLi(in);
			MPL.push_back(MPLi);
		}			
	}
};

std::ostream & operator<<(std::ostream & out, EvidenceList const & EL);
#endif
