#include "template.h"
#include "Ray.h"

enum Material { DIFFUSE, MIRROR, DIELECTRIC };

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