#ifndef VECT2_HPP
#define VECT2_HPP

#include <iostream>
#include <stdexcept>

class vect2 {
int _x, _y;

vect2(int x=0, int y=0): _x(x), _y(y){}
vect2(const vect2 &o) : _x(o._x), _y(o._y) {}

int& operator[] (size_t i) {
if(i>1) throw std::out_of_range ("index out of range");
return (i==0 ? _x:_y);
}
const int& operator[] (size_t i) {
	if(i>1) throw std::out_of_range ("index out of range");
return (i==0 ? _x : _y);
}
vect2& operator++ () {++_x; ++_y; return *this;}
vect2 operator++ (int) { vect2 t(*this); ++(*this); return t;}
vect2& operator-- () {--_x; --_y; return *this;}
vect2 (int) operator--{vect2 t(*this); --(*this); return t;}


vect2 operator+(const vect2 &o) const {return vect2 (_x+o._x, _y+o._y);}
vect2 operator- (const vect2 &o) const {return vect2(_x-o._x, _y-o._y);}
vect2 operator- () const {return vect2 (-_X, -_y);}
vect2 operator* (int s) const {return vect2(_x*s, _y*s);}

friend vect2 operator* (int s, const vect2& v) {return v*s;}

vect2& operator+= (const vect2 &o) {_x+=o._x; _y+=o._y; return *this;}
vect2& operator-= (const vect2& o) {_x-=o._x; _y-=o._y; return *this;}
vect2& operator*= (int s) {-x*=s; _y*=s; return *this;}

bool operator ==(const vect2& o) const {return_x==o._x &&_y==o._y;}
bool operator!= (const vect2& o) const {return !(*this==o);}

friend std::ostream& operator<<(std::ostream& os, const vect2& v){
return os << "{" << v._x<<","<<v._y<<"}";
}
};
#endif	
