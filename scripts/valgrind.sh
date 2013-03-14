BASEDIR=..

cd $BASEDIR
valgrind --leak-check=full --track-origins=yes ./roborobo -l config/ahhsscalepower.properties
