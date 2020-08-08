
SOURCE_DIR = src
INCLUDE_DIR = include
OUTPUT_DIR = obj
CXX = g++
CXXFLAGS = -I$(INCLUDE_DIR)

LIBS = -lSDL2 -lpthread

_DEPS = RTTypes.h Scene.h Ray.h Lens.h Graphical.h Renderer.h PostProcessing.h SceneObjects.h
DEPS =  $(patsubst %,$(INCLUDE_DIR)/%,$(_DEPS))

_OBJ = RayTracer.o RTTypes.o Scene.o Ray.o Lens.o Graphical.o Renderer.o PostProcessing.o SceneObjects/Plane.o SceneObjects/Sphere.o SceneObjects/TileFloor.o
OBJ = $(patsubst %,$(OUTPUT_DIR)/%,$(_OBJ))

$(OUTPUT_DIR)/%.o : $(SOURCE_DIR)/%.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

RayTracer : $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(OUTPUT_DIR)/*.o *~ core $(INCLUDE_DIR)/*~ 