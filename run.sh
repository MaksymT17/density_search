echo "Start Density Searcher test..." &&
mkdir -p build &&
cd build &&
cmake .. &&
make -j2 &&
echo "Build coplete. Run default test..." &&
./desnsity_search ../input/oil1.bmp | tee oil1.txt
