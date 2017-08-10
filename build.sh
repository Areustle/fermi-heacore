cd BUILD_DIR
export CFLAGS='-I${PREFIX}/include -O2 -Wall --pedantic -Wno-comment -Wno-long-long -g  -ffloat-store -fPIC'
export CXXFLAGS='-I${PREFIX}/include -O2 -Wall --pedantic -Wno-comment -Wno-long-long -g  -ffloat-store -fPIC'
export CPPFLAGS="-I${PREFIX}/include"
export LDFLAGS="-L${PREFIX}/lib"
./configure --prefix=${PREFIX} --exec_prefix=${PREFIX} LDFLAGS="-Wl,-rpath,${PREFIX}/lib -L${PREFIX}/lib"
./hmake HD_EXEC_PFX=${PREFIX} HD_TOP_EXEC_PFX=${PREFIX}
./hmake install HD_EXEC_PFX=${PREFIX} HD_TOP_EXEC_PFX=${PREFIX}
cd $PREFIX
ls
rm -rf BUILD_DIR
unlink headas-init.sh
unlink headas-init.csh
