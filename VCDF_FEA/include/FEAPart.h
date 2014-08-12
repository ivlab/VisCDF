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

#ifndef FEAPART_H_
#define FEAPART_H_

#include "memory/CountedPtr.h"
#include "VcGroup.h"
#include "FEAFrame.h"

namespace vcdf_fea {

/*! @brief FEAPart is a specific geometric part / mesh of the simulation.*/
class FEAPart {
friend class FEASimulation;
public:
	virtual ~FEAPart();
	/*! @brief Get's the part name.*/
	std::string getName();
	/*! @brief Get's the number of elements (i.e. hexahedra, tetrahedra, etc...).*/
	int getNumElements();
	/*! @brief Get's the number of nodes (vertices).*/
	int getNumNodes();
	/*! @brief Defines the geometry of the element (i.e. hexahedron = 8, tetrahedra = 4).*/
	int getConnectivity();
	/*! @brief Gets the available node specific variables.*/
	std::vector<std::string> getVariables(int resultSet);
	/*! @brief Gets the time frames for a simulation.*/
	std::vector<FEAFrameRef> getFrames(int resultSet);

	/*! @brief Load's the nodes and element parts of the mesh or geometry.
	 *  @remarks Nodes and elements should be allocated before passing in.
	 *  	Nodes should be allocated: float node[NUM_NODES][3]
	 *  	Elements should be allocated: int elements[NUM_ELEMENTS][CONNECTIVITY]
	 *  	Here is an example of how to call loadMesh:
	 *  		float nodes[numNodes][3];
	 *  		int elements[numElements][connectivity];
	 *			part.loadMesh(&nodes[0][0], &elements[0][0]);
	 *  		
	 *  		or
	 *  		
	 *  		float* nodes = new float[numNodes*3];
	 *  		int *elements = new int[numElements*connectivity];
	 *			part.loadMesh(nodes, elements);
	 */
	void loadMesh(float* nodes, int* elements);
private:
	FEAPart(viscdfcore::IVcGroup::IVcGroupRef meshGroup, viscdfcore::IVcGroup::IVcGroupRef resultGroup) : _meshGroup(meshGroup), _resultGroup(resultGroup) {}

	viscdfcore::IVcGroup::IVcGroupRef _meshGroup;
	viscdfcore::IVcGroup::IVcGroupRef _resultGroup;
};

typedef viscdfcore::CountedPtr<FEAPart> FEAPartRef;

} /* namespace vcdf_fea */

#endif /* FEAPART_H_ */
