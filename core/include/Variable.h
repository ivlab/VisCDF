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

#ifndef VARIABLE_H_
#define VARIABLE_H_

#include <string>
#include "AttributeCollection.h"
#include "memory/CountedPtr.h"
#include <vector>
#include "Dimension.h"

namespace viscdfcore {

class Variable : public IHasAttributes {
public:
	virtual ~Variable();
	virtual std::string getName() = 0;
	virtual std::vector<Dimension> getDimensions() = 0;
	virtual void getData(float *data) = 0;
	virtual void getData(int *data) = 0;
	virtual unsigned int getDataType() = 0;

	enum {
		VARIABLE_INT = 4,
		VARIABLE_FLOAT = 5
	};
};

typedef CountedPtr<Variable> VariableRef;

} /* namespace viscdfcore */

#endif /* VARIABLE_H_ */
