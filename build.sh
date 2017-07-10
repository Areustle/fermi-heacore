cd BUILD_DIR
export CFLAGS='-I${PREFIX}/include -O2 -Wall --pedantic -Wno-comment -Wno-long-long -g  -ffloat-store -fPIC'
export CXXFLAGS='-I${PREFIX}/include -O2 -Wall --pedantic -Wno-comment -Wno-long-long -g  -ffloat-store -fPIC'
export CPPFLAGS="-I${PREFIX}/include"
export LDFLAGS="-L${PREFIX}/lib"
./configure --prefix=${PREFIX} --exec_prefix=${PREFIX}
./hmake HD_EXEC_PFX=${PREFIX} HD_TOP_EXEC_PFX=${PREFIX}
./hmake install HD_EXEC_PFX=${PREFIX} HD_TOP_EXEC_PFX=${PREFIX}
./hmake HD_EXEC_PFX=${PREFIX} HD_TOP_EXEC_PFX=${PREFIX} test
./hmake install HD_EXEC_PFX=${PREFIX} HD_TOP_EXEC_PFX=${PREFIX} install-test
