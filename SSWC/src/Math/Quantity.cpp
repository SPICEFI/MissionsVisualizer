#include "Quantity.h"

double powi(double val, int exp)
{
	if(exp == 0)
		return 1.0;

	if(exp < 0)
		return 1.0 / powi(val, -exp);

	double result = 1.0;

	for(int power = 0; power < exp; power++)
	{
		result *= val;
	}

	return result;
}

// Common
const TimeUnit Units::Common::seconds = TimeUnit::BaseUnit("sec");
const TimeUnit Units::Common::minutes = TimeUnit::ScaledUnit(60.0, Units::Common::seconds, "min");
const TimeUnit Units::Common::hours = TimeUnit::ScaledUnit(60.0, Units::Common::minutes, "hr");
const TimeUnit Units::Common::days = TimeUnit::ScaledUnit(24.0, Units::Common::hours, "days");

// Metric
const LengthUnit Units::Metric::meters = LengthUnit::BaseUnit("m");
const LengthUnit Units::Metric::centimeters = LengthUnit::ScaledUnit(0.01, Units::Metric::meters, "cm");
const LengthUnit Units::Metric::kilometers = LengthUnit::ScaledUnit(1000.0, Units::Metric::meters, "km");

const MassUnit Units::Metric::kilograms = MassUnit::BaseUnit("kg");
const MassUnit Units::Metric::grams = MassUnit::ScaledUnit(0.001, Units::Metric::kilograms, "g");
const MassUnit Units::Metric::tons = MassUnit::ScaledUnit(1000.0, Units::Metric::kilograms, "t");

const VelocityUnit Units::Metric::kmph = VelocityUnit::DerivedUnit(Units::Metric::kilometers, Units::Common::hours, "km/h");
const VelocityUnit Units::Metric::kmps = VelocityUnit::DerivedUnit(Units::Metric::kilometers, Units::Common::seconds, "km/s");
const VelocityUnit Units::Metric::mps = VelocityUnit::DerivedUnit(Units::Metric::meters, Units::Common::seconds, "m/s");

const AccelerationUnit Units::Metric::ms2 = AccelerationUnit::DerivedUnit(Units::Metric::meters, Units::Common::seconds, "m/s^2");

// Imperial
const LengthUnit Units::Imperial::inches = LengthUnit::ScaledUnit(0.0254, Units::Metric::meters, "in");
const LengthUnit Units::Imperial::feet = LengthUnit::ScaledUnit(0.3048, Units::Metric::meters, "ft");
const LengthUnit Units::Imperial::yards = LengthUnit::ScaledUnit(3.0, Units::Imperial::feet, "yd");
const LengthUnit Units::Imperial::miles = LengthUnit::ScaledUnit(5280.0, Units::Imperial::feet, "mi");

const MassUnit Units::Imperial::pounds = MassUnit::ScaledUnit(453.59237, Units::Metric::grams, "lb");
const MassUnit Units::Imperial::ounces = MassUnit::ScaledUnit(1.0/16.0, Units::Imperial::pounds, "oz");
const MassUnit Units::Imperial::tons = MassUnit::ScaledUnit(2240.0, Units::Imperial::pounds, "t");

const VelocityUnit Units::Imperial::mph = VelocityUnit::DerivedUnit(Units::Imperial::miles, Units::Common::hours, "mph");

// Common
const GravitationalParameterUnit Units::Metric::gm = GravitationalParameterUnit::DerivedUnit(Units::Metric::kilometers, Units::Common::seconds, "km3/s2");
