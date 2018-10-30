/**
 *	Title:	ppc.cpp
 *	Author: David Hrusa
 *	Date:	9/14/2018
 *	Desc:	A Perspective Projection Camera
 */

#include "ppc.h"

#include "framebuffer.h"

PPC::PPC(float hfov, int _w, int _h) : horizontal(1.0f, 0.0f, 0.0f), vertical(0.0f, -1.0f, 0.0f),
pos(0.0f, 0.0f, 0.0f), w(_w), h(_h) {
	float hfovr = hfov * 3.14159f / 180.0f;
	float f = -(float)w / (2.0f *tanf(hfovr / 2.0f));
	topleft = V3(-(float)w / 2.0f, (float)h / 2.0f, f);
}

PPC::PPC(V3 _hor, V3 _ver, V3 _tl, V3 _pos) {
	horizontal = _hor;
	vertical = _ver;
	topleft = _tl;
	pos = _pos;
}

void PPC::setTo(PPC ppc) {
	horizontal = ppc.horizontal;
	vertical = ppc.vertical;
	topleft = ppc.topleft;
	pos = ppc.pos;
}

void PPC::changeFocalLength(float units) {
	topleft = topleft + forward()*units;
}

void PPC::pan(float angle) {
	V3 axis = vertical * -1;
	V3 hor2 = horizontal.vectorRotate(axis, angle);
	V3 ver2 = vertical.vectorRotate(axis, angle);
	V3 tl2 = topleft.vectorRotate(axis, angle);
	//
	horizontal = hor2;
	vertical = ver2;
	topleft = tl2;
}

void PPC::panFlat(float angle, V3 normal) {
	V3 axis = normal;
	V3 hor2 = horizontal.vectorRotate(axis, angle);
	V3 ver2 = vertical.vectorRotate(axis, angle);
	V3 tl2 = topleft.vectorRotate(axis, angle);
	//
	horizontal = hor2;
	vertical = ver2;
	topleft = tl2;
}

void PPC::tilt(float angle) {
	V3 axis = horizontal;
	V3 hor2 = horizontal.vectorRotate(axis, angle);
	V3 ver2 = vertical.vectorRotate(axis, angle);
	V3 tl2 = topleft.vectorRotate(axis, angle);
	//
	horizontal = hor2;
	vertical = ver2;
	topleft = tl2;
}

void PPC::roll(float angle) {
	V3 axis = vertical % horizontal;
	V3 hor2 = horizontal.vectorRotate(axis, angle);
	V3 ver2 = vertical.vectorRotate(axis, angle);
	V3 tl2 = topleft.vectorRotate(axis, angle);
	//
	horizontal = hor2;
	vertical = ver2;
	topleft = tl2;
}

void PPC::translate(V3 trans) {
	pos = pos + (horizontal%vertical).norm()*trans[2]
		+ (vertical*-1).norm()*trans[1]
		+ (horizontal).norm()*trans[0];
}
void PPC::translateFlat(V3 trans) {
	V3 f = forward();
	f[1] = 0;
	f = f.norm();
	pos = pos + f * trans[0]
		+ (vertical*-1)%f.norm()*trans[2]
		+ (horizontal).norm()*trans[1];
}

int PPC::project(V3 p, V3& pp) {
	/*M33 m, minv;
	m = m.setColumn(0, horizontal);
	m = m.setColumn(1, vertical);
	m = m.setColumn(2, topleft);
	m.inverse(&minv);*/
	V3 q = abcinv * (p - pos);
	if (q[2] < 0.0f)
	{
		//cerr << "not projected:" << q[2] << " [" << p << "]" << endl;
		return 0;
	}
	//cerr << "projected" << endl;
	pp[2] = q[2];
	pp[0] = q[0] / q[2];
	pp[1] = q[1] / q[2];
	return 1;
}

int PPC::contained(V3 p) {
	/*M33 m, minv;
	m = m.setColumn(0, horizontal);
	m = m.setColumn(1, vertical);
	m = m.setColumn(2, topleft);
	m.inverse(&minv);*/
	V3 q = abcinv * (p - pos);
	if (q[2] < 0.0f)
	{
		return 0;
	}
	float val = q[0] / q[2];
	if (val < 0 || val > w) {
		return 0;
	}
	val = q[1] / q[2];
	if (val < 0 || val > h) {
		return 0;
	}
	return 1;
}


float PPC::getFocalLength() {
	//cross product between hor and vert gives us direction.
	//dot with norm gives us length.
	return topleft * (horizontal % vertical).norm();
}

V3 PPC::forward() {
	return (horizontal % vertical).norm();
}

V3 PPC::up() {
	return (vertical).norm();
}

void PPC::postitionAndOrient(V3 C1, V3 L1, V3 vpv) {
	V3 vd1 = (L1 - C1).norm();
	V3 a1 = (vd1 % vpv).norm() * horizontal.len();
	V3 b1 = (vd1 % a1).norm() * vertical.len();
	V3 c1 = vd1 * getFocalLength() - b1 * ((float)h / 2.0f) - a1 * ((float)w / 2.0f);
}

