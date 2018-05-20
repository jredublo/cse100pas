make clean
make bst
valgrind --tool=memcheck --leak-check=full  --error-exitcode=1 ./bst 
TEST_STAT=$?
echo "-------- IF YOU SEE 0 BELOW, YOU ARE GOOD --------"
echo $TEST_STAT
echo "--------------------------------------------------"
