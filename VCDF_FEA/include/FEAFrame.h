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

#ifndef FEAFRAME_H_
#define FEAFRAME_H_

#include "memory/CountedPtr.h"
#include "VcGroup.h"

namespace vcdf_fea {

/*! @brief FEAFrame is a specific time frame of a simulation.*/
class FEAFrame {
friend class FEAPart;
public:
	virtual ~FEAFrame();
	/*! @brief Gets the time step for the frame.*/
	float getStepTime() const { return _stepTime; }
	/*! @brief Loads the displacement data.
	 *  @remarks Displacement array should be allocated float displacement[numNodes][3]*/
	void getDisplacement(float* displacement);
	/*! @brief Calculates the new node locations based on the displacement.
	 *  @remarks The new nodes should be the same allocated dimension set as the old nodes.*/
	void calcDisplacement(float* newNodes, float* nodes, int numNodes);
	/*! @brief Gets data for a specific variable for this frame.
	 *  @remarks The data should be allocated to the dimensions for the variable.*/
	void getVariable(std::string name, float* data);
private:
	FEAFrame(viscdfcore::IVcGroup::IVcGroupRef frameGroup, float stepTime) : _frameGroup(frameGroup), _stepTime(stepTime) {}
	viscdfcore::VariableRef getFrameVariable(std::string name);
	viscdfcore::IVcGroup::IVcGroupRef _frameGroup;
	float _stepTime;
};

typedef viscdfcore::CountedPtr<FEAFrame> FEAFrameRef;

} /* namespace vcdf_fea */

#endif /* FEAFRAME_H_ */