void PPC::interpolate(PPC* target, PPC* set, float rate) {
	V3 pdiff = target->pos - pos;
	V3 fwv = ((forward()*(1 - rate) + target->forward() * rate)).norm();
	V3 upv = ((up()*(1 - rate) + target->up() * rate)).norm();
	float foclen = getFocalLength()*(1 - rate) + target->getFocalLength()*rate;
	float horlen = horizontal.len() * (1 - rate) + target->horizontal.len()*rate;
	float verlen = vertical.len() * (1 - rate) + target->vertical.len()*rate;
	//
	set->horizontal = (upv % fwv).norm() * horlen;
	set->vertical = (fwv % set->horizontal).norm() * verlen;
	set->topleft = (fwv * foclen) - (set->horizontal.norm() * ((float)w / 2.0f)) - (set->vertical.norm() * ((float)h / 2.0f));
	set->pos = pos + pdiff * rate;
}

//void PPC::PositionAndOrient(V3 C1, V3 L1, V3 vpv) {
//
//	V3 vd1 = (L1 - C1).UnitVector();
//	V3 a1 = (vd1 ^ vpv).UnitVector() * a.Length();
//	V3 b1 = (vd1 ^ a1).UnitVector() * b.Length();
//	V3 c1 = vd1 * GetFocalLength() - b1 * ((float)h / 2.0f) - a1 * ((float)w / 2.0f);
//
//	C = C1;
//	a = a1;
//	b = b1;
//	c = c1;
//
//}

void PPC::visualize(PPC* viewer, FrameBuffer* canvas, float scale) {
	V3 topv = pos + topleft * scale;
	//frame
	canvas->draw3DSegment(pos, V3(1, 1, 1), topv, V3(1, 1, 1), viewer);
	canvas->draw3DSegment(pos, V3(1, 0, 1), topv + horizontal * w * scale, V3(1, 0, 1), viewer);
	canvas->draw3DSegment(pos, V3(1, 0, 1), topv + vertical * h * scale, V3(1, 0, 1), viewer);
	canvas->draw3DSegment(pos, V3(1, 0, 1), topv + (horizontal * w + vertical * h) * scale, V3(1, 0, 1), viewer);
	canvas->draw3DSegment(topv, V3(1, 1, 0), topv + horizontal * w * scale, V3(1, 1, 0), viewer);
	canvas->draw3DSegment(topv, V3(1, 1, 0), topv + vertical * h * scale, V3(1, 1, 0), viewer);
	canvas->draw3DSegment(topv + horizontal * w * scale, V3(1, 1, 0), topv + (horizontal * w + vertical * h) * scale, V3(1, 1, 0), viewer);
	canvas->draw3DSegment(topv + vertical * h * scale, V3(1, 1, 0), topv + (horizontal * w + vertical * h)  * scale, V3(1, 1, 0), viewer);
}

void PPC::visualize(PPC* viewer, FrameBuffer* canvas, FrameBuffer* view) {
	float scale = 0.1;
	V3 topv = pos + topleft * scale;
	//frame
	canvas->draw3DSegment(pos, V3(1, 1, 1), topv, V3(1, 1, 1), viewer);
	canvas->draw3DSegment(pos, V3(1, 0, 1), topv + horizontal * w * scale, V3(1, 0, 1), viewer);
	canvas->draw3DSegment(pos, V3(1, 0, 1), topv + vertical * h * scale, V3(1, 0, 1), viewer);
	canvas->draw3DSegment(pos, V3(1, 0, 1), topv + (horizontal * w + vertical * h) * scale, V3(1, 0, 1), viewer);
	canvas->draw3DSegment(topv, V3(1, 1, 0), topv + horizontal * w * scale, V3(1, 1, 0), viewer);
	canvas->draw3DSegment(topv, V3(1, 1, 0), topv + vertical * h * scale, V3(1, 1, 0), viewer);
	canvas->draw3DSegment(topv + horizontal * w * scale, V3(1, 1, 0), topv + (horizontal * w + vertical * h) * scale, V3(1, 1, 0), viewer);
	canvas->draw3DSegment(topv + vertical * h * scale, V3(1, 1, 0), topv + (horizontal * w + vertical * h)  * scale, V3(1, 1, 0), viewer);
	//canvas
	unsigned int* data = view->getDataPtr();
	int scl = 4;
	float resscl = 1/(float)scl;
	int lenw = view->w*resscl;
	int lenh = view->h*resscl;
	for (int y = 0; y < lenh; ++y) {
		for (int x = 0; x < lenw; ++x) {
			int idx = ((lenh-y-1) * lenw *scl + x)*scl;
			V3 color = V3(data[idx]);//V3(1, 0, 0);/ 
			V3 ppos = horizontal * w * scale * (x / (float)lenw)
					+ vertical * h * scale * (y / (float)lenh);
			//canvas->draw3DPoint(topv+ppos,color,viewer,10);
			canvas->draw3DSegment(topv + ppos, color, topv + ppos + V3(1, 1, 1), color, viewer);
		}
	}
}

void PPC::saveToFile(char* path) {
	ofstream myfile;
	myfile.open(path);
	myfile << this << endl;
	myfile.close();
	cerr << "Camera info stored to: \"" << path << "\"\n";
}

void PPC::loadFromFile(char* path) {
	ifstream myfile;
	PPC* me = this;
	myfile.open(path);
	myfile >> me;
	myfile.close();
	cerr << "Camera info loaded from: \"" << path << "\"\n";
}

istream& operator>>(istream& is, PPC* c) {
	is >> c->horizontal >> c->vertical >> c->topleft >> c->pos;
	return is;
}

ostream& operator<<(ostream& ostr, const PPC *c) {
	return ostr << c->horizontal << "\n" << c->vertical << "\n" << c->topleft << "\n" << c->pos;
}
