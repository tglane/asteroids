/*
 *  PLYIO.cpp
 *
 *  Created on: Nov. 04 2018
 *      Author: Thomas Wiemann
 *
 *  Copyright (c) 2018 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */

#include "PLYIO.hpp"

#include <iostream>
#include <cstring>
#include <cstdio>

namespace asteroids
{

void LoadPLY(const std::string& filename, 
	float*& vertexBuffer, int*& indexBuffer, 
	int& vertexCount, int& faceCount)
{
	bool header_read = false;
	char line[100];

	char *token;

	FILE *ply_file = fopen(filename.c_str(), "r");

	if (ply_file == NULL)
	{
		indexBuffer = nullptr;
		vertexBuffer = nullptr;
		vertexCount = 0;
		faceCount = 0;
		std::cout << "LoadPLY: Unable to open file " << filename << std::endl;
		return;
	}

	/* Read Header */
	char* result = fgets(line, sizeof(line), ply_file);
	if(!result)
	{
		std::cout << "LoadPLY: Error reading line from file" << std::endl;
	}

	if (strcmp(line, "ply\n") != 0)
	{
		vertexBuffer = nullptr;
		indexBuffer = nullptr;
		vertexCount = 0;
		faceCount = 0;
		std::cout << "LoadPLY: This is not a .ply file" << std::endl;
		return;
	}

	while (!header_read)
	{
		char* result = fgets(line, sizeof(line), ply_file);
		if (!result)
		{
			std::cout << "LoadPLY: Error reading line from file" << std::endl;
		}

		token = strtok(line, " ");

		if (strcmp(token, "comment") == 0)
		{
			continue;
		}

		/* End Header */
		if (strcmp(token, "end_header\n") == 0)
		{
			header_read = 1;
			break;
		}

		/* Parse element */
		if (strcmp(token, "element") == 0)
		{
			token = strtok(NULL, " ");

			/* Parse vertex */
			if (strcmp(token, "vertex") == 0)
			{
				token = strtok(NULL, " ");
				vertexCount = atoi(token);
				vertexBuffer = (float *)calloc(vertexCount * 3, sizeof(float));
				std::cout << vertexCount << std::endl;
			}

			/* parse face */
			if (strcmp(token, "face") == 0)
			{
				token = strtok(NULL, " ");
				faceCount = atoi(token);
				indexBuffer = (int *)calloc(faceCount * 3, sizeof(int));
				std::cout << faceCount << std::endl;
			}
		}
	}
	size_t numReads = 3 * vertexCount;
	size_t numRead = fread(vertexBuffer, sizeof(float), 3 * vertexCount, ply_file);
	if(numReads != numRead)
	{
		std::cout << "Error reading vertex definitions" << std::endl;
		vertexCount = 0;
		vertexBuffer = nullptr;
	}

	for (int i = 0; i < faceCount; i++)
	{
		numReads = 1;
		unsigned char dummy;
		numRead = fread(&dummy, sizeof(unsigned char), 1, ply_file);
		if (numReads != numRead)
		{
			std::cout << "Error reading face index count" << std::endl;
			indexBuffer = nullptr; 
			faceCount = 0;
			return;
		}

		numReads = 3;
		numRead = fread(&indexBuffer[3 * i], sizeof(int), 3, ply_file);
		if (numReads != numRead)
		{
			std::cout << "Error reading face indices" << std::endl;
			indexBuffer = nullptr; 
			faceCount = 0;
			return;
		}
	}

	/* Load vertices */
	fclose(ply_file);
}

} // namespace asteroids
