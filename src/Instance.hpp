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
#if ! defined(INSTANCE_HPP)
#define INSTANCE_HPP

#include <InstanceBase.hpp>
#include <string>
#include <istream>
#include <libmaus2/exception/LibMausException.hpp>

struct Instance : public InstanceBase
{
	// sequencing meta information run, lane and tag
	unsigned int run;
	unsigned int lane;
	unsigned int tag;
	// type prefix
	std::string pre;
	// type
	instance_type type;
	// number of reads supporting instance
	unsigned int num;

	// constructor
	Instance()
	: run(0), lane(0), tag(0), type(instance_type_proper), num(0)
	{
	
	}
	
	// check whether string s ends on suffix
	static bool endsOn(std::string const & s, std::string const & suffix)
	{
		return
			s.size() >= suffix.size() &&
			s.substr(s.size()-suffix.size()) == suffix;
	}
	
	// compare to another instance by type, run, lane and tag
	bool operator<(Instance const & I) const
	{
		if ( type != I.type )
			return type < I.type;
		else if ( run != I.run )
			return run < I.run;
		else if ( lane != I.lane )
			return lane < I.lane;
		else if ( tag != I.tag )
			return tag < I.tag;
		else	
			return false;
	}
	
	// check for equality
	bool operator==(Instance const & I) const
	{
		return !((*this < I) || (I < *this));
	}
		
	// initialise from stream
	Instance(std::istream & in)
	{
		int c = -1;
		
		run = 0;
		while ( (c=in.get()) != -1 && isdigit(c) )
		{
			run *= 10;
			run += c-'0';	
		}
		
		if ( c != ':' )
		{
			libmaus2::exception::LibMausException lme;
			lme.getStream() << "Malformed instance (no colon after run id)" << std::endl;
			lme.finish();
			throw lme;
		}

		lane = 0;
		while ( (c=in.get()) != -1 && isdigit(c) )
		{
			lane *= 10;
			lane += c-'0';	
		}
		
		if ( c != '#' )
		{
			libmaus2::exception::LibMausException lme;
			lme.getStream() << "Malformed instance (no hash after lane)" << std::endl;
			lme.finish();
			throw lme;
		}

		tag = 0;
		while ( (c=in.get()) != -1 && isdigit(c) )
		{
			tag *= 10;
			tag += c-'0';	
		}

		if ( c != '_' )
		{
			libmaus2::exception::LibMausException lme;
			lme.getStream() << "Malformed instance (no underscore after tag)" << std::endl;
			lme.finish();
			throw lme;
		}
		
		std::vector<char> typev;

		while ( (c=in.get()) != -1 && (isalpha(c) || c=='_') )
			typev.push_back(c);

		if ( c != ',' )
		{
			libmaus2::exception::LibMausException lme;
			lme.getStream() << "Malformed instance (no comma after type)" << std::endl;
			lme.finish();
			throw lme;
		}
		
		std::string const types(typev.begin(),typev.end());
		
		if ( endsOn(types,"improper") )
			type = instance_type_improper;
		else if ( endsOn(types,"unmapped") )
			type = instance_type_unmapped;
		else if ( endsOn(types,"split") )
			type = instance_type_split;
		else if ( endsOn(types,"samestrand") )
			type = instance_type_samestrand;
		else if ( endsOn(types,"proper") )
			type = instance_type_proper;
		else
		{		
			libmaus2::exception::LibMausException lme;
			lme.getStream() << "Malformed instance, unknown type " << types << std::endl;
			lme.finish();
			throw lme;
		}
		
		std::ostringstream suflenostr;
		suflenostr << type;
		pre = types.substr(0,types.size()-suflenostr.str().size());

		num = 0;
		while ( (c=in.get()) != -1 && isdigit(c) )
		{
			num *= 10;
			num += c-'0';	
		}
		
		if ( c >= 0 && (!isspace(c)) )
		{
			libmaus2::exception::LibMausException lme;
			lme.getStream() << "Malformed instance" << std::endl;
			lme.finish();
			throw lme;
		}
		
		if ( c >= 0 )
			in.putback(c);
	}
	
	// get name of bam file
	std::string getFileName() const
	{
		std::ostringstream ostr;
		ostr << run << '_' << lane << "#" << tag << "_" << pre << type << ".bam";
		return ostr.str();
	}
};

std::ostream & operator<<(std::ostream & out, Instance const & I);
#endif
