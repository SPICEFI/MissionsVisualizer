#include "Main.h"

int main()
{
	std::ofstream fout;
	fout.open("out.txt");

	try
	{
		App app;
		app.SetLoggingFile("error.log");

		app.LoadKernel("data/meta.tm");

		app.SetReferenceFrame(Frame::ECLIPJ2000); // Set reference frame to ECLIPJ200, default is J2000
		app.SetDefaultUnits(App::UT_DEFAULT); // This is set by default
		//app.SetDefaultUnits(App::UT_METRIC);
		//app.SetDefaultUnits(App::UT_IMPERIAL);

		app.LoadChildren(SpaceObject("Solar system barycenter"), true, true);

		std::vector<KernelData> kernels = CSpiceUtil::GetLoadedKernels();

		fout << "Loaded kernels:" << std::endl;
		for(size_t i = 0; i < kernels.size(); i++)
		{
			KernelData kData = kernels[i];
			fout << "\t" << kData.filename << " (" << kData.type << ")" << std::endl;
		}
		fout << std::endl;

		size_t objectsCount = app.GetObjectsLength();
		for(size_t i = 0; i < objectsCount; i++)
		{
			const SpaceObject& obj = app.GetObjectByIndex(i);
			fout << obj.GetName() << " summary:" << std::endl;

			try
			{
				const SpaceBody& body = dynamic_cast<const SpaceBody&>(obj);

				fout << "Bulk parameters:" << std::endl;

				if(body.HasParameter(SpaceBody::BP_MASS))
					fout << "\tMass: " << body.GetMass().ValueIn(app.MassUnit()) << " " << app.MassUnit().str() << std::endl;
				if(body.HasParameter(SpaceBody::BP_GM))
					fout << "\tGM: " << body.GetGM().ValueIn(app.GMUnit()) << " " << app.GMUnit().str() << std::endl;
				if(body.HasParameter(SpaceBody::BP_ACC))
					fout << "\tg: " << body.GetSurfaceAcceleration().ValueIn(app.AccelerationUnit()) << " " << app.AccelerationUnit().str() << std::endl;
				if(body.HasParameter(SpaceBody::BP_RADIUS))
				{
					Length radius = body.GetRadius();
					std::array<Length, 3> radii = body.GetRadii();
					fout << "\tRadius: " << radius.ValueIn(app.LengthUnit()) << " (" << radii[0].ValueIn(app.LengthUnit()) << ", " << radii[1].ValueIn(app.LengthUnit()) << ", " << radii[2].ValueIn(app.LengthUnit()) << ") " << app.LengthUnit().str() << std::endl;
				}

				fout << std::endl;
			}
			catch(const std::bad_cast&)
			{

			}

			Date t("Aug 17 2000 15:51:01 UTC-5");
			t += Time(1.0, Units::Common::days);

			Window spkCoverage = obj.GetCoverage();
			std::vector<Interval> spkIntervals = spkCoverage.GetIntervals();

			fout << "SPK state:" << std::endl;

			fout << "\tCoverage:" << std::endl;
			if(spkIntervals.size() == 0)
			{
				fout << "\t\tObject does not contain any state data" << std::endl;
			}

			for(size_t i = 0; i < spkIntervals.size(); i++)
			{
				Interval interval = spkIntervals[i];

				Date begin(interval.GetLeft());
				Date end(interval.GetRight());

				fout << "\t\t" << begin.AsString() << " - " << end.AsString() << std::endl;
			}
			fout << std::endl;

			fout << "\t" << t.AsString() << " relative to " << app.GetReferenceFrame().GetName() << ":" << std::endl;

			if(spkCoverage.IsIncluded(t.AsDouble()))
			{
				Vector3T<Length> pos = obj.GetPosition(t, SpaceObject::SSB, app.GetReferenceFrame());
				Vector3T<Velocity> vel = obj.GetVelocity(t, SpaceObject::SSB, app.GetReferenceFrame());

				fout << "\t\tPos: " << pos.Length().ValueIn(app.LengthUnit()) << " (" << pos.x.ValueIn(app.LengthUnit()) << ", " << pos.y.ValueIn(app.LengthUnit()) << ", " << pos.z.ValueIn(app.LengthUnit()) << ") " << app.LengthUnit().str() << std::endl;
				fout << "\t\tVel: " << vel.Length().ValueIn(app.VelocityUnit()) << " (" << vel.x.ValueIn(app.VelocityUnit()) << ", " << vel.y.ValueIn(app.VelocityUnit()) << ", " << vel.z.ValueIn(app.VelocityUnit()) << ") " << app.VelocityUnit().str() << std::endl;
			}
			else
			{
				fout << "\t\tNo state data on this epoch" << std::endl;
			}
			fout << std::endl;

			try
			{
				const SpaceBody& body = dynamic_cast<const SpaceBody&>(obj);

				fout << "PCK orientation:" << std::endl;

				if(body.HasDefaultFrame())
				{					
					Frame bodyFrame = body.GetDefaultFrame();

					fout << "(using frame " << bodyFrame.GetName() << ")" << std::endl;

					if(bodyFrame.HasAvailableData())
					{
						bool dataAvailableAtT = true;

						fout << "\tCoverage:" << std::endl;
						if(bodyFrame.HasLimitedCoverage())
						{
							Window pckCoverage = bodyFrame.GetCoverage();
							std::vector<Interval> pckIntervals = pckCoverage.GetIntervals();

							dataAvailableAtT = pckCoverage.IsIncluded(t.AsDouble());

							if(pckIntervals.size() == 0)
							{
								fout << "\t\tObject does not contain any orientation data" << std::endl;
							}

							for(size_t i = 0; i < pckIntervals.size(); i++)
							{
								Interval interval = pckIntervals[i];

								Date begin(interval.GetLeft());
								Date end(interval.GetRight());

								fout << "\t\t" << begin.AsString() << " - " << end.AsString() << std::endl;
							}
							fout << std::endl;
						}
						else
						{
							fout << "\t\tUnlimited" << std::endl;
						}

						fout << "\t" << t.AsString() << app.GetReferenceFrame().GetName() << ":" << std::endl;

						if(dataAvailableAtT)
						{
							Vector3 axisX = bodyFrame.AxisX(t, app.GetReferenceFrame());
							Vector3 axisY = bodyFrame.AxisY(t, app.GetReferenceFrame());
							Vector3 axisZ = bodyFrame.AxisZ(t, app.GetReferenceFrame());

							fout << "\t\tX axis: (" << axisX.x << ", " << axisX.y << ", " << axisX.z << ")" << std::endl;
							fout << "\t\tY axis: (" << axisY.x << ", " << axisY.y << ", " << axisY.z << ")" << std::endl;
							fout << "\t\tZ axis: (" << axisZ.x << ", " << axisZ.y << ", " << axisZ.z << ")" << std::endl;

							const Matrix3x3& matrix = bodyFrame.GetTransformationMatrix(t, app.GetReferenceFrame());
							const float* m = matrix.Get();

							fout << "\t\tTransformation matrix:" << std::endl;
							for(int row = 0; row < 3; row++)
							{
								fout << "\t\t\t";
								for(int col = 0; col < 3; col++)
								{
									fout << *(m + 3 * row + col) << "\t";
								}
								fout << std::endl;
							}
						}
						else
						{
							fout << "\t\tNo orientation data available on this epoch" << std::endl;
						}
					}
					else
					{
						fout << "\tFrame does not contain any orientation data" << std::endl;
					}
				}
				else
				{
					fout << "\tObject does not contain any orientation data" << std::endl;
				}
			}
			catch(const std::bad_cast&)
			{

			}

			fout << "=======================================================================" << std::endl;
			fout << std::endl;
		}

		std::cout << "All is fine. See out.txt for results" << std::endl;
	}
	catch(const std::exception& ex)
	{
		std::cout << std::endl;

		std::cout << "Error encountered:" << std::endl;
		std::cout << ex.what() << std::endl;
		std::cout << "See details in the error log file" << std::endl;
	}

	std::getchar();
	return 0;
}
