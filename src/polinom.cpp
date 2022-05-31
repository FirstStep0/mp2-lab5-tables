#include "../include/polinom.h"
//monom
int32_t monom::DegToIndex(int32_t degx, int32_t degy, int32_t degz) {
	return (degx << 16) ^ (degy << 8) ^ (degz);
	//return (degx*base+degy)*base+degz
}

int32_t monom::sumoverflow(monom& i1, monom& i2) {
	return DegToIndex(over(i1.degx(), i2.degx()), over(i1.degy(), i2.degy()), over(i1.degz(), i2.degz()));
};

int32_t monom::over(int32_t deg1, int32_t deg2) {
	int32_t deg = deg1 + deg2;
	if (deg >= base)throw string("overflow monom index");
	return deg;
}

//polinom
void polinom::unique() {
	iterator<monom> it = this->begin();
	while (it != end()) {
		iterator<monom> next_it = it->next;
		if ((next_it) != end() && (*it) == (*next_it)) {
			(*it) += (*next_it);
			erase(next_it);
		}
		else {
			iterator<monom> old_it = it++;
			type_coef m = (*old_it).coef;
			m = (m >= 0) ? m : -m;
			if (m < eps) {
				erase(old_it);
			}
		}
	}
}

polinom operator*(const polinom& p, const polinom& q) {
	polinom r;
	polinom* _p = const_cast<polinom*>(&p);
	polinom* _q = const_cast<polinom*>(&q);
	for (auto it1 = _p->begin(); it1 != _p->end(); ++it1) {
		for (auto it2 = _q->begin(); it2 != _q->end(); ++it2) {
			r.push_back((*it1) * (*it2));
		}
	}
	r.sort();
	r.unique();
	return r;
};