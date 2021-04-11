
[[ -d build ]] && cd build
[[ -f ../infra/mts/lib/swigmts_wrap.c ]] && \
	echo "removing swigmts_wrap.c ..." && \
	rm ../infra/mts/lib/swigmts_wrap.c
[[ -f ins/x86e/final/libmts/sup/swigmts_wrap.lo ]] && \
	echo "removing swigmts_wrap.lo ..." && \
	rm ins/x86e/final/libmts/sup/swigmts_wrap.lo
[[ -f ins/x86e/final/libmts/sup/_swigmts.so ]] && \
	echo "removing _swigmts.so ..." && \
	rm ins/x86e/final/libmts/sup/_swigmts.so

[[ ! -f ../infra/mts/lib/swigmts.i ]] && \
	ln -s /ws/jkolanje-sjc/sandbox/swig/libmts/swigmts.i ../infra/mts/lib/swigmts.i && \
	echo "ln -s /ws/jkolanje-sjc/sandbox/swig/libmts/swigmts.i ../infra/mts/lib/swigmts.i"
/router/bin/swig -python ../infra/mts/lib/swigmts.i 

[[ ! -f ../infra/mts/lib/swigmts_wrap.c ]] && echo "swig failed" && return
echo "infra/mts/lib/swigmts_wrap.c done." 

x86_64-wrs8-linux-gcc -I. -I../feature/vsh/python/Include \
	-I../feature/vsh/python -I../infra/mts/lib -I../include -I../include/isan -idirafter../routing-sw/include -idirafter../include/dcos -I../third-party/bin/linux/final.x86e-4.1/include -idirafter/auto/andpkg/rep_cache/wr-x86/8.0/sysroots/CORE-ML-28/sysroots/corei7-64-wrs-linux/usr/include/c++/5.2.0/i686-wrs-linux -I../infra/mts/include -I../infra/mts/lib -Iins/x86e/final/sup/include/cli -Iins/x86e/final/sup/include -I../feature/nxos_config/ins -I../feature/nxos_config/ins/x86e -I../feature/nxos_config/ins/x86e/sup -I../third-party/bin/x86s/openssl/ciscossl-fips/include \
  	-DNDEBUG -DFINAL -DLCINST_DIR_PREFIX=  -DARCH_HAS_VDCS -DMAX_ALLOWED_VDC=17 -D_GNU_SOURCE -DWR_TOOLCHAIN -DVMAN_PLAT_SMACK_SECLABEL=TRUE -DVMAN_PLAT_SUPPORTS_GUESTSHELL   -Wall -g -MP -pipe    -fno-omit-frame-pointer --sysroot=/auto/andpkg/rep_cache/wr-x86/8.0/sysroots/CORE-ML-28/sysroots/corei7-64-wrs-linux -Wall -Werror -std=gnu99 -fno-omit-frame-pointer -Wno-pointer-sign -fgnu89-inline -fno-strict-aliasing -march=corei7 -m32 -mno-sse -mno-mmx -mno-sse2 -mno-3dnow -mno-avx -pthread -Wno-error=unused-but-set-variable -DHAVE_RUSAGE_THREAD -DINS_GLOB_CONST -D_GLIBCXX_USE_CXX11_ABI=0 -Wno-error=implicit-function-declaration -Wno-error=switch -Wno-error=unused-but-set-variable -Wno-error=deprecated-declarations -Wno-error=unused-variable -Wno-error=unused-function -Wno-error=maybe-uninitialized -Wno-unused-parameter -Wno-uninitialized -Wno-ignored-qualifiers    -D__LIBMTS__ -Wno-uninitialized -rdynamic  -fPIC -DPIC  -c -MMD -MT \
	ins/x86e/final/libmts/sup/swigmts_wrap.lo -MF "ins/x86e/final/libmts/sup/swigmts_wrap.ld" -o ins/x86e/final/libmts/sup/swigmts_wrap.lo ../infra/mts/lib/swigmts_wrap.c

[[ ! -f ins/x86e/final/libmts/sup/swigmts_wrap.lo ]] && echo "swig wrap compile failed" && return
echo "ins/x86e/final/libmts/sup/swigmts_wrap.lo done."

ARCH=x86e  x86_64-wrs8-linux-gcc  -shared -pthread -march=corei7 -m32 -mno-sse -mno-mmx -mno-sse2 -mno-3dnow -mno-avx --sysroot=/auto/andpkg/rep_cache/wr-x86/8.0/sysroots/CORE-ML-28/sysroots/corei7-64-wrs-linux  -shared -Xlinker --start-group \
   	 ins/x86e/final/libmts/sup/swigmts_wrap.lo \
	 ins/x86e/final/libmts/xc/libmts.so \
	 ins/x86e/final/libnf/xc/libnf.so \
	 ins/x86e/final/libfsmutils/xc/libfsmutils.so \
	 -Xlinker --end-group -Wl,-soname \
	 -Wl,_swigmts.so -o ins/x86e/final/libmts/sup/_swigmts.so

#	 ins/x86e/final/libmts/sup/libmts.so \
#
[[ ! -f ins/x86e/final/libmts/sup/_swigmts.so ]] && echo "swig library compile failed" && return

echo "ins/x86e/final/libmts/sup/_swigmts.so done."
cd ../

