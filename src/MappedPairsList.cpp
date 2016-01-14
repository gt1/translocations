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
#include <MappedPairsList.hpp>

std::ostream & operator<<(std::ostream & out, MappedPairsList const & MPL)
{
	out << '\t' << MPL.inst << '\t' << MPL.num;
	
	for ( uint64_t i = 0; i < MPL.MPL.size(); ++i )
		out << '\t' << MPL.MPL[i];
		
	return out;
}
