#include <boost/multiprecision/cpp_dec_float.hpp> 
#include <boost/math/constants/constants.hpp>
#include <iostream>
#include <vector>

//typedef double xdouble;
typedef boost::multiprecision::cpp_dec_float_50 xdouble;

using namespace std; 

vector<xdouble> Baker (vector<xdouble>& v)
{
    xdouble a (1./3);
    vector<xdouble> temp (2);
    
    if (v[0] < .5) {
        temp[0] = 2*v[0];
        temp[1] = a*v[1];
    } else {
        temp[0] = 2*v[0]-1;
        temp[1] = a*v[1]+.5;
    }
    return temp;
}
  
int main() 
{ 
    xdouble pi = boost::math::constants::pi<xdouble>();
    xdouble a(3), b(3);
    a = a+b;
    vector<vector<xdouble>> baker_series (100, vector<xdouble>(2));
    vector<xdouble> baker_start (2);
    baker_start[0] = pi/6;
    baker_start[1] = .3;
    baker_series[0] = baker_start;
    cout << baker_series[0][0] << "    " << baker_series[0][1] << endl;;
    for (int i=1; i<100; ++i) {   
        baker_series[i] = Baker(baker_series[i-1]);
        cout << baker_series[i][0] << "    " << baker_series[i][1] << endl;
    }
    return 0;
}
