#include "template.h"
#include "Material.h"


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
