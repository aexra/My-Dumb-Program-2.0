#pragma once

#include "Header.h"
#include "Vertex.h"

class Edge
{
private:
	POINT crossPos;
public:
	Vertex* begin;
	Vertex* end;

	Edge(Vertex* _Begin, Vertex* _End);
	void RecalcPosition();
	void DrawDscnctCross();
};

