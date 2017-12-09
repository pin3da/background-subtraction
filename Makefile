COMMON_LIBS = -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_videoio -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_stitching -L/usr/local/lib -lopencv_imgcodecs
COMMON_FLAGS = -Wall -Ofast -fopenmp

main: main.cc
	$(CXX) $(COMMON_FLAGS) $(COMMON_LIBS) main.cc -o main.out
