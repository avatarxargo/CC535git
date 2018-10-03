#include "framebuffer.h"
#include "math.h"
#include <iostream>
#include "scene.h"
#include "v3.h"
#include <iostream>

#include "tiffio.h"

using namespace std;

FrameBuffer::FrameBuffer(int u0, int v0, int _w, int _h)
	: Fl_Gl_Window(u0, v0, _w, _h, 0) {

	w = _w;
	h = _h;
	pix = new unsigned int[w*h];
	zbuffer = new float[w*h];
	stencil = new int[w*h];

}

unsigned int* FrameBuffer::getDataPtr() {
	return pix;
}


void FrameBuffer::draw() {

	glDrawPixels(w, h, GL_RGBA, GL_UNSIGNED_BYTE, pix);

}

int FrameBuffer::handle(int event) {

	switch (event)
	{
	case FL_KEYBOARD: {
		KeyboardHandle();
		return 0;
	}
	case FL_KEYUP: {
		KeyUpHandle();
		return 0;
	}
	case FL_MOVE: {
		MouseHandle();
		return 0;
	}
	default:
		break;
	}
	return 0;
}

float FrameBuffer::getXin() {
	int tmp = inx;
	inx = 0;
	return tmp;
}

float FrameBuffer::getYin() {
	int tmp = iny;
	iny = 0;
	return tmp;
}

float FrameBuffer::getZin() {
	int tmp = inz;
	inz = 0;
	return tmp;
}

float FrameBuffer::getIin() {
	int tmp = ini;
	ini = 0;
	return tmp;
}

float FrameBuffer::getJin() {
	int tmp = inj;
	inj = 0;
	return tmp;
}

float FrameBuffer::getUin() {
	int tmp = inu;
	inu = 0;
	return tmp;
}

float FrameBuffer::getPin() {
	int tmp = inp;
	inp = 0;
	return tmp;
}

bool FrameBuffer::getPrintCam() {
	bool tmp = printCam;
	printCam = false;
	return tmp;
}

bool FrameBuffer::getLoadCam() {
	bool tmp = loadCam;
	loadCam = false;
	return tmp;
}

void FrameBuffer::input(int x, int y, int z) {
	inx += x;
	iny += y;
	inz += z;
}
void FrameBuffer::input2(float i, float j, float u, float p) {
	ini += i;
	inj += j;
	inu += u;
	inp += p;
}

void FrameBuffer::resetInput() {
	for (int i = 0; i < 20; ++i) {
		inputs[i] = 0;
	}
}

void FrameBuffer::input(int idx, float value) {
	inputs[idx] = value;
}

void FrameBuffer::inputPlus(int idx, float value) {
	inputs[idx] += value;
}

float FrameBuffer::getInput(int idx) {
	return inputs[idx];
}


void FrameBuffer::KeyUpHandle() {

	int key = Fl::event_key();
	switch (key) {

	default:
		break;
	}
}

void FrameBuffer::KeyboardHandle() {

	int key = Fl::event_key();
	switch (key) {
	case ',': {
		cerr << "INFO: pressed ," << endl;
		break;
	}
	case 'a':
		input(0, -1);
		input(-1, 0, 0);
		break;
	case 'w':
		input(1, 1);
		input(0, 1, 0);
		break;
	case 's':
		input(1, -1);
		input(0, -1, 0);
		break;
	case 'd':
		input(0, 1);
		input(1, 0, 0);
		break;
	case 'e':
		input(2, 1);
		input(0, 0, 1);
		break;
	case 'q':
		input(2, -1);
		input(0, 0, -1);
		break;
	case 'i':
		input2(1, 0, 0, 0);
		break;
	case 'k':
		input2(-1, 0, 0, 0);
		break;
	case 'j':
		input2(0, -1, 0, 0);
		break;
	case 'l':
		input2(0, 1, 0, 0);
		break;
	case 'u':
		input2(0, 0, -1, 0);
		break;
	case 'o':
		input2(0, 0, 1, 0);
		break;
	case 'p':
		input2(0, 0, 0, 1);
		break;
	case ';':
		input2(0, 0, 0, -1);
		break;
	case 'r':
		printCam = true;
		break;
	case 't':
		loadCam = true;
		break;
	case 27:
		cerr << "Closing" << endl;
		exit(0);
		//TODO fix
		break;
	default:
		cerr << "INFO: do not understand keypress" << endl;
	}
}

