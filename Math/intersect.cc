#include <cmath>
#include "intersect.h"
#include "constants.h"
#include "tools.h"

/* | algo           | difficulty | */
/* |----------------+------------| */
/* | BSPherePlane   |          1 | */
/* | BBoxBBox       |          2 | */
/* | BBoxPlane      |          4 | */

// @@ TODO: test if a BSpheres intersects a plane.
//! Returns :
//   +IREJECT outside
//   -IREJECT inside
//    IINTERSECT intersect

int BSpherePlaneIntersect(const BSphere *bs, Plane *pl) {
	/* =================== PUT YOUR CODE HERE ====================== */
	float result=0;
	float aux = pl->signedDistance(bs->m_centre);
	result = fabs(aux);
	if(result <= bs->m_radius){ // El plano y la esfera intersecan.
		return IINTERSECT;
	}
	else{ //no intersecan, hay que ver si la esfera esta fuera o dentro.
		if(pl->whichSide(bs->m_centre)==-1){
			return -IREJECT;
		}	
		else{//El caso de que wichSide devuelva 0 nunca se dara
			return +IREJECT;
		}	
	}
	/* =================== END YOUR CODE HERE ====================== */
}


// @@ TODO: test if two BBoxes intersect.
//! Returns :
//    IINTERSECT intersect
//    IREJECT don't intersect

int  BBoxBBoxIntersect(const BBox *bba, const BBox *bbb ) {
	/* =================== PUT YOUR CODE HERE ====================== */
	if ((bba->m_min[0] > bbb->m_max[0] || bbb->m_min[0] > bba->m_max[0]) ||
	(bba->m_min[1] > bbb->m_max[1] || bbb->m_min[1] > bba->m_max[1]) ||
	(bba->m_min[2] > bbb->m_max[2] || bbb->m_min[2] > bba->m_max[2])){ //Si se verifica, no intersectan
    	return IREJECT;
	}
  	else{
		return IINTERSECT;
	}
	/* =================== END YOUR CODE HERE ====================== */
}

// @@ TODO: test if a BBox and a plane intersect.
//! Returns :
//   +IREJECT outside
//   -IREJECT inside
//    IINTERSECT intersect

int  BBoxPlaneIntersect (const BBox *theBBox, Plane *thePlane) {
	/* =================== PUT YOUR CODE HERE ====================== */
	int contador = 0;
	Vector3 v;
	for (int i = 0; i <= 1; i++){
		for (int j = 0; j <= 1; j++){
			for (int k = 0; k <= 1; k++){
				/*Si las variables i, j y k son 0 se coge el valor del minimo del bounding box. Si son 1, se coge el valor maximo.
				i-> eje x; j-> eje y; k-> eje z
				De esta forma comprobamos los 8 vertices del bounding box.
				*/
				v[0] = i ? theBBox->m_min[0] : theBBox->m_max[0];
				v[1] = j ? theBBox->m_min[1] : theBBox->m_max[1];
				v[2] = k ? theBBox->m_min[2] : theBBox->m_max[2];
				contador = contador + thePlane->whichSide(v);
			}
		}
	}
	if(contador==8){
		return IREJECT;
	}
	else if (contador==-8){
		return -IREJECT;
	}
	else{
		return IINTERSECT;
	}
	/* =================== END YOUR CODE HERE ====================== */
}

// Test if two BSpheres intersect.
//! Returns :
//    IREJECT don't intersect
//    IINTERSECT intersect

int BSphereBSphereIntersect(const BSphere *bsa, const BSphere *bsb ) {

	Vector3 v;
	v = bsa->m_centre - bsb->m_centre;
	float ls = v.dot(v);
	float rs = bsa->m_radius + bsb->m_radius;
	if (ls > (rs * rs)) return IREJECT; // Disjoint
	return IINTERSECT; // Intersect
}


// Test if a BSpheres intersect a BBox.
//! Returns :
//    IREJECT don't intersect
//    IINTERSECT intersect

int BSphereBBoxIntersect(const BSphere *sphere, const BBox *box) {

	float d;
	float aux;
	float r;

	r = sphere->m_radius;
	d = 0;

	aux = sphere->m_centre[0] - box->m_min[0];
	if (aux < 0) {
		if (aux < -r) return IREJECT;
		d += aux*aux;
	} else {
		aux = sphere->m_centre[0] - box->m_max[0];
		if (aux > 0) {
			if (aux > r) return IREJECT;
			d += aux*aux;
		}
	}

	aux = (sphere->m_centre[1] - box->m_min[1]);
	if (aux < 0) {
		if (aux < -r) return IREJECT;
		d += aux*aux;
	} else {
		aux = sphere->m_centre[1] - box->m_max[1];
		if (aux > 0) {
			if (aux > r) return IREJECT;
			d += aux*aux;
		}
	}

	aux = sphere->m_centre[2] - box->m_min[2];
	if (aux < 0) {
		if (aux < -r) return IREJECT;
		d += aux*aux;
	} else {
		aux = sphere->m_centre[2] - box->m_max[2];
		if (aux > 0) {
			if (aux > r) return IREJECT;
			d += aux*aux;
		}
	}
	if (d > r * r) return IREJECT;
	return IINTERSECT;
}

// Test if a Triangle intersects a ray.
//! Returns :
//    IREJECT don't intersect
//    IINTERSECT intersect

int IntersectTriangleRay(const Vector3 & P0,
						 const Vector3 & P1,
						 const Vector3 & P2,
						 const Line *l,
						 Vector3 & uvw) {
	Vector3 e1(P1 - P0);
	Vector3 e2(P2 - P0);
	Vector3 p(crossVectors(l->m_d, e2));
	float a = e1.dot(p);
	if (fabs(a) < Constants::distance_epsilon) return IREJECT;
	float f = 1.0f / a;
	// s = l->o - P0
	Vector3 s(l->m_O - P0);
	float lu = f * s.dot(p);
	if (lu < 0.0 || lu > 1.0) return IREJECT;
	Vector3 q(crossVectors(s, e1));
	float lv = f * q.dot(l->m_d);
	if (lv < 0.0 || lv > 1.0) return IREJECT;
	uvw[0] = lu;
	uvw[1] = lv;
	uvw[2] = f * e2.dot(q);
	return IINTERSECT;
}

/* IREJECT 1 */
/* IINTERSECT 0 */

const char *intersect_string(int intersect) {

	static const char *iint = "IINTERSECT";
	static const char *prej = "IREJECT";
	static const char *mrej = "-IREJECT";
	static const char *error = "IERROR";

	const char *result = error;

	switch (intersect) {
	case IINTERSECT:
		result = iint;
		break;
	case +IREJECT:
		result = prej;
		break;
	case -IREJECT:
		result = mrej;
		break;
	}
	return result;
}
