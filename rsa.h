#include <stdio.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <math.h>
#include <complex>

using namespace std;

typedef complex<double> base;

void get_input(vector <int> &a, vector <int> &b) {
	string as, bs;
	ifstream inp("inp.txt");
    if (inp.is_open()) {
        getline(inp, as);
        getline(inp, bs);
    }
    reverse(as.begin(), as.end());
    reverse(bs.begin(), bs.end());
    for (int i = 0; i < as.length(); i++) {
        a.push_back(as[i] - 48);
    }
    for (int i = 0; i < bs.length(); i++) {
        b.push_back(bs[i] - 48);
    }
}

void vect_set(vector <int> &res, string s) {
    res.resize(0);
    reverse(s.begin(), s.end());
    for (int i = 0; i < s.length(); i++) {
        res.push_back(s[i] - 48);
    }
}

void vect_uni(vector <int> &vect) {
	int ind = vect.size() - 1;
	while (ind >= 0 && vect[ind] == 0) {
		vect.pop_back();
		ind--;
	}
    if (vect.size() == 0) {
        vect.push_back(0);
    }
}

void vect_repr(vector <int> &vect, string sep) {
    for (int i = vect.size() - 1; i >= 0; i--) {
        cout << vect[i] << sep;
    }
    cout << endl;
}

void vect_shift(vector <int> &vect, string direction) {
    if (direction == "right") {
        for (int i = 1; i < vect.size(); i++) {
            vect[i - 1] = vect[i];
        }
        vect.pop_back();
    }
    else {
        vect.push_back(0);
        for (int i = vect.size() - 1; i > 0; i--) {
            vect[i] = vect[i - 1];
        }
        vect[0] = 0;
    }
    vect_uni(vect);
    
}

void vect_copy(vector<int> &a, vector<int> &b) {
    b.resize(0);
    b.resize(a.size());
    for (int i = 0; i < a.size(); i++) {
        b[i] = a[i];
    }
}

int comp(vector <int> &a, vector <int> &b) {
    if (a.size() > b.size()) {
        return 1;
    }
    if (a.size() < b.size()) {
        return -1;
    }
    int ind = a.size() - 1;
    while (ind != -1 && a[ind] == b[ind]) {
        ind--;
    }
    if (ind == -1) {
        return 0;
    }
    if (a[ind] > b[ind]) {
        return 1;
    }
    if (a[ind] < b[ind]) {
        return -1;
    }
    return -2;
}

void fft (vector<base> &a, bool invert) {
	int n = (int)a.size();
	if (n == 1)  return;
 
	vector<base> a0(n / 2),  a1(n / 2);
	for (int i = 0, j = 0; i < n; i += 2, ++j) {
		a0[j] = a[i];
		a1[j] = a[i + 1];
	}
	fft (a0, invert);
	fft (a1, invert);
 
	double ang = 2 * M_PI / n * (invert ? -1 : 1);
	base w(1),  wn(cos(ang), sin(ang));
	for (int i = 0; i < n / 2; ++i) {
		a[i] = a0[i] + w * a1[i];
		a[i + n / 2] = a0[i] - w * a1[i];
		if (invert)
			a[i] /= 2,  a[i + n / 2] /= 2;
		w *= wn;
	}
}

void inc(vector<int> &a) {
    int ind = 0;
    while (a[ind] == 9) {
        a[ind] = 0;
        ind++;
    }
    if (ind == a.size()) {
        a.push_back(1);
    }
    else {
        a[ind]++;
    }
}

void dec(vector<int> &a) {
    int ind = 0;
    while (a[ind] == 0) {
        a[ind] = 9;
        ind++;
    }
    a[ind]--;
    vect_uni(a);
}

void add(vector<int> &a, vector<int> &b, vector<int> &res) {
    res.resize(max(a.size(), b.size()));
    for (int i = 0; i < a.size(); i++) {
        res[i] = a[i];
    }
    for (int i = 0; i < b.size(); i++) {
        res[i] += b[i];
    }
    int carry = 0;
    res.push_back(0);
	for (size_t i = 0; i < res.size(); ++i) {
		res[i] += carry;
		carry = res[i] / 10;
		res[i] %= 10;
	}
	vect_uni(res);
}

void sub(vector<int> &a, vector<int> &b, vector<int> &res) {
    res.resize(a.size());
    for (int i = 0; i < a.size(); i++) {
        res[i] = a[i];
    }
    for (int i = 0; i < b.size(); i++) {
        res[i] -= b[i];
    }
    int carry = 0;
	for (int i = 0; i < res.size(); ++i) {
		res[i] += carry;
        if (res[i] < 0) {
            carry = (res[i] - 9) / 10;
            res[i] = (10 + (res[i] % 10)) % 10;
        }
        else {
            carry = 0;
        }
	}
	vect_uni(res);
}

