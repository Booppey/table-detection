table-detect:
	g++ -o table-detect table-detect.cpp --std=c++11 -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgproc -lopencv_features2d -lopencv_flann -lopencv_nonfree -lopencv_calib3d -lopencv_objdetect
clean:
	rm -fr table-detect *~
