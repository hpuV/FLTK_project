#include <string>
#include <stdexcept>
#include <sstream>

using namespace std;

// error() simply disguises throws:
inline void error(const string& s)
{

    throw runtime_error(s);

}

inline void error(const string& s, const string& s2)
{
	error(s+s2);
}

inline void error(const string& s, int i)
{
	ostringstream os;
	os << s <<": " << i;
	error(os.str());
}