void multiply(vector<int> &a, vector<int> &b, vector<int> &res) {
	vector<base> fa (a.begin(), a.end()),  fb (b.begin(), b.end());
	size_t n = 1;
	while (n < max (a.size(), b.size()))  n <<= 1;
	n <<= 1;
	fa.resize (n),  fb.resize (n);
 
	fft (fa, false),  fft (fb, false);
	for (size_t i = 0; i < n; ++i)
		fa[i] *= fb[i];
	fft (fa, true);
    res.resize(0);
	res.resize (n);
	for (size_t i = 0; i < n; ++i)
		res[i] = int (fa[i].real() + 0.5);

	int carry = 0;
	for (size_t i = 0; i < n; ++i) {
		res[i] += carry;
		carry = res[i] / 10;
		res[i] %= 10;
	}
	vect_uni(res);
}

void division(vector<int> &a, vector<int> &b, vector <int> &res) {
    vector<int> rem;
    vector<int> cursub, resadd;
    res.resize(0);
    res.resize(a.size());
    res.push_back(0);
    resadd.push_back(1);
    vect_copy(a, rem);
    vect_copy(b, cursub);
    while (comp(a, cursub) > -1) {
        vect_shift(cursub, "left");
        vect_shift(resadd, "left");
    }
    vect_shift(cursub, "right");
    vect_shift(resadd, "right");
    while (comp(rem, b) != -1) {
        if (comp(rem, cursub) > -1) {
            sub(rem, cursub, rem);
            add(res, resadd, res);
        }
        else {
            vect_shift(cursub, "right");
            vect_shift(resadd, "right");
        }
    }
    vect_uni(res);
}

void remainder(vector<int> &a, vector<int> &b, vector<int> &res) {
    vector<int> temp;
    int diff = a.size() - b.size();
    temp.resize(a.size());
    res.resize(a.size());
    for (int i = 0; i < a.size(); i++) {
        res[i] = a[i];
    }
    for (int i = 0; i < diff; i++) {
        temp[i] = 0;
    }
    for (int i = diff; i < a.size(); i++) {
        temp[i] = b[i - diff];
    }
    while (comp(res, b) != -1) {
        if (comp(res, temp) > -1) {
            sub(res, temp, res);
        }
        else {
            vect_shift(temp, "right");
        }
    }
}

void rev(vector<int> a, vector<int> p, vector <int> &res) {
    int ind = 1;
    vector<int> zero, one, b, max_ab, t, temp;
    vector<int> q, Q, P, qp, Qp, Pp, qpp, Qpp, Ppp;
    zero.push_back(0);
    one.push_back(1);
    vect_copy(p, b);
    if (comp(a, b) > -1) {
        vect_copy(a, max_ab);
    }
    else {
        vect_copy(b, max_ab);
    }
    vect_copy(zero, Ppp);
    vect_copy(one, Pp);
    vect_copy(one, Qpp);
    vect_copy(zero, Qp);
    vect_copy(zero, qpp);
    vect_copy(zero, qp);
    
    while (comp(Qp, max_ab) != 0 && comp(Pp, max_ab) != 0) {
        
        division(a, b, q);

        multiply(q, Pp, P);
        add(P, Ppp, P);

        multiply(q, Qp, Q);
        add(Q, Qpp, Q);

        remainder(a, b, t);
        vect_copy(b, a);
        vect_copy(t, b);
        vect_copy(qp, qpp);
        vect_copy(q, qp);
        vect_copy(Qp, Qpp);
        vect_copy(Q, Qp);
        vect_copy(Pp, Ppp);
        vect_copy(P, Pp);
    }

    // cout << "res: ";
    // vect_repr(res, " ");
    if (ind % 2 == 1) {
        sub(p, Qpp, res);
        return;
    }
    vect_copy(Qpp, res);
}

void gcd(vector<int> a, vector<int> b, vector<int> &res) {
    vector<int> zero;
    zero.push_back(0);
    vect_uni(b);
    if (comp(b, zero) == 0) {
        vect_copy(a, res);
    }
    else {
        remainder(a, b, a);
        gcd(b, a, res);
    }
}

void get_e(vector<int> &phin, vector<int> &res) {
    vector<int> one, gcd_res;
    one.push_back(1);
    res.push_back(2);
    gcd(res, phin, gcd_res);
    while (comp(gcd_res, one) != 0) {
        inc(res);
        gcd(res, phin, gcd_res);
    }
}

