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

#include <stdlib.h>
#include <stdio.h>
#include <netcdf.h>
#include "netcdf/NcFileReader.h"
#include <iostream>
#include "FEASimulation.h"

#define FILE_NAME "HH168.nc"

using namespace viscdfcore;
using namespace vcdf_fea;
using namespace std;

void listDims(IVcGroup::IVcGroupRef group, int level)
{
	vector<Dimension> dims = group->getDimensions();
	for (int f = 0; f < dims.size(); f++)
	{
		for (int i = 0; i < level; i++)
		{
			cout << "\t";
		}
		cout << "-- " << dims[f].getName() << ": " << dims[f].getLength() << endl;
	}
}

void listVars(IVcGroup::IVcGroupRef group, int level)
{
	vector<VariableRef> vars = group->getVariables();
	for (int f = 0; f < vars.size(); f++)
	{
		for (int i = 0; i < level; i++)
		{
			cout << "\t";
		}

		cout << "++ " << vars[f]->getName() << "(";
		vector<Dimension> dimensions = vars[f]->getDimensions();
		for (int i = 0; i < dimensions.size(); i++)
		{
			if (i != 0)
			{
				cout << ", ";
			}
			cout << dimensions[i].getName();
		}

		cout << ")" << endl;
		if (vars[f]->getName() == "S" || vars[f]->getName() == "U")
		{
			cout << vars[f]->getAttributes()->getText("description") << endl;;
		}

		if (vars[f]->getName() == "MAGNITUDE2")
		{
			float data[vars[f]->getDimensions()[0].getLength()];
			vars[f]->getData(&data[0]);
			for (int x = 0; x < vars[f]->getDimensions()[0].getLength(); x++)
			{
				cout << data[x] << " ";
			}
			cout << endl;
		}
	}
}

void listGroups(IVcGroup::IVcGroupRef group, int level)
{
	for (int f = 0; f < level; f++)
	{
		cout << "\t";
	}
	cout << group->getName() << endl;
	listDims(group, level);
	listVars(group, level);
	vector<IVcGroup::IVcGroupRef> groups = group->getGroups();
	for (int f = 0; f < groups.size(); f++)
	{
		listGroups(groups[f], level + 1);
	}
}

int
main()
{
	NcFileReader reader(FILE_NAME);
	cout << reader.getGroup()->getAttributes()->getText("description") << endl;
	listGroups(reader.getGroup(), 0);
	reader.close();

	FEASimulation sim(string(FILE_NAME));
	vector<FEAPartRef> parts = sim.getParts();
	for (int f = 0; f < parts.size(); f++)
	{
		int numElements = parts[f]->getNumElements();
		int numNodes = parts[f]->getNumNodes();
		int connectivity = parts[f]->getConnectivity();
		vector<FEAFrameRef> frames = parts[f]->getFrames();

		cout << parts[f]->getName() << " ";
		cout << numElements << " ";
		cout << numNodes << " ";
		cout << connectivity << " ";
		cout << frames.size() << " ";
		cout << endl;

		float nodes[numNodes][3];
		int elements[numElements][connectivity];

		parts[f]->loadMesh(nodes, &elements[0][0]);

		for (int i = 0; i < 10; i++)
		{
			cout << nodes[i][0] << " " << nodes[i][1] << " " << nodes[i][2] << endl;
			cout << elements[i][0] << " " << elements[i][1] << " " << elements[i][2] << endl;
		}

		vector<string> variables = parts[f]->getVariables();

		float var[numNodes];

		for (int i = 0; i < variables.size(); i++)
		{
			cout << "Var: " << variables[i] << endl;
			frames[30]->getVariable(variables[i], var);
			cout << "  ";
			for (int j = 0; j < 5; j++)
			{
				cout << " " << var[j];
			}
			cout << endl;
		}

		for (int i = 0; i < frames.size(); i++)
		{
			//cout << "Time: " << frames[i]->getStepTime() << endl;
		}

		float displacement[numNodes][3];
		float timeMesh[numNodes][3];

		frames[10]->getDisplacement(displacement);
		frames[10]->calcDisplacement(timeMesh, nodes, numNodes);

		for (int i = 0; i < 2; i++)
		{
			cout << "n " << nodes[i][0] << " " << nodes[i][1] << " " << nodes[i][2] << endl;
			cout << "d " << displacement[i][0] << " " << displacement[i][1] << " " << displacement[i][2] << endl;
			cout << "t " << timeMesh[i][0] << " " << timeMesh[i][1] << " " << timeMesh[i][2] << endl;
		}
	}

   cout << "Success!" << endl;
   return 0;
}
