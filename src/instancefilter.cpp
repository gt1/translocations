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
#include <InstanceLine.hpp>

#include <iostream>
#include <libmaus2/util/ArgInfo.hpp>

int main(int argc, char * argv[])
{
	try
	{
		libmaus2::util::ArgInfo const arginfo(argc,argv);
		std::string const stype = arginfo.getValue<std::string>("type","split,improper,samestrand");
		bool const keepident = arginfo.getValue<unsigned int>("keepident",true);
		bool const keepoverlap = arginfo.getValue<unsigned int>("keepoverlap",true);
		uint64_t const it = InstanceBase::parseInstanceTypes(stype);
		
		while ( std::cin )
		{
			if ( std::cin.peek() < 0 )
				break;
		
			InstanceLine IS(0,std::cin);
			
			if ( 
				IS.filterInstances(it) && 
				(keepident   || (IS.geneA.name != IS.geneB.name) ) &&
				(keepoverlap || (!IS.geneA.overlaps(IS.geneB)) ) 
			)
				std::cout << IS << '\n';			
		}			
	}
	catch(std::exception const & ex)
	{
		std::cerr << ex.what() << std::endl;
		return EXIT_FAILURE;
	}
}
