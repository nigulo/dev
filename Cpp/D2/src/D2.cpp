#include "D2.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <cmath>
#include <math.h>
#include <sstream>
#include <boost/algorithm/string.hpp>

using namespace std;


vector<std::string>& split(const std::string& s, char delim, std::vector<std::string> &elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


vector<std::string> split(const std::string& s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

int main(int argc, char *argv[]) {
	ifstream input(argv[1]);
	unsigned from_col = argc > 2 ? atoi(argv[2]) : 1;
	unsigned to_col = argc > 3 ? atoi(argv[3]) + 1 : from_col + 1;
	unsigned dim = to_col - from_col;
	vector<double> x;
	vector<double*> y;
	for (string line; getline(input, line);) {
		//cout << line << endl;
		std::vector<std::string> words;
		boost::split(words, line, boost::is_any_of("\t "), boost::token_compress_on);
		for (vector<string>::iterator it = words.begin() ; it != words.end(); ++it) {
			if ((*it).length() == 0) {
				words.erase(it);
			}
		}
		if (words.size() > 0 && words[0][0] == '#') {
			//cout << "Skipping comment line: " << line << endl;
		} else if (words.size() > from_col) {
			try {
				double xVal = stod(words[0]);
				x.push_back(xVal);
				double* yVals = new double[dim];
				for (unsigned i = 0; i < dim; i++) {
					yVals[i] = stod(words[from_col + i]);
				}
				y.push_back(yVals);
			} catch (std::invalid_argument& ex) {
				cout << "Skipping line, invalid number: " << line << endl;
			}
		} else {
			cout << "Skipping line, too few columns: " << line << endl;
		}
    }
	input.close();
	double minPeriod = argc > 4 ? atof(argv[4]) : 2;
	double maxPeriod = argc > 5 ? atof(argv[5]) : 10;
	double minCoherence = argc > 6 ? atof(argv[6]) : 0;
	double maxCoherence = argc > 7 ? atof(argv[7]) : 60;//x[x.size() - 1] - x[0];
	int bootstrapSample = argc > 8 ? atoi(argv[8]) : 0;
	D2 d2(x, y, dim, minPeriod, maxPeriod, minCoherence, maxCoherence);
	if (bootstrapSample > 0) {
		for (int i = 0; i < bootstrapSample; i++) {
			d2.Compute2DSpectrum(true);
		}
	} else {
		d2.Compute2DSpectrum(false);
	}
	return 0;
}

#define square(x) ((x) * (x))

D2::D2(const vector<double>& r_x, const vector<double*>& r_y, unsigned dim, double minp, double maxp, double minc, double maxc) :
		x(r_x),
		y(r_y),
		dim(dim),
		minCoherence(minc),
		maxCoherence(maxc) {

	double wmax = 1.0 / minp;
	wmin = 1.0 / maxp;

	dmin = minCoherence * (relative ? minp : 1);
	dmax = maxCoherence * (relative ? maxp : 1);

	l = x.size();

	if (dmax < dmin || minp > maxp) {
		throw "Check Arguments";
	}
	k = dmax > dmin ? coherenceGrid : 1;
	m = round(phaseBins * (dmax - dmin) * wmax);
	a = (m - 1.0) / (dmax - dmin);
	b = -dmin * a;
	delta = (dmax - dmin) / (m - 1);

	lp = round((wmax - wmin) * (dmax - dmin) / deltaPhi);
	step = (wmax - wmin) / (lp - 1);
	eps = epsilon;
	epslim = 1.0 - eps;
	ln2 = sqrt(log(2.0));
	lnp = ln2 / eps;
}

double D2::Criterion(double d, double w) {

   unsigned j;
   double tyv, tav, dd, ww, wp, ph;

   tyv = 0.0;
   tav = 0.0;
   switch (mode) {
   case Box:
	  for (j = 0; j < td.size(); j++) {// to jj-1 do begin
		 dd = td[j];
		 if (dd <= d) {
			ph = dd*w - floor(dd*w);//Frac(dd*w);
			if (ph < 0.0) {
			   ph = ph+1;
			}
			if (ph<eps || ph>epslim) {
			   tyv = tyv+ty[j];
			   tav = tav+ta[j];
			}
		 }
	  }
	  break;
   case Gauss: //This is important, in td[] are precomputed sums of squares and counts.
   case GaussWithCosine:
	  for (j = 0; j < td.size(); j++) {// to jj-1 do begin
		 dd=td[j];
		 if (d>0.0) {
			ww=exp(-square(ln2*dd/d));
		 } else {
			ww=0.0;
		 }
		 ph=dd*w - floor(dd*w);//Frac(dd*w);
		 if (ph<0.0) {
			ph=ph+1;
		 }
		 if (ph>0.5) {
			ph=1.0-ph;
		 }
		 bool closeInPhase = true;
		 if (mode == Gauss) {
			 closeInPhase = ph<eps || ph>epslim;
			 wp=exp(-square(lnp*ph));
		 } else {
			 if (ph == 0.5) {
				 wp = 0;
			 } else if (ph == 0) {
				 wp = 1;
			 } else {
				 wp = 0.5 * (cos (0.5 * M_PI / ph) + 1);
			 }
			 if (std::isnan(wp)) {
				 wp = 0;
				 cout << "wp is still nan" << endl;
			 }
		 }
		 if (closeInPhase) {
			 tyv=tyv + ww * wp * ty[j];
			 tav=tav + ww * wp * ta[j];
		 }
	  }
	  break;
   }
   if (tav > 0.0) {
	  return 0.5 * tyv / tav;
   } else {
	  return 0.0;
   }
}

void MapTo01D(vector<double>& cum) {

	double* min = 0;
	double* max = 0;

	for (vector<double>::iterator it = cum.begin() ; it != cum.end(); ++it) {
		if (!min || *it < *min) {
			min = new double(*it);
		}
		if (!max || *it > *max) {
			max = new double(*it);
		}
	}
	if (max && min && *max > *min) {
		double range = *max - *min;
		for (vector<double>::iterator it = cum.begin() ; it != cum.end(); ++it) {
			*it = (*it - *min) / range;
		}

	} else {
		cout << "Cannot normalize" << endl;
	}

}

// Currently implemented as Frobenius norm
double D2::DiffNorm(const double y1[], const double y2[]) {
	double norm = 0;
	for (unsigned i = 0; i < dim; i++) {
		norm += square(y1[i] - y2[i]);
	}
	return norm;
}

void D2::Compute2DSpectrum(bool bootstrap) {

    cout << "l= " << l << endl;
    cout << "lp= " << lp << endl;
    cout << "k= " << k << endl;
    cout << "m= " << m << endl;
    cout << "a= " << a << endl;
    cout << "b= " << b << endl;
    cout << "dmin= " << dmin << endl;
    cout << "dmax= " << dmax << endl;
    cout << "wmin= " << wmin << endl;
    cout << "delta= " << delta << endl;
    cout << "step= " << step << endl;

	cum.assign(lp, 0);
	vector<vector<double>*> ydiffs(m, 0);
	vector<double> tty(m, 0);
	vector<double> tta(m, 0);

	// Now comes precomputation of differences and counts. They are accumulated in two grids.
	unsigned i, j;
	for (i = 0; i < l - 1; i++) {// to l-2 do
		for (j = i + 1; j < l; j++) {// to l-1 do begin
			double d = x[j] - x[i];
			if (d >= dmin && d <= dmax) {
				int kk = round(a * d + b);
				if (bootstrap) {
					if (!ydiffs[kk]) {
						ydiffs[kk] = new vector<double>(0);
					}
					ydiffs[kk]->push_back(DiffNorm(y[j],  y[i]));
				} else {
					tty[kk] = tty[kk] + DiffNorm(y[j], y[i]);
				}
				tta[kk] = tta[kk] + 1.0;
			}
		}
	}
	j=0;

	if (bootstrap) {
		default_random_engine e1(rd());
		for (i = 0; i < ydiffs.size(); i++) {
			if (!ydiffs[i]) {
				continue;
			}
			uniform_int_distribution<int> uniform_dist(0, ydiffs[i]->size() - 1);
			for (unsigned j1 = 0; j1 < ydiffs[i]->size(); j1++) {
				//if (i == 50) {
				//	cout << i << "," << j << "-" << (*ydiffs[i])[j] << endl;
				//}
				tty[i] = tty[i] + (*ydiffs[i])[uniform_dist(e1)];
			}
			delete ydiffs[i];
		}
	}
	// How many time differences was actually used?

	for (i = 0; i < m; i++) {
		if (tta[i] > 0.5) {
			j++;
		}
	}
	cout << "j=" << j << endl;
	ta.assign(j, 0);
	ty.assign(j, 0);
	td.assign(j, 0);
	cout << "td.size()=" << td.size() << endl;
	double d = dmin;

	// Build final grids for periodicity search.

	j = 0;
	for (i = 0; i < m; i++) {
		if (tta[i]>0.5) {
			ta[j]=tta[i];
			ty[j]=tty[i];
			td[j]=d;
			j++;
		}
		d=d+delta;
	}
	ofstream output("phasedisp.csv");
	ofstream output_min("phasedisp_min.csv");
	ofstream output_max("phasedisp_max.csv");

	// Basic cycle with printing for GnuPlot

	if (bootstrap) {
		k = 1;
	}

	double deltac = maxCoherence > minCoherence ? (maxCoherence - minCoherence) / (k - 1) : 0;
	for (i=0; i < k; i++) {
		d = minCoherence + i * deltac;
		for (j=0; j < lp; j++) {
			double w=wmin+j*step;
			double d1 = d;
			if (relative) {
				d1 = d / w;
			}
			double res=Criterion(d1,w);
			cum[j]=res;
		}

		// Spectrum in cum can be normalized

		if (true) {
			MapTo01D(cum);
		}

		vector<int> minima(0);
		unsigned dk = lp / 20;
		for (j = 0; j < lp; j++) {
			if (!bootstrap) {
				output << d << " " << (wmin + j * step) << " " << cum[j] << endl;
				if (d == minCoherence) {
					output_min << (wmin + j * step) << " " << cum[j] << endl;
				} else if (d == maxCoherence) {
					output_max << (wmin + j * step) << " " << cum[j] << endl;
				}
			}
			if (j > dk - 1 && j < lp - dk - 1) {
				bool isMinimum = true;
				for (unsigned k1 = j - dk; k1 <= j + dk; k1++) {
					if (cum[j] > cum[k1]) {
						isMinimum = false;
						break;
					}
				}
				if (isMinimum) {
					vector<int>::iterator it = minima.begin();
					for (; it != minima.end(); ++it) {
						if (cum[j] < cum[(*it)]) {
							break;
						}
					}
					minima.insert(it, j);
				}
			}
		}
		for (unsigned k1 = 0; k1 < minima.size(); k1++) {
			if (k1 > 0) {
				//cout << " ";
			}
			//cout << wmin + minima[k1] * step;
		}
		//cout << endl;
	}
	output.close();
	output_min.close();
	output_max.close();
}

