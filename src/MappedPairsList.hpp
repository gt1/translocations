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
#if ! defined(MAPPEDPAIRSLIST_HPP)
#define MAPPEDPAIRSLIST_HPP

#include <Instance.hpp>
#include <MappedPair.hpp>

struct MappedPairsList
{
	Instance inst;
	// maximum breakpoint support
	uint64_t num;
	std::vector<MappedPair> MPL;
	
	/*
	 * filter breakpoint with at least thres supporting read pairs
	 * return number of remaining break points
	 */
	uint64_t filterMappedPairs(uint64_t const thres)
	{
		uint64_t o = 0;
		for ( uint64_t i = 0; i < MPL.size(); ++i )
			if ( MPL[i].cnt >= thres )
				MPL[o++] = MPL[i];
				
		MPL.resize(o);
		
		if ( MPL.size() )
			num = MPL.front().cnt;
		else
			num = 0;
			
		return o;
	}

	// constructor	
	MappedPairsList() {}
	// constructor from input stream
	MappedPairsList(std::istream & in)
	{
		if ( in.peek() != '\t' )
		{
			libmaus2::exception::LibMausException lme;
			lme.getStream() << "MappedPairsList: expected tab not found." << std::endl;
			lme.finish();
			throw lme;
		}
		
		in.get();
		
		// get instance
		inst = Instance(in);

		if ( in.peek() != '\t' )
		{
			libmaus2::exception::LibMausException lme;
			lme.getStream() << "MappedPairsList: expected tab not found." << std::endl;
			lme.finish();
			throw lme;
		}
		
		in.get();
		
		// get maximum breakpoint support
		num = 0;
		while ( in.peek() >= 0 && isdigit(in.peek()) )
		{
			int c = in.get();
			num *= 10;
			num += c-'0';
		}
		
		// read breakpoints
		while ( in.peek() >= 0 && in.peek() == '\t' )
		{
			in.get();
			
			MappedPair MP(in);
			MPL.push_back(MP);
		}
		
		// read newline
		while ( in.peek() >= 0 && (in.peek() == '\r' || in.peek() == '\n') )
			in.get();
	}
};

std::ostream & operator<<(std::ostream & out, MappedPairsList const & MPL);
#endif
