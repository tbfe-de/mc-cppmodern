#ifndef MY_LENGTH_H
#define MY_LENGTH_H

namespace my {
	class length {
	private:
		const double _value = 0.0;
		explicit constexpr length(double value) : _value(value) {}
	public:
		// get value as double
		constexpr double value() const { return _value; }

		// construct value from double
		static constexpr length mm(double v) { return length{v*1e-3}; }
		static constexpr length cm(double v) { return length{v*1e-2}; }
		static constexpr length m(double v)  { return length{v}; }
		static constexpr length km(double v) { return length{v*1e3}; }

		// supported arithmetic operations
		friend constexpr auto operator+(length lhs, length rhs) {
			return length{lhs._value + rhs._value};
		}
		friend constexpr auto operator-(length lhs, length rhs) {
			return length{lhs._value - rhs._value};
		}
		friend constexpr auto operator*(length lhs, double rhs) {
			return length{lhs._value*rhs};
		}
		friend constexpr auto operator*(double lhs, length rhs) {
			return length{lhs*rhs._value};
		}
		friend constexpr auto operator/(length lhs, double rhs) {
			return length{lhs._value/rhs};
		}
		friend constexpr auto operator/(double lhs, length rhs) {
			return length{lhs/rhs._value};
		}
		friend constexpr auto operator/(length lhs, length rhs) {
			return length{lhs._value/rhs._value};
		}
	};
}

#endif //MY_LENGTH_H
