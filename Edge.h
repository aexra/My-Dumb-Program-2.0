#pragma once

#include "Header.h"
#include "Vertex.h"

class Edge
{
private:
public:
	Vertex* begin;
	Vertex* end;

	void DrawDscnctCross();
};

