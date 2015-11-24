#pragma once
#include "Traectory.h"

Traectory::Traectory() {};
void Traectory::PushBack(Vector3 newPos)
{
	path.push_back(newPos);
}

void Traectory::Render()
{
	glLineWidth(lineWidth);
	glColor3f(red, green, blue);

	glDisable(GL_LIGHTING);
	
	glBegin(GL_LINE_STRIP);
	glEnable(GL_LINE_SMOOTH);
	for (int i = 0; i < path.size(); i++)
		glVertex3f(path.at(i).x, path.at(i).y, path.at(i).z);
	glDisable(GL_LINE_SMOOTH);
	glEnd();
	
	glEnable(GL_LIGHTING);
}