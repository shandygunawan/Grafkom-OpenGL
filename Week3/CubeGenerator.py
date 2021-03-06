from VertexGenerator import VertexGenerator, Point
from RGBGenerator import RGBGenerator, Color
from sys import argv

total_args = ["input_filename", "output_vertex"]

if __name__ == "__main__":

	if len(argv)-1 != len(total_args):
		print("Error! Args must be: {}".format(" ".join(total_args)))
		exit()

	input_filename, output_vertex = argv[1:]

	temp_input = None
	# color_printer = []
	vertex_printer = []


	with open(input_filename, 'r') as f:
		temp_input = f.readlines()

	vg = VertexGenerator()
	# cg = RGBGenerator()
	for line in temp_input:
		if line.startswith("#"):
			vertex_printer.append(line)
			continue

		full_input = [str(x) for x in line.split()]

		x1, y1, z1, x2, y2, z2 = full_input[:6]
		edges = full_input[6:]

		print(edges)

		cube = vg.createCube(x1, y1, z1, x2, y2, z2, edges)
		# color = cg.createColor(Color[color])

		vertex_printer.append(cube)
		# for _ in range(6 * len(edges)):
			# color_printer.append(color)


	# with open(output_color, 'w') as f:
		# f.write("\n".join(color_printer))

	with open(output_vertex, 'w') as f:
		f.write("\n".join(vertex_printer))

