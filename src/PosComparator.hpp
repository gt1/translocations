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
#if ! defined(POSCOMPARATOR_HPP)
#define POSCOMPARATOR_HPP

#include <libmaus2/bambam/BamAlignment.hpp>

struct PosComparator
{
	static bool compare(libmaus2::bambam::BamAlignment const * A, libmaus2::bambam::BamAlignment const * B)
	{
		if ( A->getRefID() != B->getRefID() )
			return static_cast<uint32_t>(A->getRefID()) < static_cast<uint32_t>(B->getRefID());
		else if ( A->getPos() != B->getPos() )
			return A->getPos() < B->getPos();
		else if ( A->isRead1() != B->isRead1() )
			return A->isRead1();
		else
			return false;
	}

	bool operator()(libmaus2::bambam::BamAlignment const * A, libmaus2::bambam::BamAlignment const * B) const
	{
		return compare(A,B);
	}
};
#endif
