CC=g++
CFLAGS=-c -Wall
MAC_BULLSHIT_INCLUDE=-I/usr/local/include 
MAC_BULLSHIT_LIB=-L/usr/local/lib 
MAC_BULLSHIT=$(MAC_BULLSHIT_INCLUDE) $(MAC_BULLSHIT_LIB)
OPENGL_FLAGS_LMAO=-framework OpenGL -lglfw3 -lglew
SRC_DIR=src
BUILD_DIR=build

uniforms: $(SRC_DIR)/chapter5/uniforms.cpp application sb7loader
	$(CC) $(BUILD_DIR)/application.o $(BUILD_DIR)/sb7loader.o -ggdb -I$(SRC_DIR) $(SRC_DIR)/chapter5/uniforms.cpp $(MAC_BULLSHIT) $(OPENGL_FLAGS_LMAO) -o uniforms


buffers: $(SRC_DIR)/chapter5/buffers.cpp application sb7loader
	$(CC) $(BUILD_DIR)/application.o $(BUILD_DIR)/sb7loader.o -ggdb -I$(SRC_DIR) $(SRC_DIR)/chapter5/buffers.cpp $(MAC_BULLSHIT) $(OPENGL_FLAGS_LMAO) -o buffers


load_obj_from_files: $(SRC_DIR)/chapter5/load_obj_from_files.cpp application sb7loader
	$(CC) $(BUILD_DIR)/application.o $(BUILD_DIR)/sb7loader.o -ggdb -I$(SRC_DIR) $(SRC_DIR)/chapter5/load_obj_from_files.cpp $(MAC_BULLSHIT) $(OPENGL_FLAGS_LMAO) -o load_obj_from_files

geometry: $(SRC_DIR)/chapter3/geometry.cpp application
	$(CC) $(BUILD_DIR)/application.o   -I$(SRC_DIR) $(SRC_DIR)/chapter3/geometry.cpp $(MAC_BULLSHIT) $(OPENGL_FLAGS_LMAO) -o geometry

tesselation: $(SRC_DIR)/chapter3/tesselation.cpp application
	$(CC) $(BUILD_DIR)/application.o   -I$(SRC_DIR) $(SRC_DIR)/chapter3/tesselation.cpp $(MAC_BULLSHIT) $(OPENGL_FLAGS_LMAO) -o tesselation

vertex_shader: $(SRC_DIR)/chapter3/vertex_shader.cpp application
	$(CC) $(BUILD_DIR)/application.o   -I$(SRC_DIR) $(SRC_DIR)/chapter3/vertex_shader.cpp $(MAC_BULLSHIT) $(OPENGL_FLAGS_LMAO) -o vertex_shader

triangle: $(SRC_DIR)/chapter2/triangle.cpp application
	$(CC) $(BUILD_DIR)/application.o $(SRC_DIR)/chapter2/triangle.cpp  -I$(SRC_DIR) $(MAC_BULLSHIT) $(OPENGL_FLAGS_LMAO) -o triangle

point: $(SRC_DIR)/chapter2/point.cpp application
	$(CC) $(BUILD_DIR)/application.o $(SRC_DIR)/chapter2/point.cpp  -I$(SRC_DIR) $(MAC_BULLSHIT) $(OPENGL_FLAGS_LMAO) -o point

colors: $(SRC_DIR)/chapter2/colors.cpp application
	$(CC) $(BUILD_DIR)/application.o $(SRC_DIR)/chapter2/colors.cpp  -I$(SRC_DIR) $(MAC_BULLSHIT) $(OPENGL_FLAGS_LMAO) -o colors

sb7loader: $(SRC_DIR)/common/object.cpp 
	$(CC) $(CFLAGS) $(SRC_DIR)/common/object.cpp -I$(SRC_DIR) $(MAC_BULLSHIT) -o $(BUILD_DIR)/sb7loader.o

application: $(SRC_DIR)/glengine/application.cpp
	$(CC) $(CFLAGS) -I$(SRC_DIR) $(MAC_BULLSHIT_INCLUDE) src/glengine/application.cpp -o $(BUILD_DIR)/application.o

clean:
	rm $(BUILD_DIR)/*.o *.png *.bmp *.jpg
