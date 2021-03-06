/*
This Source Code Form is subject to the
terms of the Mozilla Public License, v.
2.0. If a copy of the MPL was not
distributed with this file, You can
obtain one at
http://mozilla.org/MPL/2.0/.
*/

#ifndef TEXTURE_DDS_H
#define TEXTURE_DDS_H 1

#include "GL/glew.h"

#pragma hdrstop

struct Color8888
{
    GLubyte r; // change the order of names to change the
    GLubyte g; //  order of the output ARGB or BGRA, etc...
    GLubyte b; //  Last one is MSB, 1st is LSB.
    GLubyte a;
};

struct DDS_IMAGE_DATA
{
    GLsizei width;
    GLsizei height;
    GLint components;
    GLenum format;
    GLuint blockSize;
    int numMipMaps;
    GLubyte *pixels;
};

void DecompressDXT(DDS_IMAGE_DATA lImage, const GLubyte *lCompData, GLubyte *Data);

#endif
