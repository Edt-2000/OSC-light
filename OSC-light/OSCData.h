#pragma once

namespace OSC {
	enum class DataType {
		Float = 1, 
		Integer = 2
	};

	struct Data {
	public:
		static inline bool _isBigEndian() {
			const int one = 1;
			const char sig = *(char*)&one;

			return (sig == 0);
		}

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

		// setters
		inline void set(float datum) {
			type = DataType::Float;

			data.f = datum;
		}
		inline void set(int datum) {
			type = DataType::Integer;

			data.i = datum;
		}

		void outputOSCData(char * output) {
			int chr = 0;

			if (_isBigEndian()) {
				for (int i = 0; i < 4; ++i) {
					output[chr++] = data.b[i];
				}
			}
			else {
				for (int i = 3; i >= 0; --i) {
					output[chr++] = data.b[i];
				}
			}

		}

		void inputOSCData(const char * input) {
			int chr = 0;

			if (_isBigEndian()) {
				for (int i = 0; i < 4; ++i) {
					data.b[chr++] = input[i];
				}
			}
			else {
				for (int i = 3; i >= 0; --i) {
					data.b[chr++] = input[i];
				}
			}
		}

	};
};