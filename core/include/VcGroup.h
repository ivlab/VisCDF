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

#ifndef VCGROUP_H_
#define VCGROUP_H_

#include "AttributeCollection.h"
#include "memory/CountedPtr.h"
#include <vector>
#include <string>
#include "Dimension.h"
#include "Variable.h"

namespace viscdfcore {

class IVcGroup : public IHasAttributes
{
public:
	typedef CountedPtr<IVcGroup> IVcGroupRef;

	virtual ~IVcGroup() {}
	virtual void addGroup(IVcGroupRef group) = 0;
	virtual std::vector<IVcGroupRef> getGroups() const = 0;
	virtual std::string getName() = 0;
	virtual std::vector<Dimension> getDimensions() = 0;
	virtual std::vector<VariableRef> getVariables() = 0;
	Dimension getDimension(std::string name);
	VariableRef getVariable(std::string name);
	IVcGroupRef getGroup(std::string name);
};

class VcGroup : public IVcGroup {
public:
	VcGroup();
	virtual ~VcGroup();
	virtual std::string getName() { return _name; };
	virtual void addGroup(IVcGroupRef group) { _groups.push_back(group); }
	virtual std::vector<IVcGroupRef> getGroups() const { return _groups; }

private:
	std::string _name;
	std::vector<IVcGroupRef> _groups;
};

} /* namespace viscdfcore */

#endif /* VCGROUP_H_ */
