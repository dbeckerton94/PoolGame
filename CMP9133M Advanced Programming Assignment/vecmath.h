/*------------------------------------------------------------------------
  Header for Some Vector Classes
  ------------------------------------------------------------------------*/
#ifndef vecmat_h_included
#define vecmat_h_included

#include <math.h>

/*------------------------------------------------------------------------
	vec2 : 2d Vector
  ------------------------------------------------------------------------*/
template <class D>

class vec2
{
public:
	D 	elem[2];

public:
    vec2(){}
    vec2(D x, D y){elem[0]=x; elem[1]=y;}
    vec2(D x){elem[0]=elem[1]=x;}

    D operator()(int x) const {return elem[x];}
    D &operator()(int x) {return elem[x];}

    vec2 operator *(const D x) const {vec2 res(*this); res.elem[0]*=x; res.elem[1]*=x; return res;}
    vec2 operator /(const D x) const {vec2 res(*this); res.elem[0]/=x; res.elem[1]/=x; return res;}
    vec2 operator +(const vec2 &x) const {vec2 res(*this); res.elem[0]+=x.elem[0]; res.elem[1]+=x.elem[1]; return res;}
    vec2 operator -(const vec2 &x) const {vec2 res(*this); res.elem[0]-=x.elem[0]; res.elem[1]-=x.elem[1]; return res;}
    vec2 operator -() const {vec2 res(*this); res.elem[0] = - res.elem[0]; res.elem[1] = -res.elem[1]; return res;}
    vec2 &operator *=(const D x) {elem[0]*=x; elem[1]*=x; return (*this);}
    vec2 &operator /=(const D x) {elem[0]/=x; elem[1]/=x; return (*this);}
    vec2 &operator +=(const vec2 &x) {elem[0]+=x.elem[0]; elem[1]+=x.elem[1]; return (*this);}
    vec2 &operator -=(const vec2 &x) {elem[0]-=x.elem[0]; elem[1]-=x.elem[1]; return (*this);}
    bool operator ==(const vec2 &x) const {return((elem[0] == x.elem[0])&&(elem[1] == x.elem[1]));}

    D Magnitude(void) const {return(sqrt((elem[0]*elem[0])+(elem[1]*elem[1])));}
    D Magnitude2(void) const {return((elem[0]*elem[0])+(elem[1]*elem[1]));}
    D Normalise(void) { D x = Magnitude(); elem[0]/=x; elem[1]/=x; return x;}
    vec2 Normalised(void) const {vec2 res(*this); res.Normalise(); return res;}

    D Dot(const vec2 &x) const {return ( (elem[0]*x.elem[0]) + (elem[1]*x.elem[1]) );}
};

/*------------------------------------------------------------------------
	vec3 : 3d Vector
  ------------------------------------------------------------------------*/
template <class D>
class vec3
{
public:
	D 	elem[3];

public:
    vec3(){}
    vec3(D x, D y, D z){elem[0]=x; elem[1]=y;elem[2]=z;}
    vec3(D x){elem[0]=elem[1]=elem[2]=x;}
	~vec3() {}

    D operator()(int x) const {return elem[x];}
    D &operator()(int x) {return elem[x];}

    vec3 operator *(const D x) const {vec3 res(*this); res.elem[0]*=x; res.elem[1]*=x; res.elem[2]*=x; return res;}
    vec3 operator /(const D x) const {vec3 res(*this); res.elem[0]/=x; res.elem[1]/=x; res.elem[2]/=x; return res;}
    vec3 operator +(const vec3 &x) const {vec3 res(*this); res.elem[0]+=x.elem[0]; res.elem[1]+=x.elem[1]; res.elem[2]+=x.elem[2]; return res;}
    vec3 operator -(const vec3 &x) const {vec3 res(*this); res.elem[0]-=x.elem[0]; res.elem[1]-=x.elem[1]; res.elem[2]-=x.elem[2]; return res;}
   	vec3 operator -() const { vec3 res(*this); res.elem[0] = -res.elem[0]; res.elem[1] = -res.elem[1]; res.elem[2] = -res.elem[2]; return res;}
	vec3 &operator *=(const D x) {elem[0]*=x; elem[1]*=x; elem[2]*=x; return (*this);}
    vec3 &operator /=(const D x) {elem[0]/=x; elem[1]/=x; elem[2]/=x; return (*this);}
    vec3 &operator +=(const vec3 &x) {elem[0]+=x.elem[0]; elem[1]+=x.elem[1]; elem[2]+=x.elem[2]; return (*this);}
    vec3 &operator -=(const vec3 &x) {elem[0]-=x.elem[0]; elem[1]-=x.elem[1]; elem[2]-=x.elem[2]; return (*this);}

    D Magnitude(void) const {return(sqrt((elem[0]*elem[0])+(elem[1]*elem[1])+(elem[2]*elem[2])));}
    D Magnitude2(void) const {return((elem[0]*elem[0])+(elem[1]*elem[1])+(elem[2]*elem[2]));}
    D Normalise(void) { D x = Magnitude(); elem[0]/=x; elem[1]/=x; elem[2]/=x; return x;}
    vec3 Normalised(void) const {vec3 res(*this); res.Normalise(); return res;}

    D Dot(const vec3 &x) const {return ( (elem[0]*x.elem[0]) + (elem[1]*x.elem[1]) + (elem[2]*x.elem[2]) );}
    vec3 Cross(const vec3 &x) const
    {
    	vec3 res;
    	res.elem[0] = elem[1]*x.elem[2] - elem[2]*x.elem[1];
    	res.elem[1] = elem[2]*x.elem[0] - elem[0]*x.elem[2];
    	res.elem[2] = elem[0]*x.elem[1] - elem[1]*x.elem[0];
		return res;
    }
};

#endif