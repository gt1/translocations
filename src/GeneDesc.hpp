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
#if ! defined(GENEDESC_HPP)
#define GENEDESC_HPP

#include <string>
#include <istream>
#include <sstream>
#include <libmaus2/exception/LibMausException.hpp>
#include <libmaus2/math/IntegerInterval.hpp>

// gene description
struct GeneDesc
{
	// gene name
	std::string name;
	// chromosome name
	std::string chr;
	// gene start on chromosome
	uint64_t start;
	// gene end on chromosome
	uint64_t end;
	
	// get start,end interval
	libmaus2::math::IntegerInterval<int64_t> getInterval() const
	{
		return libmaus2::math::IntegerInterval<int64_t>(start,end);
	}
	
	// check whether this description overlaps with another description
	bool overlaps(GeneDesc const & other) const
	{
		if ( chr != other.chr )
			return false;
			
		return ! ( getInterval().intersection(other.getInterval()).isEmpty() );
	}
	
	// constructor
	GeneDesc() : start(0), end(0) {}
	// constructor from input stream
	GeneDesc(std::istream & in)
	{
		int c = -1;
		
		c = in.get();
		if ( c < 0 || c != '(' )
		{
			libmaus2::exception::LibMausException lme;
			lme.getStream() << "Malformed gene description" << std::endl;
			lme.finish();
			throw lme;		
		}
		
		std::vector<char> namev;
		while ( (c=in.get()) >= 0 && c != ',' )
			namev.push_back(c);

		if ( c < 0 || c != ',' )
		{
			libmaus2::exception::LibMausException lme;
			lme.getStream() << "Malformed gene description" << std::endl;
			lme.finish();
			throw lme;		
		}
		name = std::string(namev.begin(),namev.end());

		std::vector<char> chrv;
		while ( (c=in.get()) >= 0 && c != ',' )
			chrv.push_back(c);

		if ( c < 0 || c != ',' )
		{
			libmaus2::exception::LibMausException lme;
			lme.getStream() << "Malformed gene description" << std::endl;
			lme.finish();
			throw lme;		
		}
		chr = std::string(chrv.begin(),chrv.end());

		start = 0;
		while ( (c=in.get()) != -1 && isdigit(c) )
		{
			start *= 10;
			start += c-'0';	
		}
		
		if ( c != ',' )
		{
			libmaus2::exception::LibMausException lme;
			lme.getStream() << "Malformed instance" << std::endl;
			lme.finish();
			throw lme;
		}

		end = 0;
		while ( (c=in.get()) != -1 && isdigit(c) )
		{
			end *= 10;
			end += c-'0';	
		}
		
		if ( c != ')' )
		{
			libmaus2::exception::LibMausException lme;
			lme.getStream() << "Malformed instance" << std::endl;
			lme.finish();
			throw lme;
		}
	}

	// get range in samtools notation (chr:start-end)	
	std::string getRange() const
	{
		std::ostringstream ostr;
		ostr << chr << ":" << start << "-" << end;
		return ostr.str();
	}
};

std::ostream & operator<<(std::ostream & out, GeneDesc const & GD);
#endif
