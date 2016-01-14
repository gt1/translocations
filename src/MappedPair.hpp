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
#if ! defined(MAPPEDPAIR_HPP)
#define MAPPEDPAIR_HPP

#include <libmaus2/exception/LibMausException.hpp>
#include <libmaus2/math/IntegerInterval.hpp>

struct MappedPair
{
	libmaus2::math::IntegerInterval<int64_t> first;
	libmaus2::math::IntegerInterval<int64_t> second;
	uint64_t cnt;
	
	MappedPair() : cnt(0) {}
	MappedPair(
		libmaus2::math::IntegerInterval<int64_t> const & rfirst,
		libmaus2::math::IntegerInterval<int64_t> const & rsecond
	) : first(rfirst), second(rsecond), cnt(1) {}
	
	bool operator<(MappedPair const & O) const
	{
		if ( first.from != O.first.from )
			return first.from < O.first.from;
		else if ( second.from != O.second.from )
			return second.from < O.second.from;
		else if ( first.to != O.first.to )
			return first.to < O.first.to;
		else if ( second.to != O.second.to )
			return second.to < O.second.to;
		else
			return false;
	}
	
	// check whether A and B are within an insert size
	static bool isWithinInsertSize(
		libmaus2::math::IntegerInterval<int64_t> const & A,
		libmaus2::math::IntegerInterval<int64_t> const & B,
		int64_t const insertsize
	)
	{
		// make sure A starts on the left of B
		if ( B.from < A.from )
			return isWithinInsertSize(B,A,insertsize);
	
		// sanity check
		assert ( A.from <= B.from );
		
		// return true if the two intervals overlap
		if ( ! A.intersection(B).isEmpty() )
			return true;
		
		// sanity check, B must start after the end of A
		assert ( B.from > A.to );
		
		// return true if leftmost to rightmost mapped position is at most the insert size
		return B.to - A.from <= insertsize;
	}

	// check whether A and B are within an insertsize for both ends
	static bool isInsertSizeOverlap(MappedPair const & A, MappedPair const & B, int64_t const insertsize)
	{
		return
			(
				isWithinInsertSize(A.first,B.first,insertsize) 
				&&
				isWithinInsertSize(A.second,B.second,insertsize)
			);
	}

	static bool isInsertSizeOverlapEx(MappedPair const & A, MappedPair const & B, int64_t const insertsize)
	{
		return
			(
				isWithinInsertSize(A.first,B.first,insertsize) 
				&&
				isWithinInsertSize(A.second,B.second,insertsize)
			)
			||
			(
				isWithinInsertSize(A.first,B.second,insertsize) 
				&&
				isWithinInsertSize(A.second,B.first,insertsize)
			)
			;
	}
	
	MappedPair(std::istream & in)
	: first(0,0), second(0,0), cnt(0)
	{
		char const * magic = "MappedPair";
		
		int c = -1;
		while ( (*magic != 0) && (c = in.get()) != -1 )
		{
			if ( static_cast<char>(c) != *(magic++) )
			{
				libmaus2::exception::LibMausException lme;
				lme.getStream() << "Failed to parse MappedPair." << std::endl;
				lme.finish();
				throw lme;
			}			
		}
		
		if ( c == -1 )
		{
			libmaus2::exception::LibMausException lme;
			lme.getStream() << "Unexpected EOF while parsing MappedPair." << std::endl;
			lme.finish();
			throw lme;			
		}

		if ( (c=in.get()) != '(' )
		{
			libmaus2::exception::LibMausException lme;
			lme.getStream() << "Unexpected 1 symbol " << static_cast<char>(c) << " while parsing MappedPair." << std::endl;
			lme.finish();
			throw lme;			
		}

		if ( (c=in.get()) != '[' )
		{
			libmaus2::exception::LibMausException lme;
			lme.getStream() << "Unexpected 2 symbol " << static_cast<char>(c) << " while parsing MappedPair." << std::endl;
			lme.finish();
			throw lme;			
		}

		while ( ((c=in.peek()) != -1) && isdigit(c) )
		{
			c = in.get();
			first.from *= 10;
			first.from += (c-'0');
		}


		if ( (c=in.get()) != ',' )
		{
			libmaus2::exception::LibMausException lme;
			lme.getStream() << "Unexpected 3 symbol " << static_cast<char>(c) << " while parsing MappedPair." << std::endl;
			lme.finish();
			throw lme;			
		}

		while ( ((c=in.peek()) != -1) && isdigit(c) )
		{
			c = in.get();
			first.to *= 10;
			first.to += (c-'0');
		}

		if ( (c=in.get()) != ']' )
		{
			libmaus2::exception::LibMausException lme;
			lme.getStream() << "Unexpected symbol " << static_cast<char>(c) << " while parsing MappedPair." << std::endl;
			lme.finish();
			throw lme;			
		}

		if ( (c=in.get()) != ',' )
		{
			libmaus2::exception::LibMausException lme;
			lme.getStream() << "Unexpected symbol " << static_cast<char>(c) << " while parsing MappedPair." << std::endl;
			lme.finish();
			throw lme;			
		}

		if ( (c=in.get()) != '[' )
		{
			libmaus2::exception::LibMausException lme;
			lme.getStream() << "Unexpected symbol " << static_cast<char>(c) << " while parsing MappedPair." << std::endl;
			lme.finish();
			throw lme;			
		}

		while ( ((c=in.peek()) != -1) && isdigit(c) )
		{
			c = in.get();
			second.from *= 10;
			second.from += (c-'0');
		}

		if ( (c=in.get()) != ',' )
		{
			libmaus2::exception::LibMausException lme;
			lme.getStream() << "Unexpected symbol " << static_cast<char>(c) << " while parsing MappedPair." << std::endl;
			lme.finish();
			throw lme;			
		}

		while ( ((c=in.peek()) != -1) && isdigit(c) )
		{
			c = in.get();
			second.to *= 10;
			second.to += (c-'0');
		}

		if ( (c=in.get()) != ']' )
		{
			libmaus2::exception::LibMausException lme;
			lme.getStream() << "Unexpected symbol " << static_cast<char>(c) << " while parsing MappedPair." << std::endl;
			lme.finish();
			throw lme;			
		}

		if ( (c=in.get()) != ',' )
		{
			libmaus2::exception::LibMausException lme;
			lme.getStream() << "Unexpected symbol " << static_cast<char>(c) << " while parsing MappedPair." << std::endl;
			lme.finish();
			throw lme;			
		}

		while ( ((c=in.peek()) != -1) && isdigit(c) )
		{
			c = in.get();
			cnt *= 10;
			cnt += (c-'0');
		}

		if ( (c=in.get()) != ')' )
		{
			libmaus2::exception::LibMausException lme;
			lme.getStream() << "Unexpected symbol " << static_cast<char>(c) << " while parsing MappedPair." << std::endl;
			lme.finish();
			throw lme;			
		}
	}
};

std::ostream & operator<<(std::ostream & out, MappedPair const & MP);
#endif
