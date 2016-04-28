#pragma once

#include <cmath>
#include <string>

double powi(double val, int exp);

template<int length, int time, int mass>
class Unit
{
	typedef Unit<length, time, mass> ThisUnit;

	typedef Unit<1, 0, 0> LengthUnit;
	typedef Unit<0, 0, 1> MassUnit;
	typedef Unit<0, 1, 0> TimeUnit;

public:
	static ThisUnit BaseUnit(const std::string& label = "")
	{
		return ThisUnit(label);
	}
	static ThisUnit ScaledUnit(double multiplier, const ThisUnit& ref = ThisUnit(), const std::string& label = "")
	{
		return ThisUnit(multiplier, ref, label);
	}
	static ThisUnit DerivedUnit(const LengthUnit& lengthUnit, const TimeUnit& timeUnit, const MassUnit& massUnit, const std::string& label = "")
	{
		double totalMultiplier = 1.0;

		totalMultiplier *= powi(lengthUnit.GetMultiplier(), length);
		totalMultiplier *= powi(timeUnit.GetMultiplier(), time);
		totalMultiplier *= powi(massUnit.GetMultiplier(), mass);

		return ThisUnit(totalMultiplier, label);
	}
	static ThisUnit DerivedUnit(const LengthUnit& lengthUnit, const TimeUnit& timeUnit, const std::string& label = "")
	{
		return DerivedUnit(lengthUnit, timeUnit, MassUnit::BaseUnit(), label);
	}
	static ThisUnit DerivedUnit(const TimeUnit& timeUnit, const MassUnit& massUnit, const std::string& label = "")
	{
		return DerivedUnit(LengthUnit::BaseUnit(), timeUnit, massUnit, label);
	}
	static ThisUnit DerivedUnit(const LengthUnit& lengthUnit, const MassUnit& massUnit, const std::string& label = "")
	{
		return DerivedUnit(lengthUnit, TimeUnit::BaseUnit(), massUnit, label);
	}
	//static ThisUnit DerivedUnit(const MassUnit& massUnit, const std::string& label = "")
	//{
	//	return DerivedUnit(LengthUnit::BaseUnit(), TimeUnit::BaseUnit(), massUnit, label);
	//}

	Unit(const std::string& label = "") // constructs base unit
	{
		Construct(1.0, label);
	}

private:
	Unit(double multiplier, const std::string& label)
	{
		Construct(multiplier, label);
	}

	Unit(double multiplier, const ThisUnit& ref, const std::string& label)
	{
		double rmultiplier = ref.GetMultiplier();

		Construct(multiplier * rmultiplier, label);
	}

public:
	double GetMultiplier() const
	{
		return multiplier;
	}

	void SetLabel(const std::string& label)
	{
		this->label = label;
	}

	const std::string& str() const
	{
		return this->label;
	}

	static double Convert(double value, const ThisUnit& from, const ThisUnit& to)
	{
		double fmultiplier = from.GetMultiplier();
		double tmultiplier = to.GetMultiplier();

		return value * (fmultiplier / tmultiplier);
	}

private:
	void Construct(double multiplier, const std::string& label)
	{
		this->multiplier = multiplier;
		this->label = label;
	}

private:
	double multiplier;
	std::string label;
};

template<int length, int time, int mass>
class Quantity
{
	typedef Unit<length, time, mass> CurrentUnit;
	typedef Quantity<length, time, mass> ThisQuantity;

public:
	Quantity()
	{
		Construct(0.0, CurrentUnit::BaseUnit());
	}
	Quantity(double value, const CurrentUnit& unit = CurrentUnit::BaseUnit())
	{
		Construct(value, unit);
	}

	double ValueIn(const CurrentUnit& ref) const
	{
		return Unit<length, time, mass>::Convert(value, unit, ref);
	}
	double ValueInBase() const
	{
		return ValueIn(CurrentUnit::BaseUnit());
	}

