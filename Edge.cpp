#include "Edge.h"

extern HWND FieldWnd;

vector<Edge*> Edge::edges = { };
ULONG Edge::lastID = 800;

Edge::Edge(Vertex* _Begin, Vertex* _End)
{
	begin = _Begin;
	end = _End;
	BUTTONPARAMS btp;
	btp.alignv = aligns::center;
	btp.visibleRadius = 10;
	btp.textCol = V3(190, 14, 14);
	btn = new BUTTON(FieldWnd, "X", V3(crossPos.x, crossPos.y, 0),
		id = (lastID++), V3(30, 30, 0), btp);
	RecalcPosition();
	btn->Hide();
}
POINT Edge::RecalcPosition()
{
	crossPos.x = abs(begin->GetCenter().x + end->GetCenter().x) / 2 - btn->transform.size.x / 2;
	crossPos.y = abs(begin->GetCenter().y + end->GetCenter().y) / 2 - btn->transform.size.y / 2;
	btn->MoveControl(V3(crossPos.x, crossPos.y, 0));
	btn->Invalidate();
	return crossPos;
}
POINT Edge::getpt(BOOL toRecalc)
{
	return (toRecalc? RecalcPosition() : crossPos);
}
BUTTON* Edge::getbtn()
{
	return btn;
}
void Edge::DeleteEdge(Edge* e)
{
	e->begin->Disconnect(e->end->GetID());
	e->end->Disconnect(e->begin->GetID());
	BUTTON::DeleteButton(e->btn);
	Edge::edges.erase(find(Edge::edges.begin(), Edge::edges.end(), e));
	delete e;
	InvalidateRect(FieldWnd, NULL, FALSE);
}

//bool operator==(const Edge& a, const Edge& b)
//{
//	if (a.begin == b.begin && a.end == b.end || a.begin == b.end && a.end == b.begin)
//		return 1;
//	return 0;
//}
//bool operator!=(const Edge& a, const Edge& b)
//{
//	if (a.begin == b.begin && a.end == b.end || a.begin == b.end && a.end == b.begin)
//		return 0;
//	return 1;
//}
//bool operator==(const Edge& a, const pair<Vertex*, Vertex*>& b)
//{
//	if (a.begin == b.first && a.end == b.second || a.begin == b.second && a.end == b.first)
//		return 1;
//	return 0;
//}
//bool operator!=(const Edge& a, const pair<Vertex*, Vertex*>& b)
//{
//	if (a.begin == b.first && a.end == b.second || a.begin == b.second && a.end == b.first)
//		return 0;
//	return 1;
//}
