ACLOCAL_AMFLAGS=-I m4

noinst_LTLIBRARIES = libtranslocations.la
noinst_HEADERS = \
	DecoderContainer.hpp  InstanceBase.hpp	InstanceLine.hpp		   MappedPair.hpp	    PosComparator.hpp \
	GeneDesc.hpp	  Instance.hpp	MappedPairCountComparator.hpp  MappedPairsList.hpp  SubInstKey.hpp \
	EvidenceList.hpp

libtranslocations_la_SOURCES = InstanceBase.cpp Instance.cpp GeneDesc.cpp InstanceLine.cpp MappedPair.cpp MappedPairsList.cpp EvidenceList.cpp
libtranslocations_la_LDFLAGS = -static

bin_PROGRAMS = pairhist breakpoints clipfilter instancefilter evidencefilter genepairfilter minreflenfilter falsepositivefilter

pairhist_SOURCES = pairhist.cpp
breakpoints_SOURCES = breakpoints.cpp
breakpoints_LDADD = libtranslocations.la
instancefilter_SOURCES = instancefilter.cpp
instancefilter_LDADD = libtranslocations.la
clipfilter_SOURCES = clipfilter.cpp
minreflenfilter_SOURCES = minreflenfilter.cpp
evidencefilter_SOURCES = evidencefilter.cpp
evidencefilter_LDADD = libtranslocations.la
genepairfilter_SOURCES = genepairfilter.cpp
genepairfilter_LDADD = libtranslocations.la
falsepositivefilter_SOURCES = falsepositivefilter.cpp
falsepositivefilter_LDADD = libtranslocations.la

# test_SOURCES = 
