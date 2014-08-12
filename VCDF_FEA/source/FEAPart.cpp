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

#include "FEAPart.h"

namespace vcdf_fea {

FEAPart::~FEAPart() {
}

std::string FEAPart::getName() {
	return _meshGroup->getName();
}

int FEAPart::getNumElements() {
	return _meshGroup->getDimension("elementNo").getLength();
}

int FEAPart::getNumNodes() {
	return _meshGroup->getDimension("nodeNo").getLength();
}

int FEAPart::getConnectivity() {
	return _meshGroup->getDimension("connectivity").getLength();
}

void FEAPart::loadMesh(float* nodes, int* elements) {
	_meshGroup->getVariable("nodes")->getData(nodes);
	_meshGroup->getVariable("elements")->getData(elements);
}

std::vector<std::string> FEAPart::getVariables(int resultSet) {
	std::vector<std::string> variables;
	std::vector<viscdfcore::VariableRef> vars = getFrames(resultSet)[0]->_frameGroup->getGroup("nodalData")->getVariables();
	for (int f = 0; f < vars.size(); f++)
	{
		if (vars[f]->getDimensions().size() == 1)
		{
			variables.push_back(vars[f]->getName());
		}
	}
	return variables;
}

std::vector<FEAFrameRef> FEAPart::getFrames(int resultSet) {
	std::string name = getName();
	std::vector<viscdfcore::IVcGroup::IVcGroupRef> groups = _resultGroup->getGroups()[resultSet]->getGroups();
	std::vector<FEAFrameRef> frameGroups;
	for (int f = 0; f < groups.size(); f++)
	{
		if(groups[f]->getName().find("frame_") != std::string::npos)
		{
			float stepTime = 0;
			groups[f]->getVariable("stepTime")->getData(&stepTime);
			frameGroups.push_back(new FEAFrame(groups[f]->getGroup(name), stepTime));
		}
	}

	return frameGroups;
}

} /* namespace vcdf_fea */
