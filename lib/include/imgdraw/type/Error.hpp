#ifndef DEF_ERROR
#define DEF_ERROR

namespace imgdraw
{
	enum class DrawError
	{
		Success = 0,
		InvalidPath,
		UnsupportedFormat,
		WriteFailed,
		InvalidArgument
	};
}

#endif // DEF_ERROR