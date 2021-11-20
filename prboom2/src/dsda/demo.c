//
// Copyright(C) 2020 by Ryan Krafnick
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// DESCRIPTION:
//	DSDA Demo
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "doomtype.h"
#include "doomstat.h"
#include "m_misc.h"
#include "lprintf.h"
#include "e6y.h"

#include "dsda/excmd.h"

#include "demo.h"

#define INITIAL_DEMO_BUFFER_SIZE 0x20000

static byte* dsda_demo_write_buffer;
static byte* dsda_demo_write_buffer_p;
static int dsda_demo_write_buffer_length;
static char* dsda_demo_name;

#define DSDA_DEMO_VERSION 1

static int dsda_demo_version;

static void dsda_EnsureDemoBufferSpace(size_t length) {
  int offset;

  offset = dsda_DemoBufferOffset();

  if (offset + length <= dsda_demo_write_buffer_length) return;

  while (offset + length > dsda_demo_write_buffer_length)
    dsda_demo_write_buffer_length *= 2;

  dsda_demo_write_buffer =
    (byte *)realloc(dsda_demo_write_buffer, dsda_demo_write_buffer_length);

  if (dsda_demo_write_buffer == NULL)
    I_Error("dsda_EnsureDemoBufferSpace: out of memory!");

  dsda_demo_write_buffer_p = dsda_demo_write_buffer + offset;

  lprintf(
    LO_INFO,
    "dsda_EnsureDemoBufferSpace: expanding demo buffer %d\n",
    dsda_demo_write_buffer_length
  );
}

void dsda_InitDemo(char* name) {
  size_t name_size;

  name_size = strlen(name) + 1;
  dsda_demo_name = malloc(name_size);
  memcpy(dsda_demo_name, name, name_size);

  dsda_demo_write_buffer = malloc(INITIAL_DEMO_BUFFER_SIZE);
  if (dsda_demo_write_buffer == NULL)
    I_Error("dsda_InitDemo: unable to initialize demo buffer!");

  dsda_demo_write_buffer_p = dsda_demo_write_buffer;

  dsda_demo_write_buffer_length = INITIAL_DEMO_BUFFER_SIZE;
}

void dsda_WriteToDemo(void* buffer, size_t length) {
  dsda_EnsureDemoBufferSpace(length);

  memcpy(dsda_demo_write_buffer_p, buffer, length);
  dsda_demo_write_buffer_p += length;
}

void dsda_WriteDemoToFile(void) {
  int length;

  length = dsda_DemoBufferOffset();

  if (!M_WriteFile(dsda_demo_name, dsda_demo_write_buffer, length))
    I_Error("dsda_WriteDemoToFile: Failed to write demo file.");

  free(dsda_demo_write_buffer);
  free(dsda_demo_name);
  dsda_demo_write_buffer = NULL;
  dsda_demo_write_buffer_p = NULL;
  dsda_demo_write_buffer_length = 0;
  dsda_demo_name = NULL;
}

int dsda_DemoBufferOffset(void) {
  return dsda_demo_write_buffer_p - dsda_demo_write_buffer;
}

int dsda_CopyDemoBuffer(void* buffer) {
  int offset;

  offset = dsda_DemoBufferOffset();
  memcpy(buffer, dsda_demo_write_buffer, offset);

  return offset;
}

void dsda_SetDemoBufferOffset(int offset) {
  if (dsda_demo_write_buffer == NULL) return;

  // Cannot load forward (demo buffer would desync)
  if (offset > dsda_DemoBufferOffset())
    I_Error("dsda_SetDemoBufferOffset: Impossible time traveling detected.");

  dsda_demo_write_buffer_p = dsda_demo_write_buffer + offset;
}

void dsda_JoinDemoCmd(ticcmd_t* cmd) {
  // Sometimes this bit is not available
  if (
    (demo_compatibility && !prboom_comp[PC_ALLOW_SSG_DIRECT].state) ||
    (cmd->buttons & BT_CHANGE) == 0
  )
    cmd->buttons |= BT_JOIN;
}

static const byte* dsda_ReadDSDADemoHeader(const byte* demo_p, const byte* header_p, size_t size) {
  // 7 = 6 (signature) + 1 (dsda version)
  if (demo_p - header_p + 7 > size)
    return NULL;

  if (*demo_p++ != 0x1d)
    return NULL;

  if (strncmp((const char *) demo_p, "DSDA", 4) != 0)
    return NULL;

  demo_p += 4;

  if (*demo_p++ != 0xe6)
    return NULL;

  dsda_demo_version = *demo_p++;

  if (dsda_demo_version > DSDA_DEMO_VERSION)
    return NULL;

  dsda_EnableExCmd();

  return demo_p;
}

// Strip off the defunct extended header (if we understand it) or abort (if we don't)
static const byte* dsda_ReadUMAPINFODemoHeader(const byte* demo_p, const byte* header_p, size_t size) {
  // 9 = 6 (signature) + 1 (version) + 2 (extension count)
  if (demo_p - header_p + 9 > size)
    return NULL;

  if (strncmp((const char *)demo_p, "PR+UM", 5) != 0)
    I_Error("G_ReadDemoHeader: Unknown demo format");

  demo_p += 6;

  // the defunct format had only version 1
  if (*demo_p++ != 1)
    I_Error("G_ReadDemoHeader: Unknown demo format");

  // the defunct format had only one extension (in two bytes)
  if (*demo_p++ != 1 || *demo_p++ != 0)
    I_Error("G_ReadDemoHeader: Unknown demo format");

  if (demo_p - header_p + 1 > size)
    return NULL;

  // the defunct extension had length 8
  if (*demo_p++ != 8)
    I_Error("G_ReadDemoHeader: Unknown demo format");

  if (demo_p - header_p + 8 > size)
    return NULL;

  if (strncmp((const char *)demo_p, "UMAPINFO", 8))
    I_Error("G_ReadDemoHeader: Unknown demo format");

  demo_p += 8;

  // the defunct extension stored the map lump (unused)
  if (demo_p - header_p + 8 > size)
    return NULL;

  demo_p += 8;

  return demo_p;
}

const byte* dsda_StripDemoVersion255(const byte* demo_p, const byte* header_p, size_t size) {
  const byte* dsda_p;

  dsda_p = dsda_ReadDSDADemoHeader(demo_p, header_p, size);

  if (dsda_p) return dsda_p;

  return dsda_ReadUMAPINFODemoHeader(demo_p, header_p, size);
}

void dsda_WriteDSDADemoHeader(byte** p) {
  byte* demo_p = *p;

  *demo_p++ = 255;

  // signature
  *demo_p++ = 0x1d;
  *demo_p++ = 'D';
  *demo_p++ = 'S';
  *demo_p++ = 'D';
  *demo_p++ = 'A';
  *demo_p++ = 0xe6;

  *demo_p++ = DSDA_DEMO_VERSION;

  *p = demo_p;
}
