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

#include "VcGroup.h"

namespace viscdfcore {

VcGroup::VcGroup() {
	// TODO Auto-generated constructor stub

}

VcGroup::~VcGroup() {
	// TODO Auto-generated destructor stub
}

Dimension IVcGroup::getDimension(std::string name) {
	std::vector<Dimension> dimensions = getDimensions();
	for (int f = 0; f < dimensions.size(); f++)
	{
		if (dimensions[f].getName() == name)
		{
			return dimensions[f];
		}
	}

	return Dimension(name, 0);
}

VariableRef IVcGroup::getVariable(std::string name) {
	std::vector<VariableRef> variables = getVariables();
	for (int f = 0; f < variables.size(); f++)
	{
		if (variables[f]->getName() == name)
		{
			return variables[f];
		}
	}

	return NULL;
}

IVcGroup::IVcGroupRef IVcGroup::getGroup(std::string name) {
	std::vector<IVcGroupRef> groups = getGroups();
	for (int f = 0; f < groups.size(); f++)
	{
		if (groups[f]->getName() == name)
		{
			return groups[f];
		}
	}

	return NULL;
}

} /* namespace viscdfcore */


