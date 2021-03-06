from enum import Enum, auto

class Color(Enum):
	UNGU = auto()
	UNGU_TUA = auto()
	COKELAT_TUA = auto()
	COKELAT_MUDA = auto()
	MERAH_TUA = auto()
	KUNING = auto()

class RGBGenerator():

	def __init__(self):
		pass

	def createColor(self, color):
		red = green = blue = None

		if color == Color.UNGU:
			red = float(0.588)
			green = float(0.407)
			blue = float(0.592)
		elif color == Color.UNGU_TUA:
			red = float(0.396)
			green = float(0.302)
			blue = float(0.489)
		elif color == Color.MERAH_TUA:
			red = float(0.584)
			green = float(0.012)
			blue = float(0.188)		
		elif color == Color.COKELAT_TUA:
			red = float(0.635)
			green = float(0.514)
			blue = float(0.168)	
		elif color == Color.COKELAT_MUDA:
			red = float(0.800)
			green = float(0.588)
			blue = float(0.031)				
		elif color == Color.KUNING:
			red = float(0.949)
			green = float(0.800)
			blue = float(0.208)				
		
		return " ".join(map(str, [red, green, blue]))	

if __name__ == "__main__":
	cf = RGBGenerator()
	print(cf.createColor(Color.MERAH_TUA))