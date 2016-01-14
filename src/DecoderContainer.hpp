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
#include <map>
#include <libmaus2/bambam/BamRangeDecoder.hpp>

struct DecoderContainer
{
	std::map<std::string,libmaus2::bambam::BamRangeDecoder::shared_ptr_type> decoders;
	
	DecoderContainer()
	{
	
	}
	
	libmaus2::bambam::BamRangeDecoder & get(std::string const & filename, std::string const & range)
	{
		std::map<std::string,libmaus2::bambam::BamRangeDecoder::shared_ptr_type>::const_iterator ita =
			decoders.find(filename);
		
		if ( ita == decoders.end() )
		{
			libmaus2::bambam::BamRangeDecoder::shared_ptr_type P(
				new libmaus2::bambam::BamRangeDecoder(filename,range)
			);
			decoders[filename] = P;

			ita = decoders.find(filename);
		}
		
		libmaus2::bambam::BamRangeDecoder & dec = *(ita->second);
		
		dec.setRange(range);
		
		return dec;
	}
};
