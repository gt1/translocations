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
#include <libmaus2/util/stringFunctions.hpp>
#include <map>
#include <vector>
#include <sstream>

int main()
{
	try
	{
		// string pair
		typedef std::pair<std::string,std::string> spair;
		// map string pair -> vector of strings
		std::map < spair, std::vector<std::string> > M;
	
		while ( std::cin )
		{
			std::string line;
			std::getline(std::cin,line);
			
			if ( line.size() )
			{
				// tokenise
				std::deque<std::string> tokens = libmaus2::util::stringFunctions::tokenize<std::string>(line,std::string("\t"));
				// if at least three tokens
				if ( tokens.size() >= 3 )
				{
					// insert pair instance
					M[spair(tokens[0],tokens[1])].push_back(tokens[2]);
				}
			}
		}
		
		for ( std::map < spair, std::vector<std::string> >::const_iterator ita = M.begin();
			ita != M.end(); ++ita )
		{
			// get list for pair
			std::vector<std::string> const & V = ita->second;
			
			// size of list
			std::cout << V.size() << "\t";
			// key
			std::cout << ita->first.first << "\t" << ita->first.second;
			
			// compute sum and max
			int sum = 0;
			int max = 0;
			for ( uint64_t i = 0; i < V.size(); ++i )
			{
				// find last comma
				std::string::size_type const com = V[i].find_last_of(',');
				if ( com != std::string::npos )
				{
					// behind comma
					std::string pastcom = V[i].substr(com+1);
					// parse number
					std::istringstream istr(pastcom);
					int num;
					istr >> num;
					sum += num;
					max = std::max(max,num);
				}
			}
			
			std::cout << "\t" << max;
			std::cout << "\t" << sum;
			
			for ( uint64_t i = 0; i < V.size(); ++i )
				std::cout << "\t" << V[i];
			
			std::cout.put('\n');
		}
	}
	catch(std::exception const & ex)
	{
		std::cerr << ex.what() << std::endl;
		return EXIT_FAILURE;
	}
}