bool initMouse = false;
int mouseX, mouseY;

void FrameBuffer::MouseHandle() {
	if (!initMouse) {
		initMouse = true;
		mouseX = Fl::event_x();
		mouseY = Fl::event_y();
		return;
	}
	int xpos = Fl::event_x();
	int ypos = Fl::event_y();
	float xdiff = xpos - mouseX;
	float ydiff = ypos - mouseY;
	//input2(-ydiff, 0, 0, 0);
	//input2(0, xdiff, 0, 0);
	input(10, -ydiff);
	input(11, xdiff);
	mouseX = xpos;
	mouseY = ypos;
}

void FrameBuffer::SetPixelGuarded(float u, float v, unsigned int colour) {
	int ui = floorf(u + 0.5);
	int vi = floorf(v + 0.5);
	if (u < 0 || u > w - 1 || v < 0 || v > h - 1)
		return;
	pix[ui + (h - 1 - vi) * w] = colour;
}

void FrameBuffer::SetPixel(float u, float v, unsigned int colour) {
	int ui = floorf(u + 0.5);
	int vi = floorf(v + 0.5);
	pix[ui + (h - 1 - vi) * w] = colour;
}

void FrameBuffer::Set(int u, int v, unsigned int colour) {
	pix[u + (h - 1 - v) * w] = colour;
}

void FrameBuffer::setZ(int u, int v, float z, unsigned int colour) {
	int idx = u + (h - 1 - v) * w;
	if (zbuffer[idx] >= z) {
		zbuffer[idx] = z;
		pix[idx] = colour;
	}
}

void FrameBuffer::setZBlend(int u, int v, float z, unsigned int colour, float alpha) {
	if (alpha <= 0) { return; }
	int idx = u + (h - 1 - v) * w;
	if (zbuffer[idx] >= z) {
		zbuffer[idx] = z;
		//
		V3 myc(pix[idx]);
		V3 newc(colour);
		if (alpha > 1) alpha = 1;
		myc = myc * (1 - alpha) + newc * alpha;
		pix[idx] = myc.getColor();
	}
}

//

void FrameBuffer::refreshColor(unsigned int bgr) {
	for (int uv = 0; uv < w*h; uv++)
		pix[uv] = bgr;
}

void FrameBuffer::refreshDepth(float bgr) {
	for (int uv = 0; uv < w*h; uv++)
		zbuffer[uv] = bgr;
}

void FrameBuffer::refreshStencil(int bgr) {
	for (int uv = 0; uv < w*h; uv++)
		stencil[uv] = bgr;
}

//

void FrameBuffer::drawCircle(float uf, float vf, float r, unsigned int bgr) {
	//round to int
	int ui0 = floorf(uf - r + 0.5);
	int vi0 = floorf(vf - r + 0.5);
	int ui1 = floorf(uf + r + 0.5);
	int vi1 = floorf(vf + r + 0.5);
	float r2 = r * r;

	//clamping
	if (ui0 < 0) { ui0 = 0; }
	if (vi0 < 0) { vi0 = 0; }
	if (ui1 > w - 1) { ui1 = w - 1; }
	if (vi1 > h - 1) { vi1 = h - 1; }

	//render
	for (int u = ui0; u < ui1; ++u) {
		for (int v = vi0; v < vi1; ++v) {
			if ((u - uf)*(u - uf) + (v - vf)*(v - vf) < r2) {
				Set(u, v, bgr);
			}
		}
	}
}

void FrameBuffer::drawRect(float uf, float vf, float w, float h, unsigned int bgr) {
	//round to int
	int ui0 = floorf(uf + 0.5);
	int vi0 = floorf(vf + 0.5);
	int ui1 = floorf(uf + w + 0.5);
	int vi1 = floorf(vf + h + 0.5);

	//clamping
	if (ui0 < 0) { ui0 = 0; }
	if (vi0 < 0) { vi0 = 0; }
	if (ui1 > w - 1) { ui1 = w - 1; }
	if (vi1 > h - 1) { vi1 = h - 1; }

	//render
	for (int u = ui0; u < ui1; ++u) {
		for (int v = vi0; v < vi1; ++v) {
			Set(u, v, bgr);
		}
	}
}

