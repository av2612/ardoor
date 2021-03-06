//#############################################################################
//  File:      SLGLState.h
//  Purpose:   Singleton class for global render state 
//  Author:    Marcus Hudritsch
//  Date:      February 2013
//  Copyright (c): 2002-2013 Marcus Hudritsch
//             This software is provide under the GNU General Public License
//             Please visit: http://opensource.org/licenses/GPL-3.0
//#############################################################################

#ifndef SLGLSTATE_H
#define SLGLSTATE_H

#include <stdafx.h>

//-----------------------------------------------------------------------------
static const SLint SL_MAX_LIGHTS = 8;   //!< max. number of used lights
//-----------------------------------------------------------------------------
//! Singleton class holding all OpenGL states
/*!
The main purpose of the SLGLState class is to replace all the OpenGL states and
functionality that has been removed from the core profile of OpenGL. The core
profile started from OpenGL version 3.0 has e.g. no more internal matrices, 
lighting or material states. It also has no more fixed function pipeline on the
GPU witch means, that core profile OpenGL only works with custom shader 
programs written in OpenGL Shading Language (GLSL).
The second purpose is to concentrate OpenGL functionality and to reduce 
redundant state changes.
*/
class SLGLState
{  public:
      
      static SLGLState* getInstance();          //!< global creator & getter
      static void       deleteInstance();       //!< global destruction
             void       onInitialize(SLCol4f clearColor); //!< On init GL
             void       initAll();              //! Init all states
      
      // matrices
      SLMat4f  modelViewMatrix;     //!< matrix for OpenGL modelview transform
      SLMat4f  projectionMatrix;    //!< matrix for OpenGL projection transform
      SLMat4f  viewMatrix;          //!< matrix for the acitive cameras view transform
      
      // lighting
      SLint    numLightsUsed;                   //!< Num. of lights used
      SLint    lightIsOn[SL_MAX_LIGHTS];        //!< Flag if light is on
      SLVec4f  lightPosWS[SL_MAX_LIGHTS];       //!< position of light in world space
      SLVec4f  lightPosVS[SL_MAX_LIGHTS];       //!< position of light in view space
      SLVec4f  lightAmbient[SL_MAX_LIGHTS];     //!< ambient light intensity (Ia)
      SLVec4f  lightDiffuse[SL_MAX_LIGHTS];     //!< diffuse light intensity (Id)
      SLVec4f  lightSpecular[SL_MAX_LIGHTS];    //!< specular light intensity (Is)
      SLVec3f  lightDirWS[SL_MAX_LIGHTS];       //!< spot direction in world space
      SLVec3f  lightDirVS[SL_MAX_LIGHTS];       //!< spot direction in view space
      SLfloat  lightSpotCutoff[SL_MAX_LIGHTS];  //!< spot cutoff angle 1-180 degrees
      SLfloat  lightSpotCosCut[SL_MAX_LIGHTS];  //!< cosine of spot cutoff angle
      SLfloat  lightSpotExp[SL_MAX_LIGHTS];     //!< spot exponent
      SLVec3f  lightAtt[SL_MAX_LIGHTS];         //!< att. factor (const,linear,quadr.)
      SLint    lightDoAtt[SL_MAX_LIGHTS];       //!< Flag if att. must be calc.
      SLCol4f  globalAmbientLight;              //!< global ambient light intensity

      // material
      SLCol4f  matAmbient;                      //!< ambient color reflection (ka)
      SLCol4f  matDiffuse;                      //!< diffuse color reflection (kd)
      SLCol4f  matSpecular;                     //!< specular color reflection (ks)
      SLCol4f  matEmissive;                     //!< emissive color (ke)
      SLfloat  matShininess;                    //!< shininess exponent

      // fog
      SLbool   fogIsOn;                         //!< Flag if fog blending is enabled
      SLint    fogMode;                         //!< 0=GL_LINEAR, 1=GL_EXP, 2=GL_EXP2
      SLfloat  fogDensity;                      //!< Fog density for exponential modes
      SLfloat  fogDistStart;                    //!< Fog start distance for linear mode
      SLfloat  fogDistEnd;                      //!< Fog end distance for linear mode
      SLCol4f  fogColor;                        //!< fog color blended to the final color

