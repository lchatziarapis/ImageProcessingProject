#include "Filter.h"

namespace imaging
{
	class NeighborFilter : public Filter
	{
	public:
		NeighborFilter() : Filter() {}
		NeighborFilter(Image & image) : Filter(image) {}
		void addBlurEffect(Image & image);
		void addDiffEffect(Image & image);
		void addMedianEffect(Image & image);
		void increaseBlurEffect();
	};
}