#pragma once

#include <cstring>

using namespace std;

namespace OSC
{
enum class DataType
{
	Float = 1,
	Integer = 2
};

struct Data
{
private:
	union data {
	public:
		float f;
		uint32_t i;
		unsigned char b[4];
	} data;

	DataType type;

public:
	Data()
	{
		data.f = 0.0;
		data.i = 0;
		type = (DataType)0;
	}

	// resetter
	void empty()
	{
		data.f = 0.0;
		data.i = 0;
		type = DataType::Integer;
	}

	// getters
	float getFloat()
	{
		return data.f;
	}
	uint32_t getInt()
	{
		return data.i;
	}
	DataType getDataType()
	{
		return type;
	}
	void get(char *output)
	{
		for (int i = 0; i < 4; ++i)
		{
			output[i] = data.b[i];
		}
	}

	// setters
	void setFloat(const float datum)
	{
		type = DataType::Float;

		data.f = datum;
	}
	void setInt(const uint32_t datum)
	{
		type = DataType::Integer;

		data.i = datum;
	}
	void set(const unsigned char *bytes, const DataType dataType)
	{
		type = dataType;

		memcpy(data.b, bytes, 4);
	}
	void set(const unsigned char *bytes)
	{
		memcpy(data.b, bytes, 4);

		// detect which type it is
		// HACK: is this the best method to determine if it is a float?
		if (type == (DataType)0)
		{
			auto f = data.f;
			if (f + 1 == 1)
			{
				type = DataType::Integer;
			}
			else
			{
				type = DataType::Float;
			}
		}
	}
};
}; // namespace OSC