	ThisQuantity& operator+=(const ThisQuantity& rhs)
	{
		value += rhs.ValueIn(unit);

		return *this;
	}
	ThisQuantity& operator-=(const ThisQuantity& rhs)
	{
		value -= rhs.ValueIn(unit);

		return *this;
	}
	ThisQuantity& operator*=(double rhs)
	{
		value *= rhs;

		return *this;
	}

private:
	void Construct(double value, const CurrentUnit& unit)
	{
		this->value = value;
		this->unit = unit;
	}

private:
	double value;
	CurrentUnit unit;

	friend Quantity operator+(const Quantity& lhs, const Quantity& rhs)
	{
		double newValue = lhs.value + rhs.ValueIn(lhs.unit);

		return Quantity(newValue, lhs.unit);
	}
	friend Quantity operator-(const Quantity& op)
	{
		return (-1.0) * op;
	}
	friend Quantity operator-(const Quantity& lhs, const Quantity& rhs)
	{
		return lhs + (-rhs);
	}
	friend Quantity operator*(const Quantity& lhs, double rhs)
	{
		return Quantity(lhs.value * rhs, lhs.unit);
	}
	friend Quantity operator*(double lhs, const Quantity& rhs)
	{
		return rhs * lhs;
	}
	friend Quantity operator*(const Quantity& lhs, int rhs)
	{
		return lhs * (double)rhs;
	}
	friend Quantity operator*(int lhs, const Quantity& rhs)
	{
		return rhs * lhs;
	}
	friend double operator/(const Quantity& lhs, const Quantity& rhs)
	{
		return lhs.value / rhs.ValueIn(lhs.unit);
	}
	friend Quantity operator/(const Quantity& lhs, double rhs)
	{
		return lhs * (1.0 / rhs);
	}
	friend Quantity operator/(const Quantity& lhs, int rhs)
	{
		return lhs / (double)rhs;
	}
	friend bool operator>(const Quantity& lhs, const Quantity& rhs)
	{
		return lhs.ValueInBase() > rhs.ValueInBase();
	}
	friend bool operator<(const Quantity& lhs, const Quantity& rhs)
	{
		return lhs.ValueInBase() < rhs.ValueInBase();
	}
};


typedef Quantity<1, 0, 0> Length;
typedef Quantity<0, 1, 0> Time;
typedef Quantity<0, 0, 1> Mass;
typedef Quantity<1, -1, 0> Velocity;
typedef Quantity<1, -2, 0> Acceleration;
typedef Quantity<3, -2, 0> GravitationalParameter;

typedef Unit<1, 0, 0> LengthUnit;
typedef Unit<0, 1, 0> TimeUnit;
typedef Unit<0, 0, 1> MassUnit;
typedef Unit<1, -1, 0> VelocityUnit;
typedef Unit<1, -2, 0> AccelerationUnit;
typedef Unit<3, -2, 0> GravitationalParameterUnit;

namespace Units
{
	class Metric
	{
	public:
		static const LengthUnit centimeters;
		static const LengthUnit meters;
		static const LengthUnit kilometers;

		static const MassUnit grams;
		static const MassUnit kilograms;
		static const MassUnit tons;

		static const VelocityUnit kmph;
		static const VelocityUnit kmps;
		static const VelocityUnit mps;

		static const AccelerationUnit ms2;

		static const GravitationalParameterUnit gm;
	};

	class Imperial
	{
	public:
		static const LengthUnit inches;
		static const LengthUnit feet;
		static const LengthUnit yards;
		static const LengthUnit miles;

		static const MassUnit ounces;
		static const MassUnit pounds;
		static const MassUnit tons;

		static const VelocityUnit mph;
	};

	class Common
	{
	public:
		static const TimeUnit seconds;
		static const TimeUnit minutes;
		static const TimeUnit hours;
		static const TimeUnit days;
	};
}
