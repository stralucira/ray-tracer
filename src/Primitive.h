#include "template.h"
#include "Ray.h"

class Primitive
{
protected:
	Material mat;
public:
	virtual ~Primitive()
	{
	}

	Material getMaterial() const
	{
		return this->mat;
	}
};