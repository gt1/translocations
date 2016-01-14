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
#include <SubInstKey.hpp>
#include <PosComparator.hpp>
#include <MappedPairCountComparator.hpp>
#include <InstanceLine.hpp>

#include <libmaus2/util/ArgInfo.hpp>
#include <libmaus2/util/GrowingFreeList.hpp>

// check whether A and B are the same alignment block by content
static bool sameAlignment(libmaus2::bambam::BamAlignment const * A, libmaus2::bambam::BamAlignment const * B)
{
	return
		(A->blocksize == B->blocksize) &&
		 memcmp(A->D.begin(),B->D.begin(),A->blocksize) == 0;
}

// check whether alignment A is annotated with gene
static bool hasGeneComment(libmaus2::bambam::BamAlignment const * A, std::string const & gene)
{
	char const * co = A->getAuxString("CO");
	
	if ( ! co )
		return false;
	
	std::deque<std::string> const tokens = libmaus2::util::stringFunctions::tokenize<std::string>(std::string(co),std::string(";"));
	
	for ( uint64_t i = 0; i < tokens.size(); ++i )
	{
		std::istringstream istr(tokens[i]);
		GeneDesc const GD(istr);
		
		if ( GD.name == gene )
			return true;
	}
	
	return false;
}


int main(int argc, char * argv[])
{
	try
	{
		libmaus2::util::ArgInfo const arginfo(argc,argv);
		// free list for alignments
		libmaus2::util::GrowingFreeList<libmaus2::bambam::BamAlignment> BAFL;
		// insert size
		int64_t const insertsize = arginfo.getValue<int64_t>("insertsize",800);
		// container for bam decoders
		DecoderContainer deccont;
		
		while ( std::cin )
		{
			// eof?
			if ( std::cin.peek() < 0 )
				break;
		
			// get next instance line
			InstanceLine IS(&deccont,std::cin);
			std::cout << IS << '\n';
			
			uint64_t ilow = 0;
			while ( ilow != IS.size() )
			{
				uint64_t ihigh = ilow;
				
				// get interval for one type of instance
				while ( ihigh != IS.size() && IS.instances[ihigh].type == IS.instances[ilow].type )
					++ihigh;
				
				// if type is split, improper or samestrand
				if ( 
					IS.instances[ilow].type == Instance::instance_type_split
					||
					IS.instances[ilow].type == Instance::instance_type_improper
					||
					IS.instances[ilow].type == Instance::instance_type_samestrand
				)
				{
					// key is maxcnt,num
					std::map<SubInstKey, std::vector<std::string>, std::greater<SubInstKey> > subinst;
				
					for ( uint64_t i = ilow; i < ihigh; ++i )
					{
						std::vector<libmaus2::bambam::BamAlignment *> retlist;
						
						// map from read name to vector of alignments
						std::map<std::string, std::vector<libmaus2::bambam::BamAlignment *> > amap;
						
						libmaus2::bambam::BamHeader::unique_ptr_type uheader;
					
						// get alignments from A file marked with gene A
						{
							libmaus2::bambam::BamAlignmentDecoder & deca = IS.getInstanceDecoderA(i);
							libmaus2::bambam::BamAlignment & algna = deca.getAlignment();
							libmaus2::bambam::BamHeader const & headera = deca.getHeader();

							libmaus2::bambam::BamHeader::unique_ptr_type theader(headera.uclone());
							uheader = UNIQUE_PTR_MOVE(theader);
							
							while ( deca.readAlignment() )
								if ( algna.isMapped() && hasGeneComment(&algna,IS.geneA.name) )
								{
									// std::cerr << algna.formatAlignment(headera) << std::endl;
									libmaus2::bambam::BamAlignment * palgn = BAFL.get();
									retlist.push_back(palgn);
									amap[algna.getName()].push_back(palgn);
									palgn->swap(algna);
								}
						}

						// get alignments from B file marked with gene B
						{
							libmaus2::bambam::BamAlignmentDecoder & decb = IS.getInstanceDecoderB(i);
							libmaus2::bambam::BamAlignment & algnb = decb.getAlignment();
							// libmaus2::bambam::BamHeader const & headerb = decb->getHeader();
							
							while ( decb.readAlignment() )
								if ( algnb.isMapped() && hasGeneComment(&algnb,IS.geneB.name) )
								{
									// std::cerr << algna.formatAlignment(headera) << std::endl;
									libmaus2::bambam::BamAlignment * palgn = BAFL.get();
									retlist.push_back(palgn);
									amap[algnb.getName()].push_back(palgn);
									palgn->swap(algnb);
								}
						}
						
						// pair map intervals
						std::vector<MappedPair> pairs;
						// count read pairs for gene pair
						uint64_t cnt = 0;
						// iterate over names
						for ( std::map<std::string, std::vector<libmaus2::bambam::BamAlignment *> >::iterator ita = amap.begin(); ita != amap.end(); ++ita )
						{
							// get vector of alignments for name
							std::vector<libmaus2::bambam::BamAlignment *> & V = ita->second;
							// sort by (refid,pos,read1)
							std::sort(V.begin(),V.end(),PosComparator());
							
							// remove multiple copies of same alignment
							uint64_t o = 0;
							uint64_t rcnt[2] = {0,0};
							for ( uint64_t l = 0; l < V.size(); )
							{
								// skip over multiple copies of same alignment
								uint64_t h = l;
								while ( h != V.size() && sameAlignment(V[l],V[h]) )
									++h;

								// count read 1
								if ( V[l]->isRead1() )
									rcnt[0]++;
								// count read 2
								if ( V[l]->isRead2() )
									rcnt[1]++;
								
								// copy alignment pointer
								if ( V[l]->isRead1() || V[l]->isRead2() )
									V[o++] = V[l];							
									
								l = h;
							}
														
							V.resize(o);
							
							// if we have read1 and read2
							if ( V.size() > 1 && rcnt[0] && rcnt[1] )
							{
								// iterate over read 1
								for ( uint64_t r1 = 0; r1 < rcnt[0]; ++r1 )
									// iterate over read 2
									for ( uint64_t r2 = rcnt[0]; r2 < o; ++r2 )
									{
										// get alignment for read 1
										libmaus2::bambam::BamAlignment const * algn1 = V[r1];
										// get alignment for read 2
										libmaus2::bambam::BamAlignment const * algn2 = V[r2];

										// if pair is marked with gene pair we are looking for
										if ( 
											(
												hasGeneComment(algn1,IS.geneA.name)
												&&
												hasGeneComment(algn2,IS.geneB.name)
											)
											||
											(											
												hasGeneComment(algn1,IS.geneB.name)
												&&
												hasGeneComment(algn2,IS.geneA.name)
											)
										)
										{
											#if 0
											std::cerr << "\n --- \n";
											std::cerr << algn1->formatAlignment(*uheader) << std::endl;
											std::cerr << algn2->formatAlignment(*uheader) << std::endl;
											++cnt;
											std::cerr << "subcnt=" << V.size() << std::endl;
											#endif
											
											// increment number of read pairs found
											cnt += 1;
											
											// determine smaller and larger alignment (by position)
											bool const smal1 = PosComparator::compare(algn1,algn2);
											libmaus2::bambam::BamAlignment const * algnl = smal1 ? algn1 : algn2;
											libmaus2::bambam::BamAlignment const * algnr = smal1 ? algn2 : algn1;

											// "left" interval
											libmaus2::math::IntegerInterval<int64_t> intl(
												algnl->getPos(),
												algnl->getAlignmentEnd()
											);

											// "right" interval
											libmaus2::math::IntegerInterval<int64_t> intr(
												algnr->getPos(),
												algnr->getAlignmentEnd()
											);
											
											// construct mapped pair of aligned intervals
											MappedPair MP(intl,intr);
								
											// see if this interval (modulo insert size) already exists			
											bool existing = false;
											for ( uint64_t i = 0; (!existing) && i < pairs.size(); ++i )
												// already seen?
												if ( MappedPair::isInsertSizeOverlap(pairs[i],MP,insertsize) )
												{
													// update from for left
													pairs[i].first.from = std::min(
														MP.first.from,
														pairs[i].first.from
													);
													// update to for left
													pairs[i].first.to = std::max(
														MP.first.to,
														pairs[i].first.to
													);

													// update from for right
													pairs[i].second.from = std::min(
														MP.second.from,
														pairs[i].second.from
													);
													// update to for right
													pairs[i].second.to = std::max(
														MP.second.to,
														pairs[i].second.to
													);
													
													// update number of supporting pair for this "break point"
													pairs[i].cnt += 1;
												
													// note that this break point already existed
													existing = true;
												}
												#if 0
												else
												{
													std::cerr << "no overlap" << std::endl;
													std::cerr << MP << std::endl;
													std::cerr << pairs[i] << std::endl;
												}
												#endif
											
											// push new pair if break point did not exist yet
											if ( ! existing )
												pairs.push_back(MP);
										}
									}
							
							}
						}

						// std::cerr << IS.instances[i] << " gcnt=" << cnt << ((IS.instances[i].num != cnt) ? "___" : "") << std::endl;

						std::ostringstream ostr;						
						ostr << "\t" << IS.instances[i];
						// sort by number of supporting read pairs
						std::sort(pairs.begin(),pairs.end(),MappedPairCountComparator());
						// maximum count
						uint64_t const maxcnt =  pairs.size() ? pairs.front().cnt : 0;
						ostr << "\t" << maxcnt;
						// output break points
						for ( uint64_t j = 0; j < pairs.size();++j )
							ostr << "\t" << pairs[j];
						
						// sub instances map ordered by number of read pairs supporting break point
						subinst[SubInstKey(maxcnt,IS.instances[i].num)].push_back(ostr.str());

						// return alignment to free list
						for ( uint64_t i = 0; i < retlist.size(); ++i )
							BAFL.put(retlist[i]);
					}

					// output instance lines
					for ( 
						std::map<SubInstKey, std::vector<std::string>, std::greater<SubInstKey> >::const_iterator ita = subinst.begin();
							ita != subinst.end(); ++ita 
					)
					{
						std::vector<std::string> const & V = ita->second;
						for ( uint64_t i = 0; i < V.size(); ++i )
							std::cout << V[i] << "\n";
					}
				}

				ilow = ihigh;
			}
		}			
	}
	catch(std::exception const & ex)
	{
		std::cerr << ex.what() << std::endl;
		return EXIT_FAILURE;
	}
}
