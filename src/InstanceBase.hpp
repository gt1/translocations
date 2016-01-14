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
#if ! defined(INSTANCEBASE_HPP)
#define INSTANCEBASE_HPP

#include <string>
#include <libmaus2/exception/LibMausException.hpp>

struct InstanceBase
{
	// type bit masks
	enum instance_type
	{
		instance_type_improper   = (1ull << 0),
		instance_type_orphan     = (1ull << 1),
		instance_type_unmapped   = (1ull << 2),
		instance_type_split      = (1ull << 3),
		instance_type_samestrand = (1ull << 4),
		instance_type_proper     = (1ull << 5)
	};
	
	// get full mask
	static uint64_t getFullMask()
	{
		return 
			instance_type_improper |
			instance_type_orphan |
			instance_type_unmapped |
			instance_type_split |
			instance_type_samestrand |
			instance_type_proper;
	}
	
	// parse instance type set from string
	static uint64_t parseInstanceTypes(std::string const & s)
	{
		uint64_t l = 0;
		uint64_t m = 0;
		
		while ( l != s.size() )
		{
			uint64_t h = l;
			
			while ( h != s.size() && s[h] != ',' )
				++h;
							
			m |= parseInstanceType(s.substr(l,h-l));
			
			if ( h == s.size() )
				l = h;
			else
				l = h+1;
		}
		
		return m;
	}

	// parse single instance type from string
	static instance_type parseInstanceType(std::string const & s)
	{
		if ( s == "improper" )
			return instance_type_improper;
		else if ( s == "unmapped" )
			return instance_type_unmapped;
		else if ( s == "split" )
			return instance_type_split;
		else if ( s == "samestrand" )
			return instance_type_samestrand;
		else if ( s == "proper" )
			return instance_type_proper;
		else
		{
			libmaus2::exception::LibMausException lme;
			lme.getStream() << "Unknown type " << s << std::endl;
			lme.finish();
			throw lme;
		}
	}
};

#include <ostream>

std::ostream & operator<<(std::ostream & out, InstanceBase::instance_type const type);
#endif
