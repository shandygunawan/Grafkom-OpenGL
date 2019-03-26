import sys
import time

UNGU = "ungu"
UNGUTUA = "ungu_tua"
COKELAT_TUA = "cokelat_tua"
COKELAT_MUDA = "cokelat_muda"
MERAH_TUA = "merah_tua"
KUNING = "kuning"

def getSquare(a, b, c, d):
	temp = []

	temp.append("{} {} 0".format(a, b))
	temp.append("{} {} 0".format(c, b))
	temp.append("{} {} 0".format(c, d))

	temp.append("{} {} 0".format(a, b))
	temp.append("{} {} 0".format(a, d))
	temp.append("{} {} 0".format(c, d))

	return "\n".join(temp)

def getColor(color):
	color_temp = None
	red = green = blue = None

	if color == UNGU:
		red = float(0.588)
		green = float(0.407)
		blue = float(0.592)

	elif color == UNGUTUA:
		red = float(0.396)
		green = float(0.302)
		blue = float(0.489)

	elif color == MERAH_TUA:
		red = float(0.584 )
		green = float(0.012 )
		blue = float(0.188)		

	elif color == "orange":
		red = float(0.984)
		green = float(0.596)
		blue = float(0.196)

	elif color == COKELAT_TUA:
		test = [0.635, 0.514, 0.168]
		red, green, blue = [float(x) for x in test]

	elif color == COKELAT_MUDA:
		test = [0.800, 0.588, 0.031]
		red, green, blue = [float(x) for x in test]

	elif color == KUNING:
		test = [0.949, 0.800, 0.208]
		red, green, blue = [float(x) for x in test]
 
	color_temp = "{} {} {}".format(red, green, blue)

	return color_temp

# n = int(input())
temp_input = None
color_printer = []
vertex_printer = []

with open("1.txt", 'r') as f:
	temp_input = f.readlines()

n = len(temp_input)

for line in temp_input:
	leftmostX, leftmostY, rightmostX, rightmostY, color = [str(x) for x in line.split()]

	square = getSquare(leftmostX, leftmostY, rightmostX, rightmostY)
	color_temp = getColor(color)

	vertex_printer.append(square)

	for _ in range(6):
		color_printer.append(color_temp)

vertex_printer.append("=====")
color_printer.append("=====")

with open("kuadran4_color.txt", 'w') as f:
	f.write("\n".join(color_printer))

with open("kuadran4_vertices.txt", 'w') as f:
	f.write("\n".join(vertex_printer))