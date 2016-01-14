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
#if ! defined(INSTANCELINE_HPP)
#define INSTANCELINE_HPP

#include <DecoderContainer.hpp>
#include <GeneDesc.hpp>
#include <Instance.hpp>

struct InstanceLine
{
	// decoder container
	DecoderContainer * deccont;
	// number of instances
	uint64_t numinst;
	GeneDesc geneA;
	GeneDesc geneB;
	// maximum instance
	uint64_t instmax;
	// sum over instances
	uint64_t instsum;
	std::vector<Instance> instances;
	
	// filter instances by set of instances
	bool filterInstances(std::set<Instance> const & S)
	{
		uint64_t o = 0;
		
		for ( uint64_t i = 0; i < instances.size(); ++i )
			if ( S.find(instances[i]) != S.end() )
				instances[o++] = instances[i];

		instances.resize(o);
		
		numinst = instances.size();
		instmax = 0;
		instsum = 0;
		for ( uint64_t i = 0; i < instances.size(); ++i )
		{
			instmax = std::max(instmax,static_cast<uint64_t>(instances[i].num));
			instsum += instances[i].num;
		}
		
		return instances.size();
	}
	
	// filter instances by type mask
	bool filterInstances(uint64_t const mask)
	{
		uint64_t o = 0;
		
		for ( uint64_t i = 0; i < instances.size(); ++i )
			if ( ( static_cast<uint64_t>(instances[i].type) & mask ) != 0 )
				instances[o++] = instances[i];
				
		instances.resize(o);
		
		numinst = instances.size();
		instmax = 0;
		instsum = 0;
		for ( uint64_t i = 0; i < instances.size(); ++i )
		{
			instmax = std::max(instmax,static_cast<uint64_t>(instances[i].num));
			instsum += instances[i].num;
		}
		
		return instances.size();
	}
	
	// number of instances
	uint64_t size() const
	{
		return instances.size();
	}
	
	// get BAM decoder for region of gene A
	libmaus2::bambam::BamRangeDecoder & getInstanceDecoderA(uint64_t const index)
	{
		return deccont->get(instances[index].getFileName(),geneA.getRange());
	}

	// get BAM decoder for region of gene B
	libmaus2::bambam::BamRangeDecoder & getInstanceDecoderB(uint64_t const index)
	{
		return deccont->get(instances[index].getFileName(),geneB.getRange());
	}
	
	InstanceLine() : deccont(0), numinst(0), instmax(0), instsum(0) {}
	InstanceLine(DecoderContainer * rdeccont, std::istream & in)
	: deccont(rdeccont)
	{
		int c = -1;

		// number of instances
		numinst = 0;
		while ( (c=in.get()) != -1 && isdigit(c) )
		{
			numinst *= 10;
			numinst += c-'0';	
		}
				
		if ( c != '\t' )
		{
			libmaus2::exception::LibMausException lme;
			lme.getStream() << "Malformed instance line (numinst)" << std::endl;
			lme.finish();
			throw lme;
		}

		// gene A
		geneA = GeneDesc(in);
		
		c = in.get();
		if ( c != '\t' )
		{
			libmaus2::exception::LibMausException lme;
			lme.getStream() << "Malformed instance line (gene A)" << std::endl;
			lme.finish();
			throw lme;
		}

		// gene B
		geneB = GeneDesc(in);

		c = in.get();
		if ( c != '\t' )
		{
			libmaus2::exception::LibMausException lme;
			lme.getStream() << "Malformed instance line (gene B)" << std::endl;
			lme.finish();
			throw lme;
		}

		// maximum number of instances
		instmax = 0;
		while ( (c=in.get()) != -1 && isdigit(c) )
		{
			instmax *= 10;
			instmax += c-'0';	
		}
		
		if ( c != '\t' )
		{
			libmaus2::exception::LibMausException lme;
			lme.getStream() << "Malformed instance (inst max)" << std::endl;
			lme.finish();
			throw lme;
		}

		// sum over number of instances
		instsum = 0;
		while ( (c=in.get()) != -1 && isdigit(c) )
		{
			instsum *= 10;
			instsum += c-'0';	
		}
		
		if ( c != '\t' )
		{
			libmaus2::exception::LibMausException lme;
			lme.getStream() << "Malformed instance (inst sum)" << std::endl;
			lme.finish();
			throw lme;
		}
		
		in.putback(c);
		
		// get instances
		while ( in.peek() == '\t' )
		{
			c = in.get();
			assert ( c == '\t' );
			instances.push_back(Instance(in));
		}

		if ( in.peek() != '\n' )
		{
			libmaus2::exception::LibMausException lme;
			lme.getStream() << "Malformed instance line (new line)" << std::endl;
			lme.finish();
			throw lme;
		}
		
		c = in.get();
		assert ( c == '\n' );
		
		// sanity check
		if ( numinst != instances.size() )
		{
			libmaus2::exception::LibMausException lme;
			lme.getStream() << "Malformed instance line (inconsistent)" << std::endl;
			lme.finish();
			throw lme;
		}
		
		std::sort(instances.begin(),instances.end());
	}
};

std::ostream & operator<<(std::ostream & out, InstanceLine const & IL);
#endif
