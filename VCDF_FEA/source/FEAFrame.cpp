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

#include "FEAFrame.h"

namespace vcdf_fea {

FEAFrame::~FEAFrame() {
	// TODO Auto-generated destructor stub
}

void FEAFrame::getVariable(std::string name, float* data) {
	getFrameVariable(name)->getData(data);
}

viscdfcore::VariableRef FEAFrame::getFrameVariable(std::string name) {
	return _frameGroup->getGroup("nodalData")->getVariable(name);
}


void FEAFrame::getDisplacement(float *displacement) {
	getFrameVariable("U")->getData(displacement);
}

void FEAFrame::calcDisplacement(float* newNodes, float* nodes, int numNodes)
{
	getDisplacement(newNodes);

	for (int f = 0; f < numNodes; f++)
	{
		for (int i = 0; i < 3; i++)
		{
			newNodes[f*3 + i] += nodes[f*3 + i];
		}
	}
}

} /* namespace vcdf_fea */
