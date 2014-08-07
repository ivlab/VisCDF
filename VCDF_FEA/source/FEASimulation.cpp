/*
 * Copyright Regents of the University of Minnesota, 2014.  This software is released under the following license: http://opensource.org/licenses/lgpl-3.0.html.
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Funding for this software development was provided in part by the NSF (IIS-1251069) and NIH (1R01EB018205-01).
 * Any opinions, findings, and conclusions or recommendations expressed in this material are those of the author(s) and do not necessarily reflect the views of the National Science Foundation.
 *
 * The content is solely the responsibility of the authors and does not necessarily represent the official views of the National Institutes of Health.
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include "FEASimulation.h"

using namespace viscdfcore;

namespace vcdf_fea {

FEASimulation::FEASimulation(std::string filename) : VcContainerBase(filename)
{
}

FEASimulation::~FEASimulation() {
}

std::vector<std::string> FEASimulation::getResultSets() {
	std::vector<std::string> resultSets;
	std::vector<IVcGroup::IVcGroupRef> groups = getGroup()->getGroup("results")->getGroups();

	for (int f = 0; f < groups.size(); f++)
	{
		resultSets.push_back(groups[f]->getName());
	}

	return resultSets;
}

std::vector<FEAPartRef> FEASimulation::getParts() {
	std::vector<FEAPartRef> parts;
	IVcGroup::IVcGroupRef group = getGroup();
	std::vector<IVcGroup::IVcGroupRef> groups = group->getGroups();
	IVcGroup::IVcGroupRef meshGroup;
	IVcGroup::IVcGroupRef resultGroup;
	for (int f = 0; f < groups.size(); f++)
	{
		if (groups[f]->getName() == "mesh")
		{
			meshGroup = groups[f];
		}
		else if (groups[f]->getName() == "results")
		{
			resultGroup = groups[f];
		}
	}

	groups = meshGroup->getGroups();
	for (int f = 0; f < groups.size(); f++)
	{
		parts.push_back(new FEAPart(groups[f], resultGroup));
	}

	return parts;
}

} /* namespace vcdf_fea */

