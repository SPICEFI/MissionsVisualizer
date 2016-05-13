#include "Planet.h"
#include <sstream>
//double scale = 0.0004f;
//double scale = 0.00000003f;
//double extraScale = 0.00004f;

//double distanceScale = 0.00000003f;

Planet::Planet(const SpaceBody& body, TGA* texture, const SpaceObject& obj, const Frame& frame, HDC hdc) :body(body), trajectory(obj, frame, Units::Metric::kilometers, distanceScale)
{
	this->texture = texture;
	ID = body.GetSpiceId();
	marked = false;
	font = Font(hdc);
	/*if (body.GetSpiceId() == 399)
	{
		Date t("Mar 29 2016 01:03:52.99996036291100000 (UTC+0)");
		Time time(10, Units::Common::days);
		trajectory.SetStaticParams(t - time, t + time, 100);
	}*/
	if (body.GetSpiceName() == "MOON")
	{
		Date t("Mar 29 2016 01:03:52.99996036291100000 (UTC+0)");
		Time time(10, Units::Common::days);
		trajectory.SetStaticParams(t - time, t + time, 100);
		trajectory.SetRelativeToFrameCenter();
	}
}


Planet::~Planet()
{

}

void Planet::Render(Date t, App& app)
{
	if (body.GetSpiceId() == SUN_SPICE_ID)
	{
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHTING);
		RenderObject(t, app);
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);
	}
	else
		RenderObject(t, app);
}

Vector3 Planet::GetPosition(Date t, App& app)
{
	Vector3T <Length> pos = body.GetPosition(t, SpaceObject::SSB, app.GetReferenceFrame());
	Vector3 position(pos.x.ValueIn(app.LengthUnit())* distanceScale, pos.y.ValueIn(app.LengthUnit()) * distanceScale, pos.z.ValueIn(app.LengthUnit()) * distanceScale);

	return position;
}

void Planet::RenderObject(Date t, App& app)
{
	/*if (simpleRender)
	{
		scale = 0.000002f;
	}
	else
	{
		scale = 0.00000004f;
	}
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);*/
	//if (marked){
	//	scale = 0.00005f;
	//}else{
	//	scale = 0.00004f;
	//}

//#pragma region CHECK_IF_SUN
//	if (body.GetSpiceId() == SUN_SPICE_ID){
//		scale = 0.0000004f;
//	}
//#pragma endregion

#pragma region Object_Positioning
	Vector3T <Length> pos = body.GetPosition(t, SpaceObject::SSB, app.GetReferenceFrame());
	Vector3 position(pos.x.ValueIn(app.LengthUnit())* distanceScale, pos.y.ValueIn(app.LengthUnit()) * distanceScale, pos.z.ValueIn(app.LengthUnit()) * distanceScale);

	Frame bodyFrame = body.GetDefaultFrame();
	const Matrix4x4& matrix = bodyFrame.GetTransformationMatrix(t, app.GetReferenceFrame());
	matrix.GetColumnMajor(rotationMatrix);

#pragma region DRAW_INFO_BILLBOARD
	if (clicked)
	{
		Length radius = body.GetRadius();
		DrawBillboard(position, radius.ValueIn(app.LengthUnit()) * scale);
	}
#pragma endregion

	glPushName(ID);
	glPushMatrix();

#pragma region Trajectory_Rendering
	/*glBindTexture(GL_TEXTURE_2D, texture->getTextureHandle());
	if (body.GetSpiceName() == "MOON");
		RenderTrajectory(t);*/
#pragma endregion

	glTranslatef(position.x, position.y, position.z);
	glMultMatrixf(rotationMatrix);
#pragma endregion

#pragma region Render_As_Glu_Sphere
	if (marked)
	{
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHTING);
	}

	glBindTexture(GL_TEXTURE_2D, texture->getTextureHandle());
	//render as a GLU sphere quadric object
	GLUquadricObj* quadric = gluNewQuadric();
	gluQuadricTexture(quadric, true);
	gluQuadricNormals(quadric, GLU_SMOOTH);

	Length radius = body.GetRadius();
	gluSphere(quadric, radius.ValueIn(app.LengthUnit()) * scale, 30, 30);
	gluDeleteQuadric(quadric);

#pragma endregion

#pragma region Planet_Info_Writing
	glDisable(GL_LIGHTING);
	glColor3f(255.0, 255.0, 255.0);

	std::string data;

	if (marked)
	{
		std::ostringstream strs;
		strs << "Position: (" << pos.x.ValueIn(app.LengthUnit()) << app.LengthUnit().str() << ", " << pos.y.ValueIn(app.LengthUnit()) 
			 << app.LengthUnit().str() <<  ", " << pos.z.ValueIn(app.LengthUnit()) << app.LengthUnit().str() 
			 << ") " << app.GetReferenceFrame().GetName() << std::endl;
		data = body.GetName() + " " + strs.str();
	}
	else
	{
		data = body.GetName();
	}
	glRasterPos3f(0, 0, radius.ValueIn(app.LengthUnit()) * 1.1 * scale);
	font.glPrint(data.c_str());
	glEnable(GL_LIGHTING);
#pragma endregion


	glPopMatrix();
	glPopName();

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
}

void Planet::DrawBillboard(Vector3& planetPosition, double radius)
{


}

void Planet::ShowMessageBoxWithInfo(App& app)
{
	std::ostringstream strs;
	strs << body.GetRadius().ValueIn(app.LengthUnit()) << app.LengthUnit().str() << " " << std::endl;
	std::string str = strs.str();

	std::string res = "Name: " + body.GetName() + "\nRadius: " + str;

	strs.str(std::string());
	strs.clear();
	str.clear();
	strs << body.GetMass().ValueInBase() << app.MassUnit().str() << " " << std::endl;
	str = strs.str();
	res += "Mass: "  + str;

	strs.str(std::string());
	strs.clear();
	str.clear();
	strs << body.GetGM().ValueInBase() << app.GMUnit().str() << " " << std::endl;
	str = strs.str();
	res += "Gravitational Parameter: " + str;

	strs.str(std::string());
	strs.clear();
	str.clear();
	strs << body.GetSurfaceAcceleration().ValueInBase() << app.AccelerationUnit().str() << " " << std::endl;
	str = strs.str();
	res += "Surface Acceleration: " + str;

	MessageBox(NULL, res.c_str(), "Information", MB_OK);
}

void Planet::RenderTrajectory(Date t, float lineWidth, float red, float green, float blue)
{
	glLineWidth(lineWidth);
	glColor3f(red, green, blue);

	glDisable(GL_LIGHTING);

	glBegin(GL_LINE_STRIP);
	glEnable(GL_LINE_SMOOTH);
	if (trajectory.StaticDefined())
	{
		const std::vector<Vector3>& path = trajectory.GetStaticTrajectory();
		for (int i = 0; i < path.size(); i++)
			glVertex3f(path.at(i).x * distanceScale, path.at(i).y * distanceScale, path.at(i).z * distanceScale);
	}
	glDisable(GL_LINE_SMOOTH);
	glEnd();
	glEnable(GL_LIGHTING);
}