/*****************************************************************************
*                                                                            *
*  OpenNI 2.x Alpha                                                          *
*  Copyright (C) 2012 PrimeSense Ltd.                                        *
*                                                                            *
*  This file is part of OpenNI.                                              *
*                                                                            *
*  Licensed under the Apache License, Version 2.0 (the "License");           *
*  you may not use this file except in compliance with the License.          *
*  You may obtain a copy of the License at                                   *
*                                                                            *
*      http://www.apache.org/licenses/LICENSE-2.0                            *
*                                                                            *
*  Unless required by applicable law or agreed to in writing, software       *
*  distributed under the License is distributed on an "AS IS" BASIS,         *
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  *
*  See the License for the specific language governing permissions and       *
*  limitations under the License.                                            *
*                                                                            *
*****************************************************************************/
// Undeprecate CRT functions
#ifndef _CRT_SECURE_NO_DEPRECATE 
#define _CRT_SECURE_NO_DEPRECATE 1
#endif

#include "Viewer.h"
#include "math.h"

#if (ONI_PLATFORM == ONI_PLATFORM_MACOSX)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define GL_WIN_SIZE_X	1280
#define GL_WIN_SIZE_Y	1024
#define TEXTURE_SIZE	512

#define DEFAULT_DISPLAY_MODE	DISPLAY_MODE_DEPTH1

#define MIN_NUM_CHUNKS(data_size, chunk_size)	((((data_size)-1) / (chunk_size) + 1))
#define MIN_CHUNKS_SIZE(data_size, chunk_size)	(MIN_NUM_CHUNKS(data_size, chunk_size) * (chunk_size))

Viewer* Viewer::ms_self = NULL;

void Viewer::glutIdle()
{
	glutPostRedisplay();
}
void Viewer::glutDisplay()
{
	Viewer::ms_self->display();
}
void Viewer::glutKeyboard(unsigned char key, int x, int y)
{
	Viewer::ms_self->onKey(key, x, y);
}
void Viewer::glutSpecialKeys(int key, int x, int y)
{
	Viewer::ms_self->onKey(key, x, y);
}
void Viewer::glutResize(int w, int h)
{
	Viewer::ms_self->handleResize(w, h);
}

Viewer::Viewer(const char* strSampleName, openni::VideoStream& depth1) :
	m_depth(depth1)
{
	ms_self = this;
	strncpy(m_strSampleName, strSampleName, ONI_MAX_STR);

	openni::VideoMode videoMode = m_depth.getVideoMode();
	m_width = videoMode.getResolutionX();
	m_height = videoMode.getResolutionY();

	m_inPlane = new bool[m_width * m_height];
	m_pointCloud = new float*[m_width * m_height];
	for (int i = 0; i < m_width * m_height; i++)
		m_pointCloud[i] = new float[3];
	m_binaryImage = new bool*[m_height / 2];
	for (int i = 0; i < m_height / 2; i++)
		m_binaryImage[i] = new bool[m_width / 2];
	m_blobs = new int*[m_height / 2];
	for (int i = 0; i < m_height / 2; i++)
		m_blobs[i] = new int[m_width / 2];
	m_pixelToCloud = new int[m_width * m_height];
	m_ransac = new RANSAC(50, 20);
	m_blobExtractor = new BlobExtractor(m_width / 2, m_height / 2, 150);
	angle = 0.0;
	lx = 0.0f, lz = -1.0f;
	cx = 0.0f, cz = 5.0f;
	m_stream = &m_depth;
}

Viewer::~Viewer()
{
	ms_self = NULL;
	delete[] m_inPlane;
	for (int i = 0; i < m_width * m_height; i++)
		delete[] m_pointCloud[i];
	delete[] m_pointCloud;
	for (int i = 0; i < m_height / 2; i++)
		delete[] m_binaryImage[i];
	delete[] m_binaryImage;
	for (int i = 0; i < m_height / 2; i++)
		delete[] m_blobs[i];
	delete[] m_blobs;
	delete[] m_pixelToCloud;
	delete m_ransac;
	delete m_blobExtractor;
}

openni::Status Viewer::init(int argc, char **argv)
{
	return initOpenGL(argc, argv);
}

openni::Status Viewer::run()
{
	glutMainLoop();

	return openni::STATUS_OK;
}

void Viewer::handleResize(int w, int h) 
{
	//Tell OpenGL how to convert from coordinates to pixel values
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION); //Switch to setting the camera perspective

	//Set the camera perspective
	glLoadIdentity(); //Reset the camera
	gluPerspective(60.0,				  //The camera angle
		(double)w / (double)h, //The width-to-height ratio
		100.0,				   //The near z clipping coordinate
		20000.0);				//The far z clipping coordinate
}

