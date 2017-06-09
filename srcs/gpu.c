/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gpu.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elee <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/02 17:02:45 by elee              #+#    #+#             */
/*   Updated: 2017/06/02 18:59:20 by elee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"
#include "err_code.h"

char			*getKernelName(cl_kernel kernel)
{
	char	*str;
	size_t	size;

	clGetKernelInfo(kernel, CL_KERNEL_FUNCTION_NAME, 2048, NULL, &size);
	str = (char*)malloc(sizeof(char) * (size + 1));
	clGetKernelInfo(kernel, CL_KERNEL_FUNCTION_NAME, 128, str, NULL);
	return (str);
}

char			*load_cl_file(char *filename)
{
	char	*source;
	int		fd;

	source = (char*)calloc(1, 8192);
	fd = open(filename, O_RDONLY);
	read(fd, source, 8192);
	close(fd);
	return (source);
}

cl_kernel		make_kernel(t_context *c, char *sourcefile, char *name)
{
	cl_kernel	k;
	cl_program	p;
	char		*source;
	int			err;
	size_t		len;
	char		buffer[2048];

	source = load_cl_file(sourcefile);
	p = clCreateProgramWithSource(c->context, 1, (const char**)&source, NULL, &err);
	checkError(err, "Creating Program");

	// Build the program
	err = clBuildProgram(p, 0, NULL, NULL, NULL, NULL);
	if (err != CL_SUCCESS)
	{
		printf("Error: Failed to build program executable\n%s\n", err_code(err));
		clGetProgramBuildInfo(p, c->device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
		printf("%s", buffer);
		exit(0);
	}

	// Create the compute kernel from the program
	k = clCreateKernel(p, name, &err);
	checkError(err, "Creating kernel");
	free(source);
	return (k);
}

t_context		*setup_context(void)
{
	int			err;
	cl_uint		numPlatforms;
	t_context	*c = (t_context*)calloc(1, sizeof(t_context));

	// Find number of platforms
	err = clGetPlatformIDs(0, NULL, &numPlatforms);
	checkError(err, "Getting number of platforms");
	
	// Get all platforms;
	cl_platform_id	Platform[numPlatforms];
	err = clGetPlatformIDs(numPlatforms, Platform, NULL);
	checkError(err, "Gettings platforms");

	// Set up GPU
	for (cl_uint i = 0; i < numPlatforms; i++)
	{
		err = clGetDeviceIDs(Platform[i], DEVICE, 1, &(c->device_id), NULL);
		if (err == CL_SUCCESS)
			break ;
	}
	if (c->device_id == NULL)
		checkError(err, "Finding a device");

	// Create a context;
	c->context = clCreateContext(0, 1, &(c->device_id), NULL, NULL, &err);
	checkError(err, "Creating context");

	// Create a command queue;
	c->commands = clCreateCommandQueue(c->context, (c->device_id), 0, &err);
	checkError(err, "Creating command queue");
	return (c);
}