      // stereo
      SLint    projection;                      //!< type of projection (see SLCamera)
      SLint    stereoEye;                       //!< -1=left, 0=center, 1=right
      SLMat3f  stereoColorFilter;               //!< color filter matrix for anaglyphs

      // setters
      void     invModelViewMatrix(SLMat4f &im) {_invModelViewMatrix.setMatrix(im);}
      void     normalMatrix(SLMat3f &nm) {_normalMatrix.setMatrix(nm);}
                
      // getters
      const SLMat4f* invModelViewMatrix();      //!< return inv. modelview mat.
      const SLMat3f* normalMatrix();            //!< builds returns transposed inv. mv mat.
      const SLMat4f* mvpMatrix();               //!< builds and returns proj.mat. x mv mat.
      const SLCol4f* globalAmbient();           //!< returns global ambient color
      
      void     buildInverseAndNormalMatrix();   //!< build inverse & normal mat. from MV
      void     buildNormalMatrix();             //!< build the normal matrix from MV
      void     unbindAnythingAndFlush();        //!< finishes all GL commands
      
      // light transformations into view space
      void     calcLightPosVS    (SLint nLights);
      void     calcLightDirVS    (SLint nLights);
      
      // state setters
      void     depthTest         (SLbool state);
      void     depthMask         (SLbool state);      
      void     cullFace          (SLbool state);
      void     blend             (SLbool state);
      void     multiSample       (SLbool state);
      void     polygonLine       (SLbool state);
      void     polygonOffset     (SLbool state, SLfloat factor=1.0f, SLfloat units=1.0f);
      void     viewport          (SLint x, SLint y, SLsizei w, SLsizei h);
      void     colorMask         (SLint r, SLint g, SLint b, SLint a);
      void     useProgram        (SLuint progID);
      void     bindTexture       (SLenum target, SLuint textureID);
      void     activeTexture     (SLenum textureUnit);
      void     clearColorBuffer  ()    {glClear(GL_COLOR_BUFFER_BIT);}
      void     clearDepthBuffer  ()    {glClear(GL_DEPTH_BUFFER_BIT);}
      void     clearColorDepthBuffer() {glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);}
      
      // state getters
      SLbool   blend()                 {return _blend;}
      SLstring glVersion()             {return _glVersion;}
      SLstring glVendor()              {return _glVendor;}
      SLstring glRenderer()            {return _glRenderer;}
      SLstring glGLSLVersion()         {return _glGLSLVersion;}  
      SLbool   hasExtension(SLstring e){return _glExtensions.find(e)!=string::npos;}   
      
      // stack operations
      void     pushModelViewMatrix();  //!< pushes mv matrix on stack
      void     popModelViewMatrix();   //!< pops top mv matrix
      void     pushProjectionMatrix(); //!< pushes proj. matrix on stack
      void     popProjectionMatrix();  //!< pops top proj. matrix
      
  private:
               SLGLState();            //!< private onetime constructor
              ~SLGLState();            //!< destruction in ~SLScene

      static SLGLState* instance;      //!< global singleten object
      
      SLMat4f  _invModelViewMatrix;    //!< inverse modelview transform
      SLMat3f  _normalMatrix;          //!< matrix for the normal transform
      SLMat4f  _mvpMatrix;             //!< combined modelview-projection transform
      SLVMat4f _modelViewMatrixStack;  //!< stack for modelView matrices
      SLVMat4f _projectionMatrixStack; //!< stack for projection matrices
      SLVec4f  _lightPosVS;            //!< light pos. in view space
      SLVec3f  _lightSpotDirVS;        //!< light spot direction in view space
      SLCol4f  _globalAmbient;         //!< global ambient color

      SLstring _glVersion;             //!< OpenGL Version string
      SLstring _glVendor;              //!< OpenGL Vendor string
      SLstring _glRenderer;            //!< OpenGL Renderer string
      SLstring _glGLSLVersion;         //!< GLSL Version string
      SLstring _glExtensions;          //!< OpenGL extensions string 

      // read/write states
      SLbool   _blend;                 //!< blending default false;
};
//-----------------------------------------------------------------------------
#endif
