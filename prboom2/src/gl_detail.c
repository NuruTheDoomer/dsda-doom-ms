/* Emacs style mode select   -*- C -*-
 *-----------------------------------------------------------------------------
 *
 *
 *  PrBoom: a Doom port merged with LxDoom and LSDLDoom
 *  based on BOOM, a modified and improved DOOM engine
 *  Copyright (C) 1999 by
 *  id Software, Chi Hoang, Lee Killough, Jim Flynn, Rand Phares, Ty Halderman
 *  Copyright (C) 1999-2000 by
 *  Jess Haas, Nicolas Kalkhof, Colin Phipps, Florian Schulze
 *  Copyright 2005, 2006 by
 *  Florian Schulze, Colin Phipps, Neil Stevens, Andrey Budko
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 *  02111-1307, USA.
 *
 * DESCRIPTION:
 *
 *---------------------------------------------------------------------
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "gl_opengl.h"

#include "z_zone.h"
#include <SDL.h>

#ifdef HAVE_LIBSDL2_IMAGE
#include <SDL_image.h>
#endif

#include <math.h>

#include "v_video.h"
#include "r_main.h"
#include "gl_intern.h"
#include "w_wad.h"
#include "lprintf.h"
#include "p_spec.h"
#include "m_misc.h"
#include "sc_man.h"
#include "e6y.h"
#include "i_system.h"

int render_usedetail;

int scene_has_details;
int scene_has_wall_details;
int scene_has_flat_details;

typedef enum
{
  TAG_DETAIL_WALL,
  TAG_DETAIL_FLAT,
  TAG_DETAIL_MAX,
} tag_detail_e;

static GLuint last_detail_texid = -1;

float xCamera,yCamera,zCamera;
TAnimItemParam *anim_flats = NULL;
TAnimItemParam *anim_textures = NULL;

void gld_InitDetail(void)
{
  render_usedetail = true;
  gld_EnableDetail(true);
  gld_EnableDetail(false);
  gld_FlushTextures();
}

void gld_DrawTriangleStripARB(GLWall *wall, gl_strip_coords_t *c1, gl_strip_coords_t *c2)
{
  glBegin(GL_TRIANGLE_STRIP);

  // lower left corner
  GLEXT_glMultiTexCoord2fvARB(GL_TEXTURE0_ARB,(const GLfloat*)&c1->t[0]);
  GLEXT_glMultiTexCoord2fvARB(GL_TEXTURE1_ARB,(const GLfloat*)&c2->t[0]);
  glVertex3fv((const GLfloat*)&c1->v[0]);

  // split left edge of wall
  //if (!wall->glseg->fracleft)
  //  gld_SplitLeftEdge(wall, true);

  // upper left corner
  GLEXT_glMultiTexCoord2fvARB(GL_TEXTURE0_ARB,(const GLfloat*)&c1->t[1]);
  GLEXT_glMultiTexCoord2fvARB(GL_TEXTURE1_ARB,(const GLfloat*)&c2->t[1]);
  glVertex3fv((const GLfloat*)&c1->v[1]);

  // upper right corner
  GLEXT_glMultiTexCoord2fvARB(GL_TEXTURE0_ARB,(const GLfloat*)&c1->t[2]);
  GLEXT_glMultiTexCoord2fvARB(GL_TEXTURE1_ARB,(const GLfloat*)&c2->t[2]);
  glVertex3fv((const GLfloat*)&c1->v[2]);

  // split right edge of wall
  //if (!wall->glseg->fracright)
  //  gld_SplitRightEdge(wall, true);

  // lower right corner
  GLEXT_glMultiTexCoord2fvARB(GL_TEXTURE0_ARB,(const GLfloat*)&c1->t[3]);
  GLEXT_glMultiTexCoord2fvARB(GL_TEXTURE1_ARB,(const GLfloat*)&c2->t[3]);
  glVertex3fv((const GLfloat*)&c1->v[3]);

  glEnd();
}

void gld_PreprocessDetail(void)
{
  if (gl_arb_multitexture)
  {
    GLEXT_glClientActiveTextureARB(GL_TEXTURE0_ARB);
    glTexCoordPointer(2, GL_FLOAT, sizeof(flats_vbo[0]), flats_vbo_u);

    GLEXT_glClientActiveTextureARB(GL_TEXTURE1_ARB);
    glTexCoordPointer(2, GL_FLOAT, sizeof(flats_vbo[0]), flats_vbo_u);
    GLEXT_glClientActiveTextureARB(GL_TEXTURE0_ARB);

    GLEXT_glActiveTextureARB(GL_TEXTURE1_ARB);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
    glTexEnvi(GL_TEXTURE_ENV, GL_RGB_SCALE_ARB, 2);
    GLEXT_glActiveTextureARB(GL_TEXTURE0_ARB);
  }
}


void gld_EnableDetail(int enable)
{
  if (!gl_arb_multitexture || !render_usedetail)
    return;

  gld_EnableTexture2D(GL_TEXTURE1_ARB, enable);
  gld_EnableClientCoordArray(GL_TEXTURE1_ARB, enable);
}

void gld_DrawWallWithDetail(GLWall *wall)
{
  float w, h, dx, dy;
  dboolean fake = (wall->flag == GLDWF_TOPFLUD) || (wall->flag == GLDWF_BOTFLUD);
  detail_t *detail = wall->gltexture->detail;

  w = wall->gltexture->detail_width;
  h = wall->gltexture->detail_height;
  dx = detail->offsetx;
  dy = detail->offsety;

  if (fake)
  {
    int i;
    gl_strip_coords_t c1;
    gl_strip_coords_t c2;

    gld_BindFlat(wall->gltexture, 0);

    gld_SetupFloodStencil(wall);
    gld_SetupFloodedPlaneLight(wall);
    gld_SetupFloodedPlaneCoords(wall, &c1);
    for (i = 0; i < 4; i++)
    {
      c2.t[i][0] = c1.t[i][0] * w + dx;
      c2.t[i][1] = c1.t[i][1] * h + dy;
    }

    gld_EnableTexture2D(GL_TEXTURE1_ARB, true);
    gld_DrawTriangleStripARB(wall, &c1, &c2);
    gld_EnableTexture2D(GL_TEXTURE1_ARB, false);

    gld_ClearFloodStencil(wall);
  }
  else
  {
    gld_StaticLightAlpha(wall->light, wall->alpha);
    glBegin(GL_TRIANGLE_FAN);

    // lower left corner
    GLEXT_glMultiTexCoord2fARB(GL_TEXTURE0_ARB,wall->ul,wall->vb);
    GLEXT_glMultiTexCoord2fARB(GL_TEXTURE1_ARB,wall->ul*w+dx,wall->vb*h+dy);
    glVertex3f(wall->glseg->x1,wall->ybottom,wall->glseg->z1);

    // split left edge of wall
    if (!wall->glseg->fracleft)
      gld_SplitLeftEdge(wall, true);

    // upper left corner
    GLEXT_glMultiTexCoord2fARB(GL_TEXTURE0_ARB,wall->ul,wall->vt);
    GLEXT_glMultiTexCoord2fARB(GL_TEXTURE1_ARB,wall->ul*w+dx,wall->vt*h+dy);
    glVertex3f(wall->glseg->x1,wall->ytop,wall->glseg->z1);

    // upper right corner
    GLEXT_glMultiTexCoord2fARB(GL_TEXTURE0_ARB,wall->ur,wall->vt);
    GLEXT_glMultiTexCoord2fARB(GL_TEXTURE1_ARB,wall->ur*w+dx,wall->vt*h+dy);
    glVertex3f(wall->glseg->x2,wall->ytop,wall->glseg->z2);

    // split right edge of wall
    if (!wall->glseg->fracright)
      gld_SplitRightEdge(wall, true);

    // lower right corner
    GLEXT_glMultiTexCoord2fARB(GL_TEXTURE0_ARB,wall->ur,wall->vb);
    GLEXT_glMultiTexCoord2fARB(GL_TEXTURE1_ARB,wall->ur*w+dx,wall->vb*h+dy);
    glVertex3f(wall->glseg->x2,wall->ybottom,wall->glseg->z2);

    glEnd();
  }
}

void gld_DrawWallDetail_NoARB(GLWall *wall)
{
  if (!wall->gltexture->detail)
    return;

  if (wall->flag >= GLDWF_SKY)
    return;

  {
    float w, h, dx, dy;
    dboolean fake = (wall->flag == GLDWF_TOPFLUD) || (wall->flag == GLDWF_BOTFLUD);
    detail_t *detail = wall->gltexture->detail;

    w = wall->gltexture->detail_width;
    h = wall->gltexture->detail_height;
    dx = detail->offsetx;
    dy = detail->offsety;

    gld_BindDetail(wall->gltexture, detail->texid);

    if (fake)
    {
      int i;
      gl_strip_coords_t c;

      if (gl_use_fog)
      {
        // calculation of fog density for flooded walls
        if (wall->seg->backsector)
        {
          wall->fogdensity = gld_CalcFogDensity(wall->seg->frontsector,
            wall->seg->backsector->lightlevel, GLDIT_FWALL);
        }
        gld_SetFog(wall->fogdensity);
      }

      gld_SetupFloodStencil(wall);
      gld_SetupFloodedPlaneLight(wall);
      gld_SetupFloodedPlaneCoords(wall, &c);
      for (i = 0; i < 4; i++)
      {
        c.t[i][0] = c.t[i][0] * w + dx;
        c.t[i][1] = c.t[i][1] * h + dy;
      }
      gld_DrawTriangleStrip(wall, &c);
      gld_ClearFloodStencil(wall);
    }
    else
    {
      gld_StaticLightAlpha(wall->light, wall->alpha);
      glBegin(GL_TRIANGLE_FAN);

      // lower left corner
      glTexCoord2f(wall->ul*w+dx,wall->vb*h+dy);
      glVertex3f(wall->glseg->x1,wall->ybottom,wall->glseg->z1);

      // split left edge of wall
      if (!wall->glseg->fracleft)
        gld_SplitLeftEdge(wall, true);

      // upper left corner
      glTexCoord2f(wall->ul*w+dx,wall->vt*h+dy);
      glVertex3f(wall->glseg->x1,wall->ytop,wall->glseg->z1);

      // upper right corner
      glTexCoord2f(wall->ur*w+dx,wall->vt*h+dy);
      glVertex3f(wall->glseg->x2,wall->ytop,wall->glseg->z2);

      // split right edge of wall
      if (!wall->glseg->fracright)
        gld_SplitRightEdge(wall, true);

      // lower right corner
      glTexCoord2f(wall->ur*w+dx,wall->vb*h+dy);
      glVertex3f(wall->glseg->x2,wall->ybottom,wall->glseg->z2);

      glEnd();
    }
  }
}

void gld_DrawFlatDetail_NoARB(GLFlat *flat)
{
  float w, h, dx, dy;
  int loopnum;
  GLLoopDef *currentloop;
  detail_t *detail;

  if (!flat->gltexture->detail)
    return;

  detail = flat->gltexture->detail;
  gld_BindDetail(flat->gltexture, detail->texid);

  gld_StaticLightAlpha(flat->light, flat->alpha);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glTranslatef(0.0f,flat->z,0.0f);
  glMatrixMode(GL_TEXTURE);
  glPushMatrix();

  w = flat->gltexture->detail_width;
  h = flat->gltexture->detail_height;
  dx = detail->offsetx;
  dy = detail->offsety;

  if ((flat->flags & GLFLAT_HAVE_TRANSFORM) || dx || dy)
  {
    glTranslatef(flat->uoffs * w + dx, flat->voffs * h + dy, 0.0f);
  }

  glScalef(w, h, 1.0f);

  if (flat->sectornum>=0)
  {
    // go through all loops of this sector
    for (loopnum=0; loopnum<sectorloops[flat->sectornum].loopcount; loopnum++)
    {
      currentloop=&sectorloops[flat->sectornum].loops[loopnum];
      glDrawArrays(currentloop->mode,currentloop->vertexindex,currentloop->vertexcount);
    }
  }
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
}

static int C_DECL dicmp_wall_detail(const void *a, const void *b)
{
  detail_t *d1 = ((const GLDrawItem *)a)->item.wall->gltexture->detail;
  detail_t *d2 = ((const GLDrawItem *)b)->item.wall->gltexture->detail;
  return d1 - d2;
}

static int C_DECL dicmp_flat_detail(const void *a, const void *b)
{
  detail_t *d1 = ((const GLDrawItem *)a)->item.flat->gltexture->detail;
  detail_t *d2 = ((const GLDrawItem *)b)->item.flat->gltexture->detail;
  return d1 - d2;
}

void gld_DrawItemsSortByDetail(GLDrawItemType itemtype)
{
  typedef int(C_DECL *DICMP_ITEM)(const void *a, const void *b);

  static DICMP_ITEM itemfuncs[GLDIT_TYPES] = {
    0,
    dicmp_wall_detail, dicmp_wall_detail, dicmp_wall_detail, dicmp_wall_detail, dicmp_wall_detail,
    dicmp_wall_detail, dicmp_wall_detail,
    dicmp_flat_detail, dicmp_flat_detail,
    dicmp_flat_detail, dicmp_flat_detail,
    0, 0, 0,
    0,
  };

  if (itemfuncs[itemtype] && gld_drawinfo.num_items[itemtype] > 1)
  {
    qsort(gld_drawinfo.items[itemtype], gld_drawinfo.num_items[itemtype],
      sizeof(gld_drawinfo.items[itemtype]), itemfuncs[itemtype]);
  }
}

void gld_DrawDetail_NoARB(void)
{
  int i;

  if (!scene_has_wall_details && !scene_has_flat_details)
    return;

  glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
  glBlendFunc (GL_DST_COLOR, GL_SRC_COLOR);

  last_detail_texid = -1;

  // detail flats

  if (scene_has_flat_details)
  {
    // enable backside removing
    glEnable(GL_CULL_FACE);

    // floors
    glCullFace(GL_FRONT);
    gld_DrawItemsSortByDetail(GLDIT_FLOOR);
    for (i = gld_drawinfo.num_items[GLDIT_FLOOR] - 1; i >= 0; i--)
    {
      gld_SetFog(gld_drawinfo.items[GLDIT_FLOOR][i].item.flat->fogdensity);
      gld_DrawFlatDetail_NoARB(gld_drawinfo.items[GLDIT_FLOOR][i].item.flat);
    }
    // ceilings
    glCullFace(GL_BACK);
    gld_DrawItemsSortByDetail(GLDIT_CEILING);
    for (i = gld_drawinfo.num_items[GLDIT_CEILING] - 1; i >= 0; i--)
    {
      gld_SetFog(gld_drawinfo.items[GLDIT_CEILING][i].item.flat->fogdensity);
      gld_DrawFlatDetail_NoARB(gld_drawinfo.items[GLDIT_CEILING][i].item.flat);
    }
    glDisable(GL_CULL_FACE);
  }

  // detail walls
  if (scene_has_wall_details)
  {
    gld_DrawItemsSortByDetail(GLDIT_WALL);
    for (i = gld_drawinfo.num_items[GLDIT_WALL] - 1; i >= 0; i--)
    {
      gld_SetFog(gld_drawinfo.items[GLDIT_WALL][i].item.wall->fogdensity);
      gld_DrawWallDetail_NoARB(gld_drawinfo.items[GLDIT_WALL][i].item.wall);
    }

    if (!gl_use_stencil)
    {
      gld_DrawItemsSortByDetail(GLDIT_MWALL);
    }

    for (i = gld_drawinfo.num_items[GLDIT_MWALL] - 1; i >= 0; i--)
    {
      GLWall *wall = gld_drawinfo.items[GLDIT_MWALL][i].item.wall;
      if (gl_use_stencil)
      {
        if (!(wall->gltexture->flags & GLTEXTURE_HASHOLES))
        {
          gld_SetFog(wall->fogdensity);
          gld_DrawWallDetail_NoARB(wall);
        }
      }
      else
      {
        gld_SetFog(wall->fogdensity);
        gld_DrawWallDetail_NoARB(wall);
      }
    }

    if (gld_drawinfo.num_items[GLDIT_FWALL] > 0)
    {
      glPolygonOffset(1.0f, 128.0f);
      glEnable(GL_POLYGON_OFFSET_FILL);
      glEnable(GL_STENCIL_TEST);

      gld_DrawItemsSortByDetail(GLDIT_FWALL);
      for (i = gld_drawinfo.num_items[GLDIT_FWALL] - 1; i >= 0; i--)
      {
        gld_DrawWallDetail_NoARB(gld_drawinfo.items[GLDIT_FWALL][i].item.wall);
      }

      glDisable(GL_STENCIL_TEST);
      glPolygonOffset(0.0f, 0.0f);
      glDisable(GL_POLYGON_OFFSET_FILL);
    }

    gld_DrawItemsSortByDetail(GLDIT_TWALL);
    for (i = gld_drawinfo.num_items[GLDIT_TWALL] - 1; i >= 0; i--)
    {
      gld_SetFog(gld_drawinfo.items[GLDIT_TWALL][i].item.wall->fogdensity);
      gld_DrawWallDetail_NoARB(gld_drawinfo.items[GLDIT_TWALL][i].item.wall);
    }
  }

  // restore
  glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void gld_InitFrameDetails(void)
{
  last_detail_texid = -1;

  scene_has_details =
    (render_usedetail) &&
    (scene_has_wall_details || scene_has_flat_details);
}

void gld_BindDetailARB(GLTexture *gltexture, int enable)
{
  if (scene_has_details)
  {
    gld_EnableTexture2D(GL_TEXTURE1_ARB, enable);
    gld_EnableClientCoordArray(GL_TEXTURE1_ARB, enable);

    if (enable &&
      gltexture->detail &&
      gltexture->detail->texid != last_detail_texid)
    {
      last_detail_texid = gltexture->detail->texid;

      GLEXT_glActiveTextureARB(GL_TEXTURE1_ARB);
      glBindTexture(GL_TEXTURE_2D, gltexture->detail->texid);
      GLEXT_glActiveTextureARB(GL_TEXTURE0_ARB);
    }
  }
}

void gld_BindDetail(GLTexture *gltexture, int enable)
{
  if (scene_has_details)
  {
    if (enable &&
        gltexture->detail &&
        gltexture->detail->texid != last_detail_texid)
    {
      last_detail_texid = gltexture->detail->texid;
      glBindTexture(GL_TEXTURE_2D, gltexture->detail->texid);
    }
  }
}

void gld_SetTexDetail(GLTexture *gltexture)
{
  gltexture->detail = NULL;
}
