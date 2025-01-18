#include <stdio.h>

#define CL_TARGET_OPENCL_VERSION 300
#ifdef __APPLE__
	#include <OpenCL/cl.h>
#else
	#include <CL/cl.h>
#endif


int main() {
	cl_uint numPlatforms;
	cl_int status = clGetPlatformIDs(0, NULL, &numPlatforms);
	if (status != CL_SUCCESS) {
		return 1;
	}
    
	if (numPlatforms > 0) {
		cl_platform_id* platforms = (cl_platform_id*) calloc(numPlatforms, sizeof(cl_platform_id));
		status = clGetPlatformIDs(numPlatforms, platforms, NULL);
		
		for (cl_uint i = 0; i < numPlatforms; i++) {
			cl_uint deviceCount;
			status = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, 0, NULL, &deviceCount);
			if (status != CL_SUCCESS) {
				free(platforms);
				return 2;
			}

			if (deviceCount > 0) {
				cl_device_id* devices = (cl_device_id*) calloc(deviceCount, sizeof(cl_device_id));
				clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, deviceCount, devices, NULL);

				for (cl_uint j = 0; j < deviceCount; j++) {
					char name[128];
    				clGetDeviceInfo(devices[j], CL_DEVICE_NAME, sizeof(name), name, NULL);
    				printf("1. Device Name: %s\n", name);

					char vendor[128];
    				clGetDeviceInfo(devices[j], CL_DEVICE_VENDOR, sizeof(vendor), vendor, NULL);
    				printf("2. Vendor: %s\n", vendor);

					char version[128];
    				clGetDeviceInfo(devices[j], CL_DEVICE_VERSION, sizeof(version), version, NULL);
    				printf("3. Device Version: %s\n", version);

					char driver_version[128];
    				clGetDeviceInfo(devices[j], CL_DRIVER_VERSION, sizeof(driver_version), driver_version, NULL);
    				printf("4. Driver Version: %s\n", driver_version);

    				cl_device_type type;
    				clGetDeviceInfo(devices[j], CL_DEVICE_TYPE, sizeof(type), &type, NULL);
    				printf("5. Device Type: %s\n", (type == CL_DEVICE_TYPE_GPU) ? "GPU" : "CPU");

    				cl_uint maxComputeUnits;
    				clGetDeviceInfo(devices[j], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(maxComputeUnits), &maxComputeUnits, NULL);
    				printf("6. Max Compute Units: %u\n", maxComputeUnits);

    				size_t maxWorkItemSizes[3];
    				clGetDeviceInfo(devices[j], CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(maxWorkItemSizes), maxWorkItemSizes, NULL);
    				printf("7. Max Work Item Sizes: [%zu, %zu, %zu]\n", maxWorkItemSizes[0], maxWorkItemSizes[1], maxWorkItemSizes[2]);

    				cl_ulong globalMemSize;
    				clGetDeviceInfo(devices[j], CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(globalMemSize), &globalMemSize, NULL);
    				printf("8. Global Memory Size: %lu bytes\n", globalMemSize);

    				cl_ulong maxMemAllocSize;
    				clGetDeviceInfo(devices[j], CL_DEVICE_MAX_MEM_ALLOC_SIZE, sizeof(maxMemAllocSize), &maxMemAllocSize, NULL);
    				printf("9. Max Memory Allocation Size: %lu bytes\n", maxMemAllocSize);

    				cl_uint maxClockFrequency;
    				clGetDeviceInfo(devices[j], CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(maxClockFrequency), &maxClockFrequency, NULL);
    				printf("10. Max Clock Frequency: %u MHz\n", maxClockFrequency);
				}

				free(devices);
			}
		}

		free(platforms);
	}

	return 0;
}