void get_d(vector<int> &e, vector<int> &phin, vector<int> &res) {
    rev(e, phin, res);
}

bool is_even(vector<int> &a) {
    if (a[0] % 2 == 1) {
        return true;
    }
    return false;
}

bool is_zero(vector<int> &a) {
    if (a.size() == 1 && a[0] == 0) {
        return true;
    }
    return false;
}

void gud_pow(vector<int> a, vector<int> pw, vector<int> &modulo, vector<int> &res) {
    vector<int> zero = {0}, two = {2}, temp, t;
    res.resize(0);
    res.push_back(1);
    while (!is_zero(pw)) {
        if (!is_even(pw)) {
            division(pw, two, temp);
            vect_copy(temp, pw);
            multiply(a, a, a);
            remainder(a, modulo, t);
            vect_copy(t, a);
        }
        else {
            dec(pw);
            multiply(res, a, res);
            remainder(res, modulo, temp);
            vect_copy(temp, res);
        }
    }
}

void prime_root(vector<int> &p, vector<int> &res) {
	vector<vector<int>> fact;
    vector <int> phi, n, zero, one, i1, i2, temp, t1;
    i1.push_back(2);
    res.resize(0);
    res.push_back(2);
    multiply(i1, i1, i2);
    zero.push_back(0);
    one.push_back(1);

    sub(p, one, phi);
    vect_copy(phi, n);

    for (; comp(i2, n) != 1; inc(i1), multiply(i1, i1, i2)) {
        remainder(n, i1, temp);
        if (comp(temp, zero) == 0) {
            fact.resize(fact.size() + 1);
            vect_copy(i1, fact[fact.size() - 1]);
            while (comp(temp, zero) == 0) {
                division(n, i1, temp);
                vect_copy(temp, n);
                remainder(n, i1, temp);
            }
        }
    }

    if (comp(n, one) == 1) {
        fact.resize(fact.size() + 1);
        vect_copy(n, fact[fact.size() - 1]);
    }

    for (; comp(res, p) != 1; inc(res)) {

        bool ok = true;
        for (size_t i = 0; i < fact.size() && ok; i++) {
            division(phi, fact[i], t1);
            gud_pow(res, t1, p, temp);
            if (comp(one, temp) == 0) {
                ok = false;
            }
        }

        if (ok) {
            return;
        }
    }

}

void generate_RSA_values(vector<int> &p, vector<int> &q, vector<int> &pp, vector<int> &qp, vector<int> &n, vector<int> &phin, vector<int> &e, vector<int> &d) {
    vector<int> one;
    one.push_back(1);
    sub(p, one, pp);
    sub(q, one, qp);
    multiply(p, q, n);
    multiply(pp, qp, phin);
    get_e(phin, e);
    // cout << "gt" << endl;
    get_d(e, phin, d);
}

void represent_RSA_values(vector<int> &p, vector<int> &q, vector<int> &pp, vector<int> &qp, vector<int> &n, vector<int> &phin, vector<int> &e, vector<int> &d) {

    cout << "p: ";
    vect_repr(p, "");
    cout << "q: ";
    vect_repr(q, "");
    cout << "n: ";
    vect_repr(n, "");
    cout << "pp: ";
    vect_repr(pp, "");
    cout << "pq: ";
    vect_repr(qp, "");
    cout << "phin: ";
    vect_repr(phin, "");
    cout << "e: ";
    vect_repr(e, "");
    cout << "d: ";
    vect_repr(d, "");

}

void enc_msg(string msg, vector<int> &e, vector<int> &n, vector<vector<int>> &res) {
    int t;
    string stemp;
    vector <int> t1, t2;
    res.resize(0);
    for (int i = 0; i < msg.length(); i++) {
        stemp = to_string(int(msg[i]));
        vect_set(t1, stemp);
        gud_pow(t1, e, n, t2);
        res.resize(res.size() + 1);
        vect_copy(t2, res[i]);
    }
}

void dec_msg(vector<vector<int>> &enc, vector<int> &d, vector<int> &n, string &res) {
    vector <int> t;
    int t1;
    res.clear();
    for (int i = 0; i < enc.size(); i++) {
        gud_pow(enc[i], d, n, t);
        t1 = 0;
        for (int j = 0; j < t.size(); j++) {
            t1 += pow(10, j) * t[j];
        }
        res.push_back(char(t1));
    }
}

// int main () {
// 	vector<int> p, q, pp, qp, n, e, d, phin, res;
// 	get_input(p, q);
//     generate_RSA_values(p, q, pp, qp, n, phin, e, d);
//     represent_RSA_values(p, q, pp, qp, n, phin, e, d);
// 	return 0;
// }
