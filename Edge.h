#pragma once

#include "Header.h"
#include "Vertex.h"
#include "Button.h"

class Edge
{
private:
	POINT crossPos;
	BUTTON* btn;
	static ULONG lastID;
public:
	ULONG id;
	Vertex* begin;
	Vertex* end;

	Edge(Vertex* _Begin, Vertex* _End);
	POINT RecalcPosition();
	POINT getpt(BOOL toRecalc = 0);
	BUTTON* getbtn();
	
	static vector<Edge*> edges;
	static void DeleteEdge(Edge* p_e);
};

//bool operator==(const Edge& a, const Edge& b);
//bool operator!=(const Edge& a, const Edge& b);
//bool operator==(const Edge& a, const pair<Vertex*, Vertex*>& b);
//bool operator!=(const Edge& a, const pair<Vertex*, Vertex*>& b);