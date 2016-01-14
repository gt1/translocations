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
#include <iostream>
#include <libmaus2/util/ArgInfo.hpp>
#include <libmaus2/bambam/BamDecoder.hpp>
#include <libmaus2/bambam/BamWriter.hpp>
#include <libmaus2/bambam/BamHeaderUpdate.hpp>
#include <libmaus2/bambam/BamBlockWriterBaseFactory.hpp>

#include <config.h>

int main(int argc, char * argv[])
{
	try
	{
		libmaus2::util::ArgInfo const arginfo(argc,argv);
		unsigned int const minreflen = arginfo.getValue<unsigned int>("minreflen",40);
		libmaus2::bambam::BamDecoder dec(std::cin);

		libmaus2::bambam::BamAlignment & algn = dec.getAlignment();

		libmaus2::bambam::BamHeader const & header = dec.getHeader();

		::libmaus2::bambam::BamHeader::unique_ptr_type genuphead(
			libmaus2::bambam::BamHeaderUpdate::updateHeader(arginfo,header,"minreflenfilter",std::string(PACKAGE_VERSION))
		);

		libmaus2::bambam::BamBlockWriterBase::unique_ptr_type Pwriter(libmaus2::bambam::BamBlockWriterBaseFactory::construct(*genuphead,arginfo));
		libmaus2::bambam::BamBlockWriterBase & wr = *Pwriter;
		
		while ( dec.readAlignment() )
			if ( (!algn.isMapped()) || algn.getReferenceLength() >= minreflen )
				wr.writeAlignment(algn);
	}
	catch(std::exception const & ex)
	{
		std::cerr << ex.what() << std::endl;
		return EXIT_FAILURE;
	}
}
