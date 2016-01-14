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
		uint64_t const thres = arginfo.getValue<unsigned int>("thres",2);
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
		
			// get evidence list for gene pair
			EvidenceList EL(PFIS);

			// filter with threshold
			if ( EL.filterMappedPairs(thres) )
				// output if anything left above threshold
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
