#include "header.hpp"
#include "color.hpp"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>

using namespace std;
namespace fs = experimental::filesystem;


int main(int argc, char *argv[])
{
	if (argc != 8) {
		cout << "7 args - path, x, y, z, dx, dy, dz" << endl;
		return -1;
	}

	auto path = fs::canonical(argv[1]);

	int x = atoi(argv[2]),
	    y = atoi(argv[3]),
	    z = atoi(argv[4]);

	vec d = {
		atof(argv[5]), atof(argv[6]), atof(argv[7])
	};

	header h{
		x, y, z,
		{ (-x * d.x) / 2, (-y * d.y) / 2, (-z * d.z) / 2 },
		{  (x * d.x) / 2,  (y * d.y) / 2,  (z * d.z) / 2 },
		d,
		color_format::ha
	};

	ofstream vd(path.parent_path() / (path.filename().string() + ".vd"), ios::out | ios::binary);
	vd.write(reinterpret_cast<const char *>(&h), sizeof(header));

	auto *data = new color_stnfrd[x*y];

	for (int i = 1; i <= z; ++i) {
		char buffer[5];
		_itoa_s(i, buffer, 5, 10);

		ifstream raw(path / (path.filename().string() + "." + buffer), ios::in | ios::binary);

		for (size_t i = 0; i < x * y; ++i) {
			color_stnfrd col;

			raw.read(reinterpret_cast<char *>(&col), sizeof(color_stnfrd));
			vd.write(reinterpret_cast<const char *>(&col.ha()), sizeof(color_ha));
		}
	}
	

	return 0;
}