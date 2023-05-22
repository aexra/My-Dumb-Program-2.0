#include "Edge.h"

vector<Edge> Edge::edges = { };

Edge::Edge(Vertex* _Begin, Vertex* _End)
{
	begin = _Begin;
	end = _End;
	RecalcPosition();
}

POINT Edge::RecalcPosition()
{
	crossPos.x = abs(begin->GetCenter().x - end->GetCenter().x);
	crossPos.y = abs(begin->GetCenter().y - end->GetCenter().y);
	return crossPos;
}

POINT Edge::getpt(BOOL toRecalc)
{
	return (toRecalc? RecalcPosition() : crossPos);
}

bool operator==(const Edge& a, const Edge& b)
{
	if (a.begin == b.begin && a.end == b.end || a.begin == b.end && a.end == b.begin)
		return 1;
	return 0;
}
bool operator!=(const Edge& a, const Edge& b)
{
	if (a.begin == b.begin && a.end == b.end || a.begin == b.end && a.end == b.begin)
		return 0;
	return 1;
}
bool operator==(const Edge& a, const pair<Vertex*, Vertex*>& b)
{
	if (a.begin == b.first && a.end == b.second || a.begin == b.second && a.end == b.first)
		return 1;
	return 0;
}
bool operator!=(const Edge& a, const pair<Vertex*, Vertex*>& b)
{
	if (a.begin == b.first && a.end == b.second || a.begin == b.second && a.end == b.first)
		return 0;
	return 1;
}
