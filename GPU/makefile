all: gpumm gpumms

gpumm: gpuMatMul.cu
	nvcc gpuMatMul.cu -o  gpumm

gpumms: gpuMatMulShared.cu
	nvcc gpuMatMulShared.cu -o  gpumms	



