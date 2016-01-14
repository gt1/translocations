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
#include <InstanceBase.hpp>

std::ostream & operator<<(std::ostream & out, InstanceBase::instance_type const type)
{
	switch ( type )
	{
		case InstanceBase::instance_type_improper: return out << "improper";
		case InstanceBase::instance_type_orphan: return out << "orphan";
		case InstanceBase::instance_type_unmapped: return out << "unmapped";
		case InstanceBase::instance_type_split: return out << "split";
		case InstanceBase::instance_type_samestrand: return out << "samestrand";
		case InstanceBase::instance_type_proper: return out << "proper";
		default: return out << "unknown";
	}
}
