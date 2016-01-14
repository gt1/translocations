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
#if ! defined(SUBINSTKEY_HPP)
#define SUBINSTKEY_HPP

#include <libmaus2/types/types.hpp>

struct SubInstKey
{
	uint64_t maxcnt;
	uint64_t num;
	
	SubInstKey()
	: maxcnt(0), num(0)
	{
		
	}
	SubInstKey(uint64_t const rmaxcnt, uint64_t const rnum)
	: maxcnt(rmaxcnt), num(rnum)
	{
	
	}
	
	bool operator<(SubInstKey const & O) const
	{
		if ( maxcnt != O.maxcnt )
			return maxcnt < O.maxcnt;
		else
			return num < O.num;
	}

	bool operator>(SubInstKey const & O) const
	{
		if ( maxcnt != O.maxcnt )
			return maxcnt > O.maxcnt;
		else
			return num > O.num;
	}
};
#endif
