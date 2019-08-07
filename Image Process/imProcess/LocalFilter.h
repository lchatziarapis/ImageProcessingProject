#include "Filter.h"


namespace imaging
{
	class LocalFilter : public Filter
	{

	public:
		LocalFilter() : Filter() {};
		LocalFilter(Image & img) : Filter(img) {};
		~LocalFilter() {};
		void toGrayEffect(Image & img);
		void toColorEffect(Image & img, Color color);
	};
}
