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
			uint32_t i;
			unsigned char b[4];
		} data;

		DataType type;

	public:
		Data() {
			data.f = 0.0;
			data.i = 0;
			type = (DataType)0;
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
		inline uint32_t getInt() {
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
		inline void setFloat(const float datum) {
			type = DataType::Float;

			data.f = datum;
		}
		inline void setInt(const uint32_t datum) {
			type = DataType::Integer;

			data.i = datum;
		}
		inline void set(const unsigned char * bytes, DataType dataType) {
			type = dataType;

			memcpy(data.b, bytes, 4);
		}
		inline void set(const unsigned char * bytes) {
			memcpy(data.b, bytes, 4);

			// detect which type it is
			// HACK: is this the best method to determine if it is a float?
			if(type == (DataType)0) {
				float f = data.f;
				if (f + 1 == 1) {
					type = DataType::Integer;
				}
				else {
					type = DataType::Float;
				}
			}
		}
	};
};