void Viewer::display()
{
	printf("Read\n");
	fflush(stdout);
	m_depth.readFrame(&m_depthFrame);
	printf("Done\n");
	fflush(stdout);

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	gluLookAt(cx, 1.0f, cz,
		cx + lx, 1.0f, cz + lz,
		0.0f, 1.0f,  0.0f);

	memset(m_inPlane, false, m_width * m_height * sizeof(bool));

	const openni::DepthPixel* pDepthRow = (const openni::DepthPixel*)m_depthFrame.getData();
	int rowSize = m_depthFrame.getStrideInBytes() / sizeof(openni::DepthPixel);
	int cnt = 0;
	for (int y = 0; y < m_depthFrame.getHeight(); y += 2)
	{
		const openni::DepthPixel* pDepth = pDepthRow;

		for (int x = 0; x < m_depthFrame.getWidth(); x += 2, pDepth += 2)
		{
			if (*pDepth != 0)
			{
				float wx, wy, wz;
				openni::CoordinateConverter::convertDepthToWorld(*m_stream, x, y, *pDepth, &wx, &wy, &wz);
				//depthToWorld(x, y, *pDepth, &wx, &wy, &wz);
				m_pointCloud[cnt][0] = wx;
				m_pointCloud[cnt][1] = wy;
				m_pointCloud[cnt][2] = wz;
				m_pixelToCloud[y * m_width + x] = cnt;
				cnt++;
			}
			else
				m_pixelToCloud[y * m_width + x] = -1;
		}

		pDepthRow += rowSize * 2;
	}

	m_ransac->MarkPointsInBestFitPlane(m_pointCloud, cnt, m_inPlane);

	pDepthRow = (const openni::DepthPixel*)m_depthFrame.getData();
	rowSize = m_depthFrame.getStrideInBytes() / sizeof(openni::DepthPixel);

	for (int y = 0; y < m_depthFrame.getHeight(); y += 2)
	{
		for (int x = 0; x < m_depthFrame.getWidth(); x += 2)
		{
			m_binaryImage[y / 2][x / 2] = m_pixelToCloud[y * m_width + x] != -1 ? !m_inPlane[m_pixelToCloud[y * m_width + x]] : false;
		}
	}

	int blobs = m_blobExtractor->ExtractBlobs(m_binaryImage, m_blobs);
	vector<float*> blobPoints[blobs];
	for (int y = 0; y < m_depthFrame.getHeight(); y += 2)
	{
		for (int x = 0; x < m_depthFrame.getWidth(); x += 2)
		{
			if (m_pixelToCloud[y * m_width + x] != -1)
			{
				int i = m_pixelToCloud[y * m_width + x];
				if (m_blobs[y / 2][x / 2] == 0)
				{
					glColor3f(0.1, 0.1, 0.1);
					if (m_binaryImage[y / 2][x / 2])
						glColor3f(1, 0, 0);
				}
				else
				{
					glColor3f(1 - (float)m_blobs[y / 2][x / 2] / (blobs + 1), 1, 1 - (float)m_blobs[y / 2][x / 2] / (blobs + 1));
					glBegin(GL_POINTS);
					glVertex3f(m_pointCloud[i][0], m_pointCloud[i][1], -m_pointCloud[i][2]);
					glEnd();
					blobPoints[m_blobs[y / 2][x / 2] - 1].push_back(m_pointCloud[i]);
				}
			}
		}
	}
	for (int i = 0; i < blobs; i++)
	{
		float x, y, z;
		pointCloudCenter(blobPoints[i], &x, &y, &z);
		glPointSize( 20.0 );
		glColor3f(0, 1, 1);
		glBegin(GL_POINTS);
		glVertex3f(x, y, -z);
		glEnd();
		glPointSize( 2.0 );
	}
	// Swap the OpenGL display buffers
	glutSwapBuffers();
}

void Viewer::pointCloudCenter(vector<float*> &cloud, float *x, float *y, float *z)
{
	float sumX = 0, sumY = 0, sumZ = 0;
	for (vector<float*>::iterator it = cloud.begin(); it < cloud.end(); it++)
	{
		sumX += (*it)[0];
		sumY += (*it)[1];
		sumZ += (*it)[2];
	}
	*x = sumX / cloud.size();
	*y = sumY / cloud.size();
	*z = sumZ / cloud.size();
}

float Viewer::raw_depth_to_meters(int depth_value)
{ 
	float depth_value_f = (float) depth_value; 
	if (depth_value < 2047){ 
		float depth = 1.0 / (depth_value_f  * -0.0030711016 + 3.3309495161);
		return depth; 
	}
	return 0.0f; 
} 

void Viewer::depthToWorld(float cgx, float cgy, float cgz, float *x, float *y, float *z)
{
	double fx_d = 1.0 / 5.9421434211923247e+02;
	double fy_d = 1.0 / 5.9104053696870778e+02;
	double cx_d = 3.3930780975300314e+02;
	double cy_d = 2.4273913761751615e+02;

	float depth = raw_depth_to_meters(cgz);

	*x = (float) (cgx - cx_d) * depth * fx_d;
	*y = (float) (cgy - cy_d) * depth * fy_d;
	*z = (float) depth;
}

void Viewer::onKey(unsigned char key, int /*x*/, int /*y*/)
{
	float fraction = 10.0f;

	switch (key) {
	case GLUT_KEY_LEFT :
		angle -= 0.01f;
		lx = sin(angle);
		lz = -cos(angle);
		break;
	case GLUT_KEY_RIGHT :
		angle += 0.01f;
		lx = sin(angle);
		lz = -cos(angle);
		break;
	case GLUT_KEY_UP :
		cx += lx * fraction;
		cz += lz * fraction;
		break;
	case GLUT_KEY_DOWN :
		cx -= lx * fraction;
		cz -= lz * fraction;
		break;
	case 27:
		m_depth.stop();
		m_depth.destroy();
		openni::OpenNI::shutdown();
		exit (1);
		break;
	}
}

openni::Status Viewer::initOpenGL(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(GL_WIN_SIZE_X, GL_WIN_SIZE_Y);
	glutCreateWindow (m_strSampleName);
	// 	glutFullScreen();
	glutSetCursor(GLUT_CURSOR_NONE);

	initOpenGLHooks();

	glEnable(GL_DEPTH_TEST);
	glPointSize( 2.0 );

	return openni::STATUS_OK;

}
void Viewer::initOpenGLHooks()
{
	glutKeyboardFunc(glutKeyboard);
	glutDisplayFunc(glutDisplay);
	glutIdleFunc(glutIdle);
	glutReshapeFunc(glutResize);
	glutSpecialFunc(glutSpecialKeys);
}
