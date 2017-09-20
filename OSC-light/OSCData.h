#pragma once

namespace OSC {
	enum class DataType {
		Float = 1,
		Integer = 2
	};

	struct Data {
	private:
		union data {
		public:
			float f;
#ifdef _MSC_VER
			short i;
#else
			int32_t i;
#endif
			unsigned char b[4];
		} data;

		DataType type;

	public:
		Data() {
			data.f = 0.0;
			data.i = 0;
		}

		// resetter
		void empty() {
			data.f = 0.0;
			data.i = 0;
			type = DataType::Integer;
		}

		// getters
		inline float getFloat() {
			return data.f;
		}
#ifdef _MSC_VER
		inline short getInt() {
#else
		inline int getInt() {
#endif
			return data.i;
		}
		inline DataType getDataType() {
			return type;
		}
		inline void get(char * output) {
			for (int i = 0; i < 4; ++i) {
				output[i] = data.b[i];
			}
		}

		// setters
		inline void set(const float datum) {
			type = DataType::Float;

			data.f = datum;
		}
		inline void set(const int datum) {
			type = DataType::Integer;

			data.i = datum;
		}
		inline void set(const unsigned char * bytes, DataType dataType) {
			memcpy(data.b, bytes, 4);
			type = dataType;
		}
	};
};