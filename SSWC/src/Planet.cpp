#include "Planet.h"
#include <sstream>
//double scale = 0.000002f;
double scale = 0.00000004f;
double extraScale = 0.00004f;

double distanceScale = 0.00000004f;

Planet::Planet(const SpaceBody& body, TGA* texture, const SpaceObject& obj, const Frame& frame, HDC hdc) :body(body), trajectory(obj, frame)
{
	this->texture = texture;
	ID = body.GetSpiceId();
	marked = false;
	font = Font(hdc);
	if (body.GetSpiceId() == 399)
	{
		Date t("Aug 17 2000 15:51:01 UTC-5");
		Time time(356, Units::Common::days);
		trajectory.SetDateParams(t, time, 1000);
	}
}


Planet::~Planet()
{

}

void Planet::Render(Date t, App& app)
{
	if (body.GetSpiceId() == SUN_SPICE_ID)
		RenderAsSun(t, app);
	else
		RenderAsPlanet(t, app);
}

//const Vector3& Planet::GetPosition(Date t, App& app)
Vector3 Planet::GetPosition(Date t, App& app)
{
	Vector3T <Length> pos = body.GetPosition(t, SpaceObject::SSB, app.GetReferenceFrame());
	Vector3 position(pos.x.ValueIn(app.LengthUnit())* distanceScale, pos.y.ValueIn(app.LengthUnit()) * distanceScale, pos.z.ValueIn(app.LengthUnit()) * distanceScale);

	return position;
}

void Planet::RenderAsSun(Date t, App& app)
{
	if (simpleRender)
	{
		scale = 0.000002f;
	}
	else
	{
		scale = 0.00000004f;
	}

	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);

	Vector3T <Length> pos = body.GetPosition(t, SpaceObject::SSB, app.GetReferenceFrame());

	Vector3 position(pos.x.ValueIn(app.LengthUnit())* distanceScale, pos.y.ValueIn(app.LengthUnit()) * distanceScale, pos.z.ValueIn(app.LengthUnit()) * distanceScale);

	Frame bodyFrame = body.GetDefaultFrame();
	const Matrix4x4& matrix = bodyFrame.GetTransformationMatrix(t, app.GetReferenceFrame());
	matrix.GetColumnMajor(rotationMatrix);

	glPushName(ID);
	glPushMatrix();

	glTranslatef(position.x, position.y, position.z);
	glMultMatrixf(rotationMatrix);


	glBindTexture(GL_TEXTURE_2D, texture->getTextureHandle());
	//render as a GLU sphere quadric object
	GLUquadricObj* quadric = gluNewQuadric();
	gluQuadricTexture(quadric, true);
	gluQuadricNormals(quadric, GLU_SMOOTH);

	Length radius = body.GetRadius();
	gluSphere(quadric, radius.ValueIn(app.LengthUnit()) * scale, 30, 30);
	gluDeleteQuadric(quadric);

	glPopMatrix();
	glPopName();

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
}

void Planet::RenderAsPlanet(Date t, App& app)
{
	Length len(100, LengthUnit::BaseUnit());
	if (marked)
	{
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHTING);
		scale = 0.00005f;
	}
	else
	{
		scale = 0.00000004f;
	}
	if (simpleRender)
	{
		scale = extraScale;
	}

	Vector3T <Length> pos = body.GetPosition(t, SpaceObject::SSB, app.GetReferenceFrame());

	Vector3 position(pos.x.ValueIn(app.LengthUnit())* distanceScale, pos.y.ValueIn(app.LengthUnit()) * distanceScale, pos.z.ValueIn(app.LengthUnit()) * distanceScale);

	Frame bodyFrame = body.GetDefaultFrame();
	const Matrix4x4& matrix = bodyFrame.GetTransformationMatrix(t, app.GetReferenceFrame());
	matrix.GetColumnMajor(rotationMatrix);

	glPushName(ID);
	glPushMatrix();

	glTranslatef(position.x, position.y, position.z);
	glMultMatrixf(rotationMatrix);

	glBindTexture(GL_TEXTURE_2D, texture->getTextureHandle());
	//render as a GLU sphere quadric object
	GLUquadricObj* quadric = gluNewQuadric();
	gluQuadricTexture(quadric, true);
	gluQuadricNormals(quadric, GLU_SMOOTH);

	Length radius = body.GetRadius();
	gluSphere(quadric, radius.ValueIn(app.LengthUnit()) * scale, 30, 30);
	gluDeleteQuadric(quadric);
	//trajectory.Render();
	if (body.GetSpiceId() == 399)
	{
		float lineWidth = 5.0f;
		float red = 255.0f;
		float green = 255.0f;
		float blue = 255.0f;

		glLineWidth(lineWidth);
		glColor3f(red, green, blue);

		glDisable(GL_LIGHTING);

		glBegin(GL_LINE_STRIP);
		glEnable(GL_LINE_SMOOTH);
		std::vector<Vector3> path = trajectory.GetTrajectory(t, Units::Metric::meters);
		for (int i = 0; i < path.size(); i++)
			glVertex3f(path.at(i).x, path.at(i).y, path.at(i).z);
		glDisable(GL_LINE_SMOOTH);
		glEnd();
		glEnable(GL_LIGHTING);
	}
	//if (!simpleRender)
	//{
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHTING);
		glColor3f(255.0, 255.0, 255.0);

		
		/*std::ostringstream strs;
		strs << radius.ValueIn(app.LengthUnit()) << std::endl;
		std::string str = strs.str();*/

		std::string data = /*"Name: " +*/ body.GetName();// + "\nRadius: " +str;
		glRasterPos3f(0, 0, radius.ValueIn(app.LengthUnit()) * scale);
		font.glPrint(data.c_str());

		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);
	//}

	glPopMatrix();
	glPopName();

	if (marked)
	{
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);
	}
	/*		glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);
	glColor3f(255.0, 255.0, 255.0);
	glRasterPos3f(0, 0, 0);
	font.glPrint("Putin lives here");
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);*/
}