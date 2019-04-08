class Point():

	def __init__(self, x, y, z):
		self.x = x
		self.y = y
		self.z = z

	def __repr__(self):
		return "{} {} {}".format(self.x, self.y, self.z)

class VertexGenerator():

	def __init__(self):
		pass

	def __createTriangle(self, p1, p2, p3):
		return "\n".join(map(str, [p1, p2, p3]))

	def __createSeparator(self, length):
		return "# {}".format("=" * length)

	def __createSquare(self, lower_half, upper_half):
		separator = self.__createSeparator(3)
		return "\n{}\n".format(separator).join([lower_half, upper_half])

	def __createSquareWithConstantAxis(self, a1, b1, a2, b2, c):
		temp = []

		upper_left = Point(a1, b1, c)
		upper_right = Point(a2, b1, c)
		lower_right = Point(a2, b2, c)
		lower_left = Point(a1, b2, c)

		# Lower half of square
		lower_half = self.__createTriangle(upper_left, lower_left, lower_right)

		# Upper half of square
		upper_half = self.__createTriangle(upper_left, upper_right, lower_right)

		return self.__createSquare(lower_half, upper_half)

	def __createSquareAlongXYAxis(self, x1, y1, x2, y2, z):
		return self.__createSquareWithConstantAxis(x1, y1, x2, y2, z)

	def __createSquareAlongXZAxis(self, x1, z1, x2, z2, y):
		return self.__createSquareWithConstantAxis(x1, z2, x2, z1, y)

	def __createSquareAlongYZAxis(self, y1, z1, y2, z2, x):
		return self.__createSquareWithConstantAxis(y1, z1, y2, z2, x)

	def __createCube(self, x1, y1, z1, x2, y2, z2):
		separator = self.__createSeparator(10)

		front = self.__createSquareAlongXYAxis(x1, y1, x2, y2, z1)
		back = self.__createSquareAlongXYAxis(x1, y1, x2, y2, z2)

		top = self.__createSquareAlongXZAxis(x1, z2, x2, z1, y1)
		bottom = self.__createSquareAlongXZAxis(x1, z2, x2, z1, y2)

		left = self.__createSquareAlongYZAxis(y1, z1, y2, z2, x1)
		right = self.__createSquareAlongYZAxis(y1, z1, y2, z2, x2)

		return "\n{}\n".format(separator).join([front, back, left, right, top, bottom])

	def createCube(self, x1, y1, z1, x2, y2, z2):
		return self.__createCube(x1, y1, z1, x2, y2, z2)

if __name__ == "__main__":
	sf = VertexGenerator()
	print(sf.createCube(0, 3, 0, 4, 0, 3))
