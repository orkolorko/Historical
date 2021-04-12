/*
 *	Copyright 2011-2013 Stefano Galatolo (galatolo@mail.dm.unipi.it), 
 * 						Isaia Nisoli (nisoli@im.ufrj.br) 
 *	
 *	This file is part of CompInvMeas-Ulam.
 *
 *  CompInvMeas-Ulam is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  CompInvMeas-Ulam is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with CompInvMeas-Ulam.  If not, see <http://www.gnu.org/licenses/>.
*/
//!\file rpartition.cxx implements the methods of rpartition.h
//!\ingroup Ulam

#include "rapfloat.h"
#include "rpartition.h"
#include "dynamic.h"

rpartition::rpartition(const rinterval &dom, int size)
{
	if (dom.min<dom.max) //since we are dealing with dynamic on the circle, the intervals may happen to go across 1 and back
		{
		RigScalar len=(dom.max-dom.min)/size;
			for (int i=0;i< size;i++)
			{
				//partitions the interval in size intervals
				rinterval temp(dom.min+i*len,dom.min+(i+1)*len);
				value.push_back(temp);
			}
		}
	if (dom.max<dom.min)
		{
			//this partitions the interior of the interval on the circle
			rinterval temp1(dom.min,1);
			rinterval temp2(0,dom.max);
			rpartition temppart1(temp1,size);
			rpartition temppart2(temp2,size);
			rpartition temppart3(temppart1,temppart2);
			value=temppart3.value;
		}
		
}


rpartition::rpartition(const rpartition &original)
{
	//copies the partition
	Container::const_iterator iter; 
	for (iter=(original.value).begin(); iter!=(original.value).end(); iter++)
		{
			value.push_back(*iter);
		}
}

rpartition::rpartition(const rpartition &part1, const rpartition &part2)
{
	//joins two partitions
	Container::const_iterator iter; 
	for (iter=(part1.value).begin(); iter!=(part1.value).end(); iter++)
		{
			value.push_back(*iter);
		}
	for (iter=(part2.value).begin(); iter!=(part2.value).end(); iter++)
		{
			value.push_back(*iter);
		}
}

void rpartition::printpartition(ofstream &myfile) const
{
	Container::const_iterator iter;
	//prints all the left endpoints of the partition 
	for (iter=value.begin(); iter!=value.end(); iter++)
		{
			myfile<< boost::rational_cast<double>((*iter).min) <<endl;
		}
	myfile.close();
}

void rpartition::printdynamic(ofstream &myfile) const
{
	Container::const_iterator iter; 
	//prints the value of the dynamic at the mid-points of the intervals of the partition
	for (iter=value.begin(); iter!=value.end(); iter++)
		{
			myfile<< boost::rational_cast<double>(((*iter).in)) <<" "<< boost::rational_cast<double>(dyn((*iter).in)) <<endl;
		}
	myfile.close();
}

//assignment operator, not really a good assignment operator, makes a temporary object...
rpartition& rpartition::operator=(const rpartition &rhs)
{
	//check it is not the same object
	if (this == &rhs)  return *this;
	int size=(rhs.value).size();
	//if not, builds an auxiliary object
	Container temp;
	for (int i=0;i<size;i++)
		{
			temp.push_back(rhs.value[i]);
		}
	value=temp;
	return *this;
}
