#pragma once
#include <stdint.h>



namespace ObjectModel
{

	enum class Wrapper : int8_t
	{
		PRIMITIVE = 1,
		ARRAY,
		STRING,
		OBJECT
	};

	enum class Type : int8_t
	{
		I8 = 1,
		I16,
		I32,
		I64,

		U8,
		U16,
		U32,
		U64,

		FLOAT,
		DOUBLE,

		BOOL
	};



}