void FrameBuffer::drawTriangle(float uf0, float vf0, float uf1, float vf1, float uf2, float vf2, unsigned int bgr) {
	//round to int
	int ui0 = floorf(uf0 + 0.5);
	int vi0 = floorf(vf0 + 0.5);
	int ui1 = floorf(uf1 + 0.5);
	int vi1 = floorf(vf1 + 0.5);
	int ui2 = floorf(uf2 + 0.5);
	int vi2 = floorf(vf2 + 0.5);

	//clamping
	int umin = fminf(ui2, fminf(ui0, ui1));
	int vmin = fminf(vi2, fminf(vi0, vi1));
	int umax = fmaxf(ui2, fmaxf(ui0, ui1));
	int vmax = fmaxf(vi2, fmaxf(vi0, vi1));
	if (umin < 0) { umin = 0; }
	if (vmin < 0) { vmin = 0; }
	if (umax > w - 1) { umax = w - 1; }
	if (vmax > h - 1) { vmax = h - 1; }

	//render
	for (int u = umin; u < umax; ++u) {
		for (int v = vmin; v < vmax; ++v) {
			int side1 = u * (vf1 - vf0) - v * (uf1 - uf0) - uf0 * vf1 + vf0 * uf1; //0 1
			int side2 = u * (vf2 - vf1) - v * (uf2 - uf1) - uf1 * vf2 + vf1 * uf2; //1 2
			int side3 = u * (vf0 - vf2) - v * (uf0 - uf2) - uf2 * vf0 + vf2 * uf0; //2 0 
			if (side1 > 0 && side2 > 0 && side3 > 0) {
				Set(u, v, bgr);
			}
		}
	}
}

void FrameBuffer::draw3DTriangle(V3 point1, V3 color1, V3 point2, V3 color2, V3 point3, V3 color3, PPC* camera) {
	V3 pp1, pp2, pp3;
	if (!camera->project(point1, pp1))
		return;
	if (!camera->project(point2, pp2))
		return;
	if (!camera->project(point3, pp3))
		return;

	//round to int
	int ui0 = floorf(pp1[0] + 0.5);
	int vi0 = floorf(pp1[1] + 0.5);
	int ui1 = floorf(pp2[0] + 0.5);
	int vi1 = floorf(pp2[1] + 0.5);
	int ui2 = floorf(pp3[0] + 0.5);
	int vi2 = floorf(pp3[1] + 0.5);

	//clamping
	int umin = fminf(ui2, fminf(ui0, ui1)) - 1;
	int vmin = fminf(vi2, fminf(vi0, vi1)) - 1;
	int umax = fmaxf(ui2, fmaxf(ui0, ui1)) + 1;
	int vmax = fmaxf(vi2, fmaxf(vi0, vi1)) + 1;
	if (umin < 0) { umin = 0; }
	if (vmin < 0) { vmin = 0; }
	if (umax > w - 1) { umax = w - 1; }
	if (vmax > h - 1) { vmax = h - 1; }

	//render using Barycentric Coordinates
	for (int u = umin; u < umax; ++u) {
		for (int v = vmin; v < vmax; ++v) {
			float W1 = ((pp2[1] - pp3[1])*(u - pp3[0]) + (pp3[0] - pp2[0])*(v - pp3[1])) /
				((pp2[1] - pp3[1])*(pp1[0] - pp3[0]) + (pp3[0] - pp2[0])*(pp1[1] - pp3[1]));
			float W2 = ((pp3[1] - pp1[1])*(u - pp3[0]) + (pp1[0] - pp3[0])*(v - pp3[1])) /
				((pp2[1] - pp3[1])*(pp1[0] - pp3[0]) + (pp3[0] - pp2[0])*(pp1[1] - pp3[1]));
			float W3 = 1 - W1 - W2;
			//
			//int side1 = u * (pp2[1] - pp1[1]) - v * (pp2[0] - pp1[0]) - pp1[0] * pp2[1] + pp1[1] * pp2[0]; //1 2
			//int side2 = u * (pp3[1] - pp2[1]) - v * (pp3[0] - pp2[0]) - pp2[0] * pp3[1] + pp2[1] * pp3[0]; //2 3
			//int side3 = u * (pp1[1] - pp3[1]) - v * (pp1[0] - pp3[0]) - pp3[0] * pp1[1] + pp3[1] * pp1[0]; //3 1 
			//if (side1 >= 0 && side2 >= 0 && side3 >= 0) {
			if (W1 >= 0 && W2 >= 0 && W3 >= 0) {
				/*float W1 = ((pp2[1] - pp3[1])*(u - pp3[0]) + (pp3[0] - pp2[0])*(v - pp3[1])) /
						((pp2[1] - pp3[1])*(pp1[0] - pp3[0]) + (pp3[0] - pp2[0])*(pp1[1] - pp3[1]));
				float W2 = ((pp3[1] - pp1[1])*(u - pp3[0]) + (pp1[0] - pp3[0])*(v - pp3[1])) /
						((pp2[1] - pp3[1])*(pp1[0] - pp3[0]) + (pp3[0] - pp2[0])*(pp1[1] - pp3[1]));
				float W3 = 1 - W1 - W2;*/
				//
				float z = pp1[2] * W1 + pp2[2] * W2 + pp3[2] * W3;
				V3 col = color1 * W1 + color2 * W2 + color3 * W3;
				setZ(u, v, z, col.getColor());
			}
		}
	}
}

