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
#include <EvidenceList.hpp>
#include <libmaus2/lz/PlainOrGzipStream.hpp>
#include <libmaus2/lz/GzipOutputStream.hpp>

#include <iostream>
#include <libmaus2/util/ArgInfo.hpp>

int main(int argc, char * argv[])
{
	try
	{
		libmaus2::util::ArgInfo const arginfo(argc,argv);
		
		std::vector< std::pair<std::string,std::string> > V;
		
		for ( uint64_t i = 0; i+1 < arginfo.restargs.size(); i += 2 )
		{
			std::string const sgeneA = arginfo.getUnparsedRestArg(i+0);
			std::string const sgeneB = arginfo.getUnparsedRestArg(i+1);
			V.push_back(std::pair<std::string,std::string>(sgeneA,sgeneB));
		}
		
		libmaus2::lz::PlainOrGzipStream PFIS(std::cin);
		
		std::ostream * postr = &std::cout;
		libmaus2::lz::GzipOutputStream::unique_ptr_type PGZOS;
		
		if ( arginfo.hasArg("gz" ) )
		{
			int const level = arginfo.getValue<int>("level",Z_DEFAULT_COMPRESSION);
			libmaus2::lz::GzipOutputStream::unique_ptr_type TGZOS(new libmaus2::lz::GzipOutputStream(std::cout,64*1024,level));
			PGZOS = UNIQUE_PTR_MOVE(TGZOS);
			postr = PGZOS.get();
		}
		
		std::ostream & out = *postr;
		
		while ( PFIS )
		{
			if ( PFIS.peek() < 0 )
				break;
		
			EvidenceList EL(PFIS);
			
			bool ok = false;
			for ( uint64_t i = 0; i < V.size(); ++i )
				ok = ok || EL.filterGenePair(V[i].first,V[i].second);

			if ( ok )
				out << EL;
		}
		
		PGZOS.reset();	
	}
	catch(std::exception const & ex)
	{
		std::cerr << ex.what() << std::endl;
		return EXIT_FAILURE;
	}
}
