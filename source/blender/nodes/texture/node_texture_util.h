/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * The Original Code is Copyright (C) 2005 Blender Foundation.
 * All rights reserved.
 */

/** \file
 * \ingroup nodes
 */

#pragma once

#include <math.h>
#include <string.h>

#include "MEM_guardedalloc.h"

#include "DNA_ID.h"
#include "DNA_color_types.h"
#include "DNA_image_types.h"
#include "DNA_material_types.h"
#include "DNA_node_types.h"
#include "DNA_object_types.h"
#include "DNA_scene_types.h"
#include "DNA_texture_types.h"

#include "BLI_math_vector.h"
#include "BLI_rand.h"
#include "BLI_threads.h"
#include "BLI_utildefines.h"

#include "BKE_colorband.h"
#include "BKE_colortools.h"
#include "BKE_global.h"
#include "BKE_image.h"
#include "BKE_main.h"
#include "BKE_material.h"
#include "BKE_node.h"
#include "BKE_texture.h"

#include "NOD_texture.h"
#include "node_util.h"

#include "BLT_translation.h"

#include "IMB_imbuf.h"
#include "IMB_imbuf_types.h"

#include "RE_pipeline.h"
#include "RE_texture.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct TexCallData {
  TexResult *target;
  /* all float[3] */
  const float *co;
  float *dxt, *dyt;

  int osatex;
  bool do_preview;
  bool do_manage;
  short thread;
  short which_output;
  int cfra;

  MTex *mtex;
} TexCallData;

typedef struct TexParams {
  const float *co;
  float *dxt, *dyt;
  const float *previewco;
  int cfra;
  int osatex;

  /* optional. we don't really want these here, but image
   * textures need to do mapping & color correction */
  MTex *mtex;
} TexParams;

typedef void (*TexFn)(float *out, TexParams *params, bNode *node, bNodeStack **in, short thread);

typedef struct TexDelegate {
  TexCallData *cdata;
  TexFn fn;
  bNode *node;
  bNodePreview *preview;
  bNodeStack *in[MAX_SOCKET];
  int type;
} TexDelegate;

bool tex_node_poll_default(struct bNodeType *ntype,
                           struct bNodeTree *ntree,
                           const char **r_disabled_hint);
void tex_node_type_base(struct bNodeType *ntype, int type, const char *name, short nclass);

void tex_input_rgba(float *out, bNodeStack *in, TexParams *params, short thread);
void tex_input_vec(float *out, bNodeStack *in, TexParams *params, short thread);
float tex_input_value(bNodeStack *in, TexParams *params, short thread);

void tex_output(bNode *node,
                bNodeExecData *execdata,
                bNodeStack **in,
                bNodeStack *out,
                TexFn texfn,
                TexCallData *data);

void params_from_cdata(TexParams *out, TexCallData *in);

#ifdef __cplusplus
}
#endif