void FrameBuffer::draw3DTriangleTexturedScreenspace(V3 point1, V3 uvw1, V3 point2, V3 uvw2, V3 point3, V3 uvw3, PPC* camera, Texture* tex) {
	V3 pp1, pp2, pp3;
	if (!camera->project(point1, pp1))
		return;
	if (!camera->project(point2, pp2))
		return;
	if (!camera->project(point3, pp3))
		return;

	//round to int
	int ui0 = floorf(pp1[0] + 0.5);
	int vi0 = floorf(pp1[1] + 0.5);
	int ui1 = floorf(pp2[0] + 0.5);
	int vi1 = floorf(pp2[1] + 0.5);
	int ui2 = floorf(pp3[0] + 0.5);
	int vi2 = floorf(pp3[1] + 0.5);

	//clamping
	int umin = fminf(ui2, fminf(ui0, ui1)) - 1;
	int vmin = fminf(vi2, fminf(vi0, vi1)) - 1;
	int umax = fmaxf(ui2, fmaxf(ui0, ui1)) + 1;
	int vmax = fmaxf(vi2, fmaxf(vi0, vi1)) + 1;
	if (umin < 0) { umin = 0; }
	if (vmin < 0) { vmin = 0; }
	if (umax > w - 1) { umax = w - 1; }
	if (vmax > h - 1) { vmax = h - 1; }

	//render using Barycentric Coordinates
	for (int u = umin; u < umax; ++u) {
		for (int v = vmin; v < vmax; ++v) {
			float W1 = ((pp2[1] - pp3[1])*(u - pp3[0]) + (pp3[0] - pp2[0])*(v - pp3[1])) /
				((pp2[1] - pp3[1])*(pp1[0] - pp3[0]) + (pp3[0] - pp2[0])*(pp1[1] - pp3[1]));
			float W2 = ((pp3[1] - pp1[1])*(u - pp3[0]) + (pp1[0] - pp3[0])*(v - pp3[1])) /
				((pp2[1] - pp3[1])*(pp1[0] - pp3[0]) + (pp3[0] - pp2[0])*(pp1[1] - pp3[1]));
			float W3 = 1 - W1 - W2;
			if (W1 >= 0 && W2 >= 0 && W3 >= 0) {
				float z = pp1[2] * W1 + pp2[2] * W2 + pp3[2] * W3;
				V3 uvw = uvw1 * W1 + uvw2 * W2 + uvw3 * W3;
				unsigned int col = tex->getColorNearest(uvw[0], uvw[1]);
				setZ(u, v, z, col);
			}
		}
	}
}

