const int rectangle_vertices_count = 4;
const float rectangle_vertices[rectangle_vertices_count * 8] =
{
//       Position                 Normal             Tex Coord
	-1.0f, 1.0f, 0.0f,		 0.0f, 0.0f, 1.0f,		0.0f, 1.0f,
	-1.0f,-1.0f, 0.0f,		 0.0f, 0.0f, 1.0f,		0.0f, 0.0f,
	 1.0f,-1.0f, 0.0f,		 0.0f, 0.0f, 1.0f,		1.0f, 0.0f,
	 1.0f, 1.0f, 0.0f,		 0.0f, 0.0f, 1.0f,		1.0f, 1.0f,
};

const int rectangle_indices_count = 6;
const unsigned int rectangle_indices[rectangle_indices_count] =
{
	0, 1, 2,		0, 2, 3,
};
