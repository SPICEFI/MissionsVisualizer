#include "Planet.h"

//double scale = 0.000002f;
double scale = 0.00004f;

double distanceScale = 0.00000004f;

Planet::Planet(const SpaceBody& body, TGA* texture) :body(body)
{
	this->texture = texture;
	ID = body.GetSpiceId();
	marked = false;
}


Planet::~Planet()
{

}

void Planet::Render(Date t, App& app)
{
	if (marked)
	{
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHTING);
		scale = 0.00005f;
	}
	else
	{
		scale = 0.00004f;
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

	glPopMatrix();
	glPopName();

	glLineWidth(1);			// размер точек
	glColor3f(255.0, 255.0, 255.0);		// текущий цвет примитивов
	trajectory.PushBack(position);
	trajectory.Render();

	if (marked)
	{
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);
	}
}

const Vector3& Planet::GetPosition(Date t, App& app)
{
	Vector3T <Length> pos = body.GetPosition(t, SpaceObject::SSB, app.GetReferenceFrame());
	Vector3 position(pos.x.ValueIn(app.LengthUnit())* distanceScale, pos.y.ValueIn(app.LengthUnit()) * distanceScale, pos.z.ValueIn(app.LengthUnit()) * distanceScale);

	return position;
}