void FrameBuffer::draw3DTriangleTextured(V3 point1, V3 uvw1, V3 point2, V3 uvw2, V3 point3, V3 uvw3, PPC* camera, Texture* tex) {
	V3 pp1, pp2, pp3;
	if (!camera->project(point1, pp1))
		return;
	if (!camera->project(point2, pp2))
		return;
	if (!camera->project(point3, pp3))
		return;

	//round to int
	int ui0 = floorf(pp1[0] + 0.5);
	int vi0 = floorf(pp1[1] + 0.5);
	int ui1 = floorf(pp2[0] + 0.5);
	int vi1 = floorf(pp2[1] + 0.5);
	int ui2 = floorf(pp3[0] + 0.5);
	int vi2 = floorf(pp3[1] + 0.5);

	//clamping
	int umin = fminf(ui2, fminf(ui0, ui1)) - 1;
	int vmin = fminf(vi2, fminf(vi0, vi1)) - 1;
	int umax = fmaxf(ui2, fmaxf(ui0, ui1)) + 1;
	int vmax = fmaxf(vi2, fmaxf(vi0, vi1)) + 1;
	if (umin < 0) { umin = 0; }
	if (vmin < 0) { vmin = 0; }
	if (umax > w - 1) { umax = w - 1; }
	if (vmax > h - 1) { vmax = h - 1; }

	V3 a = camera->horizontal;
	V3 b = camera->vertical;
	V3 c = camera->topleft;
	V3 v1 = point1;
	V3 v2 = point2;
	V3 v3 = point3;
	V3 C = camera->pos;
	M33 abc(a,b,c);
	abc = abc.transpose();
	M33 q = M33(v1-C,v2-C,v3-C).transpose();
	M33 qinv;
	if (!q.inverse(&qinv))
		return;
	q = qinv*abc;
	//cerr << "q:\n" << q << "\n";
	//
	V3 uvw1to2 = uvw2 - uvw1;
	V3 uvw1to3 = uvw3 - uvw1;
	float z1to2 = v2[2] - v1[2];
	float z1to3 = v3[2] - v1[2];

	bool opaque = tex->isOpaque();

	//render using Model Space Coordinates
	for (int u = umin; u < umax; ++u) {
		for (int v = vmin; v < vmax; ++v) {
			int side1 = u * (vi1 - vi0) - v * (ui1 - ui0) - ui0 * vi1 + vi0 * ui1; //0 1
			int side2 = u * (vi2 - vi1) - v * (ui2 - ui1) - ui1 * vi2 + vi1 * ui2; //1 2
			int side3 = u * (vi0 - vi2) - v * (ui0 - ui2) - ui2 * vi0 + vi2 * ui0; //2 0 
			if (side1 < 0 || side2 < 0 || side3 < 0)
				continue;
			V3 uv1 = V3(u, v, 1);
			V3 quv1 = (q * uv1);
			float w = (quv1[0] + quv1[1] + quv1[2]);
			if (w*w < 0.00001) { continue; }
			//1-k-l = kl[0], k = kl[1], l = kl[2]
			V3 kl = quv1 / w;
			//V3 localCoord = uvw1 + uvw1to2 * kl[1] + uvw1to3 * kl[2];
			V3 localCoord = uvw1 * kl[0] + uvw2 * kl[1] + uvw3 * kl[2];
			unsigned int col = tex->getColor(localCoord[0], localCoord[1]);
			if (opaque) {
				setZ(u, v, 1 / w, col);
			} else {
				float alpha = tex->getOpacityNearest(localCoord[0], localCoord[1]);
				setZBlend(u, v, 1 / w, col, alpha);
			}
		}
	}

	/*float z = pp1[2] * W1 + pp2[2] * W2 + pp3[2] * W3;
	V3 uvw = uvw1 * W1 + uvw2 * W2 + uvw3 * W3;
	unsigned int col = tex->getColorNearest(uvw[0], uvw[1]);
	setZ(u, v, z, col);*/
}


void FrameBuffer::fog(float start, float end, V3 color) {
	unsigned int col = color.getColor();
	for (int i = 0; i < h; ++i) {
		for (int j = 0; j < w; ++j) {
			float z = zbuffer[i*w + j];
			if (z < start) {
				continue;
			} else if (z > end) {
				pix[i*w + j] = col;
				continue;
			} else {
				float rate = (z - start) / (end - start);
				V3 myc(pix[i*w + j]);
				myc = myc * (1 - rate) + color * rate;
				pix[i*w + j] = myc.getColor();
			}
		}
	}
}

int iabsi(int i) {
	return i < 0 ? -i : i;
}

V3 FrameBuffer::clampVector(V3 v0) {
	V3 v1(v0);
	if (v1[0] < 0) { v1[0] = 0; }
	if (v1[1] < 0) { v1[1] = 0; }
	if (v1[0] > w - 1) { v1[0] = w - 1; }
	if (v1[1] > h - 1) { v1[1] = h - 1; }
	return v1;
}

void FrameBuffer::drawSegment(V3 v0, V3 c0, V3 v1, V3 c1) {
	V3 v0c = clampVector(v0);
	V3 v1c = clampVector(v1);
	V3 diff = v1c - v0c;
	int steps = iabsi(diff[0]);
	steps = steps > iabsi(diff[1]) ? steps : iabsi(diff[1]);
	diff = diff / (float)steps;
	V3 cdiff = (c1 - c0) / (float)steps;
	int ui, vi;
	for (int i = 0; i < steps; ++i) {
		ui = floorf(v0c[0] + diff[0] * i + 0.5);
		vi = floorf(v0c[1] + diff[1] * i + 0.5f);
		//Set(ui, vi, V3(1, 0, 0).getColor());//(c0 + cdiff).getColor()
		Set(v0c[0] + diff[0] * i, v0c[1] + diff[1] * i, (c0 + cdiff * i).getColor());//
	}
}


int FrameBuffer::Visible(int u, int v, float currz) {
	int uv = (h - 1 - v)*w + u;
	if (zbuffer[uv] < currz)
		return 0;
	zbuffer[uv] = currz;
	return 1;
}

void FrameBuffer::DrawSegment(V3 p0, V3 c0, V3 p1, V3 c1) {

	V3 p1p0 = p1 - p0;
	V3 c1c0 = c1 - c0;
	int pixN;
	if (fabsf(p1p0[0]) > fabsf(p1p0[1])) {
		pixN = (int)(fabsf(p1p0[0]) + 1);
	}
	else {
		pixN = (int)(fabsf(p1p0[1]) + 1);
	}
	for (int stepsi = 0; stepsi < pixN + 1; stepsi++) {
		float fracf = (float)stepsi / (float)(pixN);
		V3 p = p0 + p1p0 * fracf;
		V3 c = c0 + c1c0 * fracf;
		int u = (int)p[0];
		int v = (int)p[1];
		if (u < 0 || u > w - 1 || v < 0 || v > h - 1)
			continue;
		if (!Visible(u, v, p[2]))
			continue;
		Set(u, v, c.getColor());
	}

}

void FrameBuffer::draw3DPoint(V3 point, V3 color, PPC* ppc, int size) {
	V3 pp;
	if (!ppc->project(point, pp))
		return;

	if (pp[0] < 0.0f || pp[0] >= (float)w || pp[1] < 0.0f || pp[1] >= (float)h)
		return;
	int u = (int)pp[0];
	int v = (int)pp[1];
	//cerr << u << "," << v << endl;
	drawRect(u - size / 2, v - size / 2, u + size / 2, v + size / 2, color.getColor());
}

void FrameBuffer::draw3DSegment(V3 p0, V3 c0, V3 p1, V3 c1, PPC* ppc) {
	V3 pp0, pp1;
	if (!ppc->project(p0, pp0))
		return;
	if (!ppc->project(p1, pp1))
		return;

	DrawSegment(pp0, c0, pp1, c1);
}

// load a tiff image to pixel buffer
void FrameBuffer::LoadTiff(char* fname) {
	TIFF* in = TIFFOpen(fname, "r");
	if (in == NULL) {
		cerr << fname << " could not be opened" << endl;
		return;
	}

	int width, height;
	TIFFGetField(in, TIFFTAG_IMAGEWIDTH, &width);
	TIFFGetField(in, TIFFTAG_IMAGELENGTH, &height);
	if (w != width || h != height) {
		w = width;
		h = height;
		delete[] pix;
		pix = new unsigned int[w*h];
		size(w, h);
		glFlush();
		glFlush();
	}

	if (TIFFReadRGBAImage(in, w, h, pix, 0) == 0) {
		cerr << "failed to load " << fname << endl;
	}

	TIFFClose(in);
}

// save as tiff image
void FrameBuffer::SaveAsTiff(char *fname) {

	TIFF* out = TIFFOpen(fname, "w");

	if (out == NULL) {
		cerr << fname << " could not be opened" << endl;
		return;
	}

	TIFFSetField(out, TIFFTAG_IMAGEWIDTH, w);
	TIFFSetField(out, TIFFTAG_IMAGELENGTH, h);
	TIFFSetField(out, TIFFTAG_SAMPLESPERPIXEL, 4);
	TIFFSetField(out, TIFFTAG_BITSPERSAMPLE, 8);
	TIFFSetField(out, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
	TIFFSetField(out, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
	TIFFSetField(out, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);

	for (uint32 row = 0; row < (unsigned int)h; row++) {
		TIFFWriteScanline(out, &pix[(h - row - 1) * w], row);
	}

	TIFFClose(out);
}


