#ifndef VECTOR_HPP_
#define VECTOR_HPP_

#include <math.h>
namespace base {

#define MAX_VECTOR_LEN (1<<32)

class Vector2D
{
public:

	int x() {
		return x_;
	}

	int y() {
		return y_;
	}

	void set_x(int x) {
		x_ = x;
	}
	
	void set_y(int y) {
		y_ = y;
	}

	Vector2D(int x = 0 , int y = 0) :
		x_(x), y_(y) {
	}

	void zero() {
		x_ = 0; y_= 0;
	}
	
	bool is_zero() const {
		return x_== 0 && y_ == 0;
	}

	int length() const {
		return sqrt(length2());
	}

	int length2() const {
		return x_*x_ + y_* y_;
	}

	int Dot(const Vector2D& v2) const {
		return x_ * v2.x_ +	y_ * v2.y_ ;
	}

	int sign(const Vector2D& v2) const {
		return Dot(v2);			
	}
	
	int distance(const Vector2D& v2) const {
		return sqrt(distance2(v2));
	}

	int distance2(const Vector2D& v2) const {
		return (x_ - v2.x_)*(x_ - v2.x_) + (y_ - v2.y_)*(y_ - v2.y_) ;
	}

	Vector2D get_reverse() const {
		return Vector2D(-x_, -y_);
	}
	
	Vector2D operator+ (const Vector2D &rhs) {
		return Vector2D(x_+ rhs.x_, y_+ rhs.y_);
	}

	const Vector2D& operator+=(const Vector2D &rhs) {
		x_ += rhs.x_;
		y_ += rhs.y_;
		return *this;
	}

	Vector2D operator- (const Vector2D &rhs) {
		return Vector2D(x_- rhs.x_, y_- rhs.y_);
	}

	const Vector2D& operator-=(const Vector2D &rhs) {
		x_ -= rhs.x_;
		y_ -= rhs.y_;
		return *this;
	}

	bool operator== (const Vector2D& rhs) const {
		return x_ == rhs.x_ && y_ == rhs.y_ ;
	}

	const Vector2D& operator*= (int e) {
		x_ *= e;
		y_ *= e;
		return *this;
	}

	const Vector2D& operator/= (int e) {
		x_ /= e;
		y_ /= e;
		return *this;
	}

private:
	int x_;
	int y_;
};

}

#